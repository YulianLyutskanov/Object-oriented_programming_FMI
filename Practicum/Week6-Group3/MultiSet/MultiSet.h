#pragma once
#include <iostream>
class MultiSet
{
    uint8_t *_buckets = nullptr;
    unsigned _elementsInBucket = 4;
    unsigned _maxNumber = 0;
    unsigned _bucketsCount = 0;

    void copyFrom(const MultiSet &other);
    void free();
    size_t getBucketIndex(unsigned n) const;
    size_t getPositionInBucket(unsigned n) const;
    void setOccurrences(unsigned n, uint8_t to);

  public:
    MultiSet() = default;
    explicit MultiSet(int n);
    MultiSet(const MultiSet &other);

    void add(unsigned n);
    void remove(unsigned n);
    uint8_t occurrences(unsigned n) const;
    void print() const;

    friend MultiSet UnionOfMultiSets(const MultiSet &lhs, const MultiSet &rhs);
    friend MultiSet IntersectionOfMultiSets(const MultiSet &lhs, const MultiSet &rhs);

    MultiSet &operator=(const MultiSet &other);
    ~MultiSet();
};
