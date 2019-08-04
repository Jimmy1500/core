#include "Repository.h"

unique_ptr<SessionPool> Repository::repositoryPool = nullptr;
size_t Repository::repositoryCount = 0;
size_t Repository::repositoryMask = 0;

void Repository::popById(int id, DAO::Tenant & tenant) {
    Session session(repositoryPool->get());
    Poco::Data::Statement select(session);
    select << "SELECT * FROM tenant WHERE tenant_id = ?",
           Keywords::use(id),
           Keywords::into(tenant.id),
           Keywords::into(tenant.name),
           Keywords::now;
}
