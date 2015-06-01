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

#include <iostream>
#include <string>
#include <ctime>
#include <unistd.h>
#include <pty.h>     // Struct Winsize

UserSession::UserSession() :
    m_is_session_active(true),
    m_is_session_logged_in(false),
    m_is_newuser(false),
    m_node_number(0),
    m_terminal_width(80),
    m_terminal_height(24),
    m_prelogin_timeout_in_minutes(2),
    m_system_timeout_in_minutes(0),
    m_session_start(std::time(nullptr)),
    m_is_timeout_warning(false),
    m_start_timeout(0)
{ }

UserSession::~UserSession()
{ }

/*
 * Check for Window Terminal Size Changes
 */
void UserSession::CheckTerminalSize()
{
    struct winsize window_size;
    if(ioctl(STDIN_FILENO, TIOCGWINSZ, &window_size) >= 0 &&
            window_size.ws_row > 0 &&
            window_size.ws_col > 0)
    {
        m_terminal_height = window_size.ws_row;
        m_terminal_width = window_size.ws_col;
    }
}

/**
 * Check Timeout waiting for input from user.
 */
bool UserSession::InputTimeout()
{
    double time_difference = 0;
    int     minutes_passed = 0;
    int  timed_out_minutes = 0;

    // Setup which Time-out to use.
    if (m_is_session_logged_in)
        timed_out_minutes = m_system_timeout_in_minutes;
    else
        timed_out_minutes = m_prelogin_timeout_in_minutes;

    // No timeout set, then return
    if (timed_out_minutes == 0)
        return false;

    std::time_t  current_time;
    current_time = time(0);

    if ( m_start_timeout != (std::time_t)(-1) && current_time != (std::time_t)(-1) )
    {
        time_difference = std::difftime(current_time,m_start_timeout); /// (60 * 60 * 24);
        minutes_passed = (int)(time_difference / 60);
    }

    // Time Out!  Bye Bye User!
    if (minutes_passed > timed_out_minutes)
    {
        return true;
    }
    else if (minutes_passed < (timed_out_minutes - 1))
    {
        // Timeout Reset we received activity, as Long as time
        // Is below the warning threshold, we'll keep this false.
        m_is_timeout_warning = false;
    }
    // Give a Timeout Warning with 1 minute left.
    else if (minutes_passed >= (timed_out_minutes - 1)
             && m_is_timeout_warning == false)
    {
        // Send only 1 Timeout Warning.
        m_is_timeout_warning = true;

        // Craft Message Here for Timeout Warning. From Language File!
        std::cout << "Warning, System Timeout approaching: 1 Minute Left." << std::endl;
    }
    return false;
}


/*
 * Return the Escape Sequence Parsed From GetInput()
 */
std::string UserSession::GetEscapeSequence()
{
    return m_escape_sequence;
}

/**
 * Get Single Character Input from console
 * Waits for Keys Press Non-Blocking Loop
 * If we receive an ESC Sequence, this will loop
 * will capture the entire sequence.
 * Sending Back ESC int 27 to register that an ESC was received.
 * If escape_sequence_ is empty, then the plain ESC key was hit.
 */
