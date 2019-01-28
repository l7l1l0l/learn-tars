/***********************************************************************
* File Name: client.cpp
* Author: RogerLuo
* mail: 13369189710@163.com
* Created Time: Fri 25 Jan 2019 06:18:35 PM CST
***********************************************************************/
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

#include <string>
#include <iostream>

int main (int argc, char** argv) {
    int fd = -1;

    fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd < 0) {
        perror("sock created");
        return -1;
    }

    struct sockaddr_in server_addr;
    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    if (inet_aton("127.0.0.1", &server_addr.sin_addr) == 0) {
        printf("Server IP Address Error!\n");
        return -1;
    }

    server_addr.sin_port = htons(9710);
    socklen_t server_addr_length = sizeof(server_addr);
    if (connect(fd, (struct sockaddr*)&server_addr, server_addr_length) < 0) {
        printf("connect error\n");
        return -1;
    }

    printf("connect success\n");
    printf("please input request msg:\n");

    char recvBuf[1024] = { 0 };
    std::string request;
    while (getline(std::cin, request)) {
        
        int bytes = write(fd, request.c_str(), request.size());
        printf("send len[%d]\n", bytes);
        read(fd, recvBuf, sizeof(recvBuf));
        fputs(recvBuf, stdout);
        printf("\n");
        memset(recvBuf, 0, sizeof(recvBuf));

        printf("please input request msg:\n");
    }
    close(fd);
    return 0;
}

