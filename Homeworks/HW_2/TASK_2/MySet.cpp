#include "MySet.h"

unsigned MySet::getBucketIndex(uint16_t n) const
{
    return n / BUCKET_SIZE;
}

unsigned MySet::getBitIndex(uint16_t n) const
{
    return n % BUCKET_SIZE;
}

void MySet::add(int16_t n)
{

    unsigned bucketIndex = getBucketIndex(n);
    unsigned bitIndex = getBitIndex(n);

    uint8_t mask = 1 << bitIndex;

    buckets[bucketIndex] |= mask;
}

void MySet::remove(int16_t n)
{
    unsigned bucketIndex = getBucketIndex(n);
    unsigned bitIndex = getBitIndex(n);

    uint8_t mask = 1 << bitIndex;
    mask = ~mask;

    buckets[bucketIndex] &= mask;
}

bool MySet::contains(int16_t n) const
{
    unsigned bucketIndex = getBucketIndex(n);
    unsigned bitIndex = getBitIndex(n);

    uint8_t mask = 1 << bitIndex;

    return (buckets[bucketIndex] & mask) != 0;
}
