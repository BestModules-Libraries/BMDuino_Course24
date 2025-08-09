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

# SQL 語法（建立資料表）
sqlstr = """SELECT * FROM dhtdata WHERE 1 order by systime DESC;"""  #查詢 dhtdata 的語法

# 執行 SQL 語法
cursor.execute(sqlstr)  # 執行建立資料表的語法
result = cursor.execute(sqlstr)  # 執行選取dhtdata資料表的語法
print("SQL1 Result is  : %d " % result)

# 關閉連線
db.close()
