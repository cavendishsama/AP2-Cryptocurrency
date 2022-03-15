
#include <iostream>
#include <gtest/gtest.h>
#include "client.h"
#include "server.h"


int main(int argc, char **argv)
{
    if (false) // make false to run unit-tests
    {
        // debug section
        
        std::string str ("This is an example sentence.");
        std::cout << str << '\n';
        std::string str1 {str};
        str1.erase (str1.begin()+10,str1.end());                        
        std::cout << str1 << '\n';
    }
    else
    {
        ::testing::InitGoogleTest(&argc, argv);
        std::cout << "RUNNING TESTS ..." << std::endl;
        int ret{RUN_ALL_TESTS()};
        if (!ret)
            std::cout << "<<<SUCCESS>>>" << std::endl;
        else
            std::cout << "FAILED" << std::endl;
    }
    return 0;   
}