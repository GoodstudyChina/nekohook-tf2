
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
std::fstream out("/tmp/netvardump", std::ios_base::out | std::ios_base::trunc);
static void DumpTable(const RecvTable* table, int recur = 0, int offset = 0) {
    if (!table) return;
    for (int i = 0; i < recur; i++)
        out << "    ";
    out << table->name << std::endl;
    for (int i = 0; i < table->size; i++) {
        const RecvProp* prop = &table->props[i];
        if (!prop) continue;
        if (isdigit(prop->name[0])) continue;
        for (int i = 0; i < recur + 1; i++)
            out << "    ";
        out << prop->name << " 0x" << std::hex << offset + prop->offset;
        out << ' ' << TypeToString(prop->type) << std::endl;
        if (prop->table)
            DumpTable(prop->table, recur + 1, offset + prop->offset);
    }
}

void Dump() {
    for (ClientClass* cc = ifaces::base_client->GetAllClasses(); cc; cc = cc->next)
        DumpTable(cc->table);
}
};
