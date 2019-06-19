
/*
 *
 *	Init the below to Hook stuff
 *
 */

#pragma once

#include "../sdk/sdk.hpp"

namespace modules::source::hooks {

// Used to mess with user commands
extern CUserCmd* ui_cmd;   // For functions not directly in cm but ones that use
                           // the cmd, only use in world tick
extern bool using_silent;  // dont touch, only for use by the
                           // SetSilentCameraAngle function

void Init();

}  // namespace modules::source::hooks
