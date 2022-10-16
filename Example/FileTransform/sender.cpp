//
// Created by HWZ on 2022/3/20.
//

#include "client.h"
#include <string>
#include <cstring>
#include "fileStruct.h"
#include <algorithm>
#include <unordered_map>
#include <string>
#include <cstdio>
#include "hash.h"

#ifdef I_OS_LINUX
#define memcpy_s(dst, dstLen, src, srcLen) memcpy(dst, src, srcLen);
#endif


std::tuple<int, std::unordered_map<std::string, std::string>> paramParse(int argc, char **argv);


int main(int argc, char *argv[])
{
    auto [errorCode, parseRes] = paramParse(argc, argv);
    if (errorCode == -1) {
        perr("params: \n"
             "<transform file path> [-h <host>][-p <port>]\n"
             "default host: 127.0.0.1\n"
             "default port: " + std::to_string(default_port));
        return 1;
    }

    if (!parseRes.count("unkonwnParam2")) {
        perr("params: \n"
             "<transform file path> [-h <host>][-p <port>]\n"
             "default host: 127.0.0.1\n"
             "default port: " + std::to_string(default_port));
        return 1;
    }

    int port = (parseRes.count("p") ? std::stoi(parseRes["p"]) : default_port);
    std::string serverAddr = (parseRes.count("h") ? parseRes["h"] : "127.0.0.1");


    mysock::Client c(serverAddr.c_str(), port);
    if (int err = c.Connect2Server();err != mysock::SOCKETOR_SUCESS) {
        println("connect fail: ", err);
        return 1;
    }

    FILE *pFile = fopen(parseRes["unkonwnParam2"].c_str(), "rb");



    std::string name = parseRes["unkonwnParam2"];
    auto nameIt = name.end() - 1;
    while (nameIt > name.begin() && *nameIt != '\\' && *nameIt != '/') {
        --nameIt;
    }
    name = {nameIt + 1, name.end()};

    File f;
    strcpy(f.fileName, name.c_str());
    strcpy(f.filePath, parseRes["unkonwnParam2"].c_str());
    fseek(pFile, 0, SEEK_END);
    f.fileSize = ftell(pFile);
    fseek(pFile, 0, SEEK_SET);


    char *fileData = new char[f.fileSize];
    fread(fileData, f.fileSize, 1, pFile);
    fclose(pFile);

    auto sha256 = sstd::SHA256Hash(fileData, f.fileSize);
    auto strSha256 = sstd::SHA256Hash2String(sha256);

    memcpy(f.sha256, strSha256.c_str(), 64);
    f.sha256[64] = 0;

    c.Send(&f, sizeof(f));
    c.Send(fileData, f.fileSize);
    println("sended");
    c.receive();
    println("recved");

}



std::tuple<int, std::unordered_map<std::string, std::string>> paramParse(int argc, char **argv) {
    using std::unordered_map, std::string;
    unordered_map<string, string> res;
    int cnt{};
    for (int i{}; i < argc; ++i) {
        if (argv[i][0] == '-') {
            if (i == argc - 1)
                return {-1, {}};
            else {
                res[{argv[i] + 1}] = argv[i + 1];
                ++i;
            }
        } else
            res["unkonwnParam" + std::to_string(++cnt)] = argv[i];
    }
    return {1, res};
}