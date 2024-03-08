#include <fstream>
#include <iostream>

using namespace std;

const size_t MAX_LINE_LENGTH = 1024;

// ex.1
void write2NUmbersToFile()
{
    int a, b;
    cin >> a >> b;
    ofstream ofs("result1.txt");
    if (!ofs.is_open())
        cout << "Can' open file!";

    ofs << a + b << ' ' << a - b;

    ofs.close();
}

void printOriginalNumsFromFile()
{
    ifstream ifs("result1.txt");
    if (!ifs.is_open())
        cout << "Can't open file!";

    int sum, diff, a, b;
    ifs >> sum >> diff;
    a = (sum + diff) / 2; // possible loss of data
    b = sum - a;
    cout << a << ' ' << b;

    ifs.close();
}

// ex.2

int mostCommonIntInArr(const int *arr, size_t size)
{

    if (!arr || !size)
        return 0; // error

    int mostCommon = arr[0];
    int ocurrsMostCommon = 1;
    for (int i = 0; i < size; i++)
    {
        int occurs = 1;
        for (size_t j = i + 1; j < size; j++)
        {
            if (arr[i] == arr[j])
                occurs++;
        }

        if (occurs > ocurrsMostCommon) // here it must be said in the task what happens in case there are two numbers
                                       // with same repeatability
        {
            ocurrsMostCommon = occurs;
            mostCommon = arr[i];
        }
    }

    return mostCommon;
}

void mostCommonIntFromFile(const char *fileName)
{

    if (!fileName)
        return;

    ifstream ifs(fileName);

    if (!ifs.is_open())
        cout << "Can't open file!";

    size_t length;
    ifs >> length;

    int *arr = new int[length];

    for (size_t i = 0; i < length; i++)
    {
        ifs >> arr[i];
    }
    cout << mostCommonIntInArr(arr, length);

    ifs.close();
    delete[] arr;
}

// ex.3

void printMyCode()
{
    ifstream ifs(__FILE__);

    if (!ifs.is_open())
        return;

    while (true)
    {
        char ch = ifs.get();
        if (ifs.eof())
            break;
        cout << ch;
    }
    ifs.close();
}

// ex.4
void printFirstDifferentLine(const char *fileName1, const char *fileName2)
{
    if (!fileName1 || !fileName2)
        return;

    ifstream istr1(fileName1);
    ifstream istr2(fileName2);

    if (!istr1.is_open() || !istr2.is_open())
        return;

    char buff1[MAX_LINE_LENGTH] = {};
    char buff2[MAX_LINE_LENGTH] = {};
    while (true)
    {
        istr1.getline(buff1, MAX_LINE_LENGTH);
        istr2.getline(buff2, MAX_LINE_LENGTH);
        if (istr1.eof() || istr2.eof() || strcmp(buff1, buff2) != 0)
            break;
    }

    if (istr1.eof() && istr2.eof())
    {
        istr1.close();
        istr2.close();
        return;
    }

    cout << "First:" << buff1 << endl;
    cout << "Second:" << buff2 << endl;

    istr1.close();
    istr2.close();
}

// ex.5
namespace Relations
{
struct Pair
{
    unsigned n1, n2;
};

constexpr unsigned MAX_RELATION_LENGTH = 25;

struct Relation
{
    unsigned curSize = 0;
    Pair pairs[MAX_RELATION_LENGTH] = {};
};

void writePairToFile(const Pair &p, ofstream &ofstr)
{
    if (!ofstr.is_open())
        return;

    ofstr << '(' << p.n1 << ',' << p.n2 << ")\n";
}

void readPairFromFile(Pair &p, ifstream &ifstr)
{
    if (!ifstr.is_open())
        return;

    ifstr.ignore();

    ifstr >> p.n1;
    ifstr.ignore();

    ifstr >> p.n2;
    ifstr.ignore(2);
}

void writeRelationToFile(const Relation &r, const char *str)
{
    if (!str)
        return;

    ofstream ofstr(str);

    if (!ofstr.is_open())
        return;

    for (size_t i = 0; i < r.curSize; i++)
    {
        writePairToFile(r.pairs[i], ofstr);
    }

    ofstr.close();
}

void readRelationFromFile(Relation &r, const char *str)
{
    if (!str)
        return;

    ifstream ifstr(str);

    if (!ifstr.is_open())
        return;

    unsigned i = 0;
    while (i <= 25)
    {
        Pair cur;
        readPairFromFile(cur, ifstr);
        if (ifstr.eof())
            break;
        r.pairs[i] = cur;
        i++;
    }

    r.curSize = i;
    ifstr.close();
}

void addPairToRelation(Relation &rel, const Pair &pair)
{
    if (!(rel.curSize < MAX_RELATION_LENGTH))
        return;
    rel.pairs[rel.curSize++] = pair;
}
} // namespace Relations

// ex.6

