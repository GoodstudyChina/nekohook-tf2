
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

#include "sdk/ifaces/base_client.hpp"
#include "sdk/ifaces/engine.hpp"
#include "sdk/ifaces/cvar.hpp"
#include "sdk/ifaces/entity.hpp"

namespace neko::iface {
using namespace sourcesdk;

extern BaseClient* client;
extern EntityList* entity_list;
extern Engine* engine;
extern ICvar* cvar;
void Init();

}
