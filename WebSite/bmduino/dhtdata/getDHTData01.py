import sys	#作業系統套件，用於檔案、目錄資料使用
import requests	#建立雲端WinSocket連線的套件
import json	#了解json內容的json物件的套件
import time	#系統時間套件
import datetime	#時鐘物件
import math	#數學運算套件
import os	# #作業系統套件，用於檔案、目錄資料使用
import http.client	#winsock連線物件之討建
import unicodedata	#Unicode
from pathlib import Path	#存取檔案路徑之套件

from requests.exceptions import HTTPError #執行http POST/GET 等發生錯誤處理之套件
#http://localhost:8888/bigdata/dhtdata/dht2jsonwithdate2.php?MAC=E89F6DE8F3BC&start=20200101&end=20221231
url1 = "http://localhost::8888/bigdata/dhtdata/dht2jsonwithdate2.php?MAC=%s&start=%s&end=%s"
#dt = input("請您輸入查詢日期(當年月最後日):")
mac = input("請您輸入查詢裝置網路卡編號(MAC Address):")
dt1 = input("請您輸入查詢開始日期(YYYYMMDD):")
dt2 = input("請您輸入查詢結束日期(YYYYMMDD):")
url = url1 % (mac,dt1,dt2)  #將輸入資料：MAC，DT1、DT2與URL1(php網址)整合再一起，組成要求資料的Restful API
 #將輸入資料：MAC，DT1、DT2與URL1(php網址)整合再一起，組成要求資料的Restful API
print(url)
#MAC=E89F6DE8F3BC
#start=20210101
#end=20211231
try:
    res = requests.get(url)     #使用http Get將http Get 之資料擷取之連線URL 傳入連線物件，進行連線
    res.raise_for_status()      #使用http Get將http Get 狀態讀回
    table=json.loads(res.content.decode('utf-8'))    
    print(json.dumps(table, sort_keys=True, indent=4)) 
    #用json beauty 方式顯示
    print('Success!')
except HTTPError as http_err:
    print('HTTP error occurred: {http_err}')
    sys.exit(0)
except Exception as err:
    print('Other error occurred: {err}')
    sys.exit(0)



    
