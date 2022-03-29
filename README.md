# socketor

A lib which wrap and extend sock

## require
* c++20 compiler (msvc 19.30 , gcc 12.0)
* [fast_io](https://gitee.com/qabeowjbtkwb/fast_io.git) lib
* cmake

## build
```shell
git clone https://gitee.com/HWZen/socketor
cd socketor
mkdir build && cd build
cmake ../
cmake --build .
```
The above command applies to Windows and Linux platforms

## use example

You can look at [server_test.cpp](./server_test.cpp) and [client_test.cpp](./client_test.cpp)

## feature
* Cross platforms
* Simply to create a connection
* Easy to extend
* Tiny, only 500k static library * 2
* **support domain name resolution**
* Will be support async server in the future
* Thread safe

