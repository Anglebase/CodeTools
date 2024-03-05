#include "async.h"
/**
 * 异步任务队列 与 异步线程池
 */
#include <thread>
#include <queue>
#include <vector>
#include <map>
#include <string>
#include <iostream>
#include <chrono>
using namespace std;
using namespace std::chrono_literals;

chrono::microseconds silence_duration = 300ms;

AsyncThread::AsyncThread()
    : funcs{}, args{}, rets{}, execute{true},
      asyncthread{[&]()
                  {
                      while (this->execute)
                      {
                          if (this->funcs.empty())
                          {
                              this_thread::sleep_for(silence_duration);
                              continue;
                          }
                          void *ret = this->funcs.front()(this->args.front());
                          if (this->retids.front() != "")
                          {
                              this->rets[this->retids.front()] = ret;
                          }
                          this->funcs.pop();
                          delete (char *)this->args.front();
                          this->args.pop();
                          this->retids.pop();
                      }
                  }}
{
}

AsyncThread::~AsyncThread()
{
    while (this->funcs.size() > 0)
        this_thread::sleep_for(silence_duration);

    this->execute = false;
    this->asyncthread.join();
}

/// @brief 向异步任务队列中添加任务
/// @param func 运行时泛型可调用对象，即 void *(*)(void *) 型函数指针
/// @param args 可调用对象参数，若无可空也可赋予值 nullptr，且必须是new表达式创建的非数组堆指针，该指针会在函数异步执行完成后被自动释放
/// @param iden 返回值寻址id，若无返回值可空也可赋予值 ""(空字符串)
void AsyncThread::push(void *(*func)(void *), void *args, const std::string &iden)
{
    this->args.push(args);
    if (iden != "")
    {
        if (this->rets.find(iden) != this->rets.end())
            throw "AsyncThread: redefined Name.";

        this->retids.push(iden);
        this->rets[iden] = nullptr;
    }
    else
    {
        this->retids.push("");
    }
    this->funcs.push(func);
}

/// @brief 异步可调用对象返回值获取
/// @param iden 可调用对象对应的返回值寻址id
/// @return 可调用对象内创建的返回值堆指针，该指针不会被自动释放
const void *const AsyncThread::getret(const std::string &iden)
{
    if (this->rets.find(iden) == this->rets.end())
        throw "AsyncThread: undefined Name.";
    void *ret = this->rets[iden];
    this->rets.erase(iden);
    return ret;
}

AsyncThreadPool::AsyncThreadPool(std::size_t count)
    : asynclist{count > 0 ? count : 1}
{
}

AsyncThreadPool::~AsyncThreadPool() = default;

void AsyncThreadPool::push(void *(*func)(void *), void *args, const std::string &iden)
{
    if (iden != "")
        for (auto &async : this->asynclist)
            if (async.rets.find(iden) != async.rets.end())
                throw "AsyncThread: redefined Name.";

    size_t pos = 0;
    size_t min = this->asynclist[0].funcs.size();
    for (size_t i = 0; i < this->asynclist.size(); i++)
    {
        auto n = this->asynclist[i].funcs.size();
        if (n < min)
            pos = i, min = n;
    }
    this->asynclist[pos].push(func, args, iden);
}

const void *const AsyncThreadPool::getret(const std::string &iden)
{
    void *ret = nullptr;
    for (auto &async : this->asynclist)
        if (async.rets.find(iden) != async.rets.end())
        {
            while (async.rets[iden] == nullptr)
                ;
            ret = async.rets[iden];
            async.rets.erase(iden);
            break;
        }

    if (ret == nullptr)
        throw "AsyncThread: undefined Name.";

    return ret;
}
