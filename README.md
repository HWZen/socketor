# socketor

A lib which wrap and extend sock

## require
* c++ compiler support c++17
* cmake
* some examples need [fast_io](https://gitee.com/qabeowjbtkwb/fast_io.git) lib and c++20 support 

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

You can look at [server_test.cpp](Example/test/server_test.cpp) and [client_test.cpp](Example/test/client_test.cpp)

## feature
* Cross platforms
* Simply to create a connection
* Easy to extend
* Tiny, only 500k static library * 2
* **support domain name resolution**
* Will be support async server in the future
* Thread safe

