#ifndef TESTS_HPP
#define TESTS_HPP

#include <exception>
#include <iomanip>
#include <iostream>
#include <string>

#include "../include/pet.hpp"


/**
 * @brief Run a test and display the result
 */
#define RUN_TEST(name)\
    try\
    {\
        std::cout << "Running " << std::setw(30) << std::left << #name;\
        name();\
        std::cout << "OK" << std::endl;\
    }\
    catch(const TestFailed& err)\
    {\
        std::cout << "FAIL" << std::endl << "\t" << err.what() << std::endl;\
    }\
    catch(const pet::Error& err)\
    {\
        std::cout << "FAIL" << std::endl << "\t" << err.what() << std::endl;\
    }


/**
 * @brief Assert that statement is true
 */
#define ASSERT(stmt)\
    if(!(stmt))\
        throw TestFailed("[" + std::to_string(__LINE__) + std::string("] Assertion failed: ") + #stmt);


/**
 * @brief Assert that statement throws an error
 */
#define ASSERT_ERROR(stmt)\
    try\
    {\
        stmt;\
        throw TestFailed("[" + std::to_string(__LINE__) + std::string("] No error caught: ") + #stmt);\
    }\
    catch(const pet::Error&) {}



/**
 * @brief Exception thrown when a test fails
 */
class TestFailed: public std::exception
{
    public:
        TestFailed(const std::string& msg): m_msg(msg) {}
        const char *what() const override {return m_msg.c_str();}

    private:
        std::string m_msg;
};


/**
 * @brief Test TablePrinter
 */
void testTablePrinter();

/**
 * @brief Test PEFile
 */
void testPEFile();

/**
 * @brief test Dumper
 */
void testDumper();

#endif
