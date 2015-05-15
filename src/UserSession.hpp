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

    data::UserRecord          user_record_;
    data::ConfigurationRecord system_record_;

    bool is_session_active_;     // Session is Active
    bool is_session_logged_in_;  // User Logged In
    bool is_newuser_;            // First Login
    int node_number_;
    int terminal_width_;
    int terminal_height_;
    int prelogin_timeout_in_minutes_; // Timeout for during Login Process
    int system_timeout_in_minutes_;   // Timeout once logged in as Registered User.

    std::string ip_address_;
    std::string ip_hostname_;
    std::string program_path_;        // Create / Check for Configuration File
    std::string home_directory_;      // Create / Check for Configuration File Alternate

    // io
    std::string escape_sequence_;

    // Settings
    std::string terminal_type_;
    std::time_t session_start_;

    bool is_timeout_warning_;
    std::time_t start_timeout_;

    // Notification Queue
    std::queue<std::string> notifications_;

    // Class IO Functions
    void CheckTerminalSize();
    bool InputTimeout();
    std::string GetEscapeSequence();
    std::string GetInput();


};


#endif // USERSESSION_HPP
