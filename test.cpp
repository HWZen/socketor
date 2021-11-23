//
// Created by 10414 on 2021/11/9.
//
#include "function.h"
#include <iostream>
using namespace std;

enum my_enum{
    type1,
    type2,
    type3
};

int main()
{
    cout << EnumName<my_enum, my_enum::type2>() << endl;
}
