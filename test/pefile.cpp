#include "tests.hpp"

#include "../include/pet.hpp"

using namespace pet;


bool stringInList(const std::vector<std::string>& list, const std::string& str)
{
    for(const std::string& s: list)
        if(s.find(str)!=std::string::npos)
            return true;
    return false;
}

bool dllInList(const std::vector<ImportTable::ImportedDLL>& list, const std::string& str)
{
    for(const ImportTable::ImportedDLL& dll: list)
        if(dll.dllName == str)
            return true;
    return false;
}


void testPEFile1()
{
    PEFile file("test1.dll");
    ExportTable et = file.exportTable();

    ASSERT(stringInList(et.names, "uselessFunction"));
    ASSERT(stringInList(et.names, "add"));
    ASSERT(stringInList(et.names, "Class"));
    ASSERT(stringInList(et.names, "privateMethod2"));
    ASSERT(stringInList(et.names, "publicMethod1"));
    ASSERT(stringInList(et.names, "publicMethod2"));
    ASSERT(stringInList(et.names, "protectedMethod1"));
    ASSERT(stringInList(et.names, "protectedMethod2"));
    ASSERT(stringInList(et.names, "privateMethod1"));
    ASSERT(stringInList(et.names, "privateMethod2"));
}


void testPEFile2()
{
    PEFile file("test2.dll");
    ExportTable et = file.exportTable();

    ASSERT(stringInList(et.names, "uselessFunction"));
    ASSERT(stringInList(et.names, "add"));
    ASSERT(stringInList(et.names, "add_double"));
    ASSERT(stringInList(et.names, "Struct"));
}


void testPEFile3()
{
    PEFile file("test3.exe");

    ExportTable et = file.exportTable();
    ASSERT(et.names.size() == 0);

    ImportTable it = file.importTable();
    ASSERT(dllInList(it.importedDlls, "test2.dll"));
    for(const ImportTable::ImportedDLL& dll: it.importedDlls)
        if(dll.dllName == "test2.dll")
            ASSERT(stringInList(dll.names, "add"));
}


void testPEFile4()
{
    ASSERT_ERROR(PEFile file("non_existing_file.dll"));
}


void testPEFile()
{
    RUN_TEST(testPEFile1);
    RUN_TEST(testPEFile2);
    RUN_TEST(testPEFile3);
    RUN_TEST(testPEFile4);
}
