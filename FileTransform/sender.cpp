//
// Created by HWZ on 2022/3/20.
//
#define __cpp_concepts
#include <sock_client.h>
#include <fast_io.h>
#include<fast_io_device.h>
#include<fast_io_crypto.h>
#include <string>
#include "fileStruct.h"

int main(int argc, char *argv[])
try
{
    if(argc < 2)
    {
        perr("please select a file path as parm");
        return 1;
    }
    mysock::Client c("127.0.0.1", 2233);
    if(int err = c.connect2server();err != mysock::SUCESS){
        perr("connect fail");
        return 1;
    }
    using namespace fast_io::mnp;
    fast_io::ibuf_file ibuf(::fast_io::mnp::os_c_str(argv[1]));

    fast_io::sha256_context ctx;
    auto transmitted{transmit(as_file(ctx), ibuf)};
    ctx.do_final();
    ibuf.close();

    std::string name = argv[1];
    auto it = name.end() - 1;
    while(it > name.begin() && *it != '\\' && *it != '/')
    {
        --it;
    }
    name = {it + 1, name.end()};

    fast_io::native_file_loader loader(os_c_str(argv[1]));
    File f;
    strcpy_s(f.fileName, name.size(), name.c_str());
    strcpy_s(f.filePath, strlen(argv[1]), argv[1]);
    f.fileSize = loader.size();
    ctx.digest_to_byte_ptr(f.sha256);


    char *pfile = new char[f.fileSize];
    memcpy_s(pfile, f.fileSize, loader.data(), f.fileSize);

    c.rawSend(&f, sizeof(f));
    c.rawSend(pfile, f.fileSize);

}
catch(fast_io::error e)
{
    perrln(e);
}