#ifndef USERSESSION_HPP
#define USERSESSION_HPP

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

#include "DataStructure.hpp"
#include "StandardIO.hpp"

#include <ctime>
#include <string>
#include <queue>
#include <stdint.h>

/*
 * UserSession is the Main Global Class
 * That stores all states of the current connection
 * This class is passed between all interfaces
 */
class UserSession
{
public:
    UserSession();
    ~UserSession();

    data::UserRecord          m_user_record;
    data::ConfigurationRecord m_system_record;

    bool m_is_session_active;     // Session is Active
    bool m_is_session_logged_in;  // User Logged In
    bool m_is_newuser;            // First Login
    int m_node_number;
    int m_terminal_width;
    int m_terminal_height;
    int m_prelogin_timeout_in_minutes; // Timeout for during Login Process
    int m_system_timeout_in_minutes;   // Timeout once logged in as Registered User.

    std::string m_ip_address;
    std::string m_ip_hostname;
    std::string m_program_path;        // Create / Check for Configuration File
    std::string m_home_directory;      // Create / Check for Configuration File Alternate

    // io
    std::string m_escape_sequence;

    // Settings
    std::string m_terminal_type;
    std::time_t m_session_start;

    bool m_is_timeout_warning;
    std::time_t m_start_timeout;

    // Notification Queue
    std::queue<std::string> m_notifications;

    // Class IO Functions
    void CheckTerminalSize();
    bool InputTimeout();
    std::string GetEscapeSequence();
    std::string GetInput();
};

#endif // USERSESSION_HPP
