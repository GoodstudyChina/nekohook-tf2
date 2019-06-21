
#pragma once

#include "../vft.hpp"

#include "../client_class.hpp"

namespace sourcesdk {

class BaseClient {
public:
    ClientClass* GetAllClasses() {
        using Func = ClientClass* (*)();
        return GetVirtual<Func>(this, sizeof(void*) * 8)();
    }
};

}
