#include "tests.hpp"

#include <sstream>

#include "../src/core/dumper.hpp"


PET_USE_NAMESPACE


void testDumper1()
{
    std::string bin;
    for(int k=0; k<48; k++)
        bin += (char)(48+k);

    std::istringstream iss(bin);

    Dumper dumper(iss);
    dumper.dump(20);
    dumper.dump(4);
    dumper.dump(8);

    std::string str = ""
            "00000000   30 31 32 33 34 35 36 37 38 39 3a 3b 3c 3d 3e 3f   0123456789:;<=>?\n"
            "00000010   40 41 42 43                                       @ABC            \n"
            "00000010               44 45 46 47                               DEFG        \n"
            "00000010                           48 49 4a 4b 4c 4d 4e 4f           HIJKLMNO\n";

    ASSERT(dumper.str() == str);
}


void testDumper2()
{
    std::string bin;
    for(int k=0; k<48; k++)
        bin += (char)(48+k);

    std::istringstream iss(bin);

    Dumper dumper(iss);
    dumper.setBytesPerLine(8);
    dumper.dump(20);
    dumper.addTextLine();
    dumper.addTextLine("Section");
    dumper.dump(4);
    dumper.dump(2);
    dumper.addTextLine("Section");
    dumper.dump(6);

    std::string str = ""
            "00000000   30 31 32 33 34 35 36 37   01234567\n"
            "00000008   38 39 3a 3b 3c 3d 3e 3f   89:;<=>?\n"
            "00000010   40 41 42 43               @ABC    \n"
            "\n"
            "Section\n"
            "00000010               44 45 46 47       DEFG\n"
            "00000018   48 49                     HI      \n"
            "Section\n"
            "00000018         4a 4b 4c 4d 4e 4f     JKLMNO\n";

    auto s = dumper.str();
    ASSERT(dumper.str() == str);
}


void testDumper()
{
    RUN_TEST(testDumper1);
    RUN_TEST(testDumper2);
}
