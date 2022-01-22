#include <netinet/in.h>  // address 구조체, htons() 함수
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#define PORT 8080

#include <iostream>

// 함수
// socket : create socket fd => socket fd(int)
// setsocketopt : attaching socket to the port 8080
// inet_addr
// getsockname
// connect
// recv
// bind
// send
// listen
// accept
//

int main() {
    int sfd = socket(AF_INET, SOCK_STREAM, 0);
    // AF_INET: Internet domain sockets => domain
    // SOCK_STREAM: Byte-stream socket => type
    // 0: => protocol(0 is default)
    std::cout << "[ socket open ] fd is " << sfd << std::endl;

    int opt = 1;
    setsockopt(sfd, SOL_SOCKET, SO_REUSEPORT,
               &opt, sizeof(opt));
    std::cout << "[ socket setting ] port is now reusable " << sfd << std::endl;
    // SOL_SOCKET: 소켓수준의 옵션 설정을 하겠다는 뜻
    // SO_REUSEPORT: POSIX 표준이 아님, 멀티스레드를 가능하게 함
    // &opt: value, 옵션을 비활성화할지 활성화할지 결정(true or false)
    // sizeof(opt): 옵션 value의 사이즈

    struct sockaddr_in address;
    int addrlen = sizeof(address);

    // address 구조체 세팅
    address.sin_family = AF_INET;          // Internet domain sockets
    address.sin_addr.s_addr = INADDR_ANY;  // localhost address
    address.sin_port = htons(PORT);        // 저장하는 endian 방식 차이 때문에 사용
    // 네트워크는 Big endian, 8080을 네트워크 byte order로 변경하여 구조체에 넣어줌
    std::cout << "[ address structure setting ] " << sfd << std::endl;

    if (bind(sfd, (struct sockaddr *)&address,
             sizeof(address)) < 0) {
        perror("[ 8080 bind failed ]");
        // exit(EXIT_FAILURE);
        // address.sin_port = htons(PORT2);  // port 변경
        // if (bind(sfd, (struct sockaddr *)&address,
        //          sizeof(address)) < 0) {
        //     exit(EXIT_FAILURE);
        // }
    }
    listen(sfd, 1000);
    std::cout << "[ listening... ] " << sfd << std::endl;
    while (1) {
        char buffer[1024] = {0};
        int new_socket, valread;
        if ((new_socket = accept(sfd, (struct sockaddr *)&address,
                                 (socklen_t *)&addrlen)) < 0) {
            perror("accept");
            exit(EXIT_FAILURE);
        }
        valread = read(new_socket, buffer, 1024);

        std::cout << buffer << std::endl;
        std::string message = "message from server";
        send(new_socket, message.c_str(), strlen(message.c_str()), 0);
        close(new_socket);
    }
};