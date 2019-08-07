#ifndef GLOBAL_H
#define GLOBAL_H

#include <mutex>
#include <string>
#include <vector>
#include <thread>
#include <iostream>
#include <functional>

#ifndef FOREACH
#define FOREACH(X, A)   \
        for (X) { A }
#endif

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
        Tenant() = default;
        Tenant(int id, std::string & name) : id(id), name(name) { }
        int         id;
        std::string name;
    };

    struct User {
        User() = default;
        User(std::string& id, std::string& firstName, std::string& middleName, std::string& lastName) :
            id(id),
            firstName(firstName),
            middleName(middleName),
            lastName(lastName) { }

        std::string id;
        std::string firstName;
        std::string middleName;
        std::string lastName;
    };
}

namespace SYS {
    enum Flags {
        DB_SESSION_POOL = 1,
        NUM_SYS_FLAGS
    };

    struct Registry {
        size_t controllerCount;
        size_t repositoryMask;
    };

    static volatile Registry registry{0, 0};
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
