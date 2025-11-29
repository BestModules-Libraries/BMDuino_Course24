// ===== 匯入函式庫 =====
#include "ThingSpeakPublish.h"
#include "BMC81M001.h"
#include <MQ2.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "BMS56M605.h"
#include <BMH06203.h>

// ===== 門檻（可自行微調）=====
const float kTiltWarnDeg     = 15.0f;  // 傾斜預警
const float kTiltFallDeg     = 30.0f;  // 倒伏
const float kVibStrongDeltaG = 0.5f;   // |a|-1g 超過此值視為劇烈震動（建議 0.3~0.5）
const float kPeakShockG      = 1.5f;   // 瞬間峰值（|a|）大於此值視為衝擊
const float kTempWarnC       = 50.0f;  // 物體溫度警報門檻（BMH06203）

// ===== 蜂鳴器樣式（可自行微調）=====
const int kWarnFreqHz   = 1200;  // 狀態1：頻率
const int kWarnOnMs     = 200;   // 狀態1：鳴叫時長
const int kWarnOffMs    = 800;   // 狀態1：靜音時長
const int kAlarmFreqHz  = 1800;  // 狀態2：頻率
const int kAlarmOnMs    = 250;   // 狀態2：鳴叫時長
const int kAlarmOffMs   = 250;   // 狀態2：靜音時長

// ===== 硬體設定 =====
#define Analog_Input A0
int buzzerPin = 10;
int INA = 9;
int INB = 8;
int solenoidPin = 7;

// ===== 物件宣告 =====
BMH06203 mytherm(&Wire);
BMC81M001 Wifi(&Serial1);
MQ2 mq2(Analog_Input);
LiquidCrystal_I2C lcd(0x27, 16, 2);
BMS56M605 Mpu(0x68);

// ===== 全域變數 =====
int lpg;

unsigned long lastLcdNumUpdateMs = 0;
const unsigned long lcdNumUpdateIntervalMs = 15000;

unsigned long lastUploadMs = 0;
const unsigned long uploadIntervalMs = 15000;

// 蜂鳴器狀態
unsigned long buzzerLastToggleMs = 0;
bool buzzerIsOn = false;

// ===== 輔助函式 =====
static void lcdPrintLabelsOnce() {
	lcd.clear();
	lcd.setCursor(0, 0);  lcd.print("gas:");
	lcd.setCursor(12, 0); lcd.print("ppm");
}

static void lcdReinit() {
	Wire.begin();
	lcd.begin();
	lcd.backlight();
	lcd.noAutoscroll();
	lcdPrintLabelsOnce();
}

void setup() {
	Serial.begin(115200);

	pinMode(solenoidPin, OUTPUT);
	digitalWrite(solenoidPin, LOW);

	Wire.begin();
	#if defined(TWBR)
		Wire.setClock(50000);
	#endif
	#if defined(WIRE_HAS_TIMEOUT)
		Wire.setWireTimeout(1000, true);
	#endif

	lcd.begin();
	lcd.backlight();
	lcd.noAutoscroll();
	lcdPrintLabelsOnce();

	mytherm.begin();
	mq2.begin();
	Wifi.begin();
	Mpu.begin();
	Wifi.reset();
	delay(200);

	pinMode(INA, OUTPUT);
	pinMode(INB, OUTPUT);
	pinMode(buzzerPin, OUTPUT);

	Serial.print("WIFI Connection Results:");
	if (Wifi.connectToAP(WIFI_SSID, WIFI_PASS) == 0) Serial.println("fail"); 
	else Serial.println("success");

	Serial.print("ThingSpeak Connection Results:");
	if (Wifi.configMqtt(CLIENTLID, USERNAME, PASSWORD, MQTT_HOST, SERVER_PORT) == 0) Serial.println("fail"); 
	else Serial.println("success");

	Wifi.setPublishTopic(PUBLISHTOPIC);
	Wifi.setSubscribetopic(SUBSCRIBERTOPIC3);
	Wifi.setSubscribetopic(SUBSCRIBERTOPIC2);
	Wifi.setSubscribetopic(SUBSCRIBERTOPIC1);
	topic = PUBLISHTOPIC;

	lastUploadMs = millis();
	
}

