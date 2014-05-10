#include "asynclog.hpp"
#include <iostream>
#include <sstream>
#include <cstring>

#include <unistd.h>

asynclog::silly_log g_log;

class Object {
public:
    Object(int x_) : x(x_) {}
    Object(Object const& rhs) : x(-rhs.x)
    {
        //std::cout << "Object::Object(Object const&)" << std::endl;
    }
    ~Object() {}
    Object& operator=(Object const& rhs)
    {
        x = rhs.x;
        return *this;
    }

    int get() const { return x; }

    //{
    //    return (os << x);
    //}
private:
    int x;
};


char const* format(asynclog::output_buffer* pbuffer, char const* pformat, Object const& v)
{
    if(*pformat != 's')
        return nullptr;
    char const* fmt = "d";
    format(pbuffer, fmt, v.get());
    return pformat+1;
}

Object obj(3);

void thread_test()
{
    // TODO I don't think this call does what it should do.
    //g_log.write("three numbers: %s %d %d %s\n", 66, 3.0, obj);

    g_log.write("three numbers: %s %d %d %s\n", 'B', 66, 3.0, obj);
    g_log.write("three numbers: %s %d %d %s\n", 'B', 66, 3.0, obj);
    g_log.write("three numbers: %s %d %d %s\n", 'B', 66, 3.0, obj);
}

int main()
{
    // FIXME need to be able to move log object
    asynclog::file_writer writer("dlog.txt");
    g_log.open(&writer);
    std::thread thread(&thread_test);
    //log.write("three numbers: %s %d %d %s\n", 'A', 66, 3.0, obj);
    thread.join();
    g_log.close();
    return 0;
}
