#include <iostream>
#include <kodo/version.hpp>

int main(int argc, char const* argv[])
{
    std::cout << kodo::version() << std::endl;
    return 0;
}