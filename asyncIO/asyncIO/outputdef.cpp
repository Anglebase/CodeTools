#include "include/outputdef.hpp"
/**
 * @note 浮点类型转换效率偏低
 */

#include <cstdlib>
#include <sstream>
using namespace std;
using namespace pack_asyncIO;

TypesafeAsyncOutput &pack_asyncIO::operator<<(TypesafeAsyncOutput &out, short n)
{
    char s[7];
    itoa(n, s, 10);
    return out << (const char *)s;
}
TypesafeAsyncOutput &pack_asyncIO::operator<<(TypesafeAsyncOutput &out, int n)
{
    char s[12];
    itoa(n, s, 10);
    return out << (const char *)s;
}
TypesafeAsyncOutput &pack_asyncIO::operator<<(TypesafeAsyncOutput &out, long n)
{
    char s[12];
    itoa(n, s, 10);
    return out << (const char *)s;
}
TypesafeAsyncOutput &pack_asyncIO::operator<<(TypesafeAsyncOutput &out, long long n)
{
    char s[21];
    itoa(n, s, 10);
    return out << (const char *)s;
}
TypesafeAsyncOutput &pack_asyncIO::operator<<(TypesafeAsyncOutput &out, unsigned short n)
{
    char s[6];
    itoa(n, s, 10);
    return out << (const char *)s;
}
TypesafeAsyncOutput &pack_asyncIO::operator<<(TypesafeAsyncOutput &out, unsigned int n)
{
    char s[11];
    itoa(n, s, 10);
    return out << (const char *)s;
}
TypesafeAsyncOutput &pack_asyncIO::operator<<(TypesafeAsyncOutput &out, unsigned long n)
{
    char s[11];
    itoa(n, s, 10);
    return out << (const char *)s;
}
TypesafeAsyncOutput &pack_asyncIO::operator<<(TypesafeAsyncOutput &out, unsigned long long n)
{
    char s[20];
    itoa(n, s, 10);
    return out << (const char *)s;
}
TypesafeAsyncOutput &pack_asyncIO::operator<<(TypesafeAsyncOutput &out, float n)
{
    ostringstream oss;
    oss << n;
    return out << oss.str().c_str();
}
TypesafeAsyncOutput &pack_asyncIO::operator<<(TypesafeAsyncOutput &out, double n)
{
    ostringstream oss;
    oss << n;
    return out << oss.str().c_str();
}
TypesafeAsyncOutput &pack_asyncIO::operator<<(TypesafeAsyncOutput &out, long double n)
{
    ostringstream oss;
    oss << n;
    return out << oss.str().c_str();
}
TypesafeAsyncOutput &pack_asyncIO::operator<<(TypesafeAsyncOutput &out, const std::string &s)
{
    return out << s.c_str();
}
