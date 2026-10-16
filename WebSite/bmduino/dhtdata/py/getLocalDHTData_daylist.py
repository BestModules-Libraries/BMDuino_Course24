import matplotlib.pyplot as plt
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

from requests.exceptions import HTTPError
plt.rcParams["font.family"] = ["Microsoft JhengHei"]
plt.rcParams["axes.unicode_minus"] = False

data_temp = []
data_humid = []
datalabel = []
#http://localhost:8088/bigdata/dhtdata/dht2jsonwithdaylist.php?MAC=CC50E3B5BB20&start=20220101&end=20220131
#可以用下面SQL語法，進行擷取筆數
# select  MAC, count(*) as cnt from dhtdata  where 1 group by MAC  order by MAC

url1 = "http://localhost:8088/bigdata/dhtdata/dht2jsonwithdaylist.php?MAC=%s&start=%s&end=%s"
#dt = input("請您輸入查詢日期(當年月最後日):")
mac = input("請您輸入查詢裝置網路卡編號(MAC Address):")
dt1 = input("請您輸入查詢開始日期(YYYYMMDD):")
dt2 = input("請您輸入查詢結束日期(YYYYMMDD):")
url = url1 % (mac,dt1,dt2)  #將輸入資料：MAC，DT1、DT2與URL1(php網址)整合再一起，組成要求資料的Restful API
 #將輸入資料：MAC，DT1、DT2與URL1(php網址)整合再一起，組成要求資料的Restful API
print(url)
#MAC=CC50E3B5BB20
#    CC50E3B5BB20
#start=20220101
#end=20220131

#20210429025837
#20210429031100
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
    print(json.dumps(table, sort_keys=True, indent=4)) 
    #用json beauty 方式顯示
    
s01 = table['Device']  
s02 = table['Datalist'] # get datalist array]

data_temp = []
data_humid = []
datalabel = []

for x in s02:
    d01 = x["Datetime"]
    d02 = x["Temperature"]
    d03 = x["Humidity"]
    #print(d01,d02,d03)
    data_t=[]
    for t in d02:
        data_t.append(t)
    data_h=[]
    for h in d03:
        data_h.append(h)
    data_t.sort
    data_h.sort    
    datalabel.append(d01)
    data_temp.append(data_t)
    data_humid.append(data_h)

plt.boxplot(data_temp,labels=datalabel)
plt.title("溫度",fontsize=16,color='b')
plt.show()     
#print(data_t)
#print(data_h)
        
