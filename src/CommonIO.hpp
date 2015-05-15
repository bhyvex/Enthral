#ifndef COMMONIO_HPP
#define COMMONIO_HPP

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

#include <string>

namespace common
{

/* This function will read the OS specific functions
 * To Determine where the executable is located.
 * This has only been tested in Windows, Linux, OSX.
 */
std::string GetProgramPath();

/**
 * Get The BBS System Users HOME directory
 */
std::string GetSystemHomeDirectory();

/**
 * String Lengh counting actual characters not bytes
 * This is for mixed ASCII And UTF-8 Strings.
 */
std::string::size_type NumberOfChars(const std::string &str);

/**
 * Left Trim Whitepsaces (Front)
 */
std::string LeftTrim(const std::string &str);

/**
 * Right Trim Whitepsaces (Back)
 */
std::string RightTrim(const std::string &str);

/**
 * Trim Whitepsaces from both ends
 */
std::string Trim(const std::string &str);

/**
 * UTF-8 Aware
 * Removes All Characters Between Start and End Positions.
 */
std::string EraseString(const std::string &str,
                        std::string::size_type start_position,
                        std::string::size_type end_position = 0);
/**
 * Right String Padding
 */
std::string RightPadding(const std::string &str, std::string::size_type space);

/**
 * Left String Padding
 */
std::string LeftPadding(const std::string &str, std::string::size_type space);

/**
 * Center String Padding
 * Note: Need to add ANSI / PIPE parsing!
 */
std::string CenterPadding(const std::string &str, int term_width);

/**
 * Mask String
 */
std::string MaskString(const std::string &str);

/**
 * Check Digit or Numbers in String
 * This is for mixed ASCII And UTF-8 Strings.
 */
bool IsDigit(const std::string &str);

}

#endif // COMMONIO_HPP
