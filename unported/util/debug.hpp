
/*
 *
 *	A Class dumper.
 * 	Use this as a development tool!
 *	Note: Inject ingame otherwise you get nothing useful
 *
 */

#pragma once

#include "../hacks/interfaces.hpp"

namespace modules::source {

// Use to dump classes
class ClassDumper : private CatLogger {
   public:
    ClassDumper() : CatLogger("/tmp/classdump.txt") {}

    void SaveDump() {
        ClientClass* cc = g_IBaseClient->GetAllClasses();
        while (cc) {
            log("[%d] %s", cc->m_ClassID, cc->GetName());
            cc = cc->m_pNext;
        }
    }
};

// Use to dump netvars
class NetvarDumper : private CatLogger {
   public:
    NetvarDumper() : CatLogger("/tmp/netdump.txt") {}

    void SaveDump() {
        ClientClass* cc = g_IBaseClient->GetAllClasses();
        while (cc != nullptr) {
            DumpTable(cc->m_pRecvTable);
            cc = cc->m_pNext;
        }
    }

   private:
    void DumpTable(RecvTable* pTable, int recurstions = 0,
                   int parent_offset = 0) {
        if (pTable == nullptr) return;

        // Print tabs for how far we are
        for (int i = 0; i < recurstions; i++) log("%s", "  ");

        // Print the table name
        log("%s%s", pTable->GetName(), "\n");

        // Loop thru children
        for (int i = 0; i < pTable->GetNumProps(); i++) {
            // Get new prop
            RecvProp* pProp = pTable->GetProp(i);

            // Checks
            if (pProp == nullptr) continue;
            if (isdigit(pProp->GetName()[0])) continue;

            // one loggy boi
            log("%s%s%s%s%s%s%s%s", pProp->GetName(), " : 0x",
                pProp->GetOffset(), " (0x", parent_offset + pProp->GetOffset(),
                ") [", TypeToString(pProp->GetType()), "]\n");

            // Check for children
            if (pProp->GetDataTable()) {
                // Indent for child
                for (int i = 0; i < recurstions; i++) log("%s", "  ");

                // Recurse into child
                DumpTable(pProp->GetDataTable(), recurstions + 1,
                          pProp->GetOffset());
            }
        }
    }
    // This makes things easy
    const char* TypeToString(SendPropType type) const {
        switch (type) {
            case DPT_Int:
                return "INT";
            case DPT_Float:
                return "FLOAT";
            case DPT_Vector:
                return "VECTOR3";
            case DPT_VectorXY:
                return "VECTOR2";
            case DPT_Array:
                return "ARRAY";
            case DPT_String:
                return "STRING";
            case DPT_DataTable:
                return "TABLE";
            default:
                return "UNKNOWN";
        }
    }
};

}  // namespace modules::source
