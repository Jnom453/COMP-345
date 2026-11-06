#ifndef FONT_LOADER_H
#define FONT_LOADER_H

#include <SFML/Graphics.hpp>

/**
 * @file FontLoader.h
 * @brief Loads a system font on macOS, Windows or Linux.
 */

/**
 * @brief Tries a list of common system font files until one loads.
 *
 * The observer windows need a font to draw text. The font file lives in a
 * different place on each operating system, so several locations are tried.
 *
 * @param font The font to fill.
 * @return true if a font was loaded, false if none of the locations worked
 *         (the window is then drawn without text).
 */
inline bool loadSystemFont(sf::Font& font)
{
    const char* paths[] = {
        "/System/Library/Fonts/Supplemental/Arial.ttf",
        "/Library/Fonts/Arial.ttf",
        "C:/Windows/Fonts/arial.ttf",
        "/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf",
        "/usr/share/fonts/dejavu/DejaVuSans.ttf",
        "/usr/share/fonts/TTF/DejaVuSans.ttf",
        "/usr/share/fonts/truetype/liberation/LiberationSans-Regular.ttf"
    };

    for (const char* path : paths)
    {
        if (font.openFromFile(path))
        {
            return true;
        }
    }
    return false;
}

#endif
