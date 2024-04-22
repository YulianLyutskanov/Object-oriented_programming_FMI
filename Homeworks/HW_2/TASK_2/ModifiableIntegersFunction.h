#pragma once
#include "MySet.h"
#include <iostream>

// Function is implemented as an array of functional values
// The array is indexed afrom 0 ---> 2^16, where the values are from -2^15 ...0... 2^15-1
// The array is on the heap, which is counterintuitive, because we know the exact length of it before the start of the
// program, but this design is chosen because the array itself is too big and can clogg up the stack, causing stack
// overflow in some functions. The call to the dynamic memory is minimised to optimise speed.
// Used custom made MySet class, that can contain numbers from -2^15 to 2^15-1, which itself is on the stack, because
// it's only 8192 bytes, also it's memory efficient, similarly to 1st task from the HW

class ModifiableIntegersFunction
{
  private:
    int16_t (*func)(int16_t) = nullptr;
    int16_t *values = nullptr;
    MySet isDefined;

    mutable bool isSurjective = false;
    mutable bool isCheckedForSurjection = false;
    mutable bool isInjective = false;
    mutable bool isCheckedForInjection = false;

    void free();
    void copyFrom(const ModifiableIntegersFunction &other);

    bool isDefinedIn(int16_t input) const;

  public:
    ModifiableIntegersFunction();
    ModifiableIntegersFunction(int16_t (*func)(int16_t));
    ModifiableIntegersFunction(const ModifiableIntegersFunction &other);
    ModifiableIntegersFunction &operator=(const ModifiableIntegersFunction &other);
    ~ModifiableIntegersFunction();

    void setPoint(int16_t input, int16_t to);
    void removePoint(int16_t input);

    int16_t operator()(int16_t param) const;

    bool isInjection() const;
    bool isSurjection() const;
    bool isBijection() const;

    void serialize(const char *dest) const;
    void deserialize(const char *source);

    void drawGraphicInInterval(int16_t x1, int16_t y1) const;

    friend ModifiableIntegersFunction operator+(const ModifiableIntegersFunction &lhs,
                                                const ModifiableIntegersFunction &rhs);

    friend ModifiableIntegersFunction operator-(const ModifiableIntegersFunction &lhs,
                                                const ModifiableIntegersFunction &rhs);

    friend bool operator<(const ModifiableIntegersFunction &lhs, const ModifiableIntegersFunction &rhs);
    friend bool operator>(const ModifiableIntegersFunction &lhs, const ModifiableIntegersFunction &rhs);
    friend bool operator==(const ModifiableIntegersFunction &lhs, const ModifiableIntegersFunction &rhs);
    friend bool operator!=(const ModifiableIntegersFunction &lhs, const ModifiableIntegersFunction &rhs);
    friend bool operator<=(const ModifiableIntegersFunction &lhs, const ModifiableIntegersFunction &rhs);
    friend bool operator>=(const ModifiableIntegersFunction &lhs, const ModifiableIntegersFunction &rhs);
    friend bool operator||(const ModifiableIntegersFunction &lhs, const ModifiableIntegersFunction &rhs);

    friend ModifiableIntegersFunction operator*(
        const ModifiableIntegersFunction &outer,
        const ModifiableIntegersFunction &inner); // like int maths gºf = g(f(x)) for composition
    friend ModifiableIntegersFunction operator^(const ModifiableIntegersFunction &f, int iterations);
};
