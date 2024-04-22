#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#pragma warning(disable : 4996)
using std::cout;

namespace GlobalConstants
{
const uint16_t MAX_CELL_SIZE = 50 + 1; // for the terminating zero
const uint16_t MAX_CELLS_IN_ROW = 30;
const uint16_t MAX_ROWS_IN_TABLE = 300;
const char TABLE_TAG[] = "table";
const char ROW_TAG[] = "tr";
const char CELL_TAG[] = "td";
const char HEADER_TAG[] = "th";
const char BEGIN_TAG = '<';
const char END_TAG = '>';
const char CLOSE_TAG = '/';
const char HEADER_SYMBOL = '*';
const char EMPTY_SPACE = ' ';
const char CHARACTER_ENTITY[] = "&#";
const char indentation[] = "  ";
const uint16_t BUFFER_SIZE = 1024;
const uint8_t MAX_TAG_SIZE = 64;

} // namespace GlobalConstants
using namespace GlobalConstants;

namespace HelpingFunctions
{
void characterEntityConvert(char *str)
{
    if (!str)
        return;

    std::stringstream ss(str);

    char ch;
    uint16_t chConverted;

    while (true)
    {
        ch = ss.get();
        if (ss.eof())
            break;

        if (ch == CHARACTER_ENTITY[0] && ss.peek() == CHARACTER_ENTITY[1])
        {
            ss.ignore();                  // ignore the the second symbol (#)
            ss >> chConverted;            // get the asci code as int
            *(str++) = (char)chConverted; // put the char in the new string
            continue;
        }
        *(str++) = ch; // copy the current char in the new string
    }
    *str = '\0'; // end the string
}
const char *trimSpaces(char *str)
{
    if (!str)
        return nullptr;

    char *put = str;
    char *get = str;
    bool spaceFound = false;

    // Skip leading spaces and collapse consecutive spaces
    while (*get)
    {
        if (*get == ' ' || *get == '\t' || *get == '\n')
        {
            if (!spaceFound)
            {
                *(put++) = ' ';
                spaceFound = true;
            }
        }
        else
        {
            *(put++) = *get;
            spaceFound = false;
        }
        ++get;
    }

    // Handle the case where the trimmed string is empty
    if (put == str)
        *put++ = '\0';
    else
    {
        // Move the end pointer back to the last non-space character
        while (put > str && (*(put - 1) == ' ' || *(put - 1) == '\t' || *(put - 1) == '\n' || *(put - 1) == '\r'))
            --put;
        *put = '\0';
    }

    return str;
}
} // namespace HelpingFunctions
using namespace HelpingFunctions;

enum class Direction : int8_t
{
    UP = 1,
    DOWN = -1,
    UNKNOWN = 0
};

struct Cell
{
    char cell[MAX_CELL_SIZE] = {};
    bool isHeader = false;

    uint16_t getCellStrLength() const
    {
        return strlen(cell) + 2; // adding 2 spaces on both ends of table
    }
    void printCell(uint16_t allignement) const
    {
        cout << '|';

        if (isHeader)
            cout << HEADER_SYMBOL << cell << std::setw(allignement - strlen(cell) - 1) << HEADER_SYMBOL;
        else
            cout << EMPTY_SPACE << cell << std::setw(allignement - strlen(cell) - 1) << EMPTY_SPACE;
    }
};

class Row
{
  private:
    Cell cells[MAX_CELLS_IN_ROW] = {};
    uint16_t cellsCount = 0;

  public:
    const Cell *getCells() const
    {
        return cells;
    }
    uint16_t getCellsCount() const
    {
        return cellsCount;
    }

    Row(const Cell *_cells, uint16_t _cellsCount)
    {
        if (!_cells)
            return;

        for (uint16_t i = 0; i < _cellsCount; i++)
            cells[i] = _cells[i];

        cellsCount = _cellsCount;
    }
    Row()
    {
        cellsCount = 0;
    }

