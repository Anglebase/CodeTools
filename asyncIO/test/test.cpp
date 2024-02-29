#include "../asyncIO.hpp"
using namespace pack_asyncIO; // 直接使用命名空间
// namespace pkio = pack_asyncIO;   // 或使用别名

#include <thread>
#include <chrono>
using namespace std;
using namespace std::chrono_literals;

int main()
{
    // --------------------------------通用操作--------------------------------
    asyncIO.read("GetInput", 1, 10, stdin); // 通过异步获取命令行输入

    // ... // 执行此线程的其他代码
    // ... // 此线程此处不会阻塞，但异步IO线程由于等待输入因此会产生阻塞
    // ... // 当输入也完成时，会继续执行后续任务，不会等待值被读取

    // 获取异步结果，如果结果未产生则会阻塞直至结果产生
    // 该函数调用后，缓冲区将移除该值
    auto data = asyncIO.get("GetInput");
    // asyncIO.get("GetInput"); // 将会抛出 name_undefined() 异常

    asyncIO.write(data.data, data.size, data.count, stdout); // 将结果打印输出

    // 文件操作
    FILE *fp = fopen("./db/test.txt", "w+");
    char s[] = "This project is best!\n";
    asyncIO.write(s, sizeof(char), sizeof(s) - 1, fp); // 写入文件
    // 异步修改文件指针
    asyncIO.fseek(SEEK_SET, fp);

    // 定义以 "from 'test' file" 为别名的数据
    asyncIO.read("from 'test' file", sizeof(char), sizeof(s) - 1, fp); // 读取文件

    // ... // 执行其它代码
    // ... // 异步线程在执行完读取后不会阻塞，会继续执行后续任务

    asyncIO.write("Task\n", 1, 6, stdout);
    // 通过别名获取数据
    auto getdata = asyncIO.get("from 'test' file");
    asyncIO.write(getdata.data, getdata.size, getdata.count, stdout);

    // ------------------------------类型安全操作------------------------------
    // 包内部已定义了一个用于控制台输出且类型安全的异步输出得到封装，它的使用与标准库相同
    out << "This is typesafe async output" << '\n';
    int i = 0;
    out << 123 << ' ' << 12.3 << ' ' << &i;
    // 对于文件，与前面相似
    fout foutput{fp};
    foutput << "Two line" << 12345 << '\n';

    // ----------------------------------------------------------------------
    // 用于等待所有异步IO任务都执行结束
    while (!asyncIO.finish())
        this_thread::sleep_for(100ms);

    fclose(fp); // 对于文件可以使用封装，只需保证在异步IO线程操作文件任务结束之后再关闭文件
    return 0;
}
