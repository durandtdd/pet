#ifndef PRINTFUNCTIONS_HPP
#define PRINTFUNCTIONS_HPP

namespace pet
{
    class PEFile;
}

/**
 * @brief Print help
 */
void printHelp();

/**
 * @brief Print image, linker and system versions
 */
void printVersions(const pet::PEFile& file);

/**
 * @brief Print sections information
 */
void printSections(const pet::PEFile& file);

/**
 * @brief Print data directories information
 */
void printDataDirectories(const pet::PEFile& file);

/**
 * @brief Print all exported functions
 */
void printExportedFunctions(const pet::PEFile& file);

/**
 * @brief Print all imported functions
 */
void printImportedFunctions(const pet::PEFile& file);

/**
 * @brief Print dump
 */
void printDump(const pet::PEFile& file);

#endif
