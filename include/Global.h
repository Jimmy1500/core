#ifndef GLOBAL_H
#define GLOBAL_H

#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <mutex>

static std::mutex mtx;

namespace HTTP {
    enum Method {
        GET = 0,
        POST,
        PUT,
        DELETE,
        PATCH
    };

    static std::map<std::string, Method> Methods = {
        {"GET",     GET},
        {"POST",    POST},
        {"PUT",     PUT},
        {"PATCH",   PATCH},
        {"DELETE",  DELETE}
    };
}

#endif
