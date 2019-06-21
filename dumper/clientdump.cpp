
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
