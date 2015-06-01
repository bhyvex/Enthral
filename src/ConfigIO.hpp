#ifndef CONFIGIO_HPP
#define CONFIGIO_HPP

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
#include "UserSession.hpp"

/*
 * User Session is passed to the ConfigIO where Inital BBS Configuration
 * Settings are loaded on a Per-Session Basis.
 */

class ConfigIO
{
public:

    UserSession *m_session;

    ConfigIO(UserSession *session)
    {
        m_session = session;
    }
    ~ConfigIO() { }

    // Add Methods for Config Files Exists for Both Program and Home!

    // Methods
    bool GenerateNewSystemConfigProgramPath();
    bool GenerateNewSystemConfigHome();
    bool CreateConfigFile(const std::string &path, bool use_home_directory);

};

#endif // CONFIGIO_HPP