void loop() {
	unsigned long now = millis();

	// ===== 讀取氣體濃度 =====
	lpg = mq2.readLPG();
	int mq2Value = analogRead(Analog_Input);

	// ===== 讀取陀螺儀/加速度/溫度 =====
	Mpu.getEvent();                 // 先更新感測值
	float gyroTempC = Mpu.temperature;

	// 加速度重力向量
	float ax = Mpu.accX;
	float ay = Mpu.accY;
	float az = Mpu.accZ;

	// 若單位為 m/s^2，請轉換為 g（此範例預設已是 g）
	// const float G = 9.81f; ax/=G; ay/=G; az/=G;

	// 向量長度（g）
	float amag = sqrt(ax*ax + ay*ay + az*az);
	if (amag < 1e-6f) amag = 1e-6f; // 避免除零

	// 傾斜角（Z 軸朝上假設）
	float tiltDeg = acos(constrain(az / amag, -1.0f, 1.0f)) * 180.0f / PI;

	// 傾斜狀態：0 正常、1 預警（≥15°）、2 倒伏（≥30°）
	int tiltStatus = 0;
	if (tiltDeg >= kTiltFallDeg)       tiltStatus = 2;
	else if (tiltDeg >= kTiltWarnDeg)  tiltStatus = 1;

	// 劇烈震動狀態：0 正常、1 劇烈（|a|-1g > 0.5g）、2 衝擊峰值（|a| > 1.5g）
	bool vibStrong = fabs(amag - 1.0f) > kVibStrongDeltaG;
	bool vibPeak   = amag > kPeakShockG;
	int vibrationStatus = vibPeak ? 2 : (vibStrong ? 1 : 0);

	// ===== 讀取 BMH06203 物體溫度 =====
	float objTempC = mytherm.readTemperature(OBJ_TEMP);

	// ===== 氣體超標處理（不再直接控制蜂鳴器）=====
	if (mq2Value > 300) {
		digitalWrite(solenoidPin, HIGH);
		digitalWrite(INA, HIGH);
		digitalWrite(INB, LOW);
	} else {
		digitalWrite(solenoidPin, LOW);
		digitalWrite(INA, LOW);
		digitalWrite(INB, LOW);
	}

	// ===== 蜂鳴器：根據傾斜/震動/氣體/溫度狀態發聲 =====
	int tiltVibSeverity = 0; // 0 正常、1 輕微、2 嚴重
	if (tiltStatus == 2 || vibrationStatus == 2) {
		tiltVibSeverity = 2;
	} else if (tiltStatus == 1 || vibrationStatus == 1) {
		tiltVibSeverity = 1;
	}

	// 氣體超標或溫度超標時強制快節奏警報
	if (mq2Value > 300 || objTempC >= kTempWarnC) tiltVibSeverity = 2;

	if (tiltVibSeverity == 0) {
		if (buzzerIsOn) { noTone(buzzerPin); buzzerIsOn = false; }
	} else {
		const int freq = (tiltVibSeverity == 2) ? kAlarmFreqHz : kWarnFreqHz;
		const unsigned long onMs  = (tiltVibSeverity == 2) ? kAlarmOnMs  : kWarnOnMs;
		const unsigned long offMs = (tiltVibSeverity == 2) ? kAlarmOffMs : kWarnOffMs;

		unsigned long dwell = buzzerIsOn ? onMs : offMs;
		if (now - buzzerLastToggleMs >= dwell) {
			buzzerIsOn = !buzzerIsOn;
			buzzerLastToggleMs = now;
			if (buzzerIsOn) tone(buzzerPin, freq);
			else            noTone(buzzerPin);
		}
	}

	// ===== LCD：固定數字輸出（不閃爍）=====
	if (now - lastLcdNumUpdateMs >= lcdNumUpdateIntervalMs) {
		char numBuf[6];
		snprintf(numBuf, sizeof(numBuf), "%4d", mq2Value);
		lcd.setCursor(5, 0);
		lcd.print(numBuf);
		lastLcdNumUpdateMs = now;

		// 顯示物體溫度至第2列（簡短狀態），避免覆蓋上傳回饋時機
		lcd.setCursor(0, 1);
		lcd.print("T:");
		lcd.print(objTempC, 1);
		lcd.print((char)223); // 度數符號
		lcd.print("C        ");
	}

	// ===== 每 15 秒上傳一次資料 =====
	if (now - lastUploadMs >= uploadIntervalMs) {
		// field1: MQ2、field2: 物體溫度(℃)、field3: 傾斜狀態(0/1/2)、field4: 劇烈震動狀態(0/1/2)
		String data = "field1=" + String(mq2Value) +
		              "&field2=" + String(objTempC, 2) +
		              "&field3=" + String(tiltStatus) +
		              "&field4=" + String(vibrationStatus);

		bool uploadOk = Wifi.writeString(data, topic);

		lcd.setCursor(0, 1);
		if (uploadOk) lcd.print("Upload success  ");
		else          lcd.print("Upload failed   ");

		Serial.print("MQ2="); Serial.print(mq2Value);
		Serial.print(", ObjTempC="); Serial.print(objTempC, 2);
		Serial.print(", GyroTempC="); Serial.print(gyroTempC, 2);
		Serial.print(", TiltDeg="); Serial.print(tiltDeg, 1);
		Serial.print(", TiltStatus="); Serial.print(tiltStatus);
		Serial.print(", |a|="); Serial.print(amag, 2);
		Serial.print(", VibStatus="); Serial.println(vibrationStatus);

		lastUploadMs = now;
	}

	delay(800);
}