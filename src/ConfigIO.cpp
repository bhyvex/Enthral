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

#include "ConfigIO.hpp"

#include <libconfig.h++>
#include <iostream>
#include <string>

#include <sys/stat.h> // mkdir


// Add Methods for Config Files Exists for Both Program and Home!


/*
 * This will Set the Path and Attempt To Create a
 * New Configuration file where the Executable is located.
 */
bool ConfigIO::GenerateNewSystemConfigProgramPath()
{
    return CreateConfigFile(session_->program_path_, false);
}

/*
 * This will Set the Path and Attempt To Create a
 * New Configuration file where the Users Home Directory is located.
 * This will also create an Enthral subdirectory inside of $HOME
 */
bool ConfigIO::GenerateNewSystemConfigHome()
{
    return CreateConfigFile(session_->home_directory_, true);
}

/*
 * Create Configuration File, If home directory is passed,
 * Then Create Enthral Subfolder to hold configuration.
 */
bool ConfigIO::CreateConfigFile(const std::string &path, bool use_home_directory)
{
    using namespace libconfig;
    Config cfg;

    std::string full_path = path;
    if (use_home_directory)
    {
        // Add Enthral Subfolder to Home Directory.
        full_path += "Enthral";

        // Set Permission Owner R/W/X, Group, And Everyone Else None!
        if (mkdir(full_path.c_str(), 0700) == -1 && errno != EEXIST)
        {
            std::cout << "Error: unable to create " << full_path << ", check permissions!"
                      << std::endl;
        }

        // Append Forward Slash that Enthral.
        full_path += "/";
    }

    // Add Configuration Filename to Path

    cfg.setTabWidth(4);
    cfg.setIncludeDir(full_path.c_str());

    // Add Filename to Path
    full_path += "enthral.conf";

    // Testing Initial Creation, Will need a dialog to ask questions!
    Setting &set = cfg.getRoot();
    set.add("bbs_name", Setting::TypeString) = "haunting the chapel";

    try
    {
        cfg.writeFile(full_path.c_str());
        std::cerr << "Updated configuration successfully written to: " << full_path
                  << std::endl;
    }
    catch(const FileIOException &fioex)
    {
        std::cerr << "I/O error while writing file: " << full_path << std::endl;
        return(EXIT_FAILURE);
    }


    /*
    // Setup Configuration Tree.
    Setting &root = cfg.getRoot();

    if(!root.exists("inventory"))
    root.add("inventory", Setting::TypeGroup);

    Setting &inventory = root["inventory"];

    if(! inventory.exists("movies"))
    inventory.add("movies", Setting::TypeList);

    Setting &movies = inventory["movies"];

    // Create the new movie entry.
    Setting &movie = movies.add(Setting::TypeGroup);

    movie.add("title", Setting::TypeString) = "Buckaroo Banzai";
    movie.add("media", Setting::TypeString) = "DVD";
    movie.add("price", Setting::TypeFloat) = 12.99;
    movie.add("qty", Setting::TypeInt) = 20;

    // Write out the updated configuration.
    try
    {
    cfg.writeFile(output_file);
    cerr << "Updated configuration successfully written to: " << output_file
    << endl;

    }
    catch(const FileIOException &fioex)
    {
    cerr << "
    I/O error while writing file: " << output_file << endl;
    return(EXIT_FAILURE);
    }
    */


    return true;
}
