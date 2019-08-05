#ifndef GLOBAL_H
#define GLOBAL_H

#include <mutex>
#include <string>
#include <vector>
#include <thread>
#include <iostream>
#include <functional>

static std::mutex mtx;

namespace HTTP {
    enum Method {
        GET = 0,
        POST,
        PUT,
        DELETE,
        PATCH,
        NUM_HTTP_METHODS
    };

    static std::map<std::string, Method> Methods
    ({
        {"GET",     GET},
        {"PUT",     PUT},
        {"POST",    POST},
        {"PATCH",   PATCH},
        {"DELETE",  DELETE}
    });
}

namespace DAO { // Data Access Objects
    struct Tenant {
        int         id;
        std::string name;
    };
}

namespace SYS {
    enum Flags {
        DB_SESS_POOL = 1,
        NUM_SYS_FLAGS
    };
}

namespace BIN {
    static inline size_t isDirty(size_t mask, size_t flag){ return mask & flag; }
    static inline void clearDirty(size_t mask, size_t flag){ mask &= ~flag; }
    static inline void markDirty(size_t mask, size_t flag){ mask |= flag; }
    static inline void clearDirtyAll(size_t mask){ mask = 0; }
    static inline void markDirtyAll(size_t mask, size_t begin, size_t end){
        size_t i; for ( i = begin; i < end; i <<= 1u){ markDirty(mask, i); }
    }
}
#endif
