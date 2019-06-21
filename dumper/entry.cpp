
#include "ifaces.hpp"
#include "clientdump.hpp"
#include "netvar.hpp"
#include <thread>
namespace neko::dumper {

void __attribute__((constructor)) Init(){
    std::thread t([](){
        ifaces::Init();
        clientclass::Dump();
        netvar::Dump();
    });
    t.detach();
}

}
