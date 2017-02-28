#include "readers.hpp"

#include "pet/streamreader.hpp"


PET_BEGIN_NAMESPACE


HeaderCOFF pet::readCOFFHeader(StreamReader& reader)
{
    HeaderCOFF header;
    header = reader.read<HeaderCOFF>();
    return header;
}


HeaderImage readImageHeader(StreamReader& reader)
{
    HeaderImage header;

    header.magic = reader.read<uint16>();
    header.majorLinkerVersion = reader.read<uint8>();
    header.minorLinkerVersion = reader.read<uint8>();
    header.sizeOfCode = reader.read<uint32>();
    header.sizeOfInitializedData = reader.read<uint32>();
    header.sizeOfUninitializedData = reader.read<uint32>();
    header.addressOfEntryPoint = reader.read<uint32>();
    header.baseOfCode = reader.read<uint32>();
    if(header.magic == 0x010b)
        header.baseOfData = reader.read<uint32>();

    if(header.magic == 0x010b)
        header.imageBase = reader.read<uint32>();
    else
        header.imageBase = reader.read<uint64>();
    header.sectionAlignment = reader.read<uint32>();
    header.fileAlignement = reader.read<uint32>();
    header.majorOperatingSystemVersion = reader.read<uint16>();
    header.minorOperatingSystemVersion = reader.read<uint16>();
    header.majorImageVersion = reader.read<uint16>();
    header.minorImageVersion = reader.read<uint16>();
    header.majorSubsystemVersion = reader.read<uint16>();
    header.minorSubsystemVersion = reader.read<uint16>();
    header.win32VersionValue = reader.read<uint32>();
    header.sizeOfImage = reader.read<uint32>();
    header.sizeOfHeaders = reader.read<uint32>();
    header.checksum = reader.read<uint32>();
    header.subsystem = reader.read<uint16>();
    header.dllCharacteristics = reader.read<uint16>();
    if(header.magic == 0x010b)
    {
        header.sizeOfStackReserve = reader.read<uint32>();
        header.sizeOfStackCommit = reader.read<uint32>();
        header.sizeOfHeapReserve = reader.read<uint32>();
        header.sizeOfHeapCommit = reader.read<uint32>();
    }
    else
    {
        header.sizeOfStackReserve = reader.read<uint64>();
        header.sizeOfStackCommit = reader.read<uint64>();
        header.sizeOfHeapReserve = reader.read<uint64>();
        header.sizeOfHeapCommit = reader.read<uint64>();
    }
    header.loaderFlags = reader.read<uint32>();
    header.numberOfRvaAndSizes = reader.read<uint32>();

    return header;
}


HeaderDataDirectories readImageDataDirectories(StreamReader& reader, int nEntries)
{
    HeaderDataDirectories header;

    if(nEntries >= 1)
        header.exportTable = reader.read<HeaderDataDirectories::Entry>();

    if(nEntries >= 2)
        header.importTable = reader.read<HeaderDataDirectories::Entry>();

    if(nEntries >= 3)
        header.resourceTable = reader.read<HeaderDataDirectories::Entry>();

    if(nEntries >= 4)
        header.exceptionTable = reader.read<HeaderDataDirectories::Entry>();

    if(nEntries >= 5)
        header.certificateTable = reader.read<HeaderDataDirectories::Entry>();

    if(nEntries >= 6)
        header.baseRelocationTable = reader.read<HeaderDataDirectories::Entry>();

    if(nEntries >= 7)
        header.debug = reader.read<HeaderDataDirectories::Entry>();

    if(nEntries >= 8)
        header.architecture = reader.read<HeaderDataDirectories::Entry>();

    if(nEntries >= 9)
        header.globalPtr = reader.read<HeaderDataDirectories::Entry>();

    if(nEntries >= 10)
        header.tlsTable = reader.read<HeaderDataDirectories::Entry>();

    if(nEntries >= 11)
        header.loadConfigTable = reader.read<HeaderDataDirectories::Entry>();

    if(nEntries >= 12)
        header.boundImport = reader.read<HeaderDataDirectories::Entry>();

    if(nEntries >= 13)
        header.importAddressTable = reader.read<HeaderDataDirectories::Entry>();

    if(nEntries >= 14)
        header.delayImportDescriptor = reader.read<HeaderDataDirectories::Entry>();

    if(nEntries >= 15)
        header.clrRuntimeHeader = reader.read<HeaderDataDirectories::Entry>();

    return header;
}


std::map<std::string, HeaderSection> readSectionHeaders(StreamReader& reader, int nSections)
{
    std::map<std::string, HeaderSection> headers;

    // Read every section
    for(uint16 k=0; k<nSections; k++)
    {
        // Read section
        HeaderSection sh = reader.read<HeaderSection>();

        // Convert name (remove null chars)
        std::string name((char*)sh.name, 8);
        name.erase(name.begin() + name.find_first_of('\0'), name.end());

        // Save
        headers[name] = sh;
    }

    return headers;
}


