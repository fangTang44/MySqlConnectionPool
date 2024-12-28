#include <iostream>
#include <string>
#include <cstring>
#include <thread>
#include <arpa/inet.h>
#include <unistd.h>
#include <netinet/in.h>

const int PORT = 8080;
const std::string SERVER_IP = "192.168.31.35";

void receive_messages(int client_socket, sockaddr_in server_addr) {
    char buffer[1024];
    socklen_t addr_len = sizeof(server_addr);

    while (true) {
        memset(buffer, 0, sizeof(buffer));
        int bytes_received = recvfrom(client_socket, buffer, sizeof(buffer), 0,
                                      (struct sockaddr*)&server_addr, &addr_len);
        // if (bytes_received <= 0) {
        //     std::cout << "与服务器断开连接。\n";
        //     break;
        // }

        std::string message = buffer;

        // 忽略空消息
        if (message.empty()) {
            continue;
        }

        std::cout << message << "\n";
    }
}

bool verify_server_connection(int client_socket, sockaddr_in server_addr) {
    const std::string test_message = "ping";
    sendto(client_socket, test_message.c_str(), test_message.size(), 0,
           (struct sockaddr*)&server_addr, sizeof(server_addr));

    char buffer[1024];
    memset(buffer, 0, sizeof(buffer));
    socklen_t addr_len = sizeof(server_addr);

    struct timeval timeout;
    timeout.tv_sec = 5; // 超时时间5秒
    timeout.tv_usec = 0;
    setsockopt(client_socket, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));

    int bytes_received = recvfrom(client_socket, buffer, sizeof(buffer), 0,
                                  (struct sockaddr*)&server_addr, &addr_len);

    // 验证是否收到 "pong"
    return bytes_received > 0 && std::string(buffer, bytes_received) == "pong";
}

int main() {
    int client_socket = socket(AF_INET, SOCK_DGRAM, 0);
    if (client_socket == -1) {
        perror("套接字创建失败");
        return -1;
    }

    sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    inet_pton(AF_INET, SERVER_IP.c_str(), &server_addr.sin_addr);

    // 验证与服务器的连接
    if (!verify_server_connection(client_socket, server_addr)) {
        std::cerr << "无法连接到服务器，请检查服务器状态。\n";
        close(client_socket);
        return -1;
    }

    std::cout << "成功连接到服务器。\n";

    // 获取用户名
    std::string username;
    std::cout << "请输入您的用户名: ";
    std::getline(std::cin, username);

    // 启动接收消息线程
    std::thread(receive_messages, client_socket, server_addr).detach();

    while (true) {
        std::string message;
        // std::cout << "请输入消息: ";
        std::getline(std::cin, message);
        if (message == "退出") {
            break;
        }

        // 在消息前加上用户名
        std::string message_with_username = username + ": " + message;

        sendto(client_socket, message_with_username.c_str(), message_with_username.size(), 0,
               (struct sockaddr*)&server_addr, sizeof(server_addr));
    }

    close(client_socket);
    return 0;
}
