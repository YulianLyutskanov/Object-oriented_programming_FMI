#include <assert.h>
#include <iostream>

using std::cout;
using std::endl;

enum class Major
{
    Undefined,
    SoftwareEngineering,
    Maths,
    Informatics,
    ComputerScience,
    DataAnalysis
};

struct Student
{
    const char *name = nullptr;
    unsigned fn = 0;
    Major major = Major::Undefined;
};

constexpr unsigned MAX_STUDENTS_COUT = 15;

const char *getMajor(const Student &st)
{
    switch (st.major)
    {
    case Major::Undefined:
        return " Undefined";
    case Major::SoftwareEngineering:
        return " SoftwareEngineering";
    case Major::Maths:
        return " Maths";
    case Major::Informatics:
        return " Informatics";
    case Major::ComputerScience:
        return " ComputerScience";
    case Major::DataAnalysis:
        return " DataAnalysis";
    }
}

struct StudentDatabase
{
    unsigned studentsCount = 0;
    Student students[MAX_STUDENTS_COUT];
};

void printStudent(const Student &st)
{
    cout << st.name << " " << st.fn << " " << getMajor(st);
}

void printDatabase(const StudentDatabase &db)

{
    assert(db.studentsCount <= MAX_STUDENTS_COUT);

    for (size_t i = 0; i < db.studentsCount; i++)
    {
        printStudent(db.students[i]);
        cout << endl;
    }
}

enum class Criteria
{
    BY_NAME,
    BY_FN,
    BY_MAJOR
};

void sortDatabase(StudentDatabase &db, bool (*isLess)(const Student &, const Student &))
{
    for (size_t i = 0; i < db.studentsCount; i++)
    {
        int minIdex = i;
        for (size_t j = i + 1; j < db.studentsCount; j++)
        {
            if (isLess(db.students[j], db.students[minIdex]))
            {
                minIdex = j;
            }
        }
        if (minIdex != i)
            std::swap(db.students[i], db.students[minIdex]);
    }
}

void sortByCriteria(StudentDatabase &db, Criteria crit)
{
    switch (crit)
    {
    case Criteria::BY_NAME:
        return sortDatabase(
            db, [](const Student &left, const Student &right) { return strcmp(left.name, right.name) < 0; });
    case Criteria::BY_FN:
        return sortDatabase(db, [](const Student &left, const Student &right) { return left.fn < right.fn; });
    case Criteria::BY_MAJOR:
        return sortDatabase(db, [](const Student &left, const Student &right) { return left.major < right.major; });
    }
}

int main()
{

    StudentDatabase db = {4,
                          {
                              {"Yulian Lyutskanov", 600295, Major::SoftwareEngineering},
                              {"Boris Ivanov", 600345, Major::Maths},
                              {"Atanas Petrov", 500297, Major::ComputerScience},
                              {"Angel Dimitriev", 200297, Major::Informatics},
                          }};

    printDatabase(db);
    cout << endl;
    sortByCriteria(db, Criteria::BY_FN);
    printDatabase(db);
    cout << endl;
    sortByCriteria(db, Criteria::BY_MAJOR);
    printDatabase(db);
    cout << endl;
    sortByCriteria(db, Criteria::BY_NAME);
    printDatabase(db);
}
