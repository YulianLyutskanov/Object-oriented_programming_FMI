#include "ModifiableIntegersFunction.h"
#include "MySet.h"
#include <fstream>

constexpr int INTERVAL_FOR_PRINTING = 20;
constexpr char EMPTY_SYMBOL = ' ';

namespace HelpingFunctions
{
int16_t defaultFunction(int16_t n)
{
    return 0;
}

int16_t identity(int16_t n)
{
    return n;
}

void fillWithSymbol(char matrix[][INTERVAL_FOR_PRINTING], char ch)
{
    for (size_t i = 0; i < INTERVAL_FOR_PRINTING; i++)
    {
        for (size_t j = 0; j < INTERVAL_FOR_PRINTING; j++)
        {
            matrix[i][j] = ' ';
        }
    }
}

void printLine(size_t length, char ch)
{
    for (size_t i = 0; i < length; i++)
    {
        std::cout << ch;
    }
}
} // namespace HelpingFunctions
using namespace HelpingFunctions;

void ModifiableIntegersFunction::free()
{
    delete[] values;
    values = nullptr;
    func = nullptr;
    isDefined = MySet(); // set all to undefined
    isSurjective = false;
    isCheckedForSurjection = false;
    isInjective = false;
    isCheckedForInjection = false;
}

void ModifiableIntegersFunction::copyFrom(const ModifiableIntegersFunction &other)
{
    values = new int16_t[NUMBERS_COUNT];

    for (size_t i = 0; i < NUMBERS_COUNT; i++)
        values[i] = other.values[i];

    func = other.func; // not actually needed
    isInjective = other.isInjective;
    isSurjective = other.isSurjective;
    isCheckedForInjection = other.isCheckedForInjection;
    isCheckedForSurjection = other.isCheckedForSurjection;
    isDefined = other.isDefined;
}

ModifiableIntegersFunction::ModifiableIntegersFunction(const ModifiableIntegersFunction &other)
{
    copyFrom(other);
}

ModifiableIntegersFunction &ModifiableIntegersFunction::operator=(const ModifiableIntegersFunction &other)
{
    if (this != &other)
    {
        free();
        copyFrom(other);
    }
    return *this;
}

ModifiableIntegersFunction::~ModifiableIntegersFunction()
{
    free();
}

ModifiableIntegersFunction::ModifiableIntegersFunction() : ModifiableIntegersFunction(defaultFunction)
{
}

ModifiableIntegersFunction::ModifiableIntegersFunction(int16_t (*func)(int16_t))
{
    if (!func)
        return;

    values = new int16_t[NUMBERS_COUNT]{0};

    this->func = func;

    for (int i = MIN; i <= MAX; i++)
        setPoint(i, func(i));
}

void ModifiableIntegersFunction::setPoint(int16_t input, int16_t to)
{
    values[(int)input + MAX + 1] = to;
    isDefined.add(input);
    isCheckedForInjection = false;
    isCheckedForSurjection = false;
}

void ModifiableIntegersFunction::removePoint(int16_t input)
{
    if (!isDefinedIn(input)) // if its already removed
        return;

    isDefined.remove(input);
    isCheckedForInjection = false;
    isCheckedForSurjection = true;
    isSurjective = false; // Dirichlet
}

bool ModifiableIntegersFunction::isDefinedIn(int16_t input) const
{
    return isDefined.contains(input);
}

int16_t ModifiableIntegersFunction::operator()(int16_t param) const
{
    if (!isDefinedIn(param))
        throw std::invalid_argument("Function not defined");

    return values[(int)param + MAX + 1];
}

ModifiableIntegersFunction operator+(const ModifiableIntegersFunction &lhs, const ModifiableIntegersFunction &rhs)
{
    ModifiableIntegersFunction res; // can't set pointer here

    for (int i = MIN; i <= MAX; i++)
    {
        if (!lhs.isDefinedIn(i) || !rhs.isDefinedIn(i))
        {
            res.removePoint(i);
        }
        else
        {
            int sum = (int)lhs(i) + (int)rhs(i);
            if (sum > MAX || sum < MIN) // if overflow happens
                res.removePoint(i);
            else
                res.setPoint(i, sum);
        }
    }
    return res;
}

ModifiableIntegersFunction operator-(const ModifiableIntegersFunction &lhs, const ModifiableIntegersFunction &rhs)
{
    ModifiableIntegersFunction res; // can't set pointer here

    for (int i = MIN; i <= MAX; i++)
    {
        if (!lhs.isDefinedIn(i) || !rhs.isDefinedIn(i))
        {
            res.removePoint(i);
        }
        else
        {
            int sum = (int)lhs(i) - (int)rhs(i);
            if (sum > MAX || sum < MIN) // if overflow happens
                res.removePoint(i);
            else
                res.setPoint(i, sum);
        }
    }
    return res;
}

