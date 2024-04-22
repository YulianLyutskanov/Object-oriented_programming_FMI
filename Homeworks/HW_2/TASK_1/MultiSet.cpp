#include "MultiSet.h"
#include <fstream>

#define MAX(a, b) a > b ? a : b
#define MIN(a, b) a < b ? a : b

namespace BitwiseFunctions
{
bool isBitAtPositionSet(unsigned number, uint8_t bitPosition)
{
    return number & (1 << bitPosition);
}

void setBitAtPosition(uint8_t &number, uint8_t bitPosition)
{
    number |= (1 << bitPosition);
}

void clearBitAtPosition(uint8_t &number, uint8_t bitPosition)
{
    number &= ~(1 << bitPosition);
}

void changeBitAtPosition(uint8_t &number, uint8_t bitPosition, bool to)
{
    if (to)
        setBitAtPosition(number, bitPosition);
    else
        clearBitAtPosition(number, bitPosition);
}

void printInBinary(unsigned num)
{
    for (int8_t i = 31; i >= 0; i--)
        std::cout << isBitAtPositionSet(num, i);
}

void printInBinary(uint8_t num)
{
    for (int8_t i = 7; i >= 0; i--)
        std::cout << isBitAtPositionSet(num, i);
}

unsigned powOfTwo(unsigned n)
{
    return 1 << n;
}
} // namespace BitwiseFunctions
using namespace BitwiseFunctions;

uint8_t MultiSet::bucketSizeInBits = 8;

MultiSet::MultiSet(unsigned maxNumber, uint8_t bitsInElement)
{
    if (bitsInElement > 8 || bitsInElement == 0) // 1 <=k <= 8
        return;

    this->bitsInElement = bitsInElement;
    this->maxNumber = maxNumber;

    bucketsCount = ((maxNumber + 1) * bitsInElement) / bucketSizeInBits +        //+1 because the 0 is included
                   (bool)(((maxNumber + 1) * bitsInElement) % bucketSizeInBits); // to round it up if needed
    buckets = new uint8_t[bucketsCount]{0};
}

void MultiSet::free()
{
    delete[] buckets;
    buckets = nullptr;
    bucketsCount = maxNumber = 0;
    bitsInElement = 0;
}

void MultiSet::copyFrom(const MultiSet &other)
{
    buckets = new uint8_t[other.bucketsCount];
    bucketsCount = other.bucketsCount;
    bitsInElement = other.bitsInElement;
    maxNumber = other.maxNumber;

    for (size_t i = 0; i < bucketsCount; i++)
        buckets[i] = other.buckets[i]; // copy the data
}

MultiSet::MultiSet(const MultiSet &other)
{
    copyFrom(other);
}

MultiSet &MultiSet::operator=(const MultiSet &other)
{
    if (this != &other)
    {
        free();
        copyFrom(other);
    }
    return *this;
}

MultiSet::~MultiSet()
{
    free();
}

unsigned MultiSet::getBucketIndex(unsigned num) const
{
    return (num * bitsInElement) / bucketSizeInBits;
}

uint8_t MultiSet::getStartingPositionInBucket(unsigned num) const
{
    return (num * bitsInElement) % bucketSizeInBits;
}

unsigned MultiSet::occurrences(unsigned num) const
{
    if (num > maxNumber)
        return 0;

    unsigned bucketIndex = getBucketIndex(num);
    uint8_t startingPosInBucket = getStartingPositionInBucket(num);
    uint8_t curPosInBucket = startingPosInBucket;

    unsigned res = 0; // mult could be also unsigned if we want the class to work with k>8
    for (uint8_t i = 0, mult = 1; i < bitsInElement; i++, mult *= 2) // calculating from binary
    {
        if (i != 0 && (startingPosInBucket + i) % bucketSizeInBits == 0)
            // if we go to the next bucket when reading the number from memory,excluding the first step
            bucketIndex++;

        curPosInBucket = (startingPosInBucket + i) % bucketSizeInBits;
        res += isBitAtPositionSet(buckets[bucketIndex], curPosInBucket) * mult;
    }

    return res;
}

void MultiSet::setOccurrences(unsigned num, uint8_t to)
{
    if (num > maxNumber || to > (powOfTwo(bitsInElement) - 1))
        // second condition is impossible, the method is private and well capsulated, but added for completeness,
        // also if we decide to make it public
        return;

    unsigned bucketIndex = getBucketIndex(num);
    uint8_t startingPosInBucket = getStartingPositionInBucket(num);
    uint8_t curPosInBucket = startingPosInBucket;

    for (uint8_t i = 0; i < bitsInElement; i++, to /= 2) // calc to binary
                                                         // could change i to uint16 or uint32 if k>8
    {
        if (i != 0 && (startingPosInBucket + i) % bucketSizeInBits == 0)
            // if we go to the next bucket when reading the number from memory, i != 0 becauce we can't move to next on
            // the 1st try
            bucketIndex++;

        curPosInBucket = (startingPosInBucket + i) % bucketSizeInBits;
        changeBitAtPosition(buckets[bucketIndex], curPosInBucket, to % 2);
    }
}

