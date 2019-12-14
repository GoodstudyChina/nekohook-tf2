
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

namespace sourcesdk {

class RecvTable;
class RecvProp {
public:
    const char* name;
    enum Type {
        kInt,
        kFloat,
        kVector,
        kVectorXY,  // Only encodes the XY of a vector, ignores Z
        kString,
        kArray,  // An array of the base types (can't be of datatables).
        kDataTable,
        kInt64
    };
    const Type type = Type::kInt;
    const int flags = 0;
    const int str_size = 0;
private:
    bool g1;
    void *g2, *g3, *g4, *g5, *g6;
public:
    RecvTable* table;      // For RDT_DataTable.
    const int offset = 0;  // offset from an entity to the requested property
private:
    int g7, g8;
    void* g9;
};

class RecvTable {
public:
    const RecvProp* props;
    const int size = 0;
    const void* decoder;
    const char* name;
};

class ClientClass {
    void *g1, *g2;
public:
    const char* name;
    RecvTable* table;
    ClientClass* next;
    int id;  // Managed by the engine.
};

}
