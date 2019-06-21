
/*
 *
 * Client Class, no idea what to call this.
 * Maybe just easy way to get class names
 *
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
    bool g1;
    void *g2, *g3, *g4, *g5, *g6;
    RecvTable* table;      // For RDT_DataTable.
    const int offset = 0;  // offset from an entity to the requested property
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
