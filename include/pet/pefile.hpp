#ifndef PET_PEFILE_HPP
#define PET_PEFILE_HPP

#include <fstream>
#include <map>
#include <string>

#include "def.hpp"
#include "streamreader.hpp"
#include "structures.hpp"


PET_BEGIN_NAMESPACE


/**
 * @brief Class representing a Portable Executable file
 */
class PET_DLL PEFile
{
    public:
        /**
         * @brief PE file format
         */
        enum Format
        {
            PE32,     // 32 bits
            PE32Plus, // 64 bits
            Unknown
        };

        /**
         * @brief PE file headers
         */
        struct Header
        {
            /** DOS header */
            HeaderDOS dos;

            /** COFF header */
            HeaderCOFF coff;

            /** Image header */
            HeaderImage image;

            /** Data directories header */
            HeaderDataDirectories dataDirectories;

            /** Sections header */
            std::map<std::string, HeaderSection> sections;
        };


    public:
        /**
         * @brief Construct a PEFile by analysing a file
         * @param filename Filename
         *
         * If an error occurs during the construction, PEFile becomes an invalid
         * file. Validity can be checked with isValid()
         */
        PEFile(const std::string& filename);

        /**
         * @brief Check if the file is valid
         * @return True if image id valid
         */
        bool isValid() const;

        /**
         * @brief Get file format
         * @return File format
         */
        Format format() const;

        /**
         * @brief Get header
         * @return Header
         */
        Header header() const;

        /**
         * @brief Get export table
         * @return Export table
         */
        ExportTable exportTable() const;

        /**
         * @brief Get export table
         * @return Export table
         */
        ImportTable importTable() const;

        /**
         * @brief Dump file to a string
         * @return String of the dump
         *
         * If an error occurs, a null string is returned
         */
        std::string dump() const;


    private:
        /** Flag indicating a valid file */
        bool m_isValid;

        /** Filename */
        std::string m_filename;

        /** File */
        std::ifstream m_file;

        /** Stream reader */
        StreamReader m_reader;

        /** Format */
        Format m_format;

        /** Headers */
        Header m_header;

        /** Export table */
        ExportTable m_exportTable;

        /** Export table */
        ImportTable m_importTable;
};


PET_END_NAMESPACE


#endif
