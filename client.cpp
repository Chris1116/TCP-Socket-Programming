#include <iostream>
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

std::string sendRequest(const std::string& request) {
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd < 0) {
        std::cerr << "Error opening socket" << std::endl;
        return "Error";
    }

    struct sockaddr_in serverAddress = {};
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(1234); // Server port
    serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1"); // Server IP

    if (connect(sockfd, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0) {
        std::cerr << "Error connecting" << std::endl;
        close(sockfd);
        return "Error";
    }

    if (write(sockfd, request.c_str(), request.size()) < 0) {
        std::cerr << "Error writing to socket" << std::endl;
        close(sockfd);
        return "Error";
    }

    char buffer[1024] = {};
    if (read(sockfd, buffer, sizeof(buffer)) < 0) {
        std::cerr << "Error reading from socket" << std::endl;
        close(sockfd);
        return "Error";
    }

    close(sockfd);
    return std::string(buffer);
}

int main() {
    while (true) {
        int choice;
        std::string request, response;
        std::cout << "What's your requirement? 1. DNS 2. QUERY 3. QUIT : ";
        std::cin >> choice;
        switch(choice) {
            case 1:
                std::cout << "Input URL : ";
                std::cin >> request;
                response = sendRequest(request);
                if (response != "Error") {
                    std::cout << "IP get from server : " << response << std::endl;
                }
                break;
            case 2:
                std::cout << "Input student ID : ";
                std::cin >> request;
                response = sendRequest(request);
                if (response != "Error") {
                    std::cout << "Email get from server : " << response << std::endl;
                }
                break;
            case 3:
                return 0;
            default:
                std::cout << "Wrong command! Number should be 1 to 3" << std::endl;
        }
    }
    return 0;
}
