import socket

def handle_client(connection, client_address):
    print(f"New connection from {client_address}")
    with connection:
        data = connection.recv(1024)
        if data:
            print(f"$$ Received from {client_address}")
            print(data.decode())
            content = "{\"status\":\"ok\"}"
            response = (
                "HTTP/1.1 200 OK\r\n"
                "Content-Type: application/json\r\n"
                "Connection: closed\r\n"
                f"Content-Length: {len(content)}\r\n"
                "\r\n"
                + content
            )
            connection.send(response.encode())
        else:
            print(f"Connection closed by {client_address}")

with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as server_socket: # TCP
    server_socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    server_address = ('localhost', 6789)
    server_socket.bind(server_address)
    server_socket.listen(5)
    print(f"Server listening on {server_address[0]}:{server_address[1]}")

    connection, client_address = server_socket.accept()
    handle_client(connection, client_address)
