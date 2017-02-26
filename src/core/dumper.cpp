#include "dumper.hpp"

#include <vector>

PET_USE_NAMESPACE


Dumper::Dumper(std::istream& stream):
    m_stream(stream),
    m_bytesPerLine(16)
{
}


size_t Dumper::bytesPerLine() const
{
    return m_bytesPerLine;
}


void Dumper::setBytesPerLine(std::size_t bytesPerLine)
{
    m_bytesPerLine = (bytesPerLine>0)? bytesPerLine : 16;
}


void Dumper::seekg(std::streampos pos)
{
    m_stream.seekg(pos);
}


std::streampos Dumper::tellg() const
{
    return m_stream.tellg();
}


void Dumper::dump(std::size_t count)
{
    // Current offset in file
    std::streampos offset = m_stream.tellg();

    // Bytes already written on previous line
    std::size_t bytesInLine = offset % m_bytesPerLine;

    // Print lines
    while(count>0)
    {
        // Print min(count, bytes to fill line)
        std::size_t n = (count > m_bytesPerLine-bytesInLine)? m_bytesPerLine-bytesInLine : count;

        printLine(n, bytesInLine);

        bytesInLine = 0;
        count -= n;
    }
}


void Dumper::addTextLine(const std::string& text)
{
    m_oss << text << "\n";
}


std::string Dumper::str() const
{
    return m_oss.str();
}


void Dumper::printLine(std::size_t n, std::size_t startOffset)
{
    // Line offset
    uint32 offset = (uint32) ((m_stream.tellg() / m_bytesPerLine) * m_bytesPerLine);

    // Read bytes
    std::vector<unsigned char> bytes(n, 0);
    m_stream.read((char*)bytes.data(), n);

    // Spaces
    std::size_t ns1 = 3 + 3*startOffset; // Between offset and hex values
    std::size_t ns2 = 3 + 3*(m_bytesPerLine - bytes.size() - startOffset) + startOffset - 1; // Between hex values and text
    std::size_t ns3 = m_bytesPerLine - bytes.size() -  startOffset;

    // Line offset
    m_oss << hex(offset);

    // Spacing
    m_oss << std::string(ns1, ' ');

    // Hex values
    for(char b: bytes)
        m_oss << hex(b) << " ";

    // Spacing
    m_oss << std::string(ns2, ' ');

    // Text
    for(char b: bytes)
    {
        char ch = (b>32 && b<127)? b : '.';
        m_oss << ch;
    }

    // Spacing
    m_oss << std::string(ns3, ' ');

    m_oss << "\n";
}

