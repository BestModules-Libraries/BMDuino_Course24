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

#http://localhost:8088/fcu/dhtdata/dht2jsonwithdate.php?MAC=246F289E48D4&start=20210101&end=20211231
url1="http://localhost:8088/fcu/dhtdata/dht2jsonwithdate.php?MAC=%s&start=%s&end=%s"
url = url1 % (mac,dt1,dt2)

print(mac)
print("-----------------------")
print(dt1,"~",dt2)
print("-----------------------")
print(url1)
print("-----------------------")
print(url)
print("========================")
