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
#include <stdint.h>

namespace data
{

const std::string g_bbs_version = "Enthral/2 BBS v1.0";

typedef struct ConfigurationRecord
{
    std::string system_name;      // BBS Name

    // Paths
    std::string system_path;      // Path to Data Files
    std::string testing_path;     // Path to Data Files for Testing.

    // Settings {Most of these are new and need to be coded!}
    uint32_t max_nodes;           // Number of Instances
    uint32_t max_username_length; // Number of Instances
    uint32_t max_password_length; // Number of Instances

    bool     matrix_login;        // Matrix Menu or Normal Login
    bool     use_system_password; // Ask for password before loading the BBS.
    bool     use_session_timeout; // Timeout Active or Disabled.
    uint32_t session_timeout;     // Timelimit for no input before logoff.

} ConfigurationRecord;


typedef struct SystemRecord
{
    // Dates
    std::string startup_date;

    // History
    uint32_t total_calls;
    uint32_t total_emails;
    uint32_t total_posts;
    uint32_t total_uploads;
    uint32_t total_downloads;

} SystemRecord;


typedef struct UserRecord
{
    uint32_t index;

    // Basic User Info
    std::string handle;
    std::string real_name;
    std::string password;
    std::string gender;
    std::string email_address;
    std::string keep_email_private;
    std::string location;
    std::string affiliations;

    // Password Resets
    std::string challenge_question;
    std::string challenge_answer;

    // Dates
    std::string birth_date;
    std::string first_login;
    std::string last_login;

    // History
    uint32_t total_calls;
    uint32_t total_emails;
    uint32_t total_posts;
    uint32_t total_uploads;
    uint32_t total_downloads;

    // Preferences
    bool backspace_key_swap;
    bool hot_key_input;
    uint32_t message_reader_theme;
    uint32_t message_editor_theme;
    uint32_t fullscreen_reader;

    // Customization Menu Set / Language File.
    std::string theme;
    std::string language;

    // Character Set // ie CP437, UTF8
    std::string input_encoding;
    std::string output_encoding;

    // User Groups
    std::string user_groups;
    std::string message_groups;
    std::string file_groups;

    // User Is active or Termed.
    bool is_administrator;
    bool record_active;


} UserRecord;

}
