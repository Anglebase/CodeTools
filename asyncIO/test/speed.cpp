#include "../asyncIO.hpp"
using namespace pack_asyncIO;
#include <iostream>
#include <chrono>
using namespace std;
using namespace std::chrono_literals;

/**
 * 速度测试
 */

auto timer(void (*f)())
{
    auto start = chrono::system_clock::now();
    f();
    auto end = chrono::system_clock::now();
    return (end - start).count();
}

void sync()
{
    for (int i = 0; i <= 20000; i++)
        cout << "Hello World:" << i << endl;
}

void async()
{
    for (int i = 0; i <= 20000; i++)
        out << "Hello World:" << i << '\n';
}

int main()
{
    auto a = timer(sync);
    auto b = timer(async);

    while (!asyncIO.finish())
        this_thread::sleep_for(100ms);
    // Output frist(-g)  | second(-O4)
    cout << " Sync: " << a << endl;          //  Sync: 7073602900 | 5435828000
    cout << "Async: " << b << endl;          // Async: 83502300   | 5528900
    cout << "a / b: " << a * 1. / b << endl; // Sy/As: 84.7115    | 983.166
}