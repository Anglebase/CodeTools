#include <chrono>
using namespace std::chrono_literals;
#include <iostream>
using namespace std;
#include "../logger.hxx"

// 计时函数
// timing function
auto timer(void (*f)())
{
    auto start = chrono::system_clock::now();
    f();
    auto end = chrono::system_clock::now();
    return end - start;
}

// 同步IO(C风格)
// Synchronous IO(C style)
void func1()
{
    for (int i = 0; i < 20000; i++)
    {
        printf("CSynOut:%d\n", i);
    }
}

// 同步IO(C++风格)
// Synchronous IO(C++ style)
void func2()
{
    for (int i = 0; i < 20000; i++)
    {
        cout << "+SynOut:" << i << endl;
    }
}

// 异步IO
// Asynchronous IO
void func3()
{
    for (int i = 0; i < 20000; i++)
    {
        logger.info(str("AsynOut:", i));
    }
}

// 异步与同步的速度比较
// Comparison of speed between asynchronous and synchronous
int main()
{
    auto t1 = timer(func1); // 同步 Synchronous(C)
    auto t2 = timer(func2); // 同步 Synchronous(C++)
    auto t3 = timer(func3); // 异步 Asynchronous

    // 等待异步执行完成
    // Waiting for asynchronous execution to complete
    while (!logger.finish())
    {
        std::this_thread::sleep_for(20ms);
    }

    std::cout << std::endl;                        //   test: first         |second
    std::cout << "t1:" << t1.count() << std::endl; // Output: 11651293900   |11588594700
    std::cout << "t2:" << t2.count() << std::endl; // Output: 4619884600    |4771054800
    std::cout << "t3:" << t3.count() << std::endl; // Output: 6021100       |7002500
    std::cout << std::endl;
}