void MultiSet::add(unsigned num)
{
    if (num > maxNumber)
        return;

    uint8_t occs = occurrences(num);

    if (occs >= powOfTwo(bitsInElement) - 1)
        return; // if max occs reached just do nothing, also possible implementation would be to overflow
    else
        setOccurrences(num, occs + 1);
}

void MultiSet::remove(unsigned num)
{
    if (num > maxNumber)
        return;

    uint8_t occs = occurrences(num);
    if (occs == 0)
        return; // if 0 occs reached just do nothing (not -1)
    else
        setOccurrences(num, occs - 1);
}

void MultiSet::print() const
{
    std::cout << '{' << ' ';

    for (unsigned i = 0; i <= maxNumber; i++)
    {
        uint8_t curNumOccurences = occurrences(i);

        for (uint8_t j = 0; j < curNumOccurences; j++)
        {
            std::cout << i << ' ';
        }
    }

    std::cout << '}';
}

void MultiSet::printFromMemory() const
{

    std::cout << "Max number from set : ";
    printInBinary(maxNumber);

    std::cout << std::endl << "Number of \"buckets\": ";
    printInBinary(bucketsCount);

    std::cout << std::endl << "Numbers in memory: ";

    unsigned bitsCount = bucketsCount * bucketSizeInBits;
    for (unsigned i = 0; i < bucketsCount; i++)
    {
        printInBinary(buckets[i]);
        std::cout << ' ';
    }
}

MultiSet IntersectionOfMultiSets(const MultiSet &lhs, const MultiSet &rhs)
{
    MultiSet res(MIN(lhs.maxNumber, rhs.maxNumber), MIN(lhs.bitsInElement, rhs.bitsInElement));

    for (unsigned i = 0; i < res.maxNumber; i++)
        res.setOccurrences(i, MIN(lhs.occurrences(i), rhs.occurrences(i)));

    return res;
}

MultiSet ComplementOfMultiSet(const MultiSet &ms)
{
    MultiSet res;
    res = ms;

    for (unsigned i = 0; i < res.bucketsCount; i++)
    {
        // res.setOccurrences(i, ~res.occurrences(i)); slower with i< res.maxNumber
        res.buckets[i] = ~res.buckets[i]; // only difference is that we revert the last meaningless bits, but it doesn't
                                          // matter since the user hasn't access to them
    }
    return res;
}

MultiSet DifferenceOfMultiSets(const MultiSet &lhs, const MultiSet &rhs)
{
    MultiSet res(lhs.maxNumber, lhs.bitsInElement);

    for (unsigned i = 0; i <= lhs.maxNumber; i++)
    {
        unsigned resOccurrs = lhs.occurrences(i) > rhs.occurrences(i) ? lhs.occurrences(i) - rhs.occurrences(i) : 0;
        res.setOccurrences(i, resOccurrs);
    }

    return res;
}

MultiSet UnionOfMultiSets(const MultiSet &lhs, const MultiSet &rhs)
{
    MultiSet res(MAX(lhs.maxNumber, rhs.maxNumber), MAX(lhs.bitsInElement, rhs.bitsInElement));

    for (unsigned i = 0; i < res.maxNumber; i++)
    {

        unsigned resOccurrs = lhs.occurrences(i) + rhs.occurrences(i); // could potentially overflow if k>8
        if (resOccurrs >= powOfTwo(res.bitsInElement))
            resOccurrs = powOfTwo(res.bitsInElement) - 1;
        res.setOccurrences(i, resOccurrs);
    }

    return res;
}

void MultiSet::serialize(const char *dest) const
{
    if (!dest || bucketsCount == 0)
        return;

    std::ofstream ofs(dest, std::ios::binary);

    if (!ofs.is_open())
        return;

    ofs.write((const char *)&bucketsCount, sizeof(bucketsCount));
    ofs.write((const char *)&maxNumber, sizeof(maxNumber));
    ofs.write((const char *)&bitsInElement, sizeof(bitsInElement));

    ofs.write((const char *)buckets, sizeof(buckets[0]) * bucketsCount);

    ofs.close();
}

void MultiSet::deserialize(const char *from)
{
    if (!from)
        return;

    std::ifstream ifs(from, std::ios::binary);

    if (!ifs.is_open())
        return;

    free();

    ifs.read((char *)&bucketsCount, sizeof(bucketsCount));
    ifs.read((char *)&maxNumber, sizeof(maxNumber));
    ifs.read((char *)&bitsInElement, sizeof(bitsInElement));
    buckets = new uint8_t[bucketsCount];

    ifs.read((char *)buckets, sizeof(buckets[0]) * bucketsCount);
    ifs.close();
}
