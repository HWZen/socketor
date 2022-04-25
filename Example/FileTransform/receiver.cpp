//
// Created by HWZ on 2022/3/20.
//
#include "server.h"
#include <fast_io.h>
#include "fileStruct.h"
#include <fast_io.h>
#include<fast_io_device.h>
#include<fast_io_crypto.h>
#include <string>
#include <cstdio>
#include <algorithm>
void foo(mysock::socketor c);
int main()
{
    mysock::Server s(5150);
    if (int err = s.Listen(); err != mysock::LISTEN_SUCESS)
    {
        perrln( "Listen fail: ", err);
        return 0;
    }

    s.Accept(foo);
}

void foo(mysock::socketor c){
    using namespace fast_io::mnp;
    println(fast_io::out(), "connect: ", c.address());
    File f;
    c.receive(&f, sizeof(f));
    fast_io::out_buf_type allbuf;

    println(fast_io::out() ,os_c_str((char *)f.filePath));


    std::string tmp=f.fileName;
    tmp = "./" + tmp;
    FILE * pFile;
    pFile = fopen(tmp.c_str(), "wb");
    if(!pFile)
    {
        perr("file creat fail");
        return;
    }

    char *buf = new char[1024];
    size_t it = f.fileSize;
    int schedule{};
    while(it > 0)
    {

        auto recLen =  c.receive(buf, 1024);
        if(recLen <= 0)
            break;
        fwrite(buf, recLen, 1, pFile);
        it -= recLen;
        if(schedule < (double)(f.fileSize - it) / (double)f.fileSize * 100){
            schedule = (double)(f.fileSize - it) / (double)f.fileSize * 100;
            println(fast_io::out(), schedule);
        }
    }
    fclose(pFile);

    println(fast_io::out() ,"receved.");
    c.Send("receved");
    fast_io::sha256_context ctx;
    fast_io::ibuf_file ifile(os_c_str(tmp.c_str()));

    transmit(as_file(ctx), ifile);
    ifile.close();
    ctx.do_final();
    println(fast_io::out() ,hash_digest(ctx));


}