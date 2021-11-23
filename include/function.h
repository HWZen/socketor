//
// Created by 10414 on 2021/11/7.
//

#ifndef SOCKETOR_FUNCTION_H
#define SOCKETOR_FUNCTION_H

#include <string>
#include "osplatformutil.h"

#ifdef I_OS_LINUX
#include <time.h>

void Sleep(int ms)
{
    struct timeval delay;
    delay.tv_sec = 0;
    delay.tv_usec = ms * 1000; // 20 ms
    select(0, NULL, NULL, NULL, &delay);
}

#endif

template<typename Ty, Ty t>
std::string EnumName();

namespace mysock
{
    std::string string_To_UTF8(const std::string &str);

    char *UtfToGbk(const char *utf8);


}



template<typename Ty, Ty t>
std::string EnumName()
{
#ifdef _MSC_VER
    std::string res = __FUNCSIG__;
#endif //_MSC_VER
#ifdef __GNUC__
    std::string res = __PRETTY_FUNCTION__;
#endif
    std::string::iterator l = res.begin();
    std::string::iterator r = res.end();
    for (auto it = res.end() - 1; it != res.begin(); --it)
    {
        if (*it == '>')
            r = it;
        if (*it == ',')
        {
            l = it + 1;
            break;
        }
    }

    if (l != res.begin() && r != res.end())
        res = std::string(l, r);

    return res;
}

#endif //SOCKETOR_FUNCTION_H
