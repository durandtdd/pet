#ifndef PET_STREAMREADER_HPP
#define PET_STREAMREADER_HPP

#include <istream>
#include <string>

#include "def.hpp"


PET_BEGIN_NAMESPACE


/**
 * @brief Class used to read and interpret a binary stream
 */
class StreamReader
{
    public:
        /**
         * @brief Construct a StreamReader
         * @param stream Stream to read
         */
        StreamReader(std::istream& stream);

        /**
         * @brief Set current position in stream
         * @param pos New position
         */
        void seekg(std::streampos pos);

        /**
         * @brief Get current position in stream
         * @return Position
         */
        std::streampos tellg() const;

        /**
         * @brief Read and interpret bytes as type T
         * @return Read value
         */
        template<typename T>
        T read();

        /**
         * @brief Read the next string (chars until 0x00)
         * @return Read string
         */
        std::string readString();

        /**
         * @brief Skip the next bytes
         * @param count Number of bytes
         */
        void skip(std::streamsize count);


    private:
        /** Stream to read */
        std::istream& m_stream;
};


/*============================================================================*/
template<typename T>
T StreamReader::read()
{
    T value;
    m_stream.read(reinterpret_cast<char*>(&value), sizeof(T));
    return value;
}


PET_END_NAMESPACE

#endif // FILEREADER_HPP