    bool modifyCell(uint16_t index, const char *newValue)
    {
        if (!newValue || index >= cellsCount || strlen(newValue) >= MAX_CELL_SIZE)
            return false;

        strcpy(cells[index].cell, newValue);
        return true;
    }
    bool addCellAtBack(const char *value, bool _isHeader)
    {
        if (!value || cellsCount >= MAX_CELLS_IN_ROW || strlen(value) >= MAX_CELL_SIZE)
            return false;

        strcpy(cells[cellsCount].cell, value);
        cells[cellsCount++].isHeader = _isHeader;
        return true;
    }
};

class Table
{
  private:
    Row rows[MAX_ROWS_IN_TABLE] = {};
    uint16_t rowsCount = 0;
    bool isGood = true;

    void shiftRowsDown(uint16_t from)
    {
        for (int16_t i = rowsCount; i > from; i--)
        {
            rows[i] = rows[i - 1];
        }
        rows[from] = {}; // empty the new opened row
        rowsCount++;
    }
    void shiftRowsUp(uint16_t from)
    {
        for (int16_t i = from; i < rowsCount - 1; i++)
        {
            rows[i] = rows[i + 1];
        }
        rows[rowsCount--] = {}; // empty the new opened row at the bottom
    }
    void shiftRows(uint16_t from, Direction dir)
    {
        if (dir == Direction::DOWN)
            shiftRowsDown(from);
        if (dir == Direction::UP)
            shiftRowsUp(from);
    }
    bool getNextTag(char *dest, std::ifstream &ifs) const
    {
        if (!dest || !ifs.good())
            return false;

        while (true)
        {
            char ch = ifs.get();
            if (ch == BEGIN_TAG)
            {
                ifs.getline(dest, MAX_TAG_SIZE, END_TAG);
                return true;
            }
            if (ifs.eof())
                return false;
        }
    }
    bool readNextCell(std::ifstream &ifs, bool isHeader)
    {
        char buff[MAX_CELL_SIZE];
        ifs.get(buff, MAX_CELL_SIZE, '<'); // failbit when trying to read empty string
        ifs.clear();
        strcpy(buff, trimSpaces(buff));
        characterEntityConvert(buff);
        rows[rowsCount].addCellAtBack(buff, isHeader);
        ifs.getline(buff, MAX_CELL_SIZE, '>');
        return true;
    }
    bool readNextRow(std::ifstream &ifs)
    {
        while (true)
        {
            char tag[MAX_TAG_SIZE];
            if (!getNextTag(tag, ifs))
                return isGood = false;

            if (strcmp(tag, CELL_TAG) == 0) // for normal tag
            {
                readNextCell(ifs, false);
            }
            else if (strcmp(tag, HEADER_TAG) == 0) // for header tag
            {
                readNextCell(ifs, true);
            }
            else // if the tag is the closing tag , may be better if we do a correctness check
            {
                rowsCount++;
                return true;
            }
        }
    }
    void calcColsAlignement(uint16_t colsCount, uint16_t *colsAlignement) const
    {
        for (uint16_t i = 0; i < colsCount; i++)
        {
            colsAlignement[i] = 0;
            for (uint16_t j = 0; j < rowsCount; j++)
            {
                if (rows[j].getCells()[i].getCellStrLength() > colsAlignement[i])
                    colsAlignement[i] = rows[j].getCells()[i].getCellStrLength();
            }
        }
    }

