import pymysql as DB
# 將 pymysql 套件匯入並更名為 DB，以便後續使用

# 資料庫連線設定
# 使用 DB.connect() 方法建立與資料庫的連線
# 參數包括主機名、連接埠、使用者名稱、密碼、資料庫名稱及字符集
db = DB.connect(host='localhost', port=3306, user='big', passwd='12345678', db='big', charset='utf8')

# 建立操作游標
# 使用 db.cursor() 方法獲取一個操作游標，用於執行 SQL 語句
cursor = db.cursor()

# 定義 SQL 語法（插入資料）
# 將資料插入到 dhtdata 資料表中
sql1 = "INSERT INTO dhtdata (id, MAC, crtdatetime, temperature, humidity, systime) VALUES (NULL, 'a01110001', current_timestamp(), '25.3', '88.9', '20220624220101');"

# 執行 SQL 語法
# 使用 cursor.execute() 方法執行定義的 SQL 語句
cursor.execute(sql1)

# 選取第一筆結果
# 使用 cursor.fetchone() 方法獲取查詢結果的第一筆資料
data = cursor.fetchone()

# 打印資料庫版本資訊
print("Database version : %s " % data)

# 關閉連線
# 使用 db.close() 方法關閉與資料庫的連線
db.close()
