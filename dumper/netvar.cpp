
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

#include <fstream>
#include <string_view>
#include <sdk/client_class.hpp>

#include "ifaces.hpp"

#include "netvar.hpp"

namespace neko::dumper::netvar {
using namespace sourcesdk;

static std::string_view TypeToString(RecvProp::Type type) {
    switch (type) {
    case RecvProp::Type::kInt:
        return "INT";
    case RecvProp::Type::kFloat:
        return "FLOAT";
    case RecvProp::Type::kVector:
        return "VECTOR3";
    case RecvProp::Type::kVectorXY:
        return "VECTOR2";
    case RecvProp::Type::kArray:
        return "ARRAY";
    case RecvProp::Type::kString:
        return "STRING";
    case RecvProp::Type::kDataTable:
        return "TABLE";
    default:
        return "UNKNOWN";
    }
}
static std::fstream out("/tmp/netvardump", std::ios_base::out | std::ios_base::trunc);
static void DumpTable(const RecvTable* table, int recur = 0, int offset = 0) {
    if (!table) return;

    for (int i = 0; i < recur; i++)
        out << "    ";
    out << table->name << std::endl;

    for (int i = 0; i < table->size; i++) {
        const RecvProp& prop = table->props[i];
        if (!prop.name || isdigit(prop.name[0])) continue;

        int next_recur = recur + 1;
        for (int i = 0; i < next_recur; i++)
            out << "    ";

        int new_offset = offset + prop.offset;
        out << prop.name << " 0x" << std::hex << new_offset;
        out << ' ' << TypeToString(prop.type) << std::endl;

        if (prop.table)
            DumpTable(prop.table, next_recur, new_offset);
    }
}

void Dump() {
    for (ClientClass* cc = ifaces::base_client->GetAllClasses(); cc; cc = cc->next) {
        out << "Class: " << cc->name << std::endl;
        DumpTable(cc->table);
    }
}
};