namespace Cities
{
const size_t MAX_CITYNAME_LENGTH = 64;
struct City
{
    char name[MAX_CITYNAME_LENGTH] = {};
    unsigned population = 0;
};

const size_t MAX_CITIES_IN_DISTRICT = 20;
struct District
{
    City cities[MAX_CITIES_IN_DISTRICT] = {};
    unsigned citiesCount = 0;
};

void writeCityToFile(const City &city, ofstream &ofs)
{
    if (!ofs.is_open())
        return;
    ofs << city.name << endl << city.population << endl;
}

void writeDisctrictToFile(const District &d, const char *fileName)
{
    if (!fileName)
        return;

    ofstream ofs(fileName);
    if (!ofs.is_open())
        return;

    // ofs << "The disctrict has the following " << d.citiesCount << " cities with following population:\n";
    for (unsigned i = 0; i < d.citiesCount; i++)
    {
        writeCityToFile(d.cities[i], ofs);
    }
    ofs.close();
}

void readDisctrictFromFile(District &d, const char *fileName)
{
    if (!fileName)
        return;
    ifstream ifs(fileName);
    if (!ifs.is_open())
        return;

    while (true)
    {

        ifs.getline(d.cities[d.citiesCount].name, MAX_CITYNAME_LENGTH);

        if (ifs.eof())
            break;

        ifs >> d.cities[d.citiesCount].population;
        ifs.ignore();

        d.citiesCount++;
    }
    ifs.close();
}

void sortCitiesInDistrict(District &d, bool (*isLess)(const City &c1, const City &c2))
{
    // selection Sort to minimize swaps

    for (size_t i = 0; i < d.citiesCount; i++)
    {
        City smallest = d.cities[i];
        size_t minIndex = i;

        for (size_t j = i + 1; j < d.citiesCount; j++)
        {
            if (isLess(d.cities[j], d.cities[minIndex]))
            {
                minIndex = j;
                smallest = d.cities[j];
            }
        }

        if (minIndex != i)
            swap(d.cities[i], d.cities[minIndex]);
    }
}

//-1 point from Desi for originallity
void readDistrictFromFile_ThenSortCitiesByPopulation_ThenWriteItToFIle(const char *source, const char *dest)
{
    if (!source || !dest)
    {
        return;
    }
    District d = {};
    readDisctrictFromFile(d, source);
    sortCitiesInDistrict(d, [](const City &c1, const City &c2) { return c1.population < c2.population; });
    writeDisctrictToFile(d, dest);
}
} // namespace Cities

// ex.7
const unsigned BUFF_SIZE = 1024;
const char *getFromSource(const char *fieldName, ifstream &ifs)
{
    if (!fieldName)
        return (const char *)'\0';

    char buff[BUFF_SIZE] = {};

    while (true)
    {                // search for the field in the file
        ifs >> buff; // take the first word of the new line
        if (ifs.eof() || strcmp(fieldName, buff) == 0)
            break;

        ifs.ignore(BUFF_SIZE, '\n');
    }
    ifs.ignore(); // to ignore the interval after the field
    ifs.getline(buff, 1024);

    // give back the stream in starting position
    ifs.clear();
    ifs.seekg(0);

    return buff;
}
const char *suffix = "_copy.txt";

void myStrcpy(const char *source, char *dest)
{
    if (!source || !dest)
        return;
    while (*source)
    {
        *dest = *source;
        dest++;
        source++;
    }
    *dest = '\0';
}

void myStrcat(char *first, const char *second)
{
    if (!first || !second)
        return;

    size_t firstLen = strlen(first);
    first += firstLen;
    myStrcpy(second, first);
}

void fillPlaceholders(const char *templateName, const char *sourceName)
{
    if (!templateName || !sourceName)
        return;

    ifstream template_(templateName);
    ifstream source(sourceName);
    char *resultName = new char[strlen(sourceName) + strlen(suffix)];
    myStrcpy(sourceName, resultName);
    myStrcat(resultName, suffix);
    ofstream ofs(resultName, std::ios::out);

    if (!template_.is_open() || !source.is_open() || !ofs.is_open())
        return;

    char buff[BUFF_SIZE];

    while (true)
    {
        template_.get(buff, 1024, '{');
        template_.ignore();

        if (template_.eof())
            break;

        ofs << buff;
        template_.get(buff, 1024, '}');
        template_.ignore();
        // get the placeholder name
        ofs << getFromSource(buff, source);
    }
    template_.close();
    source.close();
    ofs.close();
}

int main()
{
    // write2NUmbersToFile();
    // printOriginalNumsFromFile();

    // mostCommonIntFromFile("test.txt");

    // printMyCode();

    // printFirstDifferentLine("test2.txt", "test3.txt");

    // using namespace Relations;
    //  Relation r = {3, {{1, 2}, {4, 5}, {0, 1}}};
    //  readRelationFromFile(r, "rel.txt");
    //  writeRelationToFile(r, "rel3.txt");

    // using namespace Cities;
    // District d = {{{"Varna", 3255}, {"Sofia", 435345}, {"Gabrovo", 23534}, {"Gorna orqhovica", 324234}}, 4};

    // readDistrictFromFile_ThenSortCitiesByPopulation_ThenWriteItToFIle("s.txt", "dest.txt");

    fillPlaceholders("template.txt", "source.txt");
}
