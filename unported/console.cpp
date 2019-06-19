
/*
 *
 * An implimentation to prevent the framework from using its own console, and to
 * use sources console instead.
 *
 */

#include "../../../util/catvars.hpp"
#include "../../../util/logging.hpp"
#include "../../../util/strings.hpp"
#include "../hacks/ifaces.hpp"

#include "console.hpp"

namespace modules::source::framework::console {

class CatConVar : public ConVar {
   public:
    CatConVar(CatVar* _cat_var)
        : ConVar(_cat_var->command_name.c_str(), _cat_var->GetValue().c_str(),
                 0, _cat_var->desc),
          cat_var(_cat_var) {}
    CatVar* cat_var;
    virtual void SetValue(const char* value) {
        cat_var->Callback(sepstr(value));
    }
};
class CatConCommand : public ConCommand {
   public:
    CatConCommand(CatCommand* _cat_com)
        : ConCommand(_cat_com->name.c_str(), [](const CCommand& con_args) {
              CallCommand(con_args.GetCommandString());
          }) {}
};

void Init() {
    // Register Commands and vars
    for (auto& tmp : CatCommand::GetList())
        iface::cvar->RegisterConCommand(new CatConCommand(tmp));
    for (auto& tmp : CatVar::GetList())
        iface::cvar->RegisterConCommand(new CatConVar(tmp));
    // ensure we sync the catvar values
    CatVar::change_event.Listen([](CatVar* changed) {
        auto con_var = iface::cvar->FindVar(changed->command_name.c_str());
        if (con_var) con_var->ConVar::SetValue(changed->GetValue().c_str());
    });

    // Logging into console ;)
    debug_log.log_event.Listen(
        [](std::string& in) { iface::cvar->ConsolePrintf("%s", in.c_str()); });
    debug_log.Puts("Now using game console!");
}

}  // namespace modules::source::framework::console
