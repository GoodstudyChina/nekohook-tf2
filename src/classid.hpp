
#pragma once

#include <queue>

#include "sdk/entity.hpp"

namespace neko {
using namespace sourcesdk;

class ClassId {
public:
    ClassId(std::string_view);
    template<typename T>
    inline T* Get(Entity* entity){
        return (entity->GetClientClass()->id == this->id) ? reinterpret_cast<T*>(entity) : nullptr;
    }
    int GetId() { return this->id; }
    static void Init();
private:
    std::string_view name;
    int id = -1;
    static inline std::queue<ClassId*> list;
};

}
