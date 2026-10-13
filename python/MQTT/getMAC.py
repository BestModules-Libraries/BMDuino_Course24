import socket
import uuid

macaddr = uuid.UUID(int = uuid.getnode()).hex[-12:]
print(macaddr)

macaddr_list = []
for i in range(0,11,2):
    print(macaddr[i:i+2])
    macaddr_list.append(macaddr[i:i+2])
print(macaddr_list)
print(type(macaddr_list))

macaddr2 = ':'.join(macaddr_list)
print(macaddr2)