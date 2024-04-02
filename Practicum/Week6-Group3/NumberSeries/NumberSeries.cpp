#include "NumberSeries.h"
#include <iostream>

NumberSeries::NumberSeries(int startingNum, int (*generatorFunc)(int))
{
    this->startingNum = startingNum;
    this->generatorFunc = generatorFunc;
    capacity = initially_saved;
    generatedSequence = new int[initially_saved]{0};
    generatedSequence[0] = startingNum;
}

void NumberSeries::free()
{
    delete[] generatedSequence;
    generatedSequence = nullptr;
    capacity = initially_saved;
}

void NumberSeries::copyFrom(const NumberSeries &other)
{
    capacity = other.capacity;
    generatedSequence = new int[capacity]{0};

    for (size_t i = 0; i < capacity; i++)
        generatedSequence[i] = other.generatedSequence[i];

    generatorFunc = other.generatorFunc;
    startingNum = other.startingNum;
}

NumberSeries::NumberSeries(const NumberSeries &other)
{
    copyFrom(other);
}

NumberSeries &NumberSeries::operator=(const NumberSeries &other)
{
    if (this != &other)
    {
        free();
        copyFrom(other);
    }

    return *this;
}

NumberSeries::~NumberSeries()
{
    free();
}

void NumberSeries::setStartingNum(int n)
{
    free();
    startingNum = n;
    capacity = initially_saved;
    generatedSequence = new int[capacity]{0};
    generatedSequence[0] = startingNum;
}

void NumberSeries::setGeneratorFunc(int (*newGen)(int))
{
    free();
    generatorFunc = newGen;
}

bool NumberSeries::contains(int n) const
{
    for (size_t i = 0; i < capacity; i++)
        if (generatedSequence[i] == n)
            return true;

    return false;
}

void NumberSeries::expand(size_t to) const
{
    if (to < capacity)
        return;

    int *res = new int[to]{0};

    for (size_t i = 0; i < capacity; i++)
        res[i] = generatedSequence[i];

    delete[] generatedSequence;
    generatedSequence = res;
    capacity = to;
}

void NumberSeries::generateNumbersToIndex(unsigned i) const
{
    if (i >= capacity)
    {
        int capacity_copy = capacity;
        while (capacity_copy <= i) // resize with the needed scale
            capacity_copy *= 2;

        expand(capacity_copy);
    }
    else if (generatedSequence[i] != 0) // if the num is already calculated
    {
        return;
    }

    for (size_t j = 1; j < i; j++) // calculate all previuos nums
    {
        if (generatedSequence[j] == 0)
            generatedSequence[j] = generatorFunc(generatedSequence[j - 1]);
    }
}

int NumberSeries::getNumberByIndex(unsigned index) const
{
    generateNumbersToIndex(index);
    return generatedSequence[index - 1];
}

void NumberSeries::print() const
{
    for (size_t i = 0; generatedSequence[i] != 0; i++)
    {
        std::cout << generatedSequence[i] << ' ';
    }
}
