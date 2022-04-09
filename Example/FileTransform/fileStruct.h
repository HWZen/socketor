//
// Created by HWZ on 2022/3/20.
//

#ifndef SOCKETOR_FILESTRUCT_H
#define SOCKETOR_FILESTRUCT_H

using File = struct File{
    char fileName[128]{};
    char filePath[256]{};
    size_t fileSize{};
    std::byte sha256[256];
};

const size_t PACK_SIZE = 65536;
using Pack = struct Pack{
    size_t size;
    char data[PACK_SIZE];
};

inline const int default_port = 5150;


#endif //SOCKETOR_FILESTRUCT_H

