
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
 
#include <sdk/client_class.hpp>
#include <fstream>

#include "ifaces.hpp"

namespace neko::dumper::clientclass {
using namespace sourcesdk;

void Dump(){
    std::fstream out("/tmp/classdump", std::ios_base::out | std::ios_base::trunc);
    out << "Client Class dump begin" << std::endl;
    for (ClientClass* cc = ifaces::base_client->GetAllClasses(); cc; cc = cc->next)
        out << '[' << cc->id << "] " << cc->name << std::endl;
    out << "Client Class dump end" << std::endl;
}

}
