#include <iostream>

#include "tableprinter.hpp"
#include "printfunctions.hpp"
#include "../../include/pet.hpp"

using namespace pet;


/**
 * @brief Structures indicating operations desired by the user
 */
struct Options
{
    std::string name = "";
    bool exported = false;
    bool imported = false;
    bool versions = false;
    bool sections = false;
    bool dataDirectories = false;
    bool help = false;
    bool dump = false;

    std::string error = "";
};


/**
 * @brief Parse arguments
 * @param argc Number of arguments
 * @param argv Arguments
 * @return Options structure indicating desired operations
 */
Options parseArguments(int argc, char* argv[])
{
    Options opt;
    for(int k=1; k<argc; k++)
    {
        std::string arg = argv[k];

        if(arg == "--exported" || arg == "-e")
            opt.exported = true;

        else if(arg == "--imported" || arg == "-i")
            opt.imported = true;

        else if(arg == "--version" || arg == "-v")
            opt.versions = true;

        else if(arg == "--sections" || arg == "-s")
            opt.sections = true;

        else if(arg == "-d" || arg == "--datadir")
            opt.dataDirectories = true;

        else if(arg == "--dump")
            opt.dump = true;

        else if(arg == "--help" || arg == "-h")
            opt.help = true;

        else if(arg[0] != '-' && opt.name == "")
            opt.name = arg;

        else
        {
            opt.error = "Unknown option: " + arg;
            return opt;
        }
    }

    return opt;
}



/**
 * @brief Main
 */
int main(int argc, char* argv[])
{
    Options opt = parseArguments(argc, argv);

    if(opt.error != "")
    {
        std::cout << opt.error << std::endl;
        printHelp();
    }
    else if(opt.help == true || opt.name == "")
        printHelp();
    else
    {
        // Default operations
        if(argc == 2)
        {
            opt.exported = true;
            opt.imported = true;
        }

        try
        {
            PEFile file(opt.name);

            if(opt.versions)
                printVersions(file);

            if(opt.sections)
                printSections(file);

            if(opt.dataDirectories)
                printDataDirectories(file);

            if(opt.exported)
                printExportedFunctions(file);

            if(opt.imported)
                printImportedFunctions(file);

            if(opt.dump)
                printDump(file);
        }
        catch(const Error& err)
        {
            std::cout << err.what() << std::endl << std::endl;
        }
    }

    return 0;
}



