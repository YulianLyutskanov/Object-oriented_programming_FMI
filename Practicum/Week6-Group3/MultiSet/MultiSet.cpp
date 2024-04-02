#include "MultiSet.h"

MultiSet::MultiSet(unsigned n)
{
    _bucketsCount = n / _elementsInBucket + 1;
    _buckets = new uint8_t[_bucketsCount]{0};
    _maxNumber = n - 1;
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

void MultiSet::free()
{
    delete[] _buckets;
    _buckets = nullptr;
    _maxNumber = _bucketsCount = 0;
}

void MultiSet::copyFrom(const MultiSet &other)
{
    _buckets = new uint8_t[other._bucketsCount];
    for (unsigned i = 0; i < other._bucketsCount; i++)
    {
        _buckets[i] = other._buckets[i];
    }
    _bucketsCount = other._bucketsCount;
    _maxNumber = other._maxNumber;
    _elementsInBucket = other._elementsInBucket;
}

size_t MultiSet::getBucketIndex(unsigned n) const
{
    return n / _elementsInBucket;
}

size_t MultiSet::getPositionInBucket(unsigned n) const
{
    return n % _elementsInBucket;
}

uint8_t MultiSet::occurrences(unsigned n) const
{
    if (n > _maxNumber)
        return 0;

    size_t bucketsIndex = getBucketIndex(n);
    size_t positionInBucket = getPositionInBucket(n);

    uint8_t mask = 3 << (positionInBucket * 2);
    uint8_t res = mask & _buckets[bucketsIndex];

    if (res == mask)
        return 3;
    else if (res == 0)
        return 0;
    else if (res == (1 << (positionInBucket * 2)))
        return 1;
    else
        return 2;
}

void MultiSet::setOccurrences(unsigned n, uint8_t to)
{
    size_t bucketsIndex = getBucketIndex(n);
    size_t positionInBucket = getPositionInBucket(n);

    switch (to)
    {
    case 0:
        _buckets[bucketsIndex] &= ~(1 << (positionInBucket * 2));     // set left bit to 0
        _buckets[bucketsIndex] &= ~(1 << (positionInBucket * 2 + 1)); // set right bit to 0
        break;
    case 1:
        _buckets[bucketsIndex] |= (1 << (positionInBucket * 2));      // set left bit to 1
        _buckets[bucketsIndex] &= ~(1 << (positionInBucket * 2 + 1)); // set right bit to 0
        break;
    case 2:
        _buckets[bucketsIndex] |= (1 << (positionInBucket * 2 + 1)); // set rigth bit to 1
        _buckets[bucketsIndex] &= ~(1 << (positionInBucket * 2));    // set left bit to 0
        break;
    case 3:
    default:                                                         // if greater than 3, set the amount to 3
        _buckets[bucketsIndex] |= (1 << (positionInBucket * 2));     // set left bit to 1
        _buckets[bucketsIndex] |= (1 << (positionInBucket * 2 + 1)); // set left bit to 1
        break;
    }
}

void MultiSet::add(unsigned n)
{
    if (n > _maxNumber)
        return;

    uint8_t occs = occurrences(n);

    if (occs < 3)
        setOccurrences(n, occs + 1); // increase the counter
}

void MultiSet::print() const
{
    std::cout << '{';
    for (size_t i = 0; i <= _maxNumber; i++)
    {
        uint8_t occs = occurrences(i);

        for (size_t j = 0; j < occs; j++)
            std::cout << i << ' ';
    }
    std::cout << '}' << '\n';
}

void MultiSet::remove(unsigned n)
{
    size_t occs = occurrences(n);

    if (n > _maxNumber || occs == 0)
        return;

    if (occs == 1)
        setOccurrences(n, 0);
    else if (occs == 2)
        setOccurrences(n, 1);
    else if (occs == 3)
        setOccurrences(n, 2);
    else
        return; // impossible
}

MultiSet UnionOfMultiSets(const MultiSet &lhs, const MultiSet &rhs)
{
    MultiSet result(std::max(rhs._maxNumber, lhs._maxNumber) + 1); //+1 for the 0

    unsigned minMaxNumberInSet = std::min(lhs._maxNumber, rhs._maxNumber);
    unsigned maxMaxNumberInSet = std::max(lhs._maxNumber, rhs._maxNumber);
    for (unsigned i = 0; i <= minMaxNumberInSet; i++)
        result.setOccurrences(i, lhs.occurrences(i) + rhs.occurrences(i));

    const MultiSet &biggerN = lhs._bucketsCount > rhs._bucketsCount ? lhs : rhs; // no copy here

    for (unsigned i = minMaxNumberInSet + 1; i <= maxMaxNumberInSet; i++)
        result.setOccurrences(i, biggerN.occurrences(i));

    return result;
}

MultiSet IntersectionOfMultiSets(const MultiSet &lhs, const MultiSet &rhs)
{
    unsigned minMaxNumber = std::min(lhs._maxNumber, rhs._maxNumber);
    MultiSet result(minMaxNumber + 1); //+1 for the 0

    for (unsigned i = 0; i <= minMaxNumber; i++)
        result.setOccurrences(i, std::min(lhs.occurrences(i), rhs.occurrences(i)));

    return result;
}

// 00 11 00 00
// 0  3  0  1
