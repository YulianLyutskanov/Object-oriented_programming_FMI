#include <fstream>
#include <iostream>
using namespace std;

const size_t BUFFER_SIZE = 1024;

// ex1
void printProgrammeCode()
{
    ifstream ifs(__FILE__);
    if (!ifs.is_open())
        return;

    while (true)
    {
        char ch = ifs.get();
        if (ifs.eof())
        {
            ifs.close();
            return;
        }
        cout << ch;
    }
}

// ex2
unsigned getCharCountFromFile(ifstream &ifs, char toSearch)
{

    if (!ifs.is_open())
        return 0;

    unsigned res = 0;
    while (true)
    {
        char ch = ifs.get();
        if (ifs.eof())
            break;

        if (ch == toSearch)
            res++;
    }
    return res;
}

int linesInFIle(const char *fileName)
{
    if (!fileName)
        return -1;

    ifstream ifs(fileName);

    if (!ifs.is_open())
        return -2;

    return getCharCountFromFile(ifs, '\n') + 1;
}

// ex.3
void copyFile(const char *source, const char *dest)
{
    if (!source || !dest)
        return;
    ifstream ifs(source);
    ofstream ofs(dest);

    if (!ifs.is_open() || !ofs.is_open())
        return;

    while (true)
    {
        char ch = ifs.get();
        if (ifs.eof())
            return;
        ofs << ch;
    }
    ifs.close();
    ofs.close();
}

// ex.4
void createNewMergedAndSortedFile(const char *fileName1, const char *fileName2, const char *dest)
{
    if (!fileName1 || !fileName2 || !dest)
        return;

    ifstream ifs1(fileName1);
    if (!ifs1.is_open())
        return;

    ifstream ifs2(fileName2);
    if (!ifs2.is_open())
        return;

    ofstream ofs(dest);
    if (!ofs.is_open())
        return;

    int n1, n2;
    ifs1 >> n1;
    ifs2 >> n2;
    while (true)
    {
        ofs << min(n1, n2) << ' ';
        if (ifs1.eof() || ifs2.eof())
        {
            ofs << max(n1, n2) << ' ';
            break;
        }

        if (n1 < n2)
            ifs1 >> n1;
        else
            ifs2 >> n2;
    }
    if (ifs1.eof())
    {
        while (true)
        {
            int n;
            ifs2 >> n;
            ofs << n << ' ';
            if (ifs2.eof())
                break;
        }
    }
    else
    {
        while (true)
        {
            int n;
            ifs1 >> n;
            ofs << n << ' ';
            if (ifs1.eof())
                break;
        }
    }

    ifs1.close();
    ifs2.close();
    ofs.close();
}

// ex.5

enum class Major
{
    Undefined,
    SoftwareEngineering,
    Maths,
    Informatics,
    ComputerScience,
    DataAnalysis
};

const char *getMajorStr(Major m)
{
    switch (m)
    {
    case Major::Undefined:
        return "Undefined";
    case Major::SoftwareEngineering:
        return "SoftwareEngineering";
    case Major::Maths:
        return "Maths";
    case Major::Informatics:
        return "Informatics";
    case Major::ComputerScience:
        return "ComputerScience";
    case Major::DataAnalysis:
        return "DataAnalysis";
    }
}

Major getMajorFromStr(const char *str)
{
    if (!str)
        return Major::Undefined;
    if (strcmp(str, "SoftwareEngineering") == 0)
        return Major::SoftwareEngineering;
    else if (strcmp(str, "Maths") == 0)
        return Major::Maths;
    else if (strcmp(str, "Informatics") == 0)
        return Major::Informatics;
    else if (strcmp(str, "ComputerScience") == 0)
        return Major::ComputerScience;
    else if (strcmp(str, "DataAnalysis") == 0)
        return Major::DataAnalysis;
    else
        return Major::Undefined;
}

struct Student
{
    char name[BUFFER_SIZE];
    unsigned fn = 0;
    Major major = Major::Undefined;
};

void printStudent(const Student &st)
{
    cout << st.name << ' ' << st.fn << ' ' << getMajorStr(st.major) << '\n';
}

void serializeStudent(const Student &s, ofstream &fs)
{
    fs << s.name << '\n' << s.fn << '\n ' << getMajorStr(s.major) << '\n';
}

void writeStudentArrInFile(const Student *arr, size_t size, const char *fileName)
{
    if (!fileName)
        return;

    ofstream ofs(fileName);
    if (!ofs.is_open())
        return;

    ofs << size << "\n";

    for (size_t i = 0; i < size; i++)
    {
        serializeStudent(arr[i], ofs);
    }
    ofs.close();
}

void readStudentFromFile(Student &s, ifstream &ifs)
{
    ifs.ignore();
    ifs.getline(s.name, BUFFER_SIZE);
    ifs >> s.fn;
    char buff[BUFFER_SIZE];
    ifs >> buff;
    s.major = getMajorFromStr(buff);
}

void readStudentArrFromFile(Student *s, const char *fileName)
{
    if (!fileName || !s)
        return;
    ifstream ifs(fileName);
    if (!ifs.is_open())
        return;

    int size;
    ifs >> size;
    for (size_t i = 0; i < size; i++)
    {
        readStudentFromFile(s[i], ifs);
    }
    ifs.close();
}

void printStudents(const Student *s, size_t size)
{
    if (!s)
        return;

    for (size_t i = 0; i < size; i++)
    {
        printStudent(s[i]);
    }
}

int main()
{
    // printProgrammeCode();
    // cout << linesInFIle(__FILE__);
    // copyFile(__FILE__, "result.txt");
    // createNewMergedAndSortedFile("f1.txt", "f2.txt", "res.txt");
    Student arr[5] = {{"Yulian Lyutskanov", 600295, Major::SoftwareEngineering},
                      {"Boris Ivanov", 600345, Major::Maths},
                      {"Atanas Petrov", 500297, Major::ComputerScience},
                      {"Angel Dimitriev", 200297, Major::Informatics},
                      {"Gergana Gocheva", 34534543, Major::Undefined}};

    // writeStudentArrInFile(arr, 5, "students.txt");
    // Student arr2[50] = {};
    // readStudentArrFromFile(arr2, "students.txt");
    // printStudents(arr2, 5);
}
