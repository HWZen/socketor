//
// Created by HWZ on 2022/3/20.
//

#include "client.h"
#include <fast_io.h>
#include<fast_io_device.h>
#include<fast_io_crypto.h>
#include <string>
#include <cstring>
#include "fileStruct.h"
#include <algorithm>
#include <unordered_map>
#include <string>

#ifdef I_OS_LINUX
#define memcpy_s(dst, dstLen, src, srcLen) memcpy(dst, src, srcLen);
#endif



std::tuple<int, std::unordered_map<std::string, std::string>> paramParse(int argc, char** argv);


int main(int argc, char* argv[])
try
{
    auto[errorCode, parseRes] = paramParse(argc, argv);
    if (errorCode == -1)
    {
        println(fast_io::out(), "params: \n"
                                "<transform file path> [-h <host>][-p <port>]\n"
                                "default host: 127.0.0.1\n"
                                "default port: " + std::to_string(default_port));
        return 1;
    }

    if(!parseRes.count("unkonwnParam2")){
        println(fast_io::out(), "params: \n"
                                "<transform file path> [-h <host>][-p <port>]\n"
                                "default host: 127.0.0.1\n"
                                "default port: " + std::to_string(default_port));
        return 1;
    }

    int port = (parseRes.count("p") ? std::stoi(parseRes["p"]) : default_port);
    std::string serverAddr = (parseRes.count("h") ? parseRes["h"] : "127.0.0.1");



    mysock::Client c(serverAddr.c_str(), port);
    if (int err = c.Connect2Server();err != mysock::SUCESS)
    {
        perrln("connect fail: ", err);
        return 1;
    }
    using namespace fast_io::mnp;
    fast_io::ibuf_file ibuf(os_c_str(parseRes["unkonwnParam2"].c_str()));

    fast_io::sha256_context ctx;
    auto transmitted{ transmit(as_file(ctx), ibuf) };
    ctx.do_final();
    ibuf.close();

    std::string name = parseRes["unkonwnParam2"];
    auto nameIt = name.end() - 1;
    while (nameIt > name.begin() && *nameIt != '\\' && *nameIt != '/')
    {
        --nameIt;
    }
    name = { nameIt + 1, name.end() };

    fast_io::native_file_loader loader(os_c_str(parseRes["unkonwnParam2"].c_str()));
    File f;
    std::strcpy(f.fileName, name.c_str());
    std::strcpy(f.filePath, parseRes["unkonwnParam2"].c_str());
    f.fileSize = loader.size();
    ctx.digest_to_byte_ptr(f.sha256);
    println(fast_io::out(), hash_digest(ctx));


    char* pfile = new char[f.fileSize];
    memcpy_s(pfile, f.fileSize, loader.data(), f.fileSize);

    c.Send(&f, sizeof(f));
    c.Send(pfile, f.fileSize);
    println(fast_io::out(), "sended");
    c.receive();
    println(fast_io::out(), "recved");

}
catch (fast_io::error &e)
{
    perrln(e);
}


std::tuple<int, std::unordered_map<std::string, std::string>> paramParse(int argc, char** argv)
{
    using std::unordered_map, std::string;
    unordered_map<string, string> res;
    int cnt{};
    for (int i{}; i < argc; ++i)
    {
        if (argv[i][0] == '-')
        {
            if (i == argc - 1)
                return { -1, {}};
            else
            {
                res[{ argv[i] + 1 }] = argv[i + 1];
                ++i;
            }
        }
        else
            res["unkonwnParam" + std::to_string(++cnt)] = argv[i];
    }
    return { 1, res };
}