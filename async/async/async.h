#pragma once
/**
 * 异步任务队列 与 异步线程池
 */
#ifndef _ASYNC_H_
#define _ASYNC_H_

#include <thread>
#include <queue>
#include <vector>
#include <map>
#include <string>
#include <mutex>

class AsyncThread
{
    std::thread asyncthread;
    std::queue<void *> args;
    std::queue<void *(*)(void *)> funcs;
    std::queue<std::string> retids;
    std::map<std::string, void *> rets;

    bool execute;
    friend class AsyncThreadPool;
    std::mutex lock;

public:
    AsyncThread();
    ~AsyncThread();

    void push(void *(*func)(void *), void *args = nullptr, const std::string &iden = "");
    const void *const getret(const std::string &iden);
};

class AsyncThreadPool
{
    std::vector<AsyncThread> asynclist;

public:
    AsyncThreadPool(std::size_t count);
    ~AsyncThreadPool();

    void push(void *(*func)(void *), void *args = nullptr, const std::string &iden = "");
    const void *const getret(const std::string &iden);
};

extern std::chrono::microseconds silence_duration;

#endif
