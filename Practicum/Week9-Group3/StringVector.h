#pragma once
#include "MyString.h"
class StringVector
{
  private:
    MyString *collection;
    size_t stringCount;
    size_t capacity;

    void resize(unsigned newAllocatedDataSize);

    void copyFrom(const StringVector &other);
    void free();
    void moveFrom(StringVector &&other);

  public:
    StringVector();
    StringVector(const StringVector &other);
    StringVector(StringVector &&other) noexcept;
    StringVector &operator=(const StringVector &other);
    StringVector &operator=(StringVector &&other) noexcept;

    ~StringVector() noexcept;

    void push_back(const MyString &toPush);
    void push_back(MyString &&toPush);
    void pop_back() noexcept;
    size_t insert(size_t at, const MyString &toInsert);
    size_t insert(size_t at, MyString &&toInsert);
    size_t erase(size_t at);
    void clear() noexcept;
    bool empty() const noexcept;
    size_t size() const noexcept;

    const MyString &operator[](size_t index) const;
    MyString &operator[](size_t index);
};
