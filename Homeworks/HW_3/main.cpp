#include "FunctionsFactory.h"
#include <iostream>

static constexpr size_t bufferSize = 1024;

int main()
{
    // char fileName[bufferSize]{0};
    // std::cin >> fileName;

    std::ifstream ifs("func.dat", std::ios::binary);

    if (!ifs.is_open())
    {
        std::cout << "Cannot open game file";
        return -1;
    }

    PartFunction *func = createFunc(ifs);

    ifs.close();

    while (true)
    {
        int n = 0;

        std::cin >> n;
        if (n == -1)
            break;
        std::cout << "f(" << n << ") = " << (*func)(n) << std::endl;
    }

    delete func;
}
