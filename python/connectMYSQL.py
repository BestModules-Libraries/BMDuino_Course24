#   資料庫連線測試    connectMYSQL.py
import pymysql as DB
# 將 pymysql 模組引入並更名為 DB，以便後續操作資料庫

# 資料庫連線設定
db = DB.connect(
    host='localhost',  # 資料庫伺服器地址
    port=3306,  # 資料庫伺服器通訊埠
    user='big',  # 資料庫使用者名稱
    passwd='12345678',  # 資料庫使用者密碼
    db='big',  # 要連接的資料庫名稱
    charset='utf8'  # 資料庫編碼
)

# 建立操作游標
cursor = db.cursor()

# SQL 語法（查詢資料庫版本）
sql = 'SELECT VERSION()'

# 執行 SQL 語法
cursor.execute(sql)

# 選取第一筆結果
data = cursor.fetchone()

# 打印資料庫版本
print("Database version : %s " % data)

# 關閉連線
db.close()
