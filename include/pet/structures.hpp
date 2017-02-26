#ifndef PET_STRUCTURES_HPP
#define PET_STRUCTURES_HPP

#include <string>
#include <vector>

#include "def.hpp"


PET_BEGIN_NAMESPACE


/**
 * @brief Class representing a version
 */
struct PET_DLL Version
{
    /** Major number */
    uint16 major;

    /** Minor number */
    uint16 minor;

    /**
     * @brief Construct a Version
     * @param major Major number
     * @param minor Minor number
     */
    Version(uint16 major = 0, uint16 minor = 0):
        major(major),
        minor(minor)
    {}

    /**
     * @brief Convert version to string major.minor
     * @return Version as a string
     */
    std::string str()
    {
        return std::to_string(major) + "." + std::to_string(minor);
    }
};


/**
 * @brief DOS header
 */
struct PET_DLL HeaderDOS
{
    uint32 size = 0;
};


/**
 * @brief COFF Header
 */
struct PET_DLL HeaderCOFF
{
    /** Target machine type */
    uint16 machine = 0;

    /** Number of sections */
    uint16 numberOfSections = 0;

    /** File creation timestamp */
    uint32 timeDateStamp = 0;

    /** COFF symbol table offset (should be 0) */
    uint32 pointerToSymbolTable = 0;

    /** Number of symbols in COFF table (should be 0) */
    uint32 numberOfSymbols = 0;

    /** Image header size */
    uint16 sizeOfOptionalHeader = 0;

    /** File characteristics */
    uint16 characteristics = 0;
};


/**
 * @brief Image header
 */
struct PET_DLL HeaderImage
{
    // Standard fields
    /** File format */
    uint16 magic = 0;

    /** Major linker version */
    uint8 majorLinkerVersion = 0;

    /** Minor linker version */
    uint8 minorLinkerVersion = 0;

    /** Code size */
    uint32 sizeOfCode = 0;

    /** Initialized data size */
    uint32 sizeOfInitializedData = 0;

    /** Uninitialized data size */
    uint32 sizeOfUninitializedData = 0;

    /** Entry point virtual address */
    uint32 addressOfEntryPoint = 0;

    /** Code virtual address */
    uint32 baseOfCode = 0;

    /** Data virtual address (only PE32) */
    uint32 baseOfData = 0;


    // Windows specific fields
    /** Preferred image virtual address*/
    uint64 imageBase = 0;

    /** Sections alignment in memory */
    uint32 sectionAlignment = 0;

    /** Alignment in file */
    uint32 fileAlignement = 0;

    /** Major required OS version */
    uint16 majorOperatingSystemVersion = 0;

    /** Minor required OS version */
    uint16 minorOperatingSystemVersion = 0;

    /** Major image version */
    uint16 majorImageVersion = 0;

    /** Minor image version */
    uint16 minorImageVersion = 0;

    /** Major subsystem version */
    uint16 majorSubsystemVersion = 0;

    /** Minor subsystem version */
    uint16 minorSubsystemVersion = 0;

    /** Reserved (must be 0) */
    uint32 win32VersionValue = 0;

    /** Image size in memory */
    uint32 sizeOfImage = 0;

    /** Size of MS-DOS, PE and section headers, aligned */
    uint32 sizeOfHeaders = 0;

    /** Image file checksum */
    uint32 checksum = 0;

    /** Subsystem required */
    uint16 subsystem = 0;

    /** DLL characteristics */
    uint16 dllCharacteristics = 0;

    /** Stack to reserve */
    uint64 sizeOfStackReserve = 0;

    /** Stack to commit */
    uint64 sizeOfStackCommit = 0;

    /** Heap to reserve */
    uint64 sizeOfHeapReserve = 0;

    /** heap to commit */
    uint64 sizeOfHeapCommit = 0;

    /** Reserved (must be 0) */
    uint32 loaderFlags = 0;

    /** Number of data directory entries */
    uint32 numberOfRvaAndSizes = 0;
};


/**
 * @brief Data-directory entries header
 */
struct PET_DLL HeaderDataDirectories
{
    struct PET_DLL Entry
    {
        uint32 virtualAddress = 0;
        uint32 size = 0;
    };

    /** Export table */
    Entry exportTable;

    /** Import table */
    Entry importTable;

    /** Resource table */
    Entry resourceTable;

    /** Exception table */
    Entry exceptionTable;

    /** Certificate table */
    Entry certificateTable;

    /** Base relocation table */
    Entry baseRelocationTable;

    /** Debug data */
    Entry debug;

    /** Reserved (must be 0) */
    Entry architecture;

    /** Global pointer register (size must be 0) */
    Entry globalPtr;

    /** Thread local storage table */
    Entry tlsTable;

    /** Lod configuration table */
    Entry loadConfigTable;

    /** Bound import table */
    Entry boundImport;

    /** Import address table */
    Entry importAddressTable;

    /** Delay import descriptor table */
    Entry delayImportDescriptor;

    /** CLR runtime header */
    Entry clrRuntimeHeader;
};


/**
 * @brief A section header
 */
struct PET_DLL HeaderSection
{
    /** UTF8 name */
    uint8 name[8] = {0, 0, 0, 0, 0, 0, 0, 0};

    /** Size in memory */
    uint32 virtualSize = 0;

    /** Address in memory (images) or address before relocation (objects) */
    uint32 virtualAddress = 0;

    /** Initialized data size (images) or section size (objects) */
    uint32 sizeOfRawData = 0;

    /** Address in file */
    uint32 pointerToRawData = 0;

    /** Address in file of relocation entries (0 for images) */
    uint32 pointerToRelocations = 0;

    /** Address of line number entries (should be 0) */
    uint32 pointerToLineNumbers = 0;

    /** Number of relocation entries (0 for images) */
    uint16 numberOfRelocations = 0;

    /** Number of line number entries (should be 0) */
    uint16 numberOfLineNumbers = 0;

    /** Section characteristics*/
    uint32 characteristics = 0;
};


/**
 * @brief Base struct for tables
 */
struct PET_DLL Table
{
    /** Virtual address */
    uint32 virtualAddress = 0;

    /** Virtual size */
    uint32 virtualSize = 0;

    /** Raw address */
    uint32 rawAddress = 0;

    /**
     * @brief Convert a virtual address to a raw address
     * @param Virutal address
     * @return Raw address
     */
    uint32 virtualToRaw(uint32 virtualAddress) const
    {
        return rawAddress + (virtualAddress-this->virtualAddress);
    }
};


/**
 * @brief Struct representing an export table
 */
struct PET_DLL ExportTable: Table
{
    /** Version */
    Version version;

    /** DLL name */
    std::string dllName;

    /** Addresses*/
    std::vector<uint32> exportAddresses;

    /** Ordinals */
    std::vector<uint16> ordinals;

    /** Names */
    std::vector<std::string> names;
};


/**
 * @brief Struct representing an import table
 */
struct PET_DLL ImportTable: Table
{
    struct ImportedDLL
    {
        std::string dllName;
        std::vector<uint16> ordinals;
        std::vector<std::string> names;
    };

    /** Imported DLL */
    std::vector<ImportedDLL> importedDlls;
};


PET_END_NAMESPACE


#endif