bool operator<(const ModifiableIntegersFunction &lhs, const ModifiableIntegersFunction &rhs)
{
    for (int i = MIN; i <= MAX; i++)
    {
        if (lhs.isDefinedIn(i) && rhs.isDefinedIn(i)) // if both are well defined
        {
            if (!(lhs(i) < rhs(i)))
                return false;
        }
        else if (!lhs.isDefinedIn(i) && !rhs.isDefinedIn(i)) // if both are undefined
        {
            return false;
        }
        else if (!lhs.isDefinedIn(i)) // if lhs in not defined, treat it like the least value
        {
            continue;
        }
        else // if rhs is not defined, treat it like the least value
        {
            return false;
        }
    }

    return true;
}

bool operator==(const ModifiableIntegersFunction &lhs, const ModifiableIntegersFunction &rhs)
{
    for (int i = MIN; i <= MAX; i++)
    {
        if ((lhs.isDefinedIn(i) == rhs.isDefinedIn(i))) // if both are either defined or undefined
        {
            if (lhs.isDefinedIn(i)) // if both are defined
            {
                if (lhs(i) == rhs(i))
                    continue;
                else
                    return false;
            }
            else // if both are undefined
            {
                continue;
            }
        }
        else
        {
            return false;
        }
    }

    return true;
}

bool operator!=(const ModifiableIntegersFunction &lhs, const ModifiableIntegersFunction &rhs)
{
    return !(lhs == rhs);
}

bool operator>(const ModifiableIntegersFunction &lhs, const ModifiableIntegersFunction &rhs)
{
    for (int i = MIN; i <= MAX; i++)
    {
        if (lhs.isDefinedIn(i) && rhs.isDefinedIn(i)) // if both are well defined
        {
            if (!(lhs(i) > rhs(i)))
                return false;
        }
        else if (!lhs.isDefinedIn(i) && !rhs.isDefinedIn(i)) // if both are undefined
        {
            return false;
        }
        else if (!rhs.isDefinedIn(i)) // if rhs in not defined, treat it like the least value
        {
            continue;
        }
        else // if lhs is not defined, treat it like the least value
        {
            return false;
        }
    }

    return true;
}

bool operator<=(const ModifiableIntegersFunction &lhs, const ModifiableIntegersFunction &rhs)
{
    return (lhs < rhs) || (lhs == rhs);
}

bool operator>=(const ModifiableIntegersFunction &lhs, const ModifiableIntegersFunction &rhs)
{
    return (lhs > rhs) || (lhs == rhs);
}

bool operator||(const ModifiableIntegersFunction &lhs, const ModifiableIntegersFunction &rhs)
{
    int curDiff = INT32_MAX; // starting diff is invalid

    for (int i = MIN; i <= MAX; i++)
    {
        if ((lhs.isDefinedIn(i) == rhs.isDefinedIn(i))) // if both are either defined or undefined
        {
            if (lhs.isDefinedIn(i)) // if both are defined
            {
                if (curDiff != INT32_MAX) // if first diff already found
                {
                    if (((int)lhs(i) - (int)rhs(i)) == curDiff) // int cast to avoid overflow
                        continue;
                    else
                        return false;
                }
                else // if there is no diff yet, save it
                {
                    curDiff = (int)lhs(i) - (int)rhs(i); // int cast to avoid overflow
                }
            }
            else // if both are undefined
            {
                continue;
            }
        }
        else
        {
            return false;
        }
    }

    return true;
}

ModifiableIntegersFunction operator*(const ModifiableIntegersFunction &outter, const ModifiableIntegersFunction &inner)
{
    ModifiableIntegersFunction res;
    res.isDefined = inner.isDefined;

    for (int i = MIN; i <= MAX; i++)
    {
        if (!inner.isDefinedIn(i)) // if the inner is undefined the outter will also be undefined
            continue;

        if (outter.isDefinedIn(inner(i)))
            res.setPoint(i, outter(inner(i)));
        else
            res.removePoint(i);
    }

    return res;
}

ModifiableIntegersFunction operator^(const ModifiableIntegersFunction &f, int iterations)
{
    if (iterations > 0) // multiple composition
    {
        ModifiableIntegersFunction res(f);

        for (size_t i = 0; i < iterations - 1; i++)
        {
            res = f * res;
        }
        return res;
    }
    else if (iterations == -1) // inverse function
    {
        if (!f.isInjection())
            throw std::logic_error("Function is not injection, it can't be reversed"); // or return fully undefined func

        ModifiableIntegersFunction res;
        res.isDefined = MySet(); // set all values to undefined
        for (int i = MIN; i < +MAX; i++)
        {
            if (f.isDefinedIn(i))
                res.setPoint(f(i), i); // inverse values, all other values in res are not defined by default
        }
        return res;
    }
    else if (iterations == 0) // identity
    {
        return ModifiableIntegersFunction(identity);
    }
    else
    {
        throw std::invalid_argument("Valid range is only positive, -1 and 0");
    }
}

