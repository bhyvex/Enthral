/***************************************************************************
 *   Enthral BBS Alpha 2                                                   *
 *   Copyright (C) 2004-2015 by Michael Griffin                            *
 *   mrmisticismo@hotmail.com                                              *
 *                                                                         *
 *   Purpose:                                                              *
 *                                                                         *
 *                                                                         *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include "StandardIO.hpp"

#include <iostream> // cout
#include <clocale>  // locale
#include <cwchar>   // wchar_t wide characters
#include <string>   // string and wstring
#include <fstream>  // ifstream file streams.
#include <termios.h>

# include <boost/locale.hpp>

namespace stdio
{

// Extra Init Variables here.
/*
 * Updates Michael Griffin:
 * Characters 1-31 remapped for CP437 control Codes to UTF-8 Glyphs
 *
 * Excluded from Translation to Glyphs.
 * http://en.wikipedia.org/wiki/Code_page_437#Characters
 *
 * 13 is mapped to CR   Excluded 'Music Note'
 * 27 is mapped to ESC  Excluded 'Left Arrow'
 */
wchar_t CP437TABLE[] =
{
    L'\u0000', L'\u263A', L'\u263B', L'\u2665', L'\u2666', L'\u2663', // 5
    L'\u2660', L'\u2022', L'\u0008', L'\u0009', L'\u000A', L'\u2642', // 11
    L'\u2640', L'\u000D', L'\u266C', L'\u263C', L'\u25BA', L'\u25C4', // 17
    L'\u2195', L'\u203C', L'\u00B6', L'\u00A7', L'\u25AC', L'\u21A8', // 23
    L'\u2191', L'\u2193', L'\u2192', L'\u001B', L'\u221F', L'\u2194', // 29
    L'\u25B2', L'\u25BC', L'\u0020', L'\u0021', L'\u0022', L'\u0023',
    L'\u0024', L'\u0025', L'\u0026', L'\u0027', L'\u0028', L'\u0029',
    L'\u002A', L'\u002B', L'\u002C', L'\u002D', L'\u002E', L'\u002F',
    L'\u0030', L'\u0031', L'\u0032', L'\u0033', L'\u0034', L'\u0035',
    L'\u0036', L'\u0037', L'\u0038', L'\u0039', L'\u003A', L'\u003B',
    L'\u003C', L'\u003D', L'\u003E', L'\u003F', L'\u0040', L'\u0041',
    L'\u0042', L'\u0043', L'\u0044', L'\u0045', L'\u0046', L'\u0047',
    L'\u0048', L'\u0049', L'\u004A', L'\u004B', L'\u004C', L'\u004D',
    L'\u004E', L'\u004F', L'\u0050', L'\u0051', L'\u0052', L'\u0053',
    L'\u0054', L'\u0055', L'\u0056', L'\u0057', L'\u0058', L'\u0059',
    L'\u005A', L'\u005B', L'\u005C', L'\u005D', L'\u005E', L'\u005F',
    L'\u0060', L'\u0061', L'\u0062', L'\u0063', L'\u0064', L'\u0065',
    L'\u0066', L'\u0067', L'\u0068', L'\u0069', L'\u006A', L'\u006B',
    L'\u006C', L'\u006D', L'\u006E', L'\u006F', L'\u0070', L'\u0071',
    L'\u0072', L'\u0073', L'\u0074', L'\u0075', L'\u0076', L'\u0077',
    L'\u0078', L'\u0079', L'\u007A', L'\u007B', L'\u007C', L'\u007D',
    L'\u007E', L'\u007F', L'\u00C7', L'\u00FC', L'\u00E9', L'\u00E2',
    L'\u00E4', L'\u00E0', L'\u00E5', L'\u00E7', L'\u00EA', L'\u00EB',
    L'\u00E8', L'\u00EF', L'\u00EE', L'\u00EC', L'\u00C4', L'\u00C5',
    L'\u00C9', L'\u00E6', L'\u00C6', L'\u00F4', L'\u00F6', L'\u00F2',
    L'\u00FB', L'\u00F9', L'\u00FF', L'\u00D6', L'\u00DC', L'\u00A2',
    L'\u00A3', L'\u00A5', L'\u20A7', L'\u0192', L'\u00E1', L'\u00ED',
    L'\u00F3', L'\u00FA', L'\u00F1', L'\u00D1', L'\u00AA', L'\u00BA',
    L'\u00BF', L'\u2310', L'\u00AC', L'\u00BD', L'\u00BC', L'\u00A1',
    L'\u00AB', L'\u00BB', L'\u2591', L'\u2592', L'\u2593', L'\u2502',
    L'\u2524', L'\u2561', L'\u2562', L'\u2556', L'\u2555', L'\u2563',
    L'\u2551', L'\u2557', L'\u255D', L'\u255C', L'\u255B', L'\u2510',
    L'\u2514', L'\u2534', L'\u252C', L'\u251C', L'\u2500', L'\u253C',
    L'\u255E', L'\u255F', L'\u255A', L'\u2554', L'\u2569', L'\u2566',
    L'\u2560', L'\u2550', L'\u256C', L'\u2567', L'\u2568', L'\u2564',
    L'\u2565', L'\u2559', L'\u2558', L'\u2552', L'\u2553', L'\u256B',
    L'\u256A', L'\u2518', L'\u250C', L'\u2588', L'\u2584', L'\u258C',
    L'\u2590', L'\u2580', L'\u03B1', L'\u00DF', L'\u0393', L'\u03C0',
    L'\u03A3', L'\u03C3', L'\u00B5', L'\u03C4', L'\u03A6', L'\u0398',
    L'\u03A9', L'\u03B4', L'\u221E', L'\u03C6', L'\u03B5', L'\u2229',
    L'\u2261', L'\u00B1', L'\u2265', L'\u2264', L'\u2320', L'\u2321',
    L'\u00F7', L'\u2248', L'\u00B0', L'\u2219', L'\u00B7', L'\u221A',
    L'\u207F', L'\u00B2', L'\u25A0', L'\u00A0'
};