ExportTable readExportTable(StreamReader& reader, const std::map<std::string, HeaderSection>& sectionHeaders, const HeaderDataDirectories& dataDirectories)
{
    ExportTable exportTable;

    // Look for table in .edata if it exists, otherwise in .rdata
    uint32 rvaExportTable = dataDirectories.exportTable.virtualAddress;
    auto section = sectionHeaders.find(".edata");
    if(section == sectionHeaders.end())
    {
        section = sectionHeaders.find(".rdata");
        if(section == sectionHeaders.end())
            return exportTable; // No table found
    }

    std::size_t pos = section->second.pointerToRawData + (rvaExportTable - section->second.virtualAddress);
    reader.seekg(pos);

    // Base addresses
    exportTable.virtualAddress = rvaExportTable;
    exportTable.virtualSize = dataDirectories.exportTable.size;
    exportTable.rawAddress = (uint32)pos;

    // Export directory table
    reader.skip(4); // Export flags
    reader.skip(4); // Timestamp
    exportTable.version.major = reader.read<uint16>();
    exportTable.version.minor = reader.read<uint16>();
    uint32 nameRVA = reader.read<uint32>();
    uint32 ordinalBase = reader.read<uint32>();
    uint32 nbAddTableEntries = reader.read<uint32>();
    uint32 nbNamePointer = reader.read<uint32>();
    uint32 addressTableRVA = reader.read<uint32>();
    uint32 namePointersRVA = reader.read<uint32>();
    uint32 ordinalTableRVA = reader.read<uint32>();

    // Export address table
    reader.seekg(exportTable.virtualToRaw(addressTableRVA));
    for(uint32 k=0; k<nbAddTableEntries; k++)
        exportTable.exportAddresses.push_back(reader.read<uint32>());

    // Name pointers table
    std::vector<uint32> pointers;
    reader.seekg(exportTable.virtualToRaw(namePointersRVA));
    for(uint32 k=0; k<nbNamePointer; k++)
        pointers.push_back(reader.read<uint32>());

    for(uint32 pointer: pointers)
    {
        reader.seekg(exportTable.virtualToRaw(pointer));
        exportTable.names.push_back(reader.readString());
    }

    // Ordinals table
    reader.seekg(exportTable.virtualToRaw(ordinalTableRVA));
    for(uint32 k=0; k<nbNamePointer; k++)
        exportTable.ordinals.push_back(reader.read<uint16>()+ordinalBase);

    // Name
    reader.seekg(exportTable.virtualToRaw(nameRVA));
    exportTable.dllName = reader.readString();

    return exportTable;
}


ImportTable readImportTable(StreamReader& reader, const std::map<std::string, HeaderSection>& sectionHeaders, const HeaderDataDirectories& dataDirectories, PEFile::Format format)
{
    ImportTable importTable;

    // Look for table in .idata if it exists, otherwise in .rdata
    uint32 rvaImportTable = dataDirectories.importTable.virtualAddress;
    auto section = sectionHeaders.find(".idata");
    if(section == sectionHeaders.end())
    {
        section = sectionHeaders.find(".rdata");
        if(section == sectionHeaders.end())
            return importTable; // No table found
    }

    std::size_t pos = section->second.pointerToRawData + (rvaImportTable - section->second.virtualAddress);
    reader.seekg(pos);

    // Base addresses
    importTable.virtualAddress = rvaImportTable;
    importTable.virtualSize = dataDirectories.importTable.size;
    importTable.rawAddress = (uint32)pos;

    // Import directory table
    struct DirectoryEntry
    {
        uint32 importLookupTableRva = 0;
        uint32 timeDate = 0;
        uint32 forwarderChain = 0;
        uint32 nameRva = 0;
        uint32 importAddressTableRva = 0;
    };

    std::vector<DirectoryEntry> directoryTable;
    DirectoryEntry de = reader.read<DirectoryEntry>();
    while(de.nameRva != 0)
    {
        directoryTable.push_back(de);
        de = reader.read<DirectoryEntry>();
    }

    // Read each DLL entries table
    for(DirectoryEntry de: directoryTable)
    {
        ImportTable::ImportedDLL dll;

        // Import address tables
        reader.seekg(importTable.virtualToRaw(de.importAddressTableRva));

        while(true)
        {
            // Read lookup entry
            uint32 lookupEntry = 0;
            if(format == PEFile::PE32)
                lookupEntry = reader.read<uint32>();
            else
            {
                uint64 raw = reader.read<uint64>();
                lookupEntry = (uint32)((raw | ((raw & 0x8000000000000000) << 31)) & 0xffffffff);
            }

            // End of table
            if(lookupEntry == 0)
                break;

            // Read ordinal / name
            if(lookupEntry & 0x80000000)
            {
                dll.ordinals.push_back((uint16)(lookupEntry & 0x7fff));
                dll.names.push_back("");
            }
            else
            {
                std::streampos pos = reader.tellg();

                reader.seekg(importTable.virtualToRaw(lookupEntry));
                uint16 hint = reader.read<uint16>();
                std::string name = reader.readString();

                dll.ordinals.push_back(0xffff);
                dll.names.push_back(name);

                reader.seekg(pos);
            }
        }

        // Name
        reader.seekg(importTable.virtualToRaw(de.nameRva));
        dll.dllName = reader.readString();

        // Save
        importTable.importedDlls.push_back(dll);
    }

    return importTable;
}

PET_END_NAMESPACE
