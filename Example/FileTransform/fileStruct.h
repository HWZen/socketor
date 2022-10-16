//
// Created by HWZ on 2022/3/20.
//

#ifndef SOCKETOR_FILESTRUCT_H
#define SOCKETOR_FILESTRUCT_H

#pragma pack(1)
struct File {
    char fileName[128]{};
    char filePath[256]{};
    size_t fileSize{};
    char sha256[65];
};

const size_t PACK_SIZE = 65536;
struct Pack {
    size_t size;
    char data[PACK_SIZE];
};
inline const int default_port = 5150;
#pragma pack()


#include <iostream>

void print(auto &&...args) {
    (std::cout << ... << std::forward<decltype(args)>(args));
}

void println(auto &&...args) {
    print(std::forward<decltype(args)>(args)..., "\n");
}

void perr(auto &&...args) {
    (std::cerr << ... << std::forward<decltype(args)>(args));
}


#endif //SOCKETOR_FILESTRUCT_H

