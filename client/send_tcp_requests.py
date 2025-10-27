# tcp_request.py
import socket

host = 'localhost'
port = 6699

def tcp_request():
    # 1. 创建 IPv4 / TCP 套接字
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    sock.settimeout(5)          # 超时时间，防止永久阻塞
    try:
        # 2. TCP 三次握手、建立连接
        sock.connect((host, port))
        # 4. 接收应答
        result = sock.recv(1024)   # 一次最多读 1 KB
        # print(result.decode())
    finally:
        # 5. 关闭连接（四次挥手）
        sock.close()

if __name__ == '__main__':
    while True:
        tcp_request()
