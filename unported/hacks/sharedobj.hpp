
/*
 *
 *	Header for use of our shared objects
 *
 *
 */

#pragma once

#include "../../../hacks/sharedobj.hpp"  // For the normal shared obj class

class SourceSo : public SharedObject {
   public:
    using SharedObject::SharedObject;
    void* CreateIFace(const char*);

   private:
    // Source literally allows us to ask for things, this is really useful
    using CreateIFace_t = void* (*)(const char*, int*);
    CreateIFace_t fptr = nullptr;
};

namespace modules::source::sharedobj {

// Create move hook + controling attack and movement
extern SourceSo client;

extern SourceSo server;
// For player names
extern SourceSo engine;
// Used for surface drawing
extern SourceSo vgui2;
// CVars
extern SourceSo vstdlib;
// Used for our paint traverse hook
extern SourceSo vguimatsurface;

}  // namespace modules::source::sharedobj
