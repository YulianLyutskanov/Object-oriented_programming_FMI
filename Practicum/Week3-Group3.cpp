#include <assert.h>
#include <fstream>
#include <iostream>
#include <sstream>
using namespace std;

namespace GlobalConstants
{
const char ColSep = ',';
const char RowSep = '|';
const size_t BUFFER_SIZE = 1024;
} // namespace GlobalConstants

void printMatrix(const int *const *m, int rows, int cols)
{
    for (size_t i = 0; i < rows; i++)
    {
        for (size_t j = 0; j < cols; j++)
        {
            cout << m[i][j] << ' ';
        }
        cout << '\n';
    }
}

void freeMatrix(int **matrix, int rows)
{
    for (size_t i = 0; i < rows; i++)
    {
        delete[] matrix[i];
    }
    delete[] matrix;
}

void changeChars_In_With(char *str, char toReplace, char with)
{
    if (!str)
        return;

    while (*str)
    {
        if (*str == toReplace)
            *str = with;
        str++;
    }
}

int getRowsCount(ifstream &ifs)
{
    if (!ifs.is_open() || !ifs.good())
        return -1; // error code

    char ch = ifs.get();
    ifs.seekg(0);
    if (ifs.eof())
    {
        ifs.clear();
        return 0; // if we dont have any content
    }

    unsigned result = 0;

    // let's count the row separators on a line
    while (true)
    {
        ch = ifs.get();
        if (ch == GlobalConstants::RowSep)
            result++;
        if (ifs.eof()) // stop if we reach eof
            break;
    }

    ifs.clear();
    ifs.seekg(0);
    return result + 1;
}

int getColsCount(ifstream &ifs)
{
    if (!ifs.is_open() || !ifs.good())
        return -1; // error code

    char ch = ifs.get();
    ifs.seekg(0);
    if (ifs.eof())
    {
        ifs.clear();
        return 0; // if we dont have any content
    }

    unsigned result = 0;

    // let's count the col separators on a line
    while (true)
    {
        ch = ifs.get();
        if (ch == GlobalConstants::ColSep)
            result++;
        if (ch == GlobalConstants::RowSep || ifs.eof()) // stop if we reach eof or endOfRow
            break;
    }

    ifs.clear();
    ifs.seekg(0);
    return result + 1;
}

bool fillMatrix(int **matrix, int rows, int cols, ifstream &ifs)
{
    if (!matrix || !ifs.is_open() || !ifs.good())
        return false;
    for (size_t i = 0; i < rows; i++)
    {
        char buffer[GlobalConstants::BUFFER_SIZE];
        ifs.getline(buffer, GlobalConstants::BUFFER_SIZE, GlobalConstants::RowSep);
        changeChars_In_With(buffer, GlobalConstants::ColSep, ' '); // change separators with ' ' for easier reading

        stringstream str(buffer);

        for (size_t j = 0; j < cols; j++)
        {
            str >> matrix[i][j];
        }
    }
    ifs.clear();
    ifs.seekg(0);
    return true;
}

void writeProductToFile(const int *const *m1, size_t m1Rows, size_t m1Cols, const int *const *m2, size_t m2Rows,
                        size_t m2Cols)
{
    assert(m1Cols == m2Rows);
    ofstream ofs("result.txt");
    if (!ofs.is_open())
        return;

    for (size_t i = 0; i < m1Rows; i++)
    {
        for (size_t j = 0; j < m2Cols; j++)
        {
            int result = 0;
            for (size_t k = 0; k < m1Cols /*or m2Rows*/; k++)
            {
                result += m1[i][k] * m2[k][j];
            }
            ofs << result
                << (i == (m1Rows - 1) && j == (m2Cols - 1) ? '\0'
                                                           : GlobalConstants::ColSep); // skip sep on last el of row
        }
        ofs << (i == (m1Rows - 1) ? '\0' : GlobalConstants::RowSep); // skip sep on last row
    }
    ofs.close();
}

void multiplyMatrices(const char *file1, const char *file2)
{
    if (!file1 || !file2)
        return;

    ifstream ifs1(file1);
    ifstream ifs2(file2);

    if (!ifs1.is_open() || !ifs2.is_open())
        return;

    int m1Cols, m1Rows, m2Cols, m2Rows;
    m1Cols = getColsCount(ifs1);
    m2Cols = getColsCount(ifs2);
    m1Rows = getRowsCount(ifs1);
    m2Rows = getRowsCount(ifs2);
    assert(m1Cols < 1 || !m1Rows < 1 || !m2Cols < 1 || !m2Rows < 1); // check for emtpy matrices

    // create the dynamic arrays
    int **matrix1 = new int *[m1Rows];
    int **matrix2 = new int *[m2Rows];

    for (size_t i = 0; i < m1Rows; i++)
        matrix1[i] = new int[m1Cols];

    for (size_t i = 0; i < m2Rows; i++)
        matrix2[i] = new int[m2Cols];

    bool m1_Filled = fillMatrix(matrix1, m1Rows, m1Cols, ifs1);
    bool m2_Filled = fillMatrix(matrix2, m2Rows, m2Cols, ifs2);

    if (!m1_Filled || !m2_Filled)
        return;
    // printMatrix(matrix1, m1Rows, m1Cols);
    // printMatrix(matrix2, m2Rows, m2Cols);

    writeProductToFile(matrix1, m1Rows, m1Cols, matrix2, m2Rows, m2Cols);
    freeMatrix(matrix1, m1Rows);
    freeMatrix(matrix2, m1Rows);
    ifs1.close();
    ifs2.close();
}

