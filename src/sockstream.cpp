//
// Created by HWZ on 2022/4/15.
//

#include "sockstream.h"

mysock::isockstream::isockstream(mysock::socketor* sock): socket(sock), stringstream()
{

}

mysock::osockstream::osockstream(mysock::socketor* sock): socket(sock), stringstream()
{
}
