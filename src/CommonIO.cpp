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


#include "CommonIO.hpp"

#include <unistd.h>
#include <sys/types.h>

#ifdef TARGET_OS_MAC
#include <mach-o/dyld.h>
#elif _WIN32
#include <windows.h>
#endif


#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>

#include <cstdio>
#include <cstring>
#include <climits>
#include <cstdlib>

#include <stdexcept>

#include <iostream>
#include <string>
#include <algorithm>
#include <functional>
#include <cctype>
#include <locale>
#include <sstream>

#include <boost/format.hpp>
#include <boost/locale.hpp>

namespace common
{


/* This function will read the OS specific functions
 * To Determine where the executable is located.
 * This has only been tested in Windows, Linux, OSX.
 */
std::string GetProgramPath()
{
    std::string program_path;
    std::string temp;

    // First check for ENTHRAL environmental variable:
    char *pPath;
    pPath = std::getenv((char *)"ENTHRAL");
    if (pPath!=NULL)
    {
        program_path = pPath;
        return program_path;
    }

    // Get the Folder the Executable runs in.
#ifdef TARGET_OS_MAC
    char current_path[PATH_MAX];
    uint32_t size = sizeof(current_path);
    if (_NSGetExecutablePath(current_path, &size) != 0)
    {
        throw std::runtime_error("getProgramPath: OSX Path");
    }

    // Remove Executable
    program_path = current_path;
    std::string::size_type position;
    position = program_path.rfind("/Enthral");
    if (position != std::string::npos)
        program_path.erase(position+1,program_path.size()-1);

    // remove extra './'
    position = program_path.rfind("./");
    if (position != std::string::npos)
        program_path.erase(position,2);

#elif _WIN32
    // Get the Current Program path.
    char current_path[PATH_MAX];

    int result = GetModuleFileName(NULL, current_path, PATH_MAX-1);
    if(result == 0)
    {
        throw std::runtime_error("GetProgramPath: Win32 Path");
    }

    program_path = current_path;
    std::string::size_type position = program_path.rfind("\\",program_path.size()-1);
    if (position != std::string::npos)
        program_path.erase(position+1);
#else

    char exe_path[PATH_MAX] = {0};
    ssize_t result = readlink("/proc/self/exe", exe_path, PATH_MAX );
    if (result < 0)
    {
        throw std::runtime_error("getProgramPath: Linux Path");
    }

    const char* t = " \t\n\r\f\v";
    program_path = exe_path;
    program_path = program_path.erase(program_path.find_last_not_of(t) + 1);
    program_path += "/";

    // Remove Executable
    std::string::size_type position;
    position = program_path.rfind("/Enthral");
    if (position != std::string::npos)
        program_path.erase(position+1,program_path.size()-1);

#endif
    return program_path;
}

/**
 * Get The BBS System Users HOME directory
 */
std::string GetSystemHomeDirectory()
{
    std::string home_directory;
    const char *homedir;

    homedir = getenv("HOME");
    if (homedir == nullptr)
    {
        homedir = getpwuid(getuid())->pw_dir;
        if (homedir == nullptr)
        {
            std::cout << "Error: Unable to locate bbs user's home directory: "
                      << homedir << std::endl;
            home_directory = "";
        }
        else
            home_directory = homedir;
    }
    else
        home_directory = homedir;

    return home_directory;
}


/**
 * String Lengh counting actual characters not bytes
 * This is for mixed ASCII And UTF-8 Strings.
 */
std::string::size_type NumberOfChars(const std::string &str)
{
    using namespace boost::locale;
    generator gen;

    // Make system default locale global
    std::locale loc = gen("");
    std::locale::global(loc);
    std::cout.imbue(loc);

    std::string::size_type count = 0;
    std::string::size_type string_size = str.size();

    std::string character;
    bool utf_found = false;

    int  char_value =  0;
    std::string::size_type i = 0;

    if (str.empty())
    {
        std::cout << "Exception (Common::NumberOfChars) string length == 0" << std::endl;
        return 0;
    }

    using namespace boost::locale;
    boundary::ssegment_index::iterator p,e;
    while(1)
    {
        if (i == string_size) break;
        char_value = str[i++];

        // Catch if we've receive multi-byte
        if (!utf_found)
        {
            if ((char_value & 0xC0) != 0xC0)
                utf_found = false; // 1 Byte

            if ((char_value & 0xE0) == 0xC0)
                utf_found = true;  // 2;

            if ((char_value & 0xF0) == 0xE0)
                utf_found = true;  // 3;

            if ((char_value & 0xF8) == 0xF0)
                utf_found = true;  // 4;
        }

        //charValue = 0x8A;  // Test singleByte extended ascii
        character += char_value;
        boundary::ssegment_index index(boundary::word,character.begin(),character.end());

        // Catch All MultiByte UTF-8 Character Sequences.
        if (!utf_found)
        {
            character.erase();
            ++count;
        }
        else
        {
            // Parse Character for complete UTF-8 Sequence
            for(p = index.begin(), e = index.end(); p != e; ++p)
            {
                if(p->rule() & boundary::word_none)
                {
                    character.erase();
                    continue;
                }
                if(p->rule() & boundary::word_any)
                {
                    character.erase();
                    ++count;
                    utf_found = false;
                }
            }
        }
    }
    return count;
}

/**
 * Left Trim Whitepsaces (Front)
 */
std::string LeftTrim(const std::string &str)
{
    std::string new_string = str;
    if (new_string.empty())
    {
        std::cout << "Exception (Common::LeftTrim) string length == 0" << std::endl;
        return new_string;
    }
    new_string.erase(new_string.begin(), std::find_if(new_string.begin(), new_string.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
    return new_string;
}

/**
 * Right Trim Whitepsaces (Back)
 */
std::string RightTrim(const std::string &str)
{
    std::string new_string = str;
    if (new_string.empty())
    {
        std::cout << "Exception (Common::RightTrim) string length == 0" << std::endl;
        return new_string;
    }
    new_string.erase(std::find_if(new_string.rbegin(), new_string.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), new_string.end());
    return new_string;
}

/**
 * Trim Whitepsaces from both ends
 */
std::string Trim(const std::string &str)
{
    std::string new_string = str;
    if (new_string.empty())
    {
        std::cout << "Exception (Common::trim) string length == 0" << std::endl;
        return new_string;
    }
    return LeftTrim(RightTrim(new_string));
}


/**
 * UTF-8 Aware
 * Removes All Data after specified number of characters
 */
std::string EraseString(const std::string &str,
                        std::string::size_type start_position,
                        std::string::size_type end_position)
{

    std::string new_string = str;
    std::string::size_type count = 0;
    std::string::size_type string_size = new_string.size();

    // 0 defaults to end of string.
    // Otherwise End Position is added for number of characters to remove.
    if (end_position == 0)
        end_position = string_size;
    else
    {
        end_position = start_position + (end_position-1);
        // Make sure we can never go past end of string!
        if (end_position > string_size)
            end_position = string_size;
    }

    std::string character;
    std::string new_string_builder;
    bool utf8Found = false;

    int char_value = 0;
    std::string::size_type i = 0;

    if (new_string.empty())
    {
        std::cout << "Exception (Common::EraseString) string length == 0" << std::endl;
        return new_string;
    }

    using namespace boost::locale;
    boundary::ssegment_index::iterator p,e;
    while(1)
    {
        if (i == string_size) break;
        char_value = new_string[i++];

        // Make sure if we hit an ESC sequence we loop through it.
        if (!utf8Found)
        {
            if ((char_value & 0xC0) != 0xC0)
                utf8Found = false; // 1 Byte

            if ((char_value & 0xE0) == 0xC0)
                utf8Found = true;  // 2;

            if ((char_value & 0xF0) == 0xE0)
                utf8Found = true;  // 3;

            if ((char_value & 0xF8) == 0xF0)
                utf8Found = true;  // 4;
        }

        //charValue = 0x8A;  // Test singleByte extended ascii
        character += char_value;
        boundary::ssegment_index index(boundary::word,character.begin(),character.end());

        // Catch All MultiByte UTF-8 Character Sequences.
        if (!utf8Found)
        {
            // Add Character to new string.
            if (count < start_position || count > end_position)
            {
                new_string_builder.append(character);
            }
            character.erase();
            ++count;
        }
        else
        {
            // Parse Character for complete UTF-8 Sequence
            for(p = index.begin(), e = index.end(); p != e; ++p)
            {
                // This is the 0x0000F ending UTF8 sequence.
                if(p->rule() & boundary::word_none)
                {
                    if (count < start_position || count > end_position)
                    {
                        std::cout << "append none" << std::endl;
                        std::cout << "count " << count << std::endl;
                        new_string_builder.append(character);
                    }
                    character.erase();
                    continue;
                }
                // Valid UTF-8 Character
                if(p->rule() & boundary::word_any)
                {
                    if (count < start_position || count > end_position)
                    {
                        new_string_builder.append(character);
                    }
                    character.erase();
                    ++count;
                    utf8Found = false;
                }
            }
        }
    }
    new_string.erase();
    new_string = new_string_builder;
    return new_string;
}

/**
 * Right String Padding
 */
std::string RightPadding(const std::string &str, std::string::size_type space)   // Pad Right
{
    std::string new_string = str;
    if (space == 0) return new_string;
    if (new_string.empty())
    {
        std::cout << "Exception (Common::RightPadding) string length == 0" << std::endl;
        return new_string;
    }

    std::string padded_line = "";
    std::string::size_type s = NumberOfChars(new_string);

    // if Line > Sapce, Erase to match length
    if (s > space)
    {
        std::string erased = EraseString(new_string, space);
        return erased;
    }

    for(std::string::size_type i = 0; i < (space-s); i++)
        padded_line += ' ';

    new_string.append(padded_line);
    return new_string;
}

/**
 * Left String Padding
 */
std::string LeftPadding(const std::string &str, std::string::size_type space)
{
    std::string new_string = str;
    if (space == 0) return new_string;
    if (new_string.empty())
    {
        std::cout << "Exception (Common::leftPadding) string length == 0" << std::endl;
        return new_string;
    }

    std::string::size_type s = NumberOfChars(new_string);

    // if Line > Sapce, Erase to match length
    if (s >= space)
    {
        std::string erased = EraseString(new_string, 0, s-space);
        return erased;
    }

    for(std::string::size_type i = 0; i < (space-s); i++)
        new_string.insert(0, " ");

    return new_string;
}

/**
 * Center String Padding
 * Note: Need to add Code to Strip Pipes and ANSI
 * for Real Screen Centering!
 */
std::string CenterPadding(const std::string &str, int term_width)
{
    std::string new_string = str;
    if (new_string.empty())
    {
        std::cout << "Exception (Common::centerPadding) string empty" << std::endl;
        return new_string;
    }

    std::string::size_type length = NumberOfChars(new_string);
    if (length == 0)
    {
        std::cout << "Exception (Common::centerPadding) string length == 0" << std::endl;
        return new_string;
    }
    int space = term_width / 2;
    space -= term_width % 2;

    space -= length / 2;
    space -= length % 2;

    // Text larger then screen, then leave alone.
    if (space <= 0) return new_string;

    std::string padded_line;
    for(int i = 0; i < space; i++)
        padded_line += ' ';

    // Appending currnet data after Padding.
    new_string.insert(0,padded_line);
    return new_string;
}

/**
 * Mask String
 */
std::string MaskString(const std::string &str)
{
    std::string new_string = str;
    if (new_string.empty())
    {
        std::cout << "Exception (Common::maskString) string empty" << std::endl;
        return new_string;
    }

    std::string::size_type string_size = NumberOfChars(new_string);
    if (string_size == 0)
    {
        std::cout << "Exception (Common::maskString) string length == 0" << std::endl;
        return new_string;
    }

    new_string.erase();
    for(std::string::size_type i = 0; i < string_size; i++)
        new_string.append("*");

    return new_string;
}


/**
 * Check Digit or Numbers in String
 * This is for mixed ASCII And UTF-8 Strings.
 */
bool IsDigit(const std::string &str)
{
    using namespace boost::locale;
    generator gen;

    // Make system default locale global
    std::locale loc = gen("");
    std::locale::global(loc);
    std::cout.imbue(loc);

    std::string::size_type count = 0;
    std::string::size_type string_size = str.size();

    std::string character;
    bool utf_found = false;
    bool is_digit = false;

    int  char_value =  0;
    std::string::size_type i = 0;

    if (str.empty())
    {
        std::cout << "Exception (Common::NumberOfChars) string length == 0" << std::endl;
        return is_digit;
    }

    using namespace boost::locale;
    boundary::ssegment_index::iterator p,e;
    while(1)
    {
        if (i == string_size) break;
        char_value = str[i++];

        // Catch if we've receive multi-byte
        if (!utf_found)
        {
            if ((char_value & 0xC0) != 0xC0)
                utf_found = false; // 1 Byte

            if ((char_value & 0xE0) == 0xC0)
                utf_found = true;  // 2;

            if ((char_value & 0xF0) == 0xE0)
                utf_found = true;  // 3;

            if ((char_value & 0xF8) == 0xF0)
                utf_found = true;  // 4;
        }

        //charValue = 0x8A;  // Test singleByte extended ascii
        character += char_value;
        boundary::ssegment_index index(boundary::word,character.begin(),character.end());

        // Catch All MultiByte UTF-8 Character Sequences.
        if (!utf_found)
        {
            // Test for Ascii Digit
            if (std::isdigit(str[count]))
                is_digit = true;
            else
            {
                is_digit = false; // Check for more then one!
                return is_digit;  // Exit on any failures!
            }
            character.erase();
            ++count;
        }
        else
        {
            // Parse Character for complete UTF-8 Sequence
            for(p = index.begin(), e = index.end(); p != e; ++p)
            {
                if(p->rule() & boundary::word_none)
                {
                    character.erase();
                    continue;
                }
                if(p->rule() & boundary::word_number)
                {
                    character.erase();
                    ++count;
                    utf_found = false;
                    is_digit = true;
                    continue;
                }
                else
                {
                    is_digit = false;
                    return is_digit;  // Exit on any failures!
                }
            }
        }
    }
    return is_digit;
}


}
