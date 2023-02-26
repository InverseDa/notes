import socket

udp_server = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
udp_server.bind(('', 8866))  # 绑定端口，接受这个端口的消息
#端口自己决定，第一个参数为空默认代表抓取本机数据
print("server is ready")
while True:
    data = udp_server.recvfrom(1024)
#因为
    clientAddr = data[1]

    clientIp = clientAddr[0]
    clientPort = clientAddr[1]

    clientData = data[0]
    clientData = str(clientData)
    clientData = clientData.replace("'","")
    clientData = clientData.replace(clientData[0],"")
    print(clientData)
udp_server.close()