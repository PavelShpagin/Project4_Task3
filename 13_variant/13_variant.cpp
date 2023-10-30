

#include <iostream>
#include <string>
#include "DataStruct.cpp"

using namespace std;

void f1(DataStruct& data) {
    data.read1();
    data.write1(5);
}

void f2(DataStruct& data) {
    data.write1(7);
    data.read1();
    std::cout << (std::string)data + '\n';
}

int main()
{
    DataStruct data;
    {
        std::jthread t1(f1, std::ref(data));
        std::jthread t2(f2, std::ref(data));
    }
    std::cout << (std::string)data;

    return 0;
}
