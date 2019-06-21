
#pragma once

#include <neko/hook/util/math.hpp>

namespace sourcesdk {

class Entity {
public:
    static inline 
    math::Vector GetOrigin() {

    }
};

class EntityList {
public:
    Entity* GetEntity(int idx) {
        using Func = Entity* (*)(int);
        return GetVirtual<Func>(this, 0x0C)(idx);
    }
    int GetSize(){
        using Func = int (*)();
        return GetVirtual<Func>(this, 0x18)();
    }
};


}
