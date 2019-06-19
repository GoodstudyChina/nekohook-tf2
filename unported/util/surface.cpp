
/*
 *
 *	Welcome to the Valve surface drawing module!
 *	Run the init function below if you wish to use this drawing module.
 *	You must be using a source 2013 game with access to the valve surface
 *interface. -Onee
 *
 */

#include <algorithm>  // Min() Max()
#include <queue>      // ez stuff

#include "../../../framework/drawing.hpp"  // To pass our functions to the drawing mgr
#include "../../../util/logging.hpp"  // We log stuff
#include "../../../util/strings.hpp"  // to format stuff
#include "../hacks/ifaces.hpp"        // We use the surface interface from here

#include "surface.hpp"

namespace modules::source::valvesurface {

// allows us to convert from rgba into something valve surface understands
static inline Color convert(const CatColor& _color) {
    return Color(_color.r, _color.g, _color.b, _color.a);
}

static void Line(int x, int y, int w, int h, CatColor color) {
    iface::surface->DrawSetColor(convert(color));
    iface::surface->DrawLine(x, y, x + w, y + h);
}

static void Rect(int x, int y, int w, int h, CatColor color) {
    iface::surface->DrawSetColor(convert(color));
    iface::surface->DrawOutlinedRect(x, y, x + w, y + h);
}

static void RectFilled(int x, int y, int w, int h, CatColor color) {
    iface::surface->DrawSetColor(convert(color));
    iface::surface->DrawFilledRect(x, y, x + w, y + h);
}

// Font system, We only use 12 fonts to save on memory
static unsigned long surface_fonts[static_cast<int>(draw::Font::kFontCount)]
                                  [32];

// Get string height and length in pixels
static std::pair<int, int> StringLength(const char* string, int font,
                                        int size) {
    // Convert our string to wchar
    wchar_t buf[1024];
    mbstowcs(buf, string, sizeof(buf));
    // Draw
    std::pair<int, int> ret;
    iface::surface->GetTextSize(surface_fonts[font][size], buf, ret.first,
                                ret.second);
    return ret;
}

// String drawing system
// Main string draw
static void String3(const char* text, int x, int y, int font, int size,
                    CatColor color) {
    // Set Precursor
    iface::surface->DrawSetTextPos(x, y);
    iface::surface->DrawSetTextColor(convert(color));
    iface::surface->DrawSetTextFont(surface_fonts[font][size]);
    // Conversion from char to wchar
    wchar_t buf[1024];
    mbstowcs(buf, text, 1024);
    // Draw
    iface::surface->DrawUnicodeString(buf);
}
// Shadow processor
#define shadow        \
    CatColor(0, 0, 0, \
             255)  // We use full as a transparent can make things weird
static void String2(const char* text, int x, int y, int font, int size,
                    CatColor color) {
    // We make a nice outline with this
    String3(text, x - 2, y - 2, font, size, shadow);  // top left
    String3(text, x + 2, y - 2, font, size, shadow);  // top right
    String3(text, x - 2, y + 2, font, size, shadow);  // bot left
    String3(text, x + 2, y + 2, font, size, shadow);  // bot right
    String3(text, x, y - 2, font, size, shadow);      // top center
    String3(text, x, y + 2, font, size, shadow);      // bot center
    String3(text, x - 2, y, font, size, shadow);      // left center
    String3(text, x + 2, y, font, size, shadow);      // right center

    String3(text, x, y, font, size, color);
}
// String input, fixes newlines
static void String1(const char* text, int x, int y, int font, int size,
                    CatColor color) {
    // We check for newlines as newlines result in a space and dont do anything
    // other than that
    size_t len = strlen(text);
    for (int i = 0; i < len; i++) {
        if (text[i] != '\n') continue;

        // We hit a newline, abandon ship bois
        std::string buf = text;
        for (char& ii : buf)
            if (ii == '\n') ii = '*';

        String2(buf.c_str(), x, y, font, size, color);
        return;
    }

    String2(text, x, y, font, size, color);
}

// Use this to init our draw functions to the drawing mgr
void Init() {
    debug_log.Fmt("Begin Valve Surface Draw Module Init...");

    // Per font
    for (int i = 0; i < static_cast<int>(draw::Font::kFontCount); i++) {
        // Per size
        for (int ii = 0; ii < 32; ii++) {
            surface_fonts[i][ii] = iface::surface->CreateFont();
            iface::surface->SetFontGlyphSet(
                surface_fonts[i][ii], draw::Fonts[i], ii + 15, 0, 0, 0, 0);
        }
    }

    // Pass the surface functions to the drawmodule manager
    draw::Line = Line;
    draw::Rect = Rect;
    draw::RectFilled = RectFilled;
    draw::String = String1;
    draw::GetStringLength = StringLength;
}

}  // namespace modules::source::valvesurface
