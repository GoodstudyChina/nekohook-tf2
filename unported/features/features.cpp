
/*
 *
 *	The cheats file that manages the additional cheat features added by this
 *module
 *
 *
 */

#include "aimbot.hpp"
#include "airstuck.hpp"
#include "esp.hpp"  // Stuff to init
#include "misc.hpp"
#include "spam.hpp"

#include "features.hpp"

namespace modules::source::features {

void Init() {
    aimbot::Init();

    esp::Init();

    airstuck::Init();

    spam::Init();

    misc::Init();
}

}  // namespace modules::source::features
