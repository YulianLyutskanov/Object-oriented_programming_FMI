#include "StringVector.h"

static unsigned roundToPowerOfTwo(unsigned v)
{
    v--;
    v |= v >> 1;
    v |= v >> 2;
    v |= v >> 4;
    v |= v >> 8;
    v |= v >> 16;
    v++;
    return v;
}

static unsigned dataToAllocByStringCount(unsigned stringLength)
{
    return std::max(roundToPowerOfTwo(stringLength + 1), 16u);
}

StringVector::StringVector()
{
    collection = nullptr;
    stringCount = 0;
    capacity = 0;
}

StringVector::StringVector(const StringVector &other)
{
    copyFrom(other);
}
StringVector::StringVector(StringVector &&other) noexcept
{
    moveFrom(std::move(other));
}
void StringVector::copyFrom(const StringVector &other)
{
    capacity = other.capacity;
    collection = new MyString[capacity];
    for (size_t i = 0; i < other.stringCount; i++)
        collection[i] = other.collection[i];

    stringCount = other.stringCount;
}

void StringVector::free()
{
    delete[] collection;
    collection = nullptr;
    stringCount = 0;
    capacity = 0;
}

void StringVector::moveFrom(StringVector &&other)
{
    collection = other.collection;
    other.collection = nullptr;

    stringCount = other.stringCount;
    other.stringCount = 0;

    capacity = other.capacity;
    other.capacity = 0;
}

StringVector &StringVector::operator=(const StringVector &other)
{
    if (this != &other)
    {
        free();
        copyFrom(other);
    }

    return *this;
}

StringVector &StringVector::operator=(StringVector &&other) noexcept
{
    if (this != &other)
    {
        free();
        moveFrom(std::move(other));
    }

    return *this;
}

StringVector::~StringVector()
{
    free();
}

void StringVector::resize(unsigned newAllocatedDataSize)
{
    capacity = newAllocatedDataSize;

    MyString *newCollection = new MyString[capacity];

    for (size_t i = 0; i < stringCount; i++)
        newCollection[i] = collection[i];

    delete[] collection;
    collection = newCollection;
}

void StringVector::push_back(const MyString &toPush)
{
    if (stringCount == capacity)
        resize(dataToAllocByStringCount(stringCount + 1));

    collection[stringCount++] = toPush;
}

void StringVector::push_back(MyString &&toPush)
{
    if (stringCount == capacity)
        resize(dataToAllocByStringCount(stringCount + 1));

    collection[stringCount++] = std::move(toPush);
}

void StringVector::pop_back() noexcept
{
    if (stringCount != 0)
        stringCount--;
}

size_t StringVector::insert(size_t at, const MyString &toInsert)
{
    if (at > stringCount)
        throw std::out_of_range("too large index");

    if (capacity <= stringCount)
    {
        resize(roundToPowerOfTwo(at + 1));
    }

    for (int i = stringCount - 1; i >= at; i--) // shift elements ahead
    {
        collection[i + 1] = std::move(collection[i]);
    }
    collection[at] = toInsert;
    return at;
}

size_t StringVector::insert(size_t at, MyString &&toInsert)
{
    if (at > stringCount)
        throw std::out_of_range("too large index");

    if (capacity <= stringCount)
    {
        resize(roundToPowerOfTwo(at + 1));
    }

    for (int i = stringCount - 1; i >= at; i--) // shift elements ahead
    {
        collection[i + 1] = std::move(collection[i]);
    }
    collection[at] = std::move(toInsert);
    return at;
}

size_t StringVector::erase(size_t at)
{
    if (at >= stringCount)
        throw std::out_of_range("too large index");

    for (size_t i = at; i < stringCount - 1; i++) // shift back
    {
        collection[i] = collection[i + 1];
    }
    stringCount--;
    return at;
}

void StringVector::clear() noexcept
{
    stringCount = 0;
}

bool StringVector::empty() const noexcept
{
    return stringCount == 0;
}

size_t StringVector::size() const noexcept
{
    return stringCount;
}

const MyString &StringVector::operator[](size_t index) const
{
    return collection[index];
}

MyString &StringVector::operator[](size_t index)
{
    return collection[index];
}
