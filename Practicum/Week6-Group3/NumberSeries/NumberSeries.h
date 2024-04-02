#pragma once
constexpr size_t initially_saved = 20;
class NumberSeries
{
  private:
    int startingNum = 0; // let's say that 0 is ininitialised num, can be fixed with enum
    int (*generatorFunc)(int num) = nullptr;
    mutable int *generatedSequence = nullptr;
    mutable size_t capacity = initially_saved;

    void free();
    void copyFrom(const NumberSeries &other);
    void expand(size_t to) const;
    void generateNumbersToIndex(unsigned i) const;

  public:
    NumberSeries() = default;
    NumberSeries(int startingNum, int (*generatorFunc)(int));
    NumberSeries(const NumberSeries &other);
    NumberSeries &operator=(const NumberSeries &other);

    int getNumberByIndex(unsigned index) const;
    bool contains(int n) const;
    void setStartingNum(int n);
    void setGeneratorFunc(int (*newGen)(int));
    void print() const;

    ~NumberSeries();
};
