
/*
 * Nekohook: Free, fast and modular cheat software
 * Copyright (C) 2018 Rebekah Rowe
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */
 
#pragma once

#include <neko/hook/util/math.hpp>
#include "netvar.hpp"
namespace sourcesdk {
using namespace neko;

class Entity {
private:
    static inline Netvar origin{{"DT_BaseEntity", "m_vecOrigin"}};
public:
    inline math::Vector GetOrigin() {
        return origin.Get<math::Vector>(this);
    }
};

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
