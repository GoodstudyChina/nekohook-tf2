
#include <cstring>
#include <fstream>
#include <iostream>
#include <hack/sharedlibrary.hpp>

#include "ifaces.hpp"

namespace neko::dumper::ifaces {
using namespace sourcesdk;


BaseClient* base_client = nullptr;

void Init() {
    hack::SharedLibrary clientso("client");
    clientso.ForceInit();
    using CreateIFace_t = void* (*)(const char*, int*);
    CreateIFace_t CreateIFace;
    CreateIFace = clientso.GetSym<CreateIFace_t>("CreateInterface");

    char iface_name[16] = "VClient";
    std::size_t size = strlen(&iface_name[0]);
    for (int i = 0; i < 999; i++) {
        sprintf(&iface_name[size], "%03i", i);
        base_client = reinterpret_cast<BaseClient*>(CreateIFace(iface_name, nullptr));
        if (base_client)
            break;
    }
    if(!base_client)
        throw std::runtime_error("can't create interface!");
}

}
