#ifndef REPOSITORY_H
#define REPOSITORY_H

#include <Poco/Data/Time.h>
#include <Poco/Data/Session.h>
#include <Poco/Data/SessionPool.h>
#include <Poco/Data/SessionFactory.h>
#include <Poco/Data/MySQL/Connector.h>
#include <Poco/Data/MySQL/MySQLException.h>

#include "Global.h"

using namespace std;
using namespace Poco::Data;

class Repository {
    private:
        static unique_ptr<SessionPool> repositoryPool;
    public:
        Repository() {
            Poco::Data::MySQL::Connector::registerConnector();
        }

        ~Repository() {
            Poco::Data::MySQL::Connector::unregisterConnector();
        }

        static inline size_t existsPool() {
            return BIN::isDirty(SYS::registry.repositoryMask, SYS::DB_SESSION_POOL);
        }

        static inline void initialize(string& connector, string& connectionString,
                size_t minSessions, size_t maxSessions, size_t idleTime)
        {
            if (!existsPool()) {
                repositoryPool = make_unique<SessionPool>(
                        connector,
                        connectionString,
                        minSessions,
                        maxSessions,
                        idleTime);
                BIN::markDirty(SYS::registry.repositoryMask, SYS::DB_SESSION_POOL);
                cout << "### Database session pool created!" << endl;
            }
        }

        static inline void uninitialize() {
            if (existsPool()) {
                repositoryPool.reset();
                BIN::clearDirty(SYS::registry.repositoryMask, SYS::DB_SESSION_POOL);
                cout << "### Database session pool destroyed!" << endl;
            }
        }

        void popById(int, DAO::Tenant &);
        void popAll(vector<DAO::Tenant> &);

};

#endif
