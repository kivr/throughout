from socket import *
s=socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_L2CAP)
t=socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_L2CAP)
s.bind(("B8:27:EB:5D:6B:60", 1))
s.listen(1)
c,a=s.accept()
t.connect(("1C:36:BB:0D:E0:4", 1))
while True:
     t.send(c.recv(1000))
     c.send(t.recv(1000))
