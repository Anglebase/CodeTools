#pragma once
/**
 * 日志
 * --------------------------------------------
 * 默认全局异步IO对象：logger(通道名为Global)
 * 功能：
 * 日志输出(4个等级):
 * void LOGGER::debug(std::string);
 * void LOGGER::info(std::string);
 * void LOGGER::warning(std::string);
 * void LOGGER::error(std::string);
 *
 * 日志输出配置
 * void LOGGER::redirect(LOGGER_LEVEL,FILE*);   // 输出重定向
 * void LOGGER::setlesslevel(LOGGER_LEVEL);     // 修改输出等级
 *
 * 等级：ERROR > WARNING > INFO > DEBUG
 * 只有比所设置的输出等级高或相同的日志信息才会执行输出
 */
#ifndef _LOGGER_HXX_
#define _LOGGER_HXX_

#include <cstdio>
#include <string>
#include <queue>
#include <thread>
#include <sstream>

template <class... Args>
std::string str(Args... args)
{
    std::ostringstream oss;
    (oss << ... << args);
    return oss.str();
}

enum LOGGER_LEVEL
{
    DEBUG_level,
    INFO_level,
    WARNING_level,
    ERROR_level
};

class LOGGER
{
private:
    // 输出通道
    FILE *m_debug;
    FILE *m_info;
    FILE *m_warning;
    FILE *m_error;
    // 对象配置
    std::string m_logger_name;
    int m_output_level;
    // 异步
    std::thread IO;
    std::queue<void (*)(LOGGER *, std::string &)> buff;
    std::queue<std::string> str_buff;
    bool enable;

public:
    /**
     * 构造函数
     * @param logger_name 日志记录器名称
     */
    LOGGER(std::string logger_name);
    LOGGER(const LOGGER &) = delete;
    LOGGER(LOGGER &&) = delete;
    ~LOGGER();
    const LOGGER &operator=(const LOGGER &) = delete;
    const LOGGER &operator=(LOGGER &&) = delete;
    // 底层实现(直接调用则为同步IO)
    void debug__(std::string debug_message);
    void info__(std::string info_message);
    void warning__(std::string warning_message);
    void error__(std::string error_message);
    // 转发接口(异步IO实现)
    /**
     * 输出 DEBUG 级日志
     * @param debug_message 日志内容
     */
    void debug(std::string debug_message);
    /**
     * 输出 INFO 级日志
     * @param info_message 日志内容
     */
    void info(std::string info_message);
    /**
     * 输出 WARNING 级日志
     * @param warning_message 日志内容
     */
    void warning(std::string warning_message);
    /**
     * 输出 ERROR 级日志
     * @param error_message 日志内容
     */
    void error(std::string error_message);
    // 日志配置
    /**
     * 通道重定向
     * @param passaway 被重定向的通道
     * @param new_path 重定向的目标
     */
    void redirect(LOGGER_LEVEL passaway, FILE *new_path);
    /**
     * 修改日志输出等级
     * @param level 最低的输出等级
     */
    void setlesslevel(LOGGER_LEVEL level);
    /**
     * 缓冲区是否已清空
     */
    bool finish();
};

// 默认全局日志对象，它的日志名称为Global
extern LOGGER logger;
#define here(...) str(__FILE__, ": ", __LINE__, "| ", __VA_ARGS__)
#define DEFAULT_OUTPUT stdout
#define NEW_OUTPUT(path) fopen(path, "w")

#endif