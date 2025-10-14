import sys  #系統用的lib
import requests #HTTP GET元件
import json     #解易json元件
import time     ##時間用的lib
import datetime #時間用的lib
from datetime import datetime       #轉unix time to date time 用的lib
import math     #數學用的lib
import os      #作業系統元件
import http.client  #網頁連線物件lib
import unicodedata  #解unicode用的lib
from pathlib import Path   #解路徑用的lib

from requests.exceptions import HTTPError       #連線錯誤的lib
myschoolid = 'brucetsao'
str1=""
str2=""
mykey = 'b0c21681ec5364b76facc9115f45d0dc'
url='http://ncnu.arduino.org.tw:9999/opendata/openweather.php?MAC=%s&f01=%s&f02=%s&f03=%s&f04=%s&f05=%s&f06=%s&f07=%s&f08=%f&f09=%f&f10=%f&f11=%f&f12=%f&f13=%f&f14=%f'
#sqlstr = 'INSERT INTO OW_curent (MAC ,dataorder, sysdatatime, timezone, siteid, sitename, country, lon, lat, temp, temp_min, temp_max, pressure, humidity, windspeed, winddeg) VALUES (NULL, 'SQL', CURRENT_TIMESTAMP, '20210513153301', '2021-05-13 16:00:00', '28800', '1670310', 'Puli Town', 'TW', '120.9695', '23.9664', '28.37', '29.82', '30.15', '1020', '88', '1.2', '260');'
sqlstr = 'INSERT INTO OW_curent (MAC ,dataorder, sysdatatime, timezone, siteid, sitename, country, lon, lat, temp, temp_min, temp_max, pressure, humidity,  windspeed, winddeg) VALUES  ("SQL", "%s", "%s", "%s", "%s", "%s", "%s", "%s", "%s", "%f", "%f", "%f", "%d",  "%f", "%f", "%f");'
# use 中央氣象局 API key的網址
#  http://api.openweathermap.org/data/2.5/weather?q=puli&appid=b0c21681ec5364b76facc9115f45d0dc'
mycity = ["Jaipur","Surat","Wau","Dakhla","Faaa","Hilo","Tofol"]
for x in mycity:

    url = 'http://api.openweathermap.org/data/2.5/weather?q=%s&appid=%s' % (x,mykey)
    # 這個替換自己的OpenData API Key
    print(url)
    try:
        res = requests.get(url)
        res.raise_for_status()
    except HTTPError as http_err:
        print('HTTP error occurred: {http_err}')
        sys.exit(0)
    except Exception as err:
        print('Other error occurred: {err}')
        sys.exit(0)
    else:
        print('Success!')
        table=json.loads(res.content.decode('utf-8'))


    s01 = table['id']
    s02 = table['name']
    s03a = table['dt']
    s03b = int(s03a)
    s03 = datetime.utcfromtimestamp(s03b).strftime('%Y-%m-%d %H:%M:%S')
    s04 = table['timezone']
    s05 = table['coord']['lat']
    s06 = table['coord']['lon']
    s07 = table['sys']['country']
    s08 = table['wind']['deg']
    s09 = table['wind']['speed']
    s10a = table['main']['temp']
    s10 = int(s10a) - 273
    s11a = table['main']['temp_min']
    s11 = int(s11a) - 273
    s12a = table['main']['temp_max']
    s12 = int(s12a) - 273
    s13 = table['main']['pressure']

    s14 = table['main']['humidity']
    s15 = table['wind']['speed']
    s16 = table['wind']['deg']


    print(s01,s02,s03,s04,s05,s06,s07,s08,s09,s10,s11)
#please modify url as http://ncnu.arduino.org.tw:9999/opendata/openweather.php?MAC=Brucetsao&f01=1620265182&f02=28800&f03=1670310&f04=Puli&f05=TW&f06=120.9695&f07=23.9664&f08=29.39&f09=29.20&f10=30.12&f11=1016&f12=76&f13=0.51&f14=10
    senddata= url='http://ncnu.arduino.org.tw:9999/opendata/openweather.php?MAC=%s&f01=%s&f02=%s&f03=%s&f04=%s&f05=%s&f06=%s&f07=%s&f08=%f&f09=%f&f10=%f&f11=%f&f12=%f&f13=%f&f14=%f' % (myschoolid,s03,s04,s01,s02,s07,s06,s05,s10,s11,s12,s13,s14,s15,s16)
    print(senddata)
    try:
        res = requests.get(senddata)
        res.raise_for_status()
    except HTTPError as http_err:
        print(f'HTTP error occurred: {http_err}')
        sys.exit(0)
    except Exception as err:
        print(f'Other error occurred: {err}')
        sys.exit(0)
    else:
        print('Success!')