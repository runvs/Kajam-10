#ifndef GAME_GAMEPROPERTIES_HPP_INCLUDEGUARD
#define GAME_GAMEPROPERTIES_HPP_INCLUDEGUARD

#include "Color.hpp"
#include <string>

class GP {
public:
    GP() = delete;

    static std::string GameName() { return "MyAwesomeGame"; }
    static std::string AuthorName() { return "Thunraz, oOBloodyOrangeOo and Laguna"; }
    static std::string JamName() { return "The Multiplayer Kajam"; }
    static std::string JamDate() { return "May 2021"; }

    static jt::Color PaletteBackground() { return GP::PaletteColor6(); }
    static jt::Color PaletteFontFront() { return GP::PaletteColor4(); }
    static jt::Color PaletteFontDanger() { return GP::PaletteColor5(); }
    static jt::Color PaletteFontShadow() { return GP::PaletteColor7(); }

    static jt::Color PaletteColor1() { return jt::Color { 151, 85, 85 }; }
    static jt::Color PaletteColor2() { return jt::Color { 32, 142, 68 }; }
    static jt::Color PaletteColor3() { return jt::Color { 133, 225, 66 }; }
    static jt::Color PaletteColor4() { return jt::Color { 239, 255, 124 }; }
    static jt::Color PaletteColor5() { return jt::Color { 251, 114, 48 }; }
    static jt::Color PaletteColor6() { return jt::Color { 184, 39, 45 }; }
    static jt::Color PaletteColor7() { return jt::Color { 64, 23, 58 }; }
    static jt::Color PaletteColor8() { return jt::Color { 34, 49, 91 }; }

    static int PhysicVelocityIterations() { return 6; }
    static int PhysicPositionIterations() { return 2; }

    static float ClientToServerTimeout() { return 2.0f; }

    static float explosionShockwaveBaseSize() { return 1.0f; }
    static float explosionShockwaveMaxSize() { return 2.5f; }
};

#endif
