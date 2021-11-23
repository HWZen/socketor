#pragma once
#include "socketor.h"
#define DEFAULT_PORT 5150

#ifdef I_OS_LINUX
#define SOCK_DEC
#endif

#ifdef I_OS_WIN
#ifdef DEC
#define SOCK_DEC  __declspec(dllexport)
#else
#define SOCK_DEC  __declspec(dllimport)
#endif // DEC
#endif // I_OS_WIN

namespace mysock
{

    void Accept_call_back(socketor s);
    class SOCK_DEC server : public socketor
    {
    private:
#ifdef I_OS_WIN
        WSADATA wsaData{};
#endif
        // ��¼�˿�

    public:
        explicit server(int Port = DEFAULT_PORT);

        // ������������
        int Listen() noexcept(false);
        
        // �������Ӳ������߳̽��պ���
         [[noreturn]] void Accept(void(*)(socketor)=Accept_call_back);


    };
} // namespace mysock

