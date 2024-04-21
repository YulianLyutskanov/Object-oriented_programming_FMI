#include "StringVector.h"
#include <iostream>
#include <string>
#define nl std::cout << std::endl
int main()
{

    StringVector vec;
    vec.push_back("ivan");
    vec.push_back("gerogi");
    vec.push_back("laino");

    for (size_t i = 0; i < vec.size(); i++)
    {
        std::cout << vec[i] << ' ';
    }
    vec.pop_back();

    nl;
    for (size_t i = 0; i < vec.size(); i++)
    {
        std::cout << vec[i] << ' ';
    }
    nl;
    std::cout << vec.erase(0);

    for (size_t i = 0; i < vec.size(); i++)
    {
        std::cout << vec[i] << ' ';
    }
    vec.clear();
    vec.push_back("pederaaaas");
    vec.push_back("geeei");
    vec.pop_back();

    for (size_t i = 0; i < vec.size(); i++)
    {
        std::cout << vec[i] << ' ';
    }
    vec.erase(0);

    std::cout << vec.size();
}
