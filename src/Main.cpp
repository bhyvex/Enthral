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


# ifdef HAVE_CONFIG_H
# include <config.h>
# endif

#include "UserSession.hpp"
#include "StandardIO.hpp"
#include "CommonIO.hpp"
#include "ConfigIO.hpp"

#include <iostream>
#include <fstream>
#include <csignal>

// Test time of Session Start
#include <ctime>
#include <unistd.h>   // usleep
#include <sys/stat.h> // mkdir
#include <cstring>    // strcmp

//#include <cstdio>
//#include <cerrno>
//#include <cstring>
//#include <cstdlib>
//#include <climits>

//#include <pty.h>     // Struct Winsize
//#include <termios.h>
//#include <sys/stat.h>
//#include <sys/wait.h>

using namespace std;

/*
 * Issue, we need some sort of global container, or
 * Singleton that can handle saving the state
 * On user Disconnect,  OR just Ignore It, stuff is updated as it happens
 * anything is is junk to be lost!  Not sure i want that!
 */

/*
 * Handle Connections that hangup on the server
 * We don't want ghost nodes in the system.
 */
void KillZombieSession(int the_signal)
{
    std::cout << "Killazombie System Shutdown (User Hung Up on the System!)." << std::endl;

    switch(the_signal)
    {
        case SIGHUP:
            std::cout << "killazombie SIGHUP." << std::endl;
            break;

        case SIGTERM:
            std::cout << "killazombie SIGTERM." << std::endl;
            break;

        case SIGINT:
            std::cout << "killazombie SIGINT." << std::endl;
            break;

        case SIGILL:
            std::cout << "killazombie SIGILL." << std::endl;
            break;

        case SIGABRT:
            std::cout << "killazombie SIGABRT." << std::endl;
            break;

        case SIGQUIT:
            std::cout << "killazombie SIGQUIT." << std::endl;
            break;

        case SIGKILL:
            std::cout << "killazombie SIGKILL." << std::endl;
            break;

            // Masked Signals
        case SIGCHLD:
            std::cout << "killazombie SIGCHLD." << std::endl;
            break;

        case SIGTTOU:
            std::cout << "killazombie SIGTTOU." << std::endl;
            break;

        default:
            std::cout << "Unknown Signal." << std::endl;
            break;
    }
    // If system was hung up on, then there is nothing
    // To reset back for the TTY since it will be closed.
    exit(the_signal);
}

/*
 * Called on System Exit
 */
void ExitTheSystem(void)
{


}

/*
 * Show Comamnd Line Help
 */
void ShowCommandLineOptions(void)
{
    std::cout << std::endl << "Enthral BBS: Commandline Arguments." << std::endl;
    std::cout << "===================================" << std::endl;


    std::cout << "-l  local login." << std::endl;
    std::cout << "-c  configuration setup." << std::endl;
    std::cout << "-v  display version." << std::endl;
    std::cout << "-h  display this message." << std::endl;
    std::cout << std::endl;
}

/*
 * Main Program Loop { Arguments passed from entelnetd }
 *
 * argv[1]: 127.0.0.1   ip address
 * argv[2]: localhost   hostname
 * argv[3]: -f          flag for username
 * argv[4]: bbs         bbs user name
 * argv[5]: /tmp/enthral/3622f86f-54ba-41e3-835c-1f8686c4fdbc
 *                      This is a unique drop file passed from entelnetd
 *                      for TERM detection from Telopt sequences.
 *
 */
