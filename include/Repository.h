#ifndef REPOSITORY_H
#define REPOSITORY_H

#include <Poco/Data/MySQL/Connector.h>
#include <Poco/Data/Time.h>
#include <Poco/Data/Session.h>
#include <Poco/Data/SessionPool.h>
#include <Poco/Data/SessionFactory.h>
#include <Poco/Data/MySQL/MySQLException.h>

#include "Global.h"

using namespace std;
using namespace Poco::Data;

class Repository {
    private:
        static unique_ptr<SessionPool> RepositoryPool;
        static size_t RepositoryCount;
        static size_t RepositoryMask;
    public:
        Repository() {
            Poco::Data::MySQL::Connector::registerConnector(); ++RepositoryCount;
            cout << MySQL::Connector::KEY << " connector # " << RepositoryCount << " registered!" << endl;
        }

        ~Repository() {
            Poco::Data::MySQL::Connector::unregisterConnector(); --RepositoryCount;
            cout << MySQL::Connector::KEY << " connector # " << RepositoryCount << " unregistered!" << endl;
        }

        static inline void PowerOn(string& connector, string& connectionString,
                size_t minSessions, size_t maxSessions, size_t idleTime)
        {
            if (!BIN::IsDirty(RepositoryMask, SYS::DB_SESS_POOL)) {
                RepositoryPool = make_unique<SessionPool>(
                        connector, connectionString, minSessions, maxSessions, idleTime);
                BIN::MarkDirty(RepositoryMask, SYS::DB_SESS_POOL);
            }
            cout << "Database session pool created!" << endl;
        }

        static inline void PowerOff() {
            if (BIN::IsDirty(RepositoryMask, SYS::DB_SESS_POOL)) {
                RepositoryPool.reset();
                BIN::ClearDirty(RepositoryMask, SYS::DB_SESS_POOL);
            }
            cout << "Database session pool destroyed!" << endl;
        }

        void PopById(int id, DAO::Tenant & tenant) {
            Session session(RepositoryPool->get());
            Poco::Data::Statement select(session);
            select << "SELECT * FROM tenant WHERE tenant_id = ?",
                   Keywords::use(id),
                   Keywords::into(tenant.id),
                   Keywords::into(tenant.name),
                   Keywords::now;
        }

};

unique_ptr<SessionPool> Repository::RepositoryPool = nullptr;
size_t Repository::RepositoryCount = 0;
#endif
