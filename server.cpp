#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>

std::map<std::string, std::string> database;

void loadData() {
    std::ifstream file("query.txt");
    std::string id, email;
    while (file >> id >> email) {
        database[id] = email;
    }
}

std::string processRequest(const std::string& request) {
    try {
        if (database.find(request) != database.end()) {
            return database[request];
        } else {
            struct hostent *he = gethostbyname(request.c_str());
            if (he == nullptr) {
                return "Invalid URL";
            } else {
                struct in_addr **addr_list = (struct in_addr **)he->h_addr_list;
                return inet_ntoa(*addr_list[0]);
            }
        }
    } catch (...) {
        return "Error while processing the request";
    }
}

int main() {
    loadData();
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        std::cerr << "Error opening socket" << std::endl;
        return -1;
    }
    else{
        std::cerr << "Succeed opening socket" << std::endl;
    }

    struct sockaddr_in serverAddress = {};
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(1234); // Server port
    serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1"); // Server IP

    if (bind(sockfd, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0) {
        std::cerr << "Error on socket binding" << std::endl;
        close(sockfd);
        return -1;
    }
    else{
        std::cerr << "Succeed on socket binding" << std::endl;
    }

    listen(sockfd, 10);

    while (true) {
        struct sockaddr_in clientAddress = {};
        socklen_t clientLen = sizeof(clientAddress);
        int clientSockfd = accept(sockfd, (struct sockaddr*)&clientAddress, &clientLen);

        if (clientSockfd < 0) {
            std::cerr << "Error on accept" << std::endl;
            continue;
        }
        else{
            std::cerr << "Client connects successfully" << std::endl;

        }

        char buffer[1024] = {};
        read(clientSockfd, buffer, sizeof(buffer));
        std::string request(buffer);
        std::string response = processRequest(request);
        write(clientSockfd, response.c_str(), response.size());

        close(clientSockfd);
    }

    close(sockfd);
    return 0;
}
