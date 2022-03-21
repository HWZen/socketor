//
// Created by HWZ on 2022/3/20.
//
#define __cpp_concepts
#include <sock_server.h>
#include <fast_io.h>
#include "fileStruct.h"
#include <fast_io.h>
#include<fast_io_device.h>
#include<fast_io_crypto.h>
#include <string>

int main()
{
    mysock::server s(5150);
    if (int err = s.Listen(); err != mysock::LISTEN_SUCESS)
    {
        println(fast_io::c_stderr(), "Listen fail: ", err);
        return 0;
    }
    using namespace fast_io::mnp;
    s.Accept([](mysock::socketor c)
    {
        File f;
        c.receive(&f, sizeof(f));
        fast_io::out_buf_type allbuf;

        char *buf = new char[f.fileSize];
        auto times = f.fileSize / 512;
        auto it = buf;
        while(times--)
        {
            c.receive(it, 512);
            it+=512;
        }
        std::string tmp=f.filePath;
        tmp += ".backup";
        fast_io::obuf_file ofile(os_c_str(tmp.c_str()));
        print(ofile, os_c_str(buf));
        ofile.close();

        fast_io::sha256_context ctx;
        fast_io::ibuf_file ifile(os_c_str(tmp.c_str()));

        transmit(as_file(ctx), ifile);
        ifile.close();
        ctx.do_final();
        println(hash_digest(ctx));


    });

}