#include "Repository.h"

unique_ptr<SessionPool> Repository::repositoryPool = nullptr;
size_t Repository::repositoryCount = 0;
size_t Repository::repositoryMask = 0;

void Repository::popById(int id, DAO::Tenant& tenant) {
    mtx.lock(); Session session(repositoryPool->get()); mtx.unlock();
    Poco::Data::Statement select(session);
    select << "SELECT * FROM tenant WHERE tenant_id = ?",
           Keywords::use(id),
           Keywords::into(tenant.id),
           Keywords::into(tenant.name),
           Keywords::now;
}

void Repository::popAll(vector<DAO::Tenant>& tenants) {
    mtx.lock(); Session session(repositoryPool->get()); mtx.unlock();
    Poco::Data::Statement select(session);

    int id; string name;
    select << "SELECT * FROM tenant",
           Keywords::into(id),
           Keywords::into(name),
           Keywords::range(0, 1);

    while (!select.done()) {
        select.execute();
        tenants.emplace_back(id, name);
    }
}
