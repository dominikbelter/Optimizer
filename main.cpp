#include "include/Defs/defs.h"
#include "3rdParty/tinyXML/tinyxml2.h"
#include <iostream>

int main()
{
    try {
        using namespace std;
        std::cout << "Nothing here\n";
    }
    catch (const std::exception& ex) {
        std::cerr << ex.what() << std::endl;
        return 1;
    }

    return 0;
}
