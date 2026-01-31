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
# sql0 = "drop table aaa;"  # 刪除資料表 aaa 的語法（目前被註解掉）
sql1 = """
CREATE TABLE dhtData (
  id int(11) NOT NULL COMMENT '主鍵',  # 主鍵 ID 欄位
  MAC varchar(12) NOT NULL COMMENT '裝置MAC值',  # 裝置 MAC 地址欄位
  crtdatetime timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP COMMENT '資料輸入時間',  # 資料輸入時間欄位，自動更新
  temperature float NOT NULL COMMENT '溫度',  # 溫度欄位
  humidity float NOT NULL COMMENT '濕度',  # 濕度欄位
  systime varchar(14) NOT NULL COMMENT '使用者更新時間'  # 使用者更新時間欄位
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
"""

sql2 = """
ALTER TABLE dhtData
  ADD PRIMARY KEY (id);  # 設定 id 欄位為主鍵
"""

sql3 = """
ALTER TABLE dhtData
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT COMMENT '主鍵';  # 設定 id 欄位為自動遞增的主鍵
"""

# 執行 SQL 語法
# cursor.execute(sql0)  # 執行刪除資料表的語法（目前被註解掉）
cursor.execute(sql1)  # 執行建立資料表的語法
cursor.execute(sql2)  # 執行添加主鍵的語法
cursor.execute(sql3)  # 執行修改主鍵為自動遞增的語法

# 選取第一筆結果
data = cursor.fetchone()

# 打印資料庫版本（實際上，由於我們沒有查詢版本資訊，這行不會打印出正確的版本資訊）
print("Database version : %s " % data)

# 關閉連線
db.close()
