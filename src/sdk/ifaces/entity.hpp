
#pragma once

#include "../vft.hpp"

namespace sourcesdk {
using namespace neko;

class Entity;
class EntityList {
public:
    Entity* GetEntity(int idx) {
        using Func = Entity* (*)(int);
        return GetVirtual<Func>(this, 3)(idx);
    }
    int GetSize(){
        using Func = int (*)();
        return GetVirtual<Func>(this, 6)();
    }
};

}
