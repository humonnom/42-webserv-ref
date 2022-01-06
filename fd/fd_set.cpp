#include <bitset>
#include <iostream>
#include <string>

#define FD_SIZE_ __DARWIN_howmany(__DARWIN_FD_SETSIZE, __DARWIN_NFDBITS)

// fd_mask: masking의 단위(byte) == long int == 4(byte)
// NFDBITS: 8 * fd_mask -> bit 크기 -> 32(bit)
// fds_bits 배열의 크기:
//						항목 당 크기: 4byte
//						개수: fd_setsize/NFDBITS == 1024/32 -> 32
//						long int = [00000000](1byte) * 4 = 4byte
//						fds_bits = long int(4byte) * 32
//

void printAll(fd_set *test_fd) {
    int const offset = sizeof(__int32_t) / sizeof(int);
    for (int i = 0; i < FD_SIZE_; i++) {
        std::bitset<32> binary(test_fd->fds_bits[i]);
        std::cout << i << "[" << binary << "]";
        std::cout << std::endl;
    }
}

int main() {
    fd_set test_fd;

    std::cout << "> create" << std::endl;
    printAll(&test_fd);  // 쓰레기값(0과 1이 혼재)

    std::cout << "> init" << std::endl;
    FD_ZERO(&test_fd);   // 0으로 초기화
    printAll(&test_fd);  // 모든 비트가 0

    std::cout << "> change bit" << std::endl;
    FD_SET(2, &test_fd);    // 2 => [2/32]0번째줄의 [2%32]2번째순서에 저장
    FD_SET(5, &test_fd);    // 5 => [5/32]0번째줄의 [5%32]5번째순서에 저장
    FD_SET(256, &test_fd);  // 256 => [256/32]8번째줄의 [256%32]0번째순서에 저장
    printAll(&test_fd);
    std::cout << "2 is on? " << std::boolalpha << !!FD_ISSET(2, &test_fd) << std::endl;
    std::cout << "5 is on? " << std::boolalpha << !!FD_ISSET(5, &test_fd) << std::endl;
    std::cout << "256 is on? " << std::boolalpha << !!FD_ISSET(256, &test_fd) << std::endl;
}