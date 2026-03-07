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

#http://ncnu.arduino.org.tw:9999/dhtdata/dht2jsonwithdaylist.php?MAC=CC50E3B5BB20&start=20220101&end=20221231
#可以用下面SQL語法，進行擷取筆數
# select  MAC, count(*) as cnt from dhtdata  where 1 group by MAC  order by MAC

url1 = "http://ncnu.arduino.org.tw:9999/dhtdata/dht2jsonwithdaylist.php?MAC=%s&start=%s&end=%s"
#dt = input("請您輸入查詢日期(當年月最後日):")
mac = input("請您輸入查詢裝置網路卡編號(MAC Address):")
dt1 = input("請您輸入查詢開始日期(YYYYMMDD):")
dt2 = input("請您輸入查詢結束日期(YYYYMMDD):")
url = url1 % (mac,dt1,dt2)  #將輸入資料：MAC，DT1、DT2與URL1(php網址)整合再一起，組成要求資料的Restful API
 #將輸入資料：MAC，DT1、DT2與URL1(php網址)整合再一起，組成要求資料的Restful API
print(url)
#MAC=CC50E3B5BB20

#start=20220101
#end=20220130

#20210429025837
#20210429031100
try:
    res = requests.get(url)     #使用http Get將http Get 之資料擷取之連線URL 傳入連線物件，進行連線
    #用chrome browser在網址列，輸入：http://ncnu.arduino.org.tw:9999/dhtdata/dht2jsonwithdaylist.php?MAC=CC50E3B5BB20&start=20220101&end=20221231
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
    #print(json.dumps(table, sort_keys=True, indent=4)) 
    #用json beauty 方式顯示
    
s01 = table['Device']  
s02 = table['Datalist'] # get datalist array]
oldyear = ""
oldmmdd = ""
#---------------------
labelmark=[]
data=[]
tmp=[]
for x in s02:
    d01 = x[ "Datetime"]
    d02 = x["Temperature"]
    d03 = x["Humidity"]
    #print(d01,d02,d03)
    newyear = "%s" % (d01[0:4])
    mmdd = d01[4:8]
    if (newyear != oldyear):
        xtmp = "%s\n%s" % (d01[0:4],d01[4:8])
        oldyear = newyear
        #oldhour = newhour
    else:
        if (mmdd != oldmmdd):
            xtmp = "%s\n%s" % (d01[4:6],d01[6:8])
            oldmmdd = mmdd 
        else:
            xtmp = "%s\n%s" % (d01[4:6],d01[6:8])
                 
    #print(xtmp)
    #print("------",xtmp)
    labelmark.append(xtmp)
    tmp=[]
    for dd in d02:  #解析temperature array
        tmp.append(dd)  #將每一個temperature array 的元素，存到tmp陣列中
    data.append(tmp)    #把解析temperature array 的ｔｍｐ　ａｒｒａｙ存入　ｄａｔａ　ａｒｒａｙ
    
print(labelmark)
#['2021\n1210', '12\n13', '12\n14', '12\n15', '12\n16', '12\n17', '12\n18', '12\n19', '12\n20', '12\n21', '12\n22', '2022\n0102', '01\n03', '01\n04', '01\n05', '01\n06', '01\n07', '01\n08', '01\n09', '01\n10', '01\n11', '01\n12', '01\n13', '01\n14', '01\n15', '01\n16', '01\n17', '01\n18', '01\n19', '01\n20', '01\n21', '01\n22', '01\n23', '01\n24', '01\n25', '01\n26', '02\n07', '02\n08']
#print(data)

my_mark = dict(markeredgecolor='g',markerfacecolor='g',marker='*')
plt.boxplot(data,labels=labelmark,flierprops=my_mark)
plt.title("裝置：%s 的溫度圖" % s01 ,fontsize=16,color='b')
plt.show() 