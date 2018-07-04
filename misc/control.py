from socket import *
s=socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_L2CAP)
t=socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_L2CAP)
s.bind(("B8:27:EB:5D:6B:60", 17))
s.listen(1)
c,a=s.accept()
t.connect(("6c:40:08:a5:02:5d", 17))
while True:
     c.send(t.recv(1000))
     t.send(c.recv(1000))
