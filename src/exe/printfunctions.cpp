#include "printfunctions.hpp"

#include <iomanip>
#include <iostream>

#include "tableprinter.hpp"
#include "../../include/pet.hpp"

using namespace pet;


void printHelp()
{
    std::string help = ""
            "Usage:\n"
            "    pet [options] <filename>\n"
            "Options\n"
            "    -d, --datadir    Show all data directories\n"
            "    -e, --exported   Show all exported functions\n"
            "    -h, --help       Show this help\n"
            "    -i, --imported   Show all imported functions\n"
            "    -s, --sections   Show all sections\n"
            "    -v, --version    Show image, os and linker versions\n"
            "    --dump           Dump file\n";

    std::cout << help << std::endl;
}


void printVersions(const PEFile& file)
{
    PEFile::Header header = file.header();

    std::cout << "Image version:     " << std::setw(4);
    std::cout << Version(header.image.majorImageVersion, header.image.minorImageVersion).str() << std::endl;

    std::cout << "Linker version:    " << std::setw(4);
    std::cout << Version(header.image.majorLinkerVersion, header.image.minorLinkerVersion).str() << std::endl;

    std::cout << "OS version:        " << std::setw(4);
    std::cout << Version(header.image.majorOperatingSystemVersion, header.image.minorOperatingSystemVersion).str() << std::endl;

    std::cout << "Subsystem version: " << std::setw(4);
    std::cout << Version(header.image.majorSubsystemVersion, header.image.minorSubsystemVersion).str() << std::endl;

    std::cout << std::endl;
}


void printSections(const PEFile& file)
{
    auto sectionHeaders = file.header().sections;

    TablePrinter tp({8, 8, 7, 9, 8},
                    {"Name", "RawAdd", "RawSz", "VirtAdd", "VirtSz"},
                    {std::ios::left,
                         std::ios::right|std::ios::hex,
                         std::ios::right|std::ios::hex,
                         std::ios::right|std::ios::hex,
                         std::ios::right|std::ios::hex},
                    "Sections");

    for(const auto& p: sectionHeaders)
    {
        const HeaderSection& sh = p.second;
        tp.addRow(sh.name, sh.pointerToRawData, sh.sizeOfRawData, sh.virtualAddress, sh.virtualSize);
    }

    std::cout << tp.str() << std::endl;
}


void printDataDirectories(const PEFile& file)
{
    HeaderDataDirectories dataDirectories = file.header().dataDirectories;

    TablePrinter tp({18, 9, 8},
                    {"Directory", "VirtAdd", "VirtSz"},
                    {std::ios::left,
                         std::ios::right | std::ios::hex,
                         std::ios::right | std::ios::hex},
                    "Data directories");

    auto addRow = [&tp](const char* str, HeaderDataDirectories::Entry dde) {tp.addRow(str, dde.virtualAddress, dde.size);};
    addRow("Export", dataDirectories.exportTable);
    addRow("Import", dataDirectories.importTable);
    addRow("Resource", dataDirectories.resourceTable);
    addRow("Exception", dataDirectories.exceptionTable);
    addRow("Certificate", dataDirectories.certificateTable);
    addRow("Base relocation", dataDirectories.baseRelocationTable);
    addRow("Debug", dataDirectories.debug);
    addRow("Architecture", dataDirectories.architecture);
    addRow("Global pointer", dataDirectories.globalPtr);
    addRow("Thread loc storage", dataDirectories.tlsTable);
    addRow("Load config", dataDirectories.loadConfigTable);
    addRow("Bound import", dataDirectories.boundImport);
    addRow("Import address", dataDirectories.importAddressTable);
    addRow("Delay import", dataDirectories.delayImportDescriptor);
    addRow("CLR runtime", dataDirectories.clrRuntimeHeader);

    std::cout << tp.str() << std::endl;
}


void printExportedFunctions(const PEFile& file)
{
    ExportTable et = file.exportTable();

    TablePrinter tp({5, 6, 40},
                    {"Ord", "Addr", "Name"},
                    {std::ios::right | std::ios::dec,
                         std::ios::right | std::ios::hex,
                         std::ios::left},
                    "Exported functions");

    for(std::size_t k=0; k<et.ordinals.size(); k++)
        tp.addRow(et.ordinals[k], et.exportAddresses[k], " " + et.names[k]);

    std::cout << tp.str() << std::endl;
}


void printImportedFunctions(const PEFile& file)
{
    ImportTable it = file.importTable();

    TablePrinter tp({20, 5, 40},
                    {"DLL", "Ord", "Name"},
                    {std::ios::left,
                         std::ios::right | std::ios::hex,
                         std::ios::left},
                    "Imported functions");

    for(const ImportTable::ImportedDLL& dll: it.importedDlls)
        for(std::size_t k=0; k<dll.names.size(); k++)
            tp.addRow(" " + dll.dllName, dll.ordinals[k], " " + dll.names[k]);

    std::cout << tp.str() << std::endl;
}

void printDump(const PEFile& file)
{
    std::string str = file.dump();
    std::cout << str << std::endl;
}
