import sys  #系統用的lib
import requests #HTTP GET元件
import json     #解譯json元件
import time     ##時間用的lib
import datetime #時間用的lib
from datetime import datetime       #轉unix time to date time 用的lib
import math     #數學用的lib
import os      #作業系統元件
import http.client  #網頁連線物件lib
import unicodedata  #解unicode用的lib
from pathlib import Path   #解路徑用的lib

from requests.exceptions import HTTPError       #連線錯誤的lib

mac = input("請輸入您要查詢裝置的網卡編號(MAC):")
dt1 = input("請您輸入查詢起始日期(當年月最後日):")
dt2 = input("請您輸入查詢結束日期(當年月最後日):")


#http://ncnu.arduino.org.tw:9999/dhtdata/dht2json.php?MAC=CC50E3B5BB20&start=20210101&end=20211231
url1="http://ncnu.arduino.org.tw:9999/dhtdata/dht2json.php?MAC=%s&start=%s&end=%s"
url = url1 % (mac,dt1,dt2)

#MAC = CC50E3B5BB20
#start = 20220101
#end = 20220430

print(mac)
print("-----------------------")
print(dt1,"~",dt2)
print("-----------------------")
print(url1)
print("-----------------------")
print(url)
print("========================")

try:
	res = requests.get(url)     #使用http Get將http Get 之資料擷取之連線URL 傳入連線物件，進行連線
	res.raise_for_status()      #使用http Get將http Get 狀態讀回
except HTTPError as http_err:
	print('HTTP error occurred: {http_err}')
	sys.exit(0)
except Exception as err:
	print('Other error occurred: {err}')
	sys.exit(0)
else:
	print('Success!')
	table=json.loads(res.content.decode('utf-8'))       
	#//把讀回網站的json資料，先轉成unicode編碼(編碼(utf-8))
	#把編碼後的json資料讀入json物件，並命名為table 
#print(table)
print("----------------------")
#print(json.dumps(table, sort_keys=True, indent=4))
s01 = table['Device']   #json的device的MAc
s02 = table['Temperature'] #抓Temperature的ARRAY裡面
s03 = table['Humidity'] #抓Humidity的ARRAY裡面
i = 0 
for x in s02:
    d01 = x 
    d02 = s03[i]
    i = i +1
    print(s01,d01,d02)
i = 0 	
for x in s03:
    d01 = x 
    d02 = s02[i]
    i = i +1
    print(s01,d01,d02)
