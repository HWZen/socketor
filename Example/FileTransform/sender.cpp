//
// Created by HWZ on 2022/3/20.
//

#include "include/sock_client.h"
#include <fast_io.h>
#include<fast_io_device.h>
#include<fast_io_crypto.h>
#include <string>
#include <cstring>
#include "fileStruct.h"
#include <algorithm>

#ifdef I_OS_LINUX
#define memcpy_s(dst, dstLen, src, srcLen) memcpy(dst, src, srcLen);
#endif


int main(int argc, char* argv[])
try
{
    if (argc < 3)
    {
        perrln("please select a file path as parm");
        return 1;
    }
    mysock::Client c(argv[2], 5150);
    if (int err = c.connect2server();err != mysock::SUCESS)
    {
        perrln("connect fail: ", err);
        return 1;
    }
    using namespace fast_io::mnp;
    fast_io::ibuf_file ibuf(::fast_io::mnp::os_c_str(argv[1]));

    fast_io::sha256_context ctx;
    auto transmitted{ transmit(as_file(ctx), ibuf) };
    ctx.do_final();
    ibuf.close();

    std::string name = argv[1];
    auto nameIt = name.end() - 1;
    while (nameIt > name.begin() && *nameIt != '\\' && *nameIt != '/')
    {
        --nameIt;
    }
    name = { nameIt + 1, name.end() };

    fast_io::native_file_loader loader(os_c_str(argv[1]));
    File f;
    std::strcpy(f.fileName, name.c_str());
    std::strcpy(f.filePath, argv[1]);
    f.fileSize = loader.size();
    ctx.digest_to_byte_ptr(f.sha256);
    println(fast_io::out() ,hash_digest(ctx));


    char* pfile = new char[f.fileSize];
    memcpy_s(pfile, f.fileSize, loader.data(), f.fileSize);

    c.rawSend(&f, sizeof(f));
    c.rawSend(pfile, f.fileSize);
    println(fast_io::out() ,"sended");
    c.receive();
    println(fast_io::out() ,"recved");

}
catch (fast_io::error e)
{
    perrln(e);
}