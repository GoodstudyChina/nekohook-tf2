
/*
 *
 *  Util init file
 *
 *
 */

#include "../../../framework/game.hpp"
#include "classes.hpp"
#include "netvars.hpp"
#include "worldtoscreen.hpp"
#if defined(NEKO_TF2)
#include "conditions.hpp"
#endif
#include "util.hpp"

namespace modules::source::util {

void Init() {
    events::world.Listen(classes::Init, cat::Order::kBefore);

    netvars::Init();

    wts::Init();

#if defined(NEKO_TF2)
    conditions::Init();
#endif
}

}  // namespace modules::source::util
