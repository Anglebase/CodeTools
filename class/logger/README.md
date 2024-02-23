# 异步IO输出

比同步IO快1000倍，执行时不会产生IO阻塞

> 它的核心原理是通过一个类管理一个独立的线程专门用于IO输出，仅此就可以获得远超同步IO的速度  
> 它的主要作用是用于日志输出，因此自带格式化信息(时间、[ 位置(可选) ])  

详见test示例

C++标准至少为C++17 (由于其使用了折叠表达式)

# Asynchronous IO output

1000 times faster than synchronous IO, no IO blocking occurs during execution

> Its core principle is to manage an independent thread dedicated to IO output through a class, which alone can achieve speeds far beyond synchronous IO
> Its main function is for log output, therefore it comes with formatted information (time, [location (optional)])

Please refer to the test example for details

The C++standard is at least C++17 (due to its use of folding expressions)


## 源文件(Source File)

+ logger.hxx
+ logger.cxx
