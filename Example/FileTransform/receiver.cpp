//
// Created by HWZ on 2022/3/20.
//
#include "server.h"
#include "fileStruct.h"
#include <string>
#include <cstdio>
#include <algorithm>
#include "hash.h"
void foo(const mysock::Server::Client& c);
int main()
{
    mysock::Server s(5150);
    if (int err = s.listen(); err != mysock::LISTEN_SUCESS)
    {
        perr( "Listen fail: ", err, "\n");
        return 0;
    }

    auto client = s.accept();
    if(client.hasConnected())
        foo(client);
}

void foo(const mysock::Server::Client& c){

    println("connect: ", c.address());
    File f;
    c.receive(&f, sizeof(f));

    println(f.filePath);


    std::string tmp=f.fileName;
    tmp = "./" + tmp;
    FILE * pFile;
    pFile = fopen(tmp.c_str(), "w+b");
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
            println(schedule);
        }
    }
    println("receved.");
    c.Send("receved");

    auto fileBuf = new sstd::byte[f.fileSize];
    fseek(pFile, 0, SEEK_SET);
    fread(fileBuf, f.fileSize, 1, pFile);
    fclose(pFile);


    auto sha256 = sstd::SHA256Hash(fileBuf, f.fileSize);
    auto strSha256 = sstd::SHA256Hash2String(sha256);

    println("file sha256: ", f.sha256);
    println("received sha256: ", strSha256);
}