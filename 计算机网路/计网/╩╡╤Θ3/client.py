import socket
udp_client = socket.socket(socket.AF_INET,socket.SOCK_DGRAM)
while True:
	data = input("please input the data: ")
	data = str(data)
	udp_client.sendto(data.encode('utf-8'),('xxx.xxx.xxx.xxx',8866))
#上面需要绑定接收端的IP地址和端口
udp_client.close()