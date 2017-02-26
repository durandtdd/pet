#include "tests.hpp"

#include "../src/exe/tableprinter.hpp"


void testTablePrinter1()
{
    TablePrinter tp({5, 5, 3});
    std::string s = tp.str();

    std::string str = ""
            "-----------------\n"
            "-----------------\n";

    ASSERT(s == str);
}


void testTablePrinter2()
{
    TablePrinter tp({5, 5, 3}, {"Name1", "Name2", "Name3"});
    std::string s = tp.str();

    std::string str = ""
            "-----------------\n"
            "|Name1|Name2|Name3|\n"
            "|---------------|\n"
            "-----------------\n";

    ASSERT(s == str);
}


void testTablePrinter3()
{
    TablePrinter tp({5, 5, 3},
        {"A", "B", "C"},
        {std::ios::dec | std::ios::left,
             std::ios::hex | std::ios::right,
             std::ios::oct | std::ios::right},
        "Title");

    tp.addRow(25, 25, 25);
    tp.addRow(255, 255, 255, 255);
    tp.addRow(0, 0);
    std::string s = tp.str();

    std::string str = ""
            "-----------------\n"
            "|     Title     |\n"
            "|---------------|\n"
            "|  A  |  B  | C |\n"
            "|---------------|\n"
            "|25   |   19| 31|\n"
            "|255  |   ff|377|\n"
            "|0    |    0|   |\n"
            "-----------------\n";

    ASSERT(s == str);
}


void testTablePrinter()
{
    RUN_TEST(testTablePrinter1);
    RUN_TEST(testTablePrinter2);
    RUN_TEST(testTablePrinter3);
}
