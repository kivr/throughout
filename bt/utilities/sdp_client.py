from socket import *
import struct

ss = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_L2CAP)
#ss.connect(("00:1F:20:3D:28:C1",1))
ss.connect(("b8:8a:60:6a:68:d6",1))
ss.send(b'\x06\x00\x00\x00\x0f5\x03\x19\x01\x00\x03\xf85\x05\n\x00\x00\xff\xff\x00')

input = ss.recv(1024)

index = 0

while input[-1] != 0:
    index = index + 1
    ss.send(b'\x06' + struct.pack('>H', index) + b'\x00\x175\x03\x19\x01\x00\x03\xf85\x05\n\x00\x00\xff\xff\x08' + input[-8:])
    input = ss.recv(100)
