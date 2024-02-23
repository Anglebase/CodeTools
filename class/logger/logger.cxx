#include "logger.hxx"
#include <cstdio>
#include <string>
#include <ctime>
#include <queue>
#include <thread>
#include <sstream>
using namespace std;

string time_format()
{
    auto t = time(nullptr);
    auto tc = localtime(&t);
    ostringstream oss;
    oss << tc->tm_year + 1900 << "-" << tc->tm_mon << "-" << tc->tm_mday << " "
        << tc->tm_hour << ":" << tc->tm_min << ":" << tc->tm_sec;
    return oss.str();
}

LOGGER::LOGGER(string name)
    : m_debug(stdout), m_info(stdout), m_warning(stdout), m_error(stdout),
      m_logger_name(name), m_output_level(DEBUG_level),
      buff{}, str_buff{}, enable{true},
      IO{[&]()
         {
             while (this->enable)
             {
                 if (this->buff.empty())
                 {
                     this_thread::sleep_for(100ms);
                     continue;
                 }
                 try
                 {
                     this->buff.front()(this, this->str_buff.front());
                     this->buff.pop();
                     this->str_buff.pop();
                 }
                 catch (...)
                 {
                     ;
                 }
             }
         }}
{
}

LOGGER::~LOGGER()
{
    fclose(this->m_debug);
    fclose(this->m_info);
    fclose(this->m_warning);
    fclose(this->m_error);
    // this_thread::sleep_for(3s);
    this->enable = false;
    this->IO.join();
}

void LOGGER::debug__(string msg)
{
    if (this->m_output_level > DEBUG_level)
        return;
    fputs(
        str(this->m_debug == stdout || this->m_debug == stderr ? "\033[37m" : "",
            time_format(),
            "|from [", this->m_logger_name, ']',
            "[DEBUG]: ", msg,
            this->m_debug == stdout || this->m_debug == stderr ? "\033[0m\n" : "\n")
            .c_str(),
        this->m_debug);
}
void LOGGER::info__(string msg)
{
    if (this->m_output_level > INFO_level)
        return;
    fputs(
        str(this->m_info == stdout || this->m_info == stderr ? "\033[32m" : "",
            time_format(),
            "|from [", this->m_logger_name, ']',
            "[INFO]: ", msg,
            this->m_info == stdout || this->m_info == stderr ? "\033[0m\n" : "\n")
            .c_str(),
        this->m_info);
}
void LOGGER::warning__(string msg)
{
    if (this->m_output_level > WARNING_level)
        return;
    fputs(
        str(this->m_warning == stdout || this->m_warning == stderr ? "\033[33m" : "",
            time_format(),
            "|from [", this->m_logger_name, ']',
            "[WARNING]: ", msg,
            this->m_warning == stdout || this->m_warning == stderr ? "\033[0m\n" : "\n")
            .c_str(),
        this->m_warning);
}
void LOGGER::error__(string msg)
{
    if (this->m_output_level > ERROR_level)
        return;
    fputs(
        str(this->m_error == stdout || this->m_error == stderr ? "\033[31m" : "",
            time_format(),
            "|from [", this->m_logger_name, ']',
            "[ERROR_]: ", msg,
            this->m_error == stdout || this->m_error == stderr ? "\033[0m\n" : "\n")
            .c_str(),
        this->m_error);
}

void LOGGER::debug(string msg)
{
    this->buff.push([](LOGGER *it, string &str)
                    { it->debug__(str); });
    this->str_buff.push(msg);
}

void LOGGER::info(string msg)
{
    this->buff.push([](LOGGER *it, string &str)
                    { it->info__(str); });
    this->str_buff.push(msg);
}

void LOGGER::warning(string msg)
{
    this->buff.push([](LOGGER *it, string &str)
                    { it->warning__(str); });
    this->str_buff.push(msg);
}

void LOGGER::error(string msg)
{
    this->buff.push([](LOGGER *it, string &str)
                    { it->error__(str); });
    this->str_buff.push(msg);
}

void LOGGER::redirect(LOGGER_LEVEL passaway, FILE *new_path)
{
    while (!this->finish())
        this_thread::sleep_for(10ms);

    switch (passaway)
    {
    case DEBUG_level:
        this->m_debug = new_path;
        break;
    case INFO_level:
        this->m_info = new_path;
        break;
    case WARNING_level:
        this->m_warning = new_path;
        break;
    case ERROR_level:
        this->m_error = new_path;
        break;
    }
}

void LOGGER::setlesslevel(LOGGER_LEVEL level)
{
    while (!this->finish())
        this_thread::sleep_for(10ms);
    this->m_output_level = level;
}

bool LOGGER::finish() { return this->buff.empty(); }

LOGGER logger{"Global"};