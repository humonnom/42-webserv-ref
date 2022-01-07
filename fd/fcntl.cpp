#include <fcntl.h>

#include <iostream>

int main() {
    int mode, fd, value;
    // file open
    fd = open("test.sh", O_WRONLY | O_CREAT);
    if (fd < 0) {
        std::cerr << "open error" << std::endl;
        exit(0);
    }

    // get fd flag
    value = fcntl(fd, F_GETFL, 0);
    if (value & O_NONBLOCK)
        std::cout << ("O_NONBLOCK") << std::endl;
    else
        std::cout << ("BLOCKING mode") << std::endl;

    // set fd flag(we can use this only in out task)
    // NONBLOCKING 모드로 변경한다.
    std::cout << "변경전 값: value ==> " << value << std::endl;
    int test = value | O_NONBLOCK;
    std::cout << "예상변경 값: value | O_NONBLOCK ==> " << test << std::endl;
    std::cout << "----changed----" << std::endl;
    fcntl(fd, F_SETFL, O_NONBLOCK);

    // check after change
    value = fcntl(fd, F_GETFL, 0);
    std::cout << "실제변경 값: value ==> " << value << std::endl;
    if (value & O_NONBLOCK)
        std::cout << ("O_NONBLOCK") << std::endl;
    else
        std::cout << ("BLOCKING mode") << std::endl;
}