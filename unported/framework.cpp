
/*
 *
 *	The framework interface init file used for connecting the interfaces
 *with the rest of the module.
 *
 */

// Stuff to init
#include "console.hpp"
#include "entitys.hpp"
#include "trace.hpp"

#include "framework.hpp"

namespace modules::source::framework {

void Init() {
    // Init the entity mgr so the framework knows about what entitys we have
    entitys::Init();

    // Ray tracing!
    trace::Init();

    // Load up cvars
    console::Init();
}

}  // namespace modules::source::framework
