# 通用全局异步IO对象
## 包结构
<pre style="font-family:'Cascadia Code'">
|- asyncIO.hpp                  包头文件(包含了所有包内的头文件)
|- asyncIO                      包文件
    |- include
        |- asyncio.hxx          底层实现声明
        |- outputdef.hpp        类型安全输出函数声明
        |- typesafeoutput.hxx   类型安全输出封装声明
    |- lib
        |- libasyncio.a         由GCC在Windows环境下编译的静态链接库
        |- libasyncio.so        由GCC在Windows环境下编译的动态链接库
    |- asyncio.cxx              底层实现
    |- outputdef.cpp            类型安全输出
    |- typesafeoutput.cxx       类型安全输出封装
</pre>

## 使用
```asyncIO```宏作为全局异步IO对象，它可用于异步地进行输出或输入

### 成员函数
声明于```asyncio.hxx```  
定义于```asyncio.cxx```  

```C++
void sync_read(void*,std::size_t,std::size_t,FILE*);
```
它用于同步地执行数据读取操作，作为异步读取的调用接口，不能够单独进行调用，否则会引发程序异常终止
```C++
void sync_write(void*,std::size_t,std::size_t,FILE*);
```
它用于同步地执行数据写入操作，作为异步写入的调用接口，不推荐单独使用，希望以同步的方式输出内容应使用```std::cout```
```C++
void read(const std::string&,std::size_t,std::size_t,FILE*);
ForwardPack get(const std::string&);
```
它们用于异步地读取数据，```read(...)```用于在任务队列中插入读数据申请，并使其可以使用别名访问读取数据，```get(...)```用于获取异步读取所得的结果，如果调用```get(...)```时异步的结果尚未产生，则会阻塞代码直至获取异步结果，当```get(...)```被调用一次之后，将会移除处于缓冲区的 *别名：参数转发包* 键值对  
+ 如果调用```read(...)```时，传入的别名已经存在于缓冲区，将会抛出```name_redefined()```异常  
+ 如果调用```get(...)```时，传入的别名不存在于缓冲区，将会抛出```name_undefined()```异常  

```C++
void write(void*,std::size_t,std::size_t,FILE*);
```
它用于异步地执行写入数据操作
```C++
void fseek(int,FILE*);
```
它用于异步地修改文件内容指针的位置

### 异步类型安全输出
它的使用与标准库中完全相同，同样为重载了<<运算符
#### 基本输出
声明于```typesafeoutput.hxx```  
定义于```typesafeoutput.cxx```  
```C++
TypesafeAsyncOutput &operator<<(TypesafeAsyncOutput &out, char);
TypesafeAsyncOutput &operator<<(TypesafeAsyncOutput &out, const char *);
```
它们分别为字符类型的重载与字符串类型的重载，是实现其它类型输出的基础

#### 扩展输出
声明于```outputdef.hxx```  
定义于```outputdef.cxx```  
```C++
TypesafeAsyncOutput &operator<<(TypesafeAsyncOutput &out, short);
TypesafeAsyncOutput &operator<<(TypesafeAsyncOutput &out, int);
TypesafeAsyncOutput &operator<<(TypesafeAsyncOutput &out, long);
TypesafeAsyncOutput &operator<<(TypesafeAsyncOutput &out, long long);
TypesafeAsyncOutput &operator<<(TypesafeAsyncOutput &out, unsigned short);
TypesafeAsyncOutput &operator<<(TypesafeAsyncOutput &out, unsigned int);
TypesafeAsyncOutput &operator<<(TypesafeAsyncOutput &out, unsigned long);
TypesafeAsyncOutput &operator<<(TypesafeAsyncOutput &out, unsigned long long);
TypesafeAsyncOutput &operator<<(TypesafeAsyncOutput &out, float);
TypesafeAsyncOutput &operator<<(TypesafeAsyncOutput &out, double);
TypesafeAsyncOutput &operator<<(TypesafeAsyncOutput &out, long double);
template<class T>
TypesafeAsyncOutput &operator<<(TypesafeAsyncOutput &out, T *);
TypesafeAsyncOutput &operator<<(TypesafeAsyncOutput &out, const std::string&);
```
它们用于其它类型数据的输出
#### 自定义输出
针对于自定义类型，与标准库相同，
对于自定义类型```T```，实现其
```C++
TypesafeAsyncOutput &operator<<(TypesafeAsyncOutput &out, T);
```
或
```C++
TypesafeAsyncOutput &operator<<(TypesafeAsyncOutput &out, const T&);
```
之一即可