
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

#include <initializer_list>
#include <string_view>
#include <queue>

#include "ifaces/base_client.hpp"

namespace sourcesdk {

class Entity;
class Netvar {
    using MapType = std::initializer_list<std::string_view>;
public:
    Netvar(MapType _map);
    template <typename Type>
    inline Type& Get(Entity* entity) {
        return *reinterpret_cast<Type*>(reinterpret_cast<uintptr_t>(entity) + this->offset);
    }
    static void Init(BaseClient*);
private:
    MapType map;
    std::ptrdiff_t offset = 0;
    static inline std::queue<Netvar*> list;
};


}