enum class ProductCategory
{
    ELECTRONICS,
    CLOTHING,
    BOOKS,
    FOOD,
    BEAUTY,
    UNKNOWN
};

const unsigned MAX_LENGTH = 100;
struct Product
{
    char name[MAX_LENGTH] = {};
    int quiantityInStock = 0;
    double price = 0;
    ProductCategory category = ProductCategory::UNKNOWN;
    char supplier[MAX_LENGTH] = {};
};

struct Stock
{
    Product products[50];
    size_t productsCount = 0;
};

const char *getCategory(ProductCategory category)
{
    switch (category)
    {
    case ProductCategory::ELECTRONICS:
        return "electronics";
    case ProductCategory::BEAUTY:
        return "beauty";
    case ProductCategory::BOOKS:
        return "books";
    case ProductCategory::CLOTHING:
        return "clothing";
    case ProductCategory::FOOD:
        return "food";
    default:
        return "unknown";
    }
}

void printProduct(const Product &p)
{
    cout << p.name << '\t' << p.quiantityInStock << '\t' << p.price << '\t' << getCategory(p.category) << '\t'
         << p.supplier;
}

void printStock(const Stock &s)
{
    cout << "name\tquant\tprice\tcat\tsupplier\n";
    for (size_t i = 0; i < s.productsCount; i++)
    {
        printProduct(s.products[i]);
        cout << endl;
    }
}

void printProductFromStockByName(const Stock &s, const char *name)
{
    if (!name)
        return;

    for (size_t i = 0; i < s.productsCount; i++)
    {
        if (strcmp(name, s.products[i].name) == 0)
        {
            printProduct(s.products[i]);
            return;
        }
    }
}

// void writeProductInBinary(const Product &p, ofstream &ofs)
//{
//     ofs.write((const char *)p.name, sizeof(p.name));
//     ofs.write((const char *)&p.quiantityInStock, sizeof(p.quiantityInStock));
//     ofs.write((const char *)&p.price, sizeof(p.price));
//     ofs.write((const char *)&p.category, sizeof(p.category));
//     ofs.write((const char *)p.supplier, sizeof(p.supplier));
// }

void writeStockInBinary(const Stock &s, fstream &fs)
{
    if (!fs.is_open())
        return;

    fs.write((const char *)&s, sizeof(s));
}

void readStockFromBinary(Stock &s, fstream &fs)

{
    if (!fs.is_open())
        return;

    fs.read((char *)&s, sizeof(s));
}

bool shouldAddToFile(const Product &p, Stock &s)
{
    // if the date is invalid
    if (!((int)p.category >= (int)ProductCategory::ELECTRONICS && (int)p.category <= (int)ProductCategory::UNKNOWN) ||
        p.price < 0 || p.quiantityInStock < 0 || s.productsCount >= 50)
        return false;

    // we search if its already on the stock
    for (size_t i = 0; i < s.productsCount; i++)
    {
        if (strcmp(p.name, s.products[i].name) == 0) // if the product is in the stock don't overwrite it
            return false;
    }

    return true;
}

ProductCategory getCategory(int n)
{
    switch (n)
    {
    case (int)ProductCategory::BEAUTY:
        return ProductCategory::BEAUTY;
    case (int)ProductCategory::BOOKS:
        return ProductCategory::BOOKS;
    case (int)ProductCategory::CLOTHING:
        return ProductCategory::CLOTHING;
    case (int)ProductCategory::ELECTRONICS:
        return ProductCategory::ELECTRONICS;
    case (int)ProductCategory::FOOD:
        return ProductCategory::FOOD;
    default:
        return ProductCategory::UNKNOWN;
    }
}

void addProductsToBinFile(fstream &fs, unsigned n)
{
    if (!fs.is_open())
        return;

    Stock s;
    readStockFromBinary(s, fs);
    fs.clear();
    fs.seekp(0, ios::end);
    for (size_t i = 0; i < n; i++)
    {
        int category = (int)ProductCategory::UNKNOWN;

        Product p;
        cin >> p.name >> p.quiantityInStock >> p.price >> category >> p.supplier;
        p.category = getCategory(category);

        if (shouldAddToFile(p, s))
        {
            s.products[s.productsCount++] = p;
            fs.write((const char *)&p, sizeof(p));
        }
    }
    printStock(s);
}

int main()
{
    // multiplyMatrices("m1.txt", "m2.txt");

    Stock s = {{{"Broom", 3, 2.5, ProductCategory::UNKNOWN, "Bachorski"},
                {"Dune", 5, 11.5, ProductCategory::BOOKS, "Egmont"},
                {"Calc", 23, 4.5, ProductCategory::ELECTRONICS, "Sony"}},
               3};

    fstream fs("prodBinary.txt", ios::in | ios::out | ios::binary);
    // printStock(s);
    // printProductFromStockByName(s, "Calc");
    // writeStockInBinary(s, fs);
    // readStockFromBinary(s, fs);
    addProductsToBinFile(fs, 3);
    printStock(s);
}
