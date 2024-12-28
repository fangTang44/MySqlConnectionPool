#include <iostream>
#include <string>
#include <cstring>
#include <map>
#include <thread>
#include <mutex>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>

const int PORT = 8080;
std::map<std::string, sockaddr_in> client_map; // 用于存储已加入的客户端用户名和地址
std::mutex client_mutex;

// 从消息中提取用户名
std::string extract_username(const std::string& message) {
    size_t delimiter_pos = message.find(":");
    if (delimiter_pos != std::string::npos) {
        return message.substr(0, delimiter_pos);
    }
    return ""; // 如果没有找到冒号，返回空字符串
}

void send_message_to_all(int server_socket, const std::string& message) {
    client_mutex.lock();
    for (const auto& [username, client_addr] : client_map) {
        sendto(server_socket, message.c_str(), message.size(), 0,
               (struct sockaddr*)&client_addr, sizeof(client_addr));
    }
    client_mutex.unlock();
}

void handle_client(int server_socket) {
    char buffer[1024];
    sockaddr_in client_addr;
    socklen_t addr_len = sizeof(client_addr);

    while (true) {
        memset(buffer, 0, sizeof(buffer));
        int bytes_received = recvfrom(server_socket, buffer, sizeof(buffer), 0,
                                      (struct sockaddr*)&client_addr, &addr_len);
        if (bytes_received <= 0) {
            continue;
        }

        std::string message = buffer;

        // 如果收到 "ping"，立即响应 "pong"
        if (message == "ping") {
            const std::string pong_message = "pong";
            sendto(server_socket, pong_message.c_str(), pong_message.size(), 0,
                   (struct sockaddr*)&client_addr, sizeof(client_addr));
            continue;
        }

        // 从消息中提取用户名
        std::string username = extract_username(message);
        if (username.empty()) {
            continue; // 如果消息中没有用户名，忽略
        }

        // 检查用户名是否已加入
        client_mutex.lock();
        if (client_map.find(username) == client_map.end()) {
            client_map[username] = client_addr; // 添加新用户
            client_mutex.unlock();

            // 发送欢迎消息给所有客户端
            std::string welcome_message = "欢迎 " + username + " 加入网络程序设计课堂讨论\n";
            send_message_to_all(server_socket, welcome_message);
            std::cout << "新客户端已连接: " << username << "\n";
        } else {
            client_mutex.unlock();
        }

        // 广播非空消息（包括发送者）
        if (!message.empty()) {
            std::cout << "客户端消息: " << message << "\n";
            send_message_to_all(server_socket, message);
        }
    }
}

int main() {
    int server_socket = socket(AF_INET, SOCK_DGRAM, 0);
    if (server_socket == -1) {
        perror("套接字创建失败");
        return -1;
    }

    sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("绑定失败");
        return -1;
    }

    std::cout << "服务器正在运行，端口号: " << PORT << "...\n";

    std::thread(handle_client, server_socket).detach();

    while (true) {
        // 保持服务器运行
    }

    close(server_socket);
    return 0;
}
