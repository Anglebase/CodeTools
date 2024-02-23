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

// 同步IO
// Synchronous IO
void func1()
{
    for (int i = 0; i < 20000; i++)
    {
        cout << "_SynOut:" << i << endl;
    }
}

// 异步IO
// Asynchronous IO
void func2()
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
    auto t1 = timer(func1);// 同步 Synchronous
    auto t2 = timer(func2);// 异步 Asynchronous

    // 等待异步执行完成
    // Waiting for asynchronous execution to complete
    while (!logger.finish())
    {
        std::this_thread::sleep_for(20ms);
    }

    std::cout << std::endl;                         //  test:  first         |second
    std::cout << "t1:" << t1.count() << std::endl;  // Output: 4488000800    |4559587200
    std::cout << "t2:" << t2.count() << std::endl;  // Output: 7026300       |6014600
    std::cout << std::endl;
}