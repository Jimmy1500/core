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
    enum Database {
        DB_SESS_POOL = 1,
        NUM_DB_FLAGS
    };
}

namespace BIN {
    static inline size_t IsDirty(size_t mask, size_t flag){ return mask & flag; }
    static inline void ClearDirty(size_t mask, size_t flag){ mask &= ~flag; }
    static inline void MarkDirty(size_t mask, size_t flag){ mask |= flag; }
    static inline void ClearDirtyAll(size_t mask){ mask = 0; }
    static inline void MarkDirtyAll(size_t mask, size_t begin, size_t end){
        size_t i; for ( i = begin; i < end; i <<= 1u){ MarkDirty(mask, i); }
    }
}
#endif