StandardIO::StandardIO() :
    term_reset_(0),
    term_echo_(1),
    term_canonical_(3)
{}

StandardIO::~StandardIO()
{}

/**
 * Set default TTY Options for Input
 * This should be the first method run
 * After creating the instance.
 */
bool StandardIO::InitConsoleTTY()
{
    // Set Current Terminal Options
    if (!SetTerminalAttributes(term_echo_))
        return false;
    if (!SetTerminalAttributes(term_canonical_))
        return false;

    return true;
}

/*
 * This should be run before exiting the bbs
 * So the terminal is reset back to default!
 */
bool StandardIO::ResetConsoleTTY()
{
    // Set Current Terminal Options
    if (!SetTerminalAttributes(term_reset_))
        return false;

    return true;
}

/**
 * Set Terminal TTY Options
 */
bool StandardIO::SetTerminalAttributes(int term_option)
{
    switch(term_option)
    {
        case 0: // Reset To Original
            if(tcgetattr(STDIN_FILENO,&terminal_attributes_) != 0) return false;
            terminal_attributes_.c_lflag |= ECHO;
            terminal_attributes_.c_lflag |= ICANON;
            terminal_attributes_.c_lflag |= ISIG;
            if(tcsetattr(STDIN_FILENO,TCSANOW,&terminal_attributes_) != 0) return false;
            break;

        case 1: // Terminal Echo Off
            if(tcgetattr(STDIN_FILENO,&terminal_attributes_) != 0) return false;
            terminal_attributes_.c_lflag &= ~(ECHO);
            if(tcsetattr(STDIN_FILENO,TCSAFLUSH,&terminal_attributes_) != 0) return false;
            break;

        case 2: // Single Key Hot-key input allow ^C and ^Z signals
            if(tcgetattr(STDIN_FILENO,&terminal_attributes_) != 0) return false;
            terminal_attributes_.c_lflag    &= ~(ICANON); // On
            terminal_attributes_.c_lflag    |= ISIG;      // Off
            terminal_attributes_.c_cc[VMIN]  = 1;
            terminal_attributes_.c_cc[VTIME] = 1;
            if(tcsetattr(STDIN_FILENO,TCSANOW,&terminal_attributes_) != 0) return false;
            break;

        case 3: // Single Key Hot-key input No ^C and ^Z Signals
            if(tcgetattr(STDIN_FILENO,&terminal_attributes_) != 0) return false;
            terminal_attributes_.c_lflag    &= ~(ICANON);
            terminal_attributes_.c_lflag    &= ~(ISIG);
            terminal_attributes_.c_cc[VMIN]  = 0;
            terminal_attributes_.c_cc[VTIME] = 0;
            if(tcsetattr(STDIN_FILENO,TCSANOW,&terminal_attributes_) != 0) return false;
            break;

        default:
            return false;
    }
    return true;
}

