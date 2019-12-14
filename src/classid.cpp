
#include "iface.hpp"

#include "classid.hpp"

namespace neko {

ClassId::ClassId(std::string_view _name) : name(_name) {
    ClassId::list.push(this);
}

void ClassId::Init() {
    for (ClassId* i = list.front(); !list.empty(); list.pop(), i = list.front()) {
        for (ClientClass* c = iface::client->GetAllClasses(); c != nullptr; c = c->next) {
            if (i->name == c->name) {
                i->id = c->id;
                break;
            }
        }
        if (i->id == -1)
            throw std::runtime_error("Unable to find classid: " + std::string(i->name));
    }
}

}