int main(int argc, char *argv[])
{
    // Make Sure Tmp folder is created and ready for accepting the drop files.
    // Permissions BBS User Read/Write/Execute, Group, Everyone Else None!
    if (mkdir("/tmp/enthral", 0700) == -1 && errno != EEXIST)
    {
        std::cout << "Error: unable to create /tmp/enthral, check permissions of /tmp!"
                  << std::endl;
    }

    // Let get a list of all commandline arguments, make sure what is passed by
    // User, vs the telnet server.  This is Temp, handy for testing.
    if (argc > 1)
    {
        for (auto count = 1; count < argc; count++)
        {
            std::cout << "argv[" << count << "]: " << argv[count] << std::endl;
        }
    }


    // The handle to the user's session and all related
    // Session Information. This is passed between classes.
    UserSession session;

    // Setup Signal Checking if Telnet was killed.
    (void) signal(SIGHUP,  KillZombieSession);
    (void) signal(SIGTERM, KillZombieSession);
    (void) signal(SIGINT,  KillZombieSession);
    (void) signal(SIGILL,  KillZombieSession);
    (void) signal(SIGKILL, KillZombieSession);
    (void) signal(SIGABRT, KillZombieSession);
    (void) signal(SIGQUIT, KillZombieSession);

    // Masked Signals.
    (void) signal(SIGTTOU, SIG_IGN);
    (void) signal(SIGCHLD, SIG_IGN);

    // Call Cleanup on System Exit.
    atexit(ExitTheSystem);

    // Detect And Setup the initial Window Size.
    // This can be Called For initial setup of Terminal Windows
    // Parameters, and also double checked while loading interfaces.
    session.CheckTerminalSize();

    // Get Users incoming IP Address
    if(argc >= 3)
    {
        // Passed from Xinetd
        session.m_ip_address = argv[1];
        session.m_ip_hostname = argv[2];
    }
    else
    {
        // Local Console
        session.m_ip_address = "127.0.0.1";
        session.m_ip_hostname = "Localhost";
    }

    // Check Terminal Type
    if(argc >= 6)
    {
        // Give the Server Time to Negotiate the Term from the Client.
        usleep(400000*3);

        // Get the Terminal Environment Passed from Xinetd
        std::ifstream in_stream;
        in_stream.open( argv[5] );
        if (in_stream.is_open())
        {
            // Not getting here.
            if(!in_stream.eof())
            {
                getline(in_stream, session.m_terminal_type);
            }
            in_stream.close();
        }
        else // Any Issues, then grab default from Terminal.
        {
            //std::cout << "Enable to Read term File" << std::endl;
            if (const char* env_p = std::getenv((char *)"TERM"))
                session.m_terminal_type = env_p;
        }
    }
    else
    {
        // Local Terminals
        //std::cout << "Getting local-term" << std::endl;
        if (const char* env_p = std::getenv((char *)"TERM"))
            session.m_terminal_type = env_p;
    }



    // Check Get Options for manual startup and setup processing.
    char tmp = 0;
    while((tmp = getopt(argc, argv, "f:s:hvl")) != -1)
    {
        switch(tmp)
        {
                //option h show the help infomation
            case 'h':
                ShowCommandLineOptions();
                break;

                //option u present the username
            case 's':
                if(strcmp(optarg,"1") == 0)
                    std::cout << "You have saved the password" << std::endl;

                else if(strcmp(optarg,"0") == 0)
                    std::cout << "You have chosen to forget the password" << std::endl;

                else
                    //invail input will get the heil infomation
                    std::cout << "Enthral BBS: Invalid Command Line Parameters Sepcified!" << std::endl;
                ShowCommandLineOptions();
                break;

                //option v show the version infomation
            case 'v':
                std::cout << "Version: " << data::g_bbs_version << std::endl;
                break;

                // BBS User ID passed from entelnetd
            case 'f':
                break;

                //invail input will get the heil infomation
            default:
                std::cout << "Enthral BBS: Invalid Command Line Parameters Sepcified!" << std::endl;
                ShowCommandLineOptions();
                break;
        }
    }

    // No valid Command Line Arguments.
    if (tmp == -1 && argc == 1)
    {
        std::cout << "Enthral BBS: No Command Line Parameters Sepcified!" << std::endl;
        ShowCommandLineOptions();

        // We Would normally exit here and wait for valid parameters.
    }

    // Read In the Path where the executable is located
    // So we can find the configuration file.
    std::string path = common::GetProgramPath();
    if (path.empty())
    {
        std::cout << "Exception, Unable to locate executable path!" << std::endl;
        exit(1);
    }
    session.m_program_path = path;

    // Get BBS Users Home Directory
    std::cout << std::endl << "Initial Session Variables" << std::endl;
    std::cout << "=========================" << std::endl;

    session.m_home_directory = common::GetSystemHomeDirectory();
    session.m_home_directory += '/'; // Append Forward Slash.
    std::cout << "HOME: " << session.m_home_directory << std::endl;

// Debug Settings.
    std::cout << "ip address: "  << session.m_ip_address << std::endl;
    std::cout << "hostname: "    << session.m_ip_hostname << std::endl;
    std::cout << "path: "        << path << std::endl;
    std::cout << "term: "        << session.m_terminal_type << std::endl;
    std::cout << "term width: "  << session.m_terminal_width << std::endl;
    std::cout << "term height: " << session.m_terminal_height << std::endl;

    std::cout << "Session Start: "
              << std::asctime(std::localtime(&session.m_session_start))
              << std::endl;


    // Test Generating a Test Config file in the Home Directory
    // Create Enthral in root, then enthral.conf inside.
    ConfigIO cfg(&session);
    cfg.GenerateNewSystemConfigHome();



    /*
    // Testing some general IO functions.

        // Setup the Terminal
        stdio::StandardIO io;
        io.InitConsoleTTY();

        // Test Input, note namespaces keep static data ! yay
        while(1)
        {
            std::string temp = session.GetInput();
            if (temp[0] == 27)
            {
                std::cout << session.GetEscapeSequence() << std::flush;
            }
            else
            {
                std::cout << temp << std::flush;

                if (toupper(temp[0]) == 'Q')
                {
                    std::cout << std::endl;
                    break;
                }
            }
        }

        // Return Terminal to initial state before exit.
        io.ResetConsoleTTY();
         */
    exit(0);
}
