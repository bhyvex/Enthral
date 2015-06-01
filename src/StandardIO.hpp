#ifndef STANDARDIO_HPP
#define STANDARDIO_HPP

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

# include <ctime>
# include <string>
# include <termios.h>

namespace stdio
{

class StandardIO
{

protected:

    // Methods
    bool SetTerminalAttributes(int term_option);
    void PrintWideCharacters(const std::wstring& wide_string);
    void TranslateUnicode(std::string standard_string);
    bool InputTimeout();

private:

    int m_term_reset;
    int m_term_echo;
    int m_term_canonical;
    struct termios m_terminal_attributes;

public:

    StandardIO();
    ~StandardIO();

    // Methods
    bool InitConsoleTTY();
    bool ResetConsoleTTY();
    std::string ReadConsoleTTY();
    bool ScanConsoleTTY();

};

}

#endif // STANDARDIO_HPP