  public:
    const Row *getRows() const
    {
        return rows;
    }
    uint16_t getRowsCount() const
    {
        return rowsCount;
    }
    uint16_t countCols() const
    {
        uint16_t res = 0;

        for (size_t i = 0; i < rowsCount; i++)
        {
            if (rows[i].getCellsCount() > res)
                res = rows[i].getCellsCount();
        }
        return res;
    }
    bool isOk() const
    {
        return isGood;
    }
    void print() const
    {
        uint16_t colsCount = this->countCols();
        uint16_t *colsAlignement = new uint16_t[colsCount];
        this->calcColsAlignement(colsCount, colsAlignement);

        for (uint16_t i = 0; i < rowsCount; i++)
        {
            for (uint16_t j = 0; j < colsCount; j++)
            {
                rows[i].getCells()[j].printCell(colsAlignement[j]);
            }
            cout << "|\n";
        }

        delete[] colsAlignement;
    }

    bool edit(uint16_t row, uint16_t col, const char *newValue)
    {
        // index for users starts from 1
        row--;
        col--;

        if (row >= rowsCount || col >= rows[row].getCellsCount() || !newValue)
            return false;

        rows[row].modifyCell(col, newValue);
        return true;
    }
    bool add(uint16_t rowIndex, const Row &newRow)
    {
        rowIndex--; // index for users starts from 1

        if (rowIndex >= MAX_ROWS_IN_TABLE)
            return false;

        shiftRows(rowIndex, Direction::DOWN);
        rows[rowIndex] = newRow;
        return true;
    }
    bool remove(uint16_t row)
    {
        row--; // index for users starts from 1
        if (row > rowsCount)
            return false;

        shiftRows(row, Direction::UP); // using the advantage that the user inputs the rows starting from index 1...
        return true;
    }

    void parseFromFile(const char *fileName)
    {
        if (!fileName)
        {
            isGood = false;
            return;
        }

        std::ifstream ifs(fileName);
        if (!ifs.is_open())
        {
            isGood = false;
            return;
        }

        while (true)
        {
            char tag[MAX_TAG_SIZE];
            if (!getNextTag(tag, ifs))
                return;

            if (strcmp(tag, ROW_TAG) == 0)
            {
                if (!readNextRow(ifs))
                {
                    isGood = false;
                    return;
                }
            }
        }
    }
    void serializeToFileInHTML(const char *fileName) const
    {
        if (!fileName)
            return;

        std::ofstream ofs(fileName);
        if (!ofs.is_open())
            return;

        uint16_t colsCount = this->countCols();
        ofs << BEGIN_TAG << TABLE_TAG << END_TAG << '\n';

        for (uint16_t i = 0; i < rowsCount; i++)
        {
            ofs << indentation << BEGIN_TAG << ROW_TAG << END_TAG << '\n';
            for (uint16_t j = 0; j < rows[i].getCellsCount(); j++)
            {
                ofs << indentation << indentation << BEGIN_TAG
                    << (rows[i].getCells()[j].isHeader ? HEADER_TAG : CELL_TAG) << END_TAG;
                ofs << rows[i].getCells()[j].cell;
                ofs << BEGIN_TAG << CLOSE_TAG << (rows[i].getCells()[j].isHeader ? HEADER_TAG : CELL_TAG) << END_TAG
                    << '\n';
            }
            ofs << indentation << BEGIN_TAG << CLOSE_TAG << ROW_TAG << END_TAG << '\n';
        }

        ofs << BEGIN_TAG << CLOSE_TAG << TABLE_TAG << END_TAG;
    }
    Table() = default;
};

int main()
{
    // char filePath[BUFFER_SIZE];
    // std::cin >> filePath;
    // Table t;
    // t.parseFromFile(filePath);
    // t.print();
    // t.serializeToFileInHTML(filePath);
    //  t.parseFromFile(filePath);
    //  t.print();

    // cout << "\n";
    // Cell cells[5] = {{"baba", true}, {"dedo", false}, {"Az", true}, {"zashto tochno az", true}, {"1232432", false}};
    // Row r(cells, 5);
    // t.add(5, r);
    // t.print();
    // cout << "\n";
    // t.remove(1);
    // t.print();
    char str[] = "      dfdfgfdgdfg    gfdg     dfdgf     ";
    cout << trimSpaces(str) << 'd';
}
