#include "tests.hpp"

int main()
{
    std::cout << "Starting tests..." << std::endl;
    testTablePrinter();
    testDumper();
    testPEFile();

    return 0;
}
