#ifndef READERS_HPP
#define READERS_HPP

#include <map>
#include <vector>

#include "../../include/pet.hpp"


PET_BEGIN_NAMESPACE

class StreamReader;


HeaderCOFF readCOFFHeader(StreamReader& reader);
HeaderImage readImageHeader(StreamReader& reader);
HeaderDataDirectories readImageDataDirectories(StreamReader& reader, int nEntries);
std::map<std::string, HeaderSection> readSectionHeaders(StreamReader& reader, int nSections);
ExportTable readExportTable(StreamReader& reader, const std::map<std::string, HeaderSection>& sectionHeaders, const HeaderDataDirectories& dataDirectories);
ImportTable readImportTable(StreamReader& reader, const std::map<std::string, HeaderSection>& sectionHeaders, const HeaderDataDirectories& dataDirectories, PEFile::Format format);

PET_END_NAMESPACE


#endif
