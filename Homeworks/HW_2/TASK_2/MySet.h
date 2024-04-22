#pragma once
#include <iostream>

constexpr int MIN = INT16_MIN;                    // 1<<15
constexpr int MAX = INT16_MAX;                    // 1<<15 - 1
constexpr unsigned NUMBERS_COUNT = MAX - MIN + 1; // both ints to avoid overflow
constexpr unsigned BUCKETS_COUNT = NUMBERS_COUNT / 8;
constexpr uint16_t BUCKET_SIZE = 8;

// this class takes advantage of the overflow between int16 and uint16
// the methods of the set are from the set class, shown on seminar, but modified to work with negatives
// in the memory the first half from the bits is for the sequence 0 1 2 ... 32767
// the next bit is for -32768, -32767 ... -1

class MySet // saves from -32768 to 32767
{
  private:
    uint8_t buckets[BUCKETS_COUNT]{0};

    unsigned getBucketIndex(uint16_t n) const;
    unsigned getBitIndex(uint16_t n) const;

  public:
    MySet() = default;

    void add(int16_t n);
    void remove(int16_t n);

    bool contains(int16_t n) const;
};
