import socket

def handle_client(connection, client_address):
    print(f"New connection from {client_address}")
    try:
        while True:
            data = connection.recv(1024)
            if data:
                print(f"Received from {client_address}: {data.decode()}")
            else:
                print(f"Connection closed by {client_address}")
                break
    finally:
        connection.close()

server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
server_address = ('localhost', 6789)
server_socket.bind(server_address)
server_socket.listen(5)
print(f"Server listening on {server_address[0]}:{server_address[1]}")

while True:
    connection, client_address = server_socket.accept()
    handle_client(connection, client_address)
