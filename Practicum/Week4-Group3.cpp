#include <fstream>
#include <iostream>
#pragma warning(disable : 4996)
using std::cout;

const uint16_t MAX_TITLE_SIZE = 64 + 1;

// ex.1

struct User
{
    char name[25]{};
};

struct Message
{
  private:
    User from;
    char *text = nullptr;

  public:
    Message() = default;
    Message(const User &u, const char *m)
    {
        setUser(u);
        setText(m);
    }

    const char *getText() const
    {
        return text;
    }
    void setUser(const User &u)
    {
        from = u;
    }
    void setText(const char *m)
    {
        if (!m)
            return;

        delete[] text;
        text = new char[strlen(m) + 1];
        strcpy(text, m);
    }
    void print() const
    {
        cout << from.name << ':' << text << '\n';
    }
    void deserialiseMessege(std::ifstream &ifs)
    {

        if (!ifs.is_open() || !ifs.good())
            return;

        ifs.ignore(); // to ignore the '<
        if (ifs.eof())
            return;
        ifs.getline(from.name, 25, '>');

        ifs.ignore(); // to ingore the '<
        int textSize;
        ifs >> textSize;
        ifs.ignore(2); // to ignore the '><'

        delete[] text;
        text = new char[textSize + 1];
        text[textSize] = '\0';
        ifs.read(text, textSize);
        ifs.ignore(); // to ignore the '>'
    }
    ~Message()
    {
        delete[] text;
    }
};

void printChat(const char *filePath)
{
    if (!filePath)
        return;

    std::ifstream ifs(filePath, std::ios::binary);
    if (!ifs.is_open())
        return;

    while (true)
    {
        Message mes;
        mes.deserialiseMessege(ifs);
        if (ifs.eof())
            break;
        mes.print();
    }
}

// ex.2

class Secret
{
  private:
    char *task = nullptr;
    char *password = nullptr;
    mutable uint32_t failedLoginAttempts = 0;

    bool doesPassMatch(const char *enteredPassword) const
    {
        if (!enteredPassword)
            return false;

        if (strcmp(enteredPassword, password) != 0)
        {
            failedLoginAttempts++;
            return false;
        }
        else
            return true;
    }

  public:
    const char *getTask(const char *pwd) const
    {
        if (!pwd)
            return nullptr;

        if (doesPassMatch(pwd))
            return task;
    }
    uint32_t getLoginFails() const
    {
        return failedLoginAttempts;
    }

    void setPassword(const char *newPassword, const char *oldPassword)
    {
        if (!newPassword || !oldPassword)
            return;

        if (doesPassMatch(oldPassword) || !password)
        {
            delete[] password;
            password = new char[strlen(newPassword) + 1];
            strcpy(password, newPassword);
        }
    }
    void setTask(const char *newTask, const char *passwd)
    {
        if (!newTask || !passwd)
            return;

        if (doesPassMatch(passwd))
        {
            delete[] task;
            task = new char[strlen(newTask) + 1];
            strcpy(task, newTask);
        }
    }

    // we shouldn't have setLoginFails(), we dont want to give the user access to modify them
    // we need a destructor
    ~Secret()
    {
        delete[] task;
        delete[] password;
    }
};

// ex.3

const double tolerance = 0.0001;

class Game
{
  private:
    char title[MAX_TITLE_SIZE] = {};
    double price = 0;
    bool isAvailable = false;

  public:
    Game() = default;
    Game(const char *title, double price, bool isAvailable)
    {
        setTitle(title);
        setPrice(price);
        setAvailability(isAvailable);
    }

    double getPrice() const
    {
        return price;
    }
    bool isAvilable() const
    {
        return isAvailable;
    }
    const char *getTitle() const
    {
        return title;
    }

    void setAvailability(bool isAvailable)
    {
        this->isAvailable = isAvailable;
    }
    void setPrice(double price)
    {
        if (price < 0)
            return;
        this->price = price;
    }
    void setTitle(const char *title)
    {
        if (!title)
            return;
        if (strlen(title) >= MAX_TITLE_SIZE)
            return;
        strcpy(this->title, title);
    }

    bool isFree() const
    {
        return price < tolerance;
    }
    void print() const
    {
        cout << "title: " << title << ", price:" << price
             << "Availability:" << (isAvailable ? "it's available" : "it's not available") << '\n';
    }
};

int main()
{
}
