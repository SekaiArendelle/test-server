#include <cstdio>
#include <cstdlib>
#include <cstdint>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>

constexpr ::std::uint16_t port{6699};

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    const char *hello = "Hello from server";

    // Create socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) <= 0) {
        __builtin_trap();
    }

    // reuse addr
    int option = 1;
    int ret = setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option));
    if (ret < 0) {
        perror("setsockopt failed");
        exit(EXIT_FAILURE);
    }

    // bind socket to port
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        __builtin_trap();
    }

    // listen to port
    if (listen(server_fd, /* backlog= */ 5) < 0) {
        __builtin_trap();
    }

    // Server is listening on port

    // Accept connection from client
    if ((new_socket = accept(server_fd, reinterpret_cast<struct sockaddr *>(__builtin_addressof(address)), (socklen_t*)&addrlen)) < 0) {
        __builtin_trap();
    }

    // Client connected

    // Send message to client
    send(new_socket, hello, strlen(hello), 0);

    // Close connection
    close(new_socket);
    close(server_fd);

    return 0;
}
