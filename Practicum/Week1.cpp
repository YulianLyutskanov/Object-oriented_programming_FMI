#include <assert.h>
#include <iostream>

using std::cin;
using std::cout;
using std::endl;

namespace ComplexNums
{

struct ComplexNum
{
    int re = 0;
    int im = 0;
};

ComplexNum sum(const ComplexNum &first, const ComplexNum &second)
{
    return {(first.re + second.re), (first.im + second.im)};
}

ComplexNum product(const ComplexNum &first, const ComplexNum &second)
{
    return {((first.re * second.re) - (first.im * second.im)), ((first.re * second.im) + (second.re * first.im))};
}

void printComplexNum(const ComplexNum &num)
{
    cout << num.re << " + " << num.im << "i";
}

} // namespace ComplexNums

namespace Books
{
enum class Genre
{
    NOVEL,
    HISTORIC,
    POETRY,
    OTHER
};
enum class Avalability
{
    AVAILABLE,
    UNAVAILABLE,
    UNKNOWN
};

constexpr unsigned MAX_NAME_LENGTH = 50;
struct Book
{
    char title[MAX_NAME_LENGTH] = {};
    char author[MAX_NAME_LENGTH] = {};
    Genre genre = Genre::OTHER;
    Avalability isAvailable = Avalability::UNKNOWN;
};

constexpr unsigned MAX_BOOKS = 15;
struct Library
{
    unsigned booksCount = 0;
    Book lib[MAX_BOOKS];
};

void getBook(Book &book)
{
    char InputGenre[MAX_NAME_LENGTH] = {};
    bool isAvaiable;

    cin >> InputGenre >> isAvaiable;

    cin >> book.title;
    cin >> book.author;

    if (strcmp(InputGenre, "NOVEL") == 0 || strcmp(InputGenre, "Novel") == 0 || strcmp(InputGenre, "novel") == 0)
    {
        book.genre = Genre::NOVEL;
    }
    else if (strcmp(InputGenre, "HISTORIC") == 0 || strcmp(InputGenre, "Historic") == 0 ||
             strcmp(InputGenre, "historic") == 0)
    {
        book.genre = Genre::HISTORIC;
    }
    else if (strcmp(InputGenre, "POETRY") == 0 || strcmp(InputGenre, "Poetry") == 0 ||
             strcmp(InputGenre, "poetry") == 0)
    {
        book.genre = Genre::POETRY;
    }
    else
    {
        book.genre = Genre::OTHER;
    }

    isAvaiable ? book.isAvailable = Avalability::AVAILABLE : book.isAvailable = Avalability::UNAVAILABLE;
}

void addBookToLibrary(Library &mylib, const Book &book)
{
    assert(mylib.booksCount <= MAX_BOOKS);

    mylib.lib[mylib.booksCount++] = book;
}

void returnBookToLibray(Library &mylib, const char *title)
{
    if (!title)
        return;

    for (size_t i = 0; i < mylib.booksCount; i++)
    {
        if (strcmp(title, mylib.lib[i].title) == 0)
        {
            mylib.lib[i].isAvailable = Avalability::AVAILABLE;
            return;
        }
    }
    cout << "This book doesn't belong to library";
}

void takeBookFromLibrary(Library &mylib, const char *title)
{
    for (size_t i = 0; i < mylib.booksCount; i++)
    {
        if (strcmp(title, mylib.lib[i].title) == 0)
        {
            mylib.lib[i].isAvailable = Avalability::UNAVAILABLE;
            return;
        }
    }
    cout << "This book is not on the Library list";
}

const char *getGenre(const Book &book)
{
    switch (book.genre)
    {
    case Genre::NOVEL:
        return "Novel";
    case Genre::HISTORIC:
        return "Historic";
    case Genre::POETRY:
        return "Poetry";
    default:
        return "Unknown";
    }
}

const char *getAvailability(const Book &book)
{
    switch (book.isAvailable)
    {
    case Avalability::AVAILABLE:
        return "Available";
    case Avalability::UNAVAILABLE:
        return "Unavailable";
    default:
        return "unknown";
    }
}

void printBook(const Book &book)
{
    cout << book.title << " by " << book.author << ". Genre: " << getGenre(book) << ". " << getAvailability(book);
    ;
}

void printLibrary(const Library &mylib)
{
    for (size_t i = 0; i < mylib.booksCount; i++)
    {
        printBook(mylib.lib[i]);
        cout << endl;
    }
}

} // namespace Books

