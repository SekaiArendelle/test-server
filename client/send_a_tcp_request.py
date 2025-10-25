# tcp_request.py
import socket

HOST = 'localhost'   # 服务器地址
PORT = 6699              # 13 是 RFC-867 daytime 服务端口，也可换成你自己的端口
REQUEST = b''          # daytime 服务什么都不用发，直接读即可；换成别的服务可在这里放协议数据

def tcp_request(host, port, request):
    # 1. 创建 IPv4 / TCP 套接字
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    sock.settimeout(5)          # 超时时间，防止永久阻塞
    try:
        # 2. TCP 三次握手、建立连接
        sock.connect((host, port))
        # 3. 发送数据（如果协议需要）
        if request:
            sock.sendall(request)
        # 4. 接收应答
        response = sock.recv(4096)   # 一次最多读 4 KB
        return response
    finally:
        # 5. 关闭连接（四次挥手）
        sock.close()

if __name__ == '__main__':
    data = tcp_request(HOST, PORT, REQUEST)
    print('Server replied:', data.decode())