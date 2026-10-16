
from datetime import datetime
import string
import socket
import uuid


def strzero(no, ll):
    tmp = str(no)
    if len(tmp) >= ll:
        return tmp
    else:
        return tmp.rjust(ll-len(tmp)+1,'0')


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

def get_mac_address():
    mac=uuid.UUID(int = uuid.getnode()).hex[-12:]
    return "".join([mac[e:e+2] for e in range(0,11,2)]).upper()

def get_mac_address2():
    mac=uuid.UUID(int = uuid.getnode()).hex[-12:]
    return ":".join([mac[e:e+2] for e in range(0,11,2)]).upper()
