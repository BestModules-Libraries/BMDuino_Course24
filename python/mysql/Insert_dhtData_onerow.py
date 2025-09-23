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

sqlstr1 = """INSERT INTO dhtdata (MAC, IP, temperature, humidity, systime) VALUES ('AABBCCDDEEFF', '192.168.99.100',  24.56, 56.88, '20241204064812'); """  # 新增 dhtdata 一筆資料的語法
sqlstr2 = """SELECT * FROM dhtdata WHERE 1 order by systime DESC limit 1; """  #查詢 dhtdata 的語法

# 執行 執行插入資料的語法 SQL 語法
cursor.execute(sqlstr1)  # 執行插入資料的語法

# 執行 執行查詢剛剛插入資料的語法 SQL 語法
cursor.execute(sqlstr2)  # 執行插入資料的語法

#選取dhtdata資料表的語法
# 獲取欄位名稱（字段名）
column_names = [desc[0] for desc in cursor.description] #取得查詢資料表之欄位標題

retdata = cursor.fetchall()	#取得查詢結果資料集

#  打印结果，有抬頭與顯示要的資料
# 顯示結果為字典
#print(column_names) # 列印資料表之欄位標題
print("運用字典形式列出查詢結果")
print(column_names[6].upper(),column_names[1].upper(),column_names[4].upper(),column_names[5].upper()) # 列印資料表之欄位標題
for row in retdata:
    row_dict = dict(zip(column_names, row))
    print(row_dict["systime"],row_dict["MAC"],row_dict["temperature"],row_dict["humidity"])
# # 關閉連線
db.close()
