#ifndef DUMPER_HPP
#define DUMPER_HPP

#include <sstream>
#include <fstream>
#include <string>

#include "pet/def.hpp"


PET_BEGIN_NAMESPACE


/**
 * @brief Class used to dump a stream to a string
 */
class Dumper
{
    public:
        /**
         * @brief Create a Dumper
         * @param stream Stream to dump
         */
        Dumper(std::istream& stream);
     
        /**
         * @brief Get the number of bytes per line
         * @return Number of bytes per line
         */
        std::size_t bytesPerLine() const;

        /**
         * @brief Set the number of bytes per line
         * @param bytesPerLine Number of bytes per line
         */
        void setBytesPerLine(std::size_t bytesPerLine);
        
        /**
         * @brief Set current position
         * @param pos New position
         */
        void seekg(std::streampos pos);

        /**
         * @brief Get current position
         * @return Position
         */
        std::streampos tellg() const;

        /**
         * @brief Dump the next bytes
         * @param count Number of bytes
         */
        void dump(size_t count);

        /**
         * @brief Add a text line to the dump
         * @param text Text
         */
        void addTextLine(const std::string& text = "");

        /**
         * @brief Get generated string
         * @return Generated string
         */
        std::string str() const;


    private:
        /**
         * @brief Print next line to string
         * @param n Number of bytes
         * @param startOffset Starting offset of the line
         */
        void printLine(size_t n, std::size_t startOffset = 0);


    private:
        /** Number of bytes per line */
        std::size_t m_bytesPerLine;

        /** Stream to dump */
        std::istream& m_stream;

        /** Out stream */
        std::ostringstream m_oss;

};


/*============================================================================*/
template <typename T>
std::string hex(T value)
{
    static char chars[] = "0123456789abcdef";

    std::string str;
    for(int k=0; k<2*sizeof(T); k++)
        str += chars[(value >> (8*sizeof(T)-4*(k+1))) & 0x0F];

    return str;
}


PET_END_NAMESPACE


#endif
