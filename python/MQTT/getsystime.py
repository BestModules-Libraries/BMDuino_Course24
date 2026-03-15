
from datetime import datetime
import string
import socket

def strzero(no, ll):
    tmp = str(no)
    if len(tmp) >= ll:
        return tmp
    else:
        return tmp.rjust(ll-len(tmp)+1,'0')
#    return tmpp

def getsystime():
    now = datetime.now()
    return strzero(now.year,4)+strzero(now.month,2)+strzero(now.day,2)+strzero(now.hour,2)+strzero(now.minute,2)+strzero(now.second,2)

def get_local_ip():
    try:
        # 创建一个socket对象
        s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        # 利用UDP协议的特性，尝试发送无需实际传输的数据
        s.connect(('10.255.255.255', 1))
        local_ip = s.getsockname()[0]
    except Exception as e:
        local_ip = '127.0.0.1'
    finally:
        s.close()
    return local_ip

now = datetime.now()
print(now)
print(getsystime())
# print("YYYY:"+strzero(now.year,4))
# print("MM:"+strzero(now.month,2))
# print("DD:"+strzero(now.day,2))
# print("hh:"+strzero(now.hour,2))
# print("mm:"+strzero(now.minute,2))
# print("ss:"+strzero(now.second,2))
print(get_local_ip())
print(type(get_local_ip()))

