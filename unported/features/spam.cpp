
#include "../../../features/spam.hpp"
#include "../hacks/ifaces.hpp"

#include "spam.hpp"

namespace modules::source::features::spam {

void Init() {
    ::features::spam::SayChat = [](const std::string& in) {
        iface::engine->ServerCmd(("say \"" + in + "\"").c_str());
    };
}

}  // namespace modules::source::features::spam
