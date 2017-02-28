#include "pet.hpp"

#include <algorithm>

#include "dumper.hpp"
#include "pet/streamreader.hpp"
#include "readers.hpp"

#include <iostream>
PET_USE_NAMESPACE


PEFile::PEFile(const std::string& filename):
    m_isValid(true),
    m_filename(filename),
    m_file(m_filename),
    m_reader(m_file),
    m_format(Unknown)
{

    // File
    m_file.exceptions(std::ifstream::failbit);
    if(!m_file)
    {
        m_isValid = false;
        m_error = "File error: Can't open file";
    }

    try
    {
        // DOS header
        uint16 magic = m_reader.read<uint16>();
        if(magic == 0x5a4d)
        {
            m_reader.seekg(0x3c);
            m_header.dos.size = m_reader.read<uint32>();

            m_reader.seekg(m_header.dos.size);
            m_reader.skip(4);
        }

        // COFF Header
        m_header.coff = readCOFFHeader(m_reader);

        std::streampos pos = m_reader.tellg();

        if(m_header.coff.sizeOfOptionalHeader>0)
        {
            // Optional Image Header
            m_header.image = readImageHeader(m_reader);

            // Optional Header Data Directories
            m_header.dataDirectories = readImageDataDirectories(m_reader, m_header.image.numberOfRvaAndSizes);

            m_format = (m_header.image.magic == 0x010b)? PE32 : PE32Plus;
        }

        // Section headers
        m_reader.seekg(pos + (std::streampos)m_header.coff.sizeOfOptionalHeader);
        m_header.sections = readSectionHeaders(m_reader, m_header.coff.numberOfSections);

        // Export table
        if(m_header.dataDirectories.exportTable.virtualAddress>0)
            m_exportTable = readExportTable(m_reader, m_header.sections, m_header.dataDirectories);

        // Import table
        if(m_header.dataDirectories.importTable.virtualAddress>0)
            m_importTable = readImportTable(m_reader, m_header.sections, m_header.dataDirectories, m_format);
    }
    catch(const std::ios::failure&)
    {
        m_isValid = false;
        m_error = "File error: Corrupted or unrecognized file";
    }
}


bool PEFile::isValid() const
{
    return m_isValid;
}


std::string PEFile::error() const
{
    return m_error;
}


PEFile::Format PEFile::format() const
{
    return m_format;
}


PEFile::Header PEFile::header() const
{
    return m_header;
}


ExportTable PEFile::exportTable() const
{
    return m_exportTable;
}


ImportTable PEFile::importTable() const
{
    return m_importTable;
}


std::string PEFile::dump() const
{
    std::ifstream file(m_filename, std::ios::binary | std::ios::in);
    file.exceptions(std::ifstream::failbit);
    if(!m_file)
        return "";

    Dumper dumper(file);

    try
    {
        dumper.addTextLine();
        dumper.addTextLine("########   MS DOS header");
        dumper.dump(m_header.dos.size);

        dumper.addTextLine();
        dumper.addTextLine("########   COFF header");
        dumper.dump(32);

        dumper.addTextLine();
        dumper.addTextLine("########   Optional header");
        dumper.dump(m_header.coff.sizeOfOptionalHeader);

        dumper.addTextLine();
        dumper.addTextLine("########   Sections header");
        dumper.dump(m_header.coff.numberOfSections * 40);


        // Sections
        struct Section
        {
            std::string name;
            uint32 rawAddress = 0;
            uint32 rawSize = 0;
            Section(const std::string& name, uint32 rawAddress, uint32 rawSize): name(name), rawAddress(rawAddress), rawSize(rawSize) {}
        };

        std::vector<Section> sections;
        for(const auto& sh: m_header.sections)
            sections.push_back(Section(sh.first, sh.second.pointerToRawData, sh.second.sizeOfRawData));

        // Sort sections
        std::sort(sections.begin(), sections.end(), [](const Section s1, const Section s2) {return s1.rawAddress < s2.rawAddress;});

        for(auto s: sections)
        {
            if(s.rawAddress > dumper.tellg())
            {
                dumper.addTextLine();
                dumper.addTextLine("########   Padding");
                dumper.dump(s.rawAddress - dumper.tellg());
            }
            dumper.addTextLine();
            dumper.addTextLine("########   " + s.name);
            dumper.dump(s.rawSize);
        }

    }
    catch(const std::ios::failure&)
    {
        return "";
    }

    return dumper.str();
}

