#include "pet/streamreader.hpp"

using namespace pet;


StreamReader::StreamReader(std::istream &stream):
    m_stream(stream)
{
    m_stream.seekg(0);
}


void StreamReader::seekg(std::streampos pos)
{
    m_stream.seekg(pos);
}


std::streampos StreamReader::tellg() const
{
    return m_stream.tellg();
}


std::string StreamReader::readString()
{
    std::string str;

    char ch = 0x00;
    m_stream.read(&ch, 1);

    while(ch != 0x00)
    {
        str += ch;
        m_stream.read(&ch, 1);
    }

    return str;
}


void StreamReader::skip(std::streamsize count)
{
    m_stream.seekg(m_stream.tellg() + count);
}

