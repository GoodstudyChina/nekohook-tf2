
/*
 *
 * Client Class, no idea what to call this.
 * Maybe just easy way to get class names
 *
 */

#pragma once

#include "../public/icliententity.h"

enum PropType {
    DPT_Int,
    DPT_Float,
    DPT_Vector,
    DPT_VectorXY,  // Only encodes the XY of a vector, ignores Z
    DPT_String,
    DPT_Array,  // An array of the base types (can't be of datatables).
    DPT_DataTable,
    DPT_Int64
};

class RecvTable;
class RecvProp {
   public:
    const char* name;
    const PropType type = DPT_Int;
    const int flags = 0;
    const int str_size = 0;
    bool g1;
    void *g2, *g3, *g4, *g5, *g6;
    RecvTable* table;      // For RDT_DataTable.
    const int offset = 0;  // offset from an entity to the requested property
};

class CRecvDecoder;
class RecvTable {
   public:
    const RecvProp* props;
    const int size = 0;
    const CRecvDecoder* decoder;
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
