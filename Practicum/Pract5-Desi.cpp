#include <iostream>

const unsigned MAX_LENGTH = 127 + 1;
const unsigned MAX_EMPLOYEES = 15;

enum class ErrorCode
{
    OK,
    NotFound,
    Duplicate,
    InvalidInput,
    TooLongString
};

class Employee
{
  private:
    unsigned id = 0;
    char name[MAX_LENGTH] = {};
    char position[MAX_LENGTH] = {};
    double salary = 0;
    ErrorCode errorState = ErrorCode::OK;

  public:
    Employee() = default;
    Employee(unsigned id, const char *name, const char *position, double salary)
    {
    }

    void setId(unsigned newID)
    {
        id = newID;
    }
    void setSalary(double newSalary)
    {
        if (newSalary < 0)
        {
            errorState = ErrorCode::InvalidInput;
            return;
        }
        else
        {
            salary = newSalary;
        }
    }
    void setName(const char *newName)
    {
        if (!newName)
        {
            errorState = ErrorCode::NotFound;
            return;
        }
        else if (strlen(newName) > MAX_LENGTH - 1)
        {
            errorState = ErrorCode::TooLongString;
        }
        else
        {
            strcpy(name, newName);
        }
    }

    double getSalary() const
    {
        return salary;
    }
    const char *getPosition() const
    {
        return position;
    }
    unsigned getId() const
    {
        return id;
    }
    const char *getName() const
    {
        return name;
    }
    ErrorCode getErrorState() const
    {
        return errorState;
    }

    ErrorCode updateSalary(double amount)
    {
        if (amount < 0)
            return ErrorCode::InvalidInput;
        else
            salary = amount;
    }
};

class Department
{
  private:
    char name[MAX_LENGTH] = {};
    Employee employees[MAX_EMPLOYEES] = {};
    unsigned countOfEmployees = 0;
    ErrorCode errorState = ErrorCode::OK;

  public:
    Department() = default;
    Department(const char *name, const Employee *employees, unsigned countOfEmployees)
    {
    }
    void setName(const char *name)
    {
        if (!name || strlen(name))
        {
        }
    }
};

int main()
{
}