/**
 * Used for printing multibyte (Unicode Translations)
 * To Console / STDOUT
 */
void StandardIO::PrintWideCharacters(const std::wstring& wide_string)
{
    using namespace boost::locale;
    using namespace std;

    generator gen;

    // Make system default locale global
    std::locale loc = gen("");
    locale::global(loc);
    cout.imbue(loc);

    std::mbstate_t state = std::mbstate_t();
    for(wchar_t wc : wide_string)
    {
        std::string mb(MB_CUR_MAX, '\0');
        int ret = std::wcrtomb(&mb[0], wc, &state);
        if ((ret == 0) || (ret > MB_CUR_MAX))
            break;

        // Skip any Trailing / Embedded null from Wide -> multibtye
        // Conversion, don't send NULL's to the screen.
        for(char ch: mb)
        {
            if (ch != '\0')
                std::cout << ch << flush;
        }
    }
}

/**
 * Main Translation loop from CP437 to Wide Unicode.
 * This is needed to match character translations from
 * High ASCII to Unicode Characters.
 *
 */
void StandardIO::TranslateUnicode(std::string standard_string)
{
    std::wstring wide_string;
    int ascii_value = 0;

    // Loop and write out after translation to Unicode
    for (std::string::size_type i = 0; i < standard_string.size(); i++)
    {
        ascii_value = std::char_traits<char>().to_int_type(standard_string[i]);
        if (ascii_value < 256)
            wide_string = CP437TABLE[ascii_value];
        else
            wide_string = standard_string[i];

        PrintWideCharacters(wide_string);
    }
}

/**
 * Reads Single Character from TTY STDIO
 * Handles ASCII, Extended ASCII and UTF-8 Input
 *
 * Otherwise Loops and waits for Keypress
 * Usually not called by itself, is used by GetKey().
 */
std::string StandardIO::ReadConsoleTTY()
{
    using namespace boost::locale;
    generator gen;

    std::locale systemLocal = gen("");
    std::locale::global(systemLocal);
    std::cout.imbue(systemLocal);

    int  char_value =  0;
    int  result     =  0;

    std::string character;
    bool utf_found = false;

    boundary::ssegment_index::iterator current, end;

    while(1)
    {
        // Read single character from the console (Loops waiting for Input)
        result = read(STDIN_FILENO, &char_value, 1);
        if (result <= 0)
        {
            // No Data is waiting, so we loop until we get data
            // Sleep here to not waste or overrun CPU usage
            usleep(400000);
            continue;

        }

        if (!utf_found)
        {
            if ((char_value & 0xC0) != 0xC0)
                utf_found = false; // 1 Byte
            else if ((char_value & 0xE0) == 0xC0)
                utf_found = true;  // 2;
            else if ((char_value & 0xF0) == 0xE0)
                utf_found = true;  // 3;
            else if ((char_value & 0xF8) == 0xF0)
                utf_found = true;  // 4;
            else
            {
                // Not a valid character,
                std::cout << "Exception: Invalid character input (NOT ASCII or UTF-8)!" << std::endl;
                continue;
            }
        }

        //charValue = 0x8A;  // Test single-byte extended ASCII
        character += char_value;
        boundary::ssegment_index index; //index(boundary::word,character.begin(),character.end());
        index.map(boundary::character,character.begin(),character.end());

        // Catch All MultiByte UTF-8 Character Sequences.
        if (utf_found)
        {
            for(current = index.begin(), end = index.end();
                    current != end; ++current)
            {
                return character;
            }
        }
        else
        {
            // Single Byte 0 - 255
            return character;
        }
    }
    // Invalid Character, return nothing!
    return "";
}

/**
 * Scan the TTY STDIO for Any Waiting Data
 * This is non-blocking so it will return
 * If there is no data to be read.
 */
bool StandardIO::ScanConsoleTTY()
{
    struct timeval tv;
    fd_set fds;

    FD_ZERO(&fds);
    FD_SET(STDIN_FILENO, &fds);
    tv.tv_sec  = 0;
    tv.tv_usec = 400000; // .4 second delay / Save CPU Usage

    // Check for Any waiting input from STDIO (Non-blocking!)
    if (select(STDIN_FILENO+1, &fds, 0, 0, &tv))
    {
        if (FD_ISSET(STDIN_FILENO, &fds))
        {
            return true;
        }
    }
    return false;
}


}