std::string UserSession::GetInput()
{
    stdio::StandardIO io;
    std::string character_buffer;
    std::string string_buffer;

    bool is_escape_sequence = false;
    bool is_system_timed_out = false;

    // Start Timeout counter
    m_start_timeout = time(0);

    while (1)
    {
        /*
         * prelogin_timeout_in_minutes_ if User Not Logged in
         * system_timeout_in_minutes_ if User Is Logged in.
         */
        is_system_timed_out = InputTimeout();
        if (is_system_timed_out)
        {
            // Then Disconnect on session.
            // Finish this code here for cleanup and logoff.

            std::cout << "Warning, System Timeout reached!" << std::endl;

            // Reset time for now, this is testing.
            m_start_timeout = time(0);
        }

        // Check STDIO (Non-blocking!) for input.
        if (io.ScanConsoleTTY())
        {
            // Get Single Character Input from Console
            character_buffer = io.ReadConsoleTTY();

            // Reset Timeout Counter on Input.
            m_start_timeout = time(0);

            // Invalid Data, Then start loop again.
            if (character_buffer.empty())
            {
                continue;
            }

            // Don't process multiple esc sequences.
            // grab only the first in a sequence.
            if (character_buffer[0] == 27 && string_buffer.empty())
            {
                if (!is_escape_sequence)
                {
                    is_escape_sequence = true;
                    m_escape_sequence.erase();
                    string_buffer = character_buffer;
                    continue; // Get next character
                }
                else
                {
                    // If we get here again and second char is another ESC, Return 27
                    m_escape_sequence.erase();
                    string_buffer.erase();
                    return "\x1b";
                }
            }

            // Reset, handle if we got a second Escape or two ESC keys in a row.
            is_escape_sequence = false;

            // Check if were appending to current buffer or returning
            if (!string_buffer.empty())
            {
                // Check for Overflow, Input ESC Sequences Should
                // Be Short and Sweet!
                if (string_buffer.size() >= 8)
                {
                    // Drop and re-loop for input.
                    string_buffer.erase();
                    continue;
                }

                // Check for Consecutive ESC sequences, if so exit on ESC
                if (character_buffer[0] == 27 && string_buffer.empty())
                {
                    m_escape_sequence.erase();
                    string_buffer.erase();
                    return "\x1b";
                }
                // Translate Hardware Keys,  ESCOA etc.. to ESC[A
                // This way we can always test for [ and not worry about O
                // Preceeding Function Keys.
                if (character_buffer[0] == 0x4f && string_buffer.size() == 1)
                    character_buffer = '[';

                // Parse ESC Sequence for Match
                if (character_buffer[0] == '[' && string_buffer.size() == 1)
                {
                    // Overwrite ESC and continue Sequence
                    // No Need to Test for Esc in the Sequence Buffer, We'll
                    // Test for everything following ESC.
                    string_buffer = character_buffer;
                    continue;
                }
                else
                {
                    switch (character_buffer[0])
                    {
                        case '[': // [[ Double Brackets F1 Keys.
                            string_buffer += character_buffer;
                            continue;

                        case 'A': // Up
                            string_buffer += character_buffer;
                            m_escape_sequence = string_buffer;
                            string_buffer.erase();
                            return "\x1b";

                        case 'B': // Dn
                            string_buffer += character_buffer;
                            m_escape_sequence = string_buffer;
                            string_buffer.erase();
                            return "\x1b";

                        case 'C': // Lt
                            string_buffer += character_buffer;
                            m_escape_sequence = string_buffer;
                            string_buffer.erase();
                            return "\x1b";

                        case 'D': // Rt
                            string_buffer += character_buffer;
                            m_escape_sequence = string_buffer;
                            string_buffer.erase();
                            return "\x1b";

                        case 'K': // End
                            string_buffer += character_buffer;
                            m_escape_sequence = string_buffer;
                            string_buffer.erase();
                            return "\x1b";

                        case 'F': // End = 0F
                            string_buffer += character_buffer;
                            m_escape_sequence = string_buffer;
                            string_buffer.erase();
                            return "\x1b";

                        case 'H': // Home
                            string_buffer += character_buffer;
                            m_escape_sequence = string_buffer;
                            string_buffer.erase();
                            return "\x1b";

                        case 'V': // PageUP
                            string_buffer += character_buffer;
                            m_escape_sequence = string_buffer;
                            string_buffer.erase();
                            return "\x1b";

                        case 'U': // PageDn
                            string_buffer += character_buffer;
                            m_escape_sequence = string_buffer;
                            string_buffer.erase();
                            return "\x1b";

                        case '1': // Home
                        case '2': // Insert
                        case '3': // DEL
                        case '4': // ESC
                        case '5': // PG UP
                        case '6': // PG DN
                        case '7': // Function Keys.
                        case '8': // Function Keys.
                        case '9': // Function Keys.
                        case '0': // Function Keys.
                            string_buffer += character_buffer;
                            continue;

                        case 80: // Function Keys. P F1
                        case 81: // Function Keys. Q F2
                        case 82: // Function Keys. R F3
                        case 83: // Function Keys. S F4
                            string_buffer += character_buffer;
                            m_escape_sequence = string_buffer;
                            string_buffer.erase();
                            return "\x1b";

                            // End of Number Sequence.
                        case '~': // Tail
                            string_buffer += character_buffer;
                            m_escape_sequence = string_buffer;
                            string_buffer.erase();
                            return "\x1b";

                        case '\0':
                            //current_buffer += ch;
                            m_escape_sequence = string_buffer;
                            string_buffer.erase();
                            return "\x1b";

                        default :
                            // Not ESC Sequence.
                            // Done with loop.
                            string_buffer += ' ';
                            m_escape_sequence = string_buffer;
                            string_buffer.erase();
                            return "\x1b";
                    }
                }
            }
            // Return normal character
            else
            {
                // Received a standard character
                break;
            }
        }
        else
        {
            // If ESC key received wtih no trailing sequence,
            // clear sequence buffer and just return ESC.
            if (string_buffer.size() == 1 && string_buffer[0] == 27)
            {
                string_buffer.erase();
                return "\x1b";
            }

            // Catch Multi-Node Messages Between Node Communication
            // If we have any waiting, then post them to STDOUT.
            // -- Update this to full Session Queue!

            //read_input();
        }
    }

    // Translate ENTER Key, *NIX Terms send CR always test for LF.
    // CRLF is a newline on output only, input is either or.
    if (character_buffer[0] == '\r')
        character_buffer[0] = '\n';

    return character_buffer;
}
