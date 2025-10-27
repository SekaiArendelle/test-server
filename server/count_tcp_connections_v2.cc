#include <atomic>
#include <cstddef>
#include <cstdint>
#include <string>
#include <thread>
#include <mutex>
#include <queue>
#include <unistd.h>
#include <arpa/inet.h>

namespace test_server {

class Server {
    static constexpr ::std::uint16_t port{6699};

    int socket_fd;
    ::sockaddr_in socket_address;

public:
    Server() {
        // Create socket
        this->socket_fd = socket(AF_INET, SOCK_STREAM, 0);
        if (socket_fd < 0) {
            __builtin_trap();
        }

        // reuse addr
        int option = 1;
        if (::setsockopt(this->socket_fd, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option)) < 0) {
            __builtin_trap();
        }

        // bind socket to port
        socket_address.sin_family = AF_INET;
        socket_address.sin_addr.s_addr = ::htonl(0x7f'00'00'01); // 127.0.0.1
        socket_address.sin_port = ::htons(port);
        if (::bind(this->socket_fd, reinterpret_cast<::sockaddr*>(__builtin_addressof(socket_address)),
                   sizeof(socket_address)) < 0) {
            __builtin_trap();
        }

        // listen to port
        if (::listen(this->socket_fd, /* backlog= */ 5) < 0) {
            __builtin_trap();
        }
    }

    ~Server() {
        ::close(this->socket_fd);
    }

    int accept(this auto&& self) {
        socklen_t socket_address_size{sizeof(self.socket_address)};
        int result_fd{::accept(self.socket_fd, reinterpret_cast<::sockaddr*>(__builtin_addressof(self.socket_address)),
                               __builtin_addressof(socket_address_size))};
        if (result_fd < 0) {
            __builtin_trap();
        }
        return result_fd;
    }
};

} // namespace test_server

namespace test_server {

template <typename T>
class ThreadSafeQueue {
public:
    ::std::queue<T> queue{};
    ::std::mutex mutex{};
    ::std::condition_variable has_new_element{};

    void push(this auto&& self, auto&& value) {
        ::std::lock_guard lock{self.mutex};
        self.queue.push(::std::forward<decltype(value)>(value));
        self.has_new_element.notify_one();
    }

    T pop(this auto&& self) {
        ::std::unique_lock<::std::mutex> lock{self.mutex};
        self.has_new_element.wait(lock, [&self] () {
            return !self.queue.empty();
        });
        T result = self.queue.front();
        self.queue.pop();
        return result;
    }

    ::std::size_t size(this auto const& self) {
        ::std::lock_guard lock{self.mutex};
        return self.queue.size();
    }
};

}

int main() {
    ::test_server::Server server{};
    ::std::queue<int> accepted_sockets{};
    ::std::mutex acceptor_sender_mutex{};
    ::std::jthread acceptor([&server, &accepted_sockets]() {
        while (true) {
            int new_socket = server.accept();
            accepted_sockets.push(new_socket);
        }
    });
    while (true) {
        int new_socket = server.accept();
        // ::send(new_socket, count.c_str(), count.size(), 0);
        ::close(new_socket);
    }
}