namespace Students
{

enum class Year
{
    UNKNOWN,
    FIRST,
    SECOND,
    THIRD,
    FORTH,
};

struct Student
{
    unsigned short fn = 0;
    Year year = Year::UNKNOWN;
    double avGrade = 0;
};

void initStudent(Student &st, unsigned short theirFn, unsigned short theirYear, double theirGrade)
{
    st.fn = theirFn;
    switch (theirYear)
    {
    case 1:
        st.year = Year::FIRST;
        break;
    case 2:
        st.year = Year::SECOND;
        break;
    case 3:
        st.year = Year::THIRD;
        break;
    case 4:
        st.year = Year::FORTH;
        break;
    default:
        st.year = Year::UNKNOWN;
        break;
    }
    st.avGrade = theirGrade;
}
const char *getYear(const Student &st)
{
    switch (st.year)
    {
    case Year::FIRST:
        return "First";
    case Year::SECOND:
        return "Second";
    case Year::THIRD:
        return "Third";
    case Year::FORTH:
        return "Forth";
    default:
        return "Unknown";
    }
}

void printStudent(const Student &st)
{
    cout << st.fn << " " << getYear(st) << " " << st.avGrade;
}
constexpr unsigned NUM_OF_STUDENTS = 3;
struct Group
{
    unsigned short studentsCount = 0;
    Student group[NUM_OF_STUDENTS] = {};
    double avGrade = 0;
};

Group createGroup()
{
    Group result;
    while (result.studentsCount < NUM_OF_STUDENTS)
    {
        Student cur;
        short fn;
        cin >> fn;
        short year;
        cin >> year;
        double avGrade;
        cin >> avGrade;
        initStudent(cur, fn, year, avGrade);
        result.group[result.studentsCount++] = cur;
        result.avGrade = (result.avGrade * (result.studentsCount - 1) + avGrade) / result.studentsCount;
    }
    return result;
}

unsigned numOfStudentsInGroupWithScholarship(const Group &gr, double minGrade)
{
    unsigned result = 0;
    for (size_t i = 0; i < gr.studentsCount; i++)
    {
        if (gr.group[i].avGrade >= minGrade)
            result++;
    }
    return result;
}

void sortByGrade(Group &gr)
{
    unsigned maxIndex = 0;
    for (size_t i = 0; i < gr.studentsCount; i++)
    {
        maxIndex = i;
        for (size_t j = i + 1; j < gr.studentsCount; j++)
        {
            if (gr.group[j].avGrade > gr.group[maxIndex].avGrade)
            {
                maxIndex = j;
            }
        }
        std::swap(gr.group[i], gr.group[maxIndex]);
    }
}

void printStipendiants(Group &gr, double minGrade)
{
    int count = numOfStudentsInGroupWithScholarship(gr, minGrade);
    sortByGrade(gr);
    for (size_t i = 0; i < count; i++)
    {
        printStudent(gr.group[i]);
    }
}

} // namespace Students

int main()
{
    // using namespace ComplexNums;
    // ComplexNum num1 = {1, 3};
    // ComplexNum num2 = {2, 9};
    // printComplexNum(product(num1, num2));

    // using namespace Books;
    // Library lib = {3,
    //                {
    //                    {"Mobi Dik", "Herman Melville", Genre::NOVEL, Avalability::AVAILABLE},
    //                    {"50 nuansa", "nqkyv gbi", Genre::HISTORIC, Avalability::UNAVAILABLE},
    //                    {"hityr petar", "narodna", Genre::OTHER, Avalability::UNKNOWN},
    //                }};

    // addBookToLibrary(lib, {"na baba mi hvar4iloto", "georgi", Genre::POETRY, Avalability::AVAILABLE});
    // returnBookToLibray(lib, "50 nuansa");
    // takeBookFromLibrary(lib, "50 nuansa");
    // printLibrary(lib);

    // using namespace Students;
    //  Student ivan;
    //  initStudent(ivan, 23452, 6, 4.55);
    //  printStudent(ivan);

    // Group gr = createGroup();
    //  cout << gr.avGrade << endl;
    //  cout << numOfStudentsInGroupWithScholarship(gr, 4.6);

    // printStipendiants(gr, 4.56);
}
