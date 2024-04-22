#pragma once
#include <iostream>

class MultiSet
{
  private:
    uint8_t *buckets = nullptr;
    static uint8_t bucketSizeInBits;
    unsigned bucketsCount = 0;
    uint8_t bitsInElement = 0; // k
    unsigned maxNumber = 0;    // n

    void free();
    void copyFrom(const MultiSet &other);

    unsigned getBucketIndex(unsigned num) const;
    uint8_t getStartingPositionInBucket(unsigned num) const;
    void setOccurrences(unsigned num, uint8_t to);

  public:
    MultiSet() = default;                                         // used to create an empty set to read from file
    explicit MultiSet(unsigned maxNumber, uint8_t bitsInElement); // n and k
    MultiSet(const MultiSet &other);
    MultiSet &operator=(const MultiSet &other);
    ~MultiSet();

    void add(unsigned num);
    void remove(unsigned num);
    unsigned occurrences(unsigned num) const; // could return uint8_t
    void print() const;

    void serialize(const char *dest) const;
    void deserialize(const char *from);
    void printFromMemory() const;

    friend MultiSet IntersectionOfMultiSets(const MultiSet &lhs, const MultiSet &rhs);
    friend MultiSet ComplementOfMultiSet(const MultiSet &ms);
    friend MultiSet DifferenceOfMultiSets(const MultiSet &lhs, const MultiSet &rhs);
    friend MultiSet UnionOfMultiSets(const MultiSet &lhs, const MultiSet &rhs);
};
