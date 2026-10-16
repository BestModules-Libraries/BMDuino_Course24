/*! 
 * voice command list  
 * 語音指令列表檔案（通常由語音模組韌體或語音編碼器所對應）
 * 本檔案主要定義語音播放模組中，每一段語音對應的識別碼 (ID)。
 * 
 * 在實際使用時，開發者只需要呼叫：
 *     SayString(VOC_33);
 * 語音模組即可播放聲音編號 0x20 的音檔。
 *
 * 語音檔通常由語音工具（例如 BMduino Voice Widget）寫入語音模組中，
 * 每段語音都會有一個各自的索引值，這裡就是將這些索引值定義成常數。
 */

#ifndef _VOICE_CMD_LIST_H  // 防止重複引用（Include Guard）
#define _VOICE_CMD_LIST_H


/*  
 * ------------------ 語音代碼定義區 ------------------
 * 每一個 VOC_xx 對應語音模組中的一段語音。
 * 括號內的 hex（例如 0x00）是實際傳送給語音 IC 的語音 ID。
 *
 * 這些語音可藉由 BMduino 語音工具 (Voice Widget) 自行修改或替換，
 * 並下載至語音播放擴充板（如 BMV31T001）。
 *
 * 開發者在程式中只需要呼叫：
 *     SayString(VOC_35);
 * 語音模組便會播放與 0x22 對應的語音。
 */

#define VOC_1          (0x00)   // 語音編號 1
#define VOC_2          (0x01)   // 語音編號 2
#define VOC_3          (0x02)   // 語音編號 3
#define VOC_4          (0x03)   // 語音編號 4
#define VOC_5          (0x04)   // 語音編號 5
#define VOC_6          (0x05)   // 語音編號 6
#define VOC_7          (0x06)   // 語音編號 7
#define VOC_8          (0x07)   // 語音編號 8
#define VOC_9          (0x08)   // 語音編號 9
#define VOC_10         (0x09)   // 語音編號 10
#define VOC_11         (0x0A)   // 語音編號 11
#define VOC_12         (0x0B)   // 語音編號 12
#define VOC_13         (0x0C)   // 語音編號 13
#define VOC_14         (0x0D)   // 語音編號 14
#define VOC_15         (0x0E)   // 語音編號 15
#define VOC_16         (0x0F)   // 語音編號 16
#define VOC_17         (0x10)   // 語音編號 17
#define VOC_18         (0x11)   // 語音編號 18
#define VOC_19         (0x12)   // 語音編號 19
#define VOC_20         (0x13)   // 語音編號 20
#define VOC_21         (0x14)   // 語音編號 21
#define VOC_22         (0x15)   // 語音編號 22
#define VOC_23         (0x16)   // 語音編號 23
#define VOC_24         (0x17)   // 語音編號 24
#define VOC_25         (0x18)   // 語音編號 25
#define VOC_26         (0x19)   // 語音編號 26
#define VOC_27         (0x1A)   // 語音編號 27
#define VOC_28         (0x1B)   // 語音編號 28
#define VOC_29         (0x1C)   // 語音編號 29
#define VOC_30         (0x1D)   // 語音編號 30
#define VOC_31         (0x1E)   // 語音編號 31
#define VOC_32         (0x1F)   // 語音編號 32
#define VOC_33         (0x20)   // 語音編號 33 → 常用於「警告：偵測到異常」
#define VOC_34         (0x21)   // 語音編號 34 → 常用於「請盡速檢查」類的尾語
#define VOC_35         (0x22)   // 語音編號 35 → 前門入侵警示
#define VOC_36         (0x23)   // 語音編號 36 → 客廳入侵警示
#define VOC_37         (0x24)   // 語音編號 37 → 倉庫入侵警示
#define VOC_38         (0x25)   // 語音編號 38 → 臥室入侵警示


/* sentence */
/*
 * 若將來要加入「多語句語音組合」，
 * 可在這裡新增如：
 * #define SENTENCE_WARNING   { VOC_33, VOC_34 }
 * 用於一次播放兩段語音。
 */

#endif  // _VOICE_CMD_LIST_H