bool ModifiableIntegersFunction::isSurjection() const
{ // simmilar aproach to the injection, because it's a discrete function
    if (isCheckedForSurjection)
        return isSurjective;

    MySet values; // set all occurences on 0
    for (int i = MIN; i <= MAX; i++)
    {
        if (!isDefinedIn(i)) // not enough values to cover the whole range
        {
            isCheckedForSurjection = true;
            isSurjective = false;
            return false;
        }
        else
        {
            if (values.contains(operator()(i)) == false) // if it's encountered for the first time
                values.add(operator()(i));
            else // if it's already encountered before it's impossible for the func to be surjection according to
                 // Dirichlet's principle
            {
                isCheckedForSurjection = true;
                isSurjective = false;
                return false;
            }
        }
    }

    isCheckedForSurjection = true;
    isSurjective = true;
    return true;
}

bool ModifiableIntegersFunction::isInjection() const
{
    if (isCheckedForInjection)
        return isInjective;

    MySet values; // set all occurences on 0

    for (int i = MIN; i <= MAX; i++)
    {
        if (isDefinedIn(i))
        {
            if ((values.contains(operator()(i)) == false)) // if it's encountered for the first time
                values.add(operator()(i));
            else // it's already encountered before
            {
                isCheckedForInjection = true;
                isInjective = false;
                return false;
            }
        }
    }

    isCheckedForInjection = true;
    isInjective = true;
    return true;
}

bool ModifiableIntegersFunction::isBijection() const
{
    return (isInjection() && isSurjection());
}

void ModifiableIntegersFunction::serialize(const char *dest) const
{
    if (!dest)
        return; // or exception

    std::ofstream ofs(dest, std::ios::binary);
    if (!ofs.is_open())
        return; // or exception

    ofs.write((const char *)values, sizeof(int16_t) * NUMBERS_COUNT);
    ofs.write((const char *)&isDefined, sizeof(isDefined));
    ofs.write((const char *)&isSurjective, sizeof(bool));
    ofs.write((const char *)&isCheckedForSurjection, sizeof(bool));
    ofs.write((const char *)&isInjective, sizeof(bool));
    ofs.write((const char *)&isCheckedForInjection, sizeof(bool));
    ofs.close();
}

void ModifiableIntegersFunction::deserialize(const char *source)
{
    if (!source)
        return; // or exception

    std::ifstream ifs(source, std::ios::binary);
    if (!ifs.is_open())
        return; // or exception

    ifs.read((char *)values, sizeof(int16_t) * NUMBERS_COUNT);
    ifs.read((char *)&isDefined, sizeof(isDefined));
    ifs.read((char *)&isSurjective, sizeof(bool));
    ifs.read((char *)&isCheckedForSurjection, sizeof(bool));
    ifs.read((char *)&isInjective, sizeof(bool));
    ifs.read((char *)&isCheckedForInjection, sizeof(bool));
    ifs.close();
}

void ModifiableIntegersFunction::drawGraphicInInterval(int16_t startX, int16_t startY) const
{
    if ((int)startX + INTERVAL_FOR_PRINTING > INT16_MAX || (int)startY + INTERVAL_FOR_PRINTING > INT16_MAX)
        throw std::invalid_argument("Interval must start on a lower number");

    char field[INTERVAL_FOR_PRINTING][INTERVAL_FOR_PRINTING];
    fillWithSymbol(field, ' ');

    int16_t endX = startX + INTERVAL_FOR_PRINTING - 1, endY = startY + INTERVAL_FOR_PRINTING - 1;

    for (int16_t i = startX, indx = 0; i <= endX; i++, indx++) // filling field
    {
        if (!isDefinedIn(i))
        {
            continue;
        }
        else
        {
            int16_t curValue = operator()(i);

            if (curValue < startY || curValue > endY)
                continue;
            else
                field[curValue - startY][indx] = 'x';
        }
    }

    for (int16_t row = INTERVAL_FOR_PRINTING - 1; row >= 0; row--) // printing field
    {
        std::cout << "\t|";
        for (int16_t col = 0; col < INTERVAL_FOR_PRINTING; col++)
        {
            std::cout << field[row][col];
        }
        std::cout << std::endl;
    }
    std::cout << '\t' << ' ';
    printLine(INTERVAL_FOR_PRINTING, '-');
    // std::cout << std::endl << '\t' << startX;
    // printLine(INTERVAL_FOR_PRINTING, ' ');
    // std::cout << endX;
}
