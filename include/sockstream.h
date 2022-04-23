//
// Created by HWZ on 2022/4/15.
//

#ifndef SOCKETOR_SOCKSTREAM_H
#define SOCKETOR_SOCKSTREAM_H

#include "socketor.h"
#include <sstream>

using std::stringstream;

namespace mysock
{
    class isockstream : private stringstream {
    public:
        explicit isockstream(socketor *sock);
        ~isockstream() override = default;

        using stringstream::precision;
        using stringstream::setf;

        template<typename T>
        isockstream &operator>>(T &t) {
            if(rdbuf()->in_avail() == 0)
                (stringstream&)*this << socket->receive();
            (stringstream&)*this >> t;
            return *this;
        }

    private:
        socketor *socket;
    };

    class osockstream : private stringstream {
    public:
        explicit osockstream(socketor *sock);
        ~osockstream() override = default;

        using stringstream::precision;
        using stringstream::setf;

        template<typename T>
        osockstream &operator<<(const T &t) {
            clear();
            (stringstream&)*this << t;
            socket->Send(str());
            return *this;
        }

    private:
        socketor *socket;
    };
}


#endif //SOCKETOR_SOCKSTREAM_H
