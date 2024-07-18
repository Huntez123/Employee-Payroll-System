#include <iostream>
#include <vector>
#include <iomanip>
#include <limits>
#include <stdexcept>
#include <string>

using namespace std;

class Person 
{
protected:
    int id;
    string name;

public:
    Person(int id, string name) : id(id), name(name) {}
    virtual ~Person() {}

    virtual void displayInfo() const 
    {
        cout << "ID: " << id << ", Name: " << name << endl;
    }

    int getId() const 
    {
        return id;
    }
};

class Employee : public Person 
{
private:
    double hourlyWage;
    double hoursWorked;
    double overtimeHours;
    double deductions;

public:
    Employee(int id, string name, double hourlyWage)
        : Person(id, name), hourlyWage(hourlyWage), hoursWorked(0), overtimeHours(0), deductions(0) {}

    void addHoursWorked(double hours) 
    {
        hoursWorked += hours;
    }

    void addOvertimeHours(double hours) 
    {
        overtimeHours += hours;
    }

    void addDeductions(double amount) 
    {
        deductions += amount;
    }

    double calculateGrossPay() const 
    {
        double regularPay = hoursWorked * hourlyWage;
        double overtimePay = overtimeHours * hourlyWage * 1.5; 
        return regularPay + overtimePay;
    }

    double calculateNetPay() const 
    {
        double grossPay = calculateGrossPay();
        double tax = grossPay * 0.2; 
        return grossPay - tax - deductions;
    }

    void reset() 
    {
        hoursWorked = 0;
        overtimeHours = 0;
        deductions = 0;
    }

    void generatePayStub() const 
    {
        cout << fixed << setprecision(2);
        cout << "Pay Stub for Employee ID: " << id << " - " << name << endl;
        cout << "Hours Worked: " << hoursWorked << endl;
        cout << "Overtime Hours: " << overtimeHours << endl;
        cout << "Gross Pay: Ksh " << calculateGrossPay() << endl;
        cout << "Deductions: Ksh " << deductions << endl;
        cout << "Net Pay: Ksh " << calculateNetPay() << endl;
        cout << "-----------------------------------" << endl;
    }

    void displayInfo() const override 
    {
        Person::displayInfo();
        cout << "Hourly Wage: Ksh " << hourlyWage << endl;
    }
};

class PayrollSystem 
{
private:
    vector<Employee*> employees;

public:
    ~PayrollSystem() 
    {
        for (Employee* emp : employees) 
        {
            delete emp;
        }
    }

    void addEmployee(int id, string name, double hourlyWage) 
    {
        employees.push_back(new Employee(id, name, hourlyWage));
    }

    Employee* findEmployee(int id) 
    {
        for (Employee* emp : employees) 
        {
            if (emp->getId() == id) 
            {
                return emp;
            }
        }
        return nullptr;
    }

    bool deleteEmployee(int id) 
    {
        for (auto it = employees.begin(); it != employees.end(); ++it) 
        {
            if ((*it)->getId() == id) 
            {
                delete *it;
                employees.erase(it);
                return true;
            }
        }
        return false;
    }

    void generateAllPayStubs() const 
    {
        for (Employee* emp : employees) 
        {
            emp->generatePayStub();
        }
    }

    void resetAllEmployees() 
    {
        for (Employee* emp : employees) 
        {
            emp->reset();
        }
    }

    void displayAllEmployees() const 
    {
        for (const Employee* emp : employees) 
        {
            emp->displayInfo();
        }
    }
};

class PayrollException : public runtime_error 
{
public:
    PayrollException(const string& message) : runtime_error(message) {}
};

void clearInputBuffer() 
{
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

int displayMenu() 
{
    int choice;
    cout << "Payroll System Menu" << endl;
    cout << "1. Add employee" << endl;
    cout << "2. Enter employee work details" << endl;
    cout << "3. Generate pay stubs" << endl;
    cout << "4. Delete employee" << endl;
    cout << "5. Display all employees" << endl;
    cout << "6. Exit" << endl;
    cout << "Enter your choice: ";
    cin >> choice;
    return choice;
}

void addEmployee(PayrollSystem& payroll) 
{
    int id;
    string name;
    double hourlyWage;

    cout << "Enter employee ID: ";
    cin >> id;
    clearInputBuffer();
    cout << "Enter employee name: ";
    getline(cin, name);
    cout << "Enter hourly wage in Ksh: ";
    cin >> hourlyWage;

    payroll.addEmployee(id, name, hourlyWage);
    cout << "Employee added successfully." << endl;
}

void enterWorkDetails(PayrollSystem& payroll) 
{
    int id;
    double hoursWorked, overtimeHours, deductions;

    cout << "Enter employee ID: ";
    cin >> id;
    Employee* emp = payroll.findEmployee(id);

    if (emp) 
    {
        cout << "Enter hours worked: ";
        cin >> hoursWorked;
        emp->addHoursWorked(hoursWorked);

        cout << "Enter overtime hours: ";
        cin >> overtimeHours;
        emp->addOvertimeHours(overtimeHours);

        cout << "Enter deductions in Ksh: ";
        cin >> deductions;
        emp->addDeductions(deductions);
    } 
    else 
    {
        cout << "Employee not found!" << endl;
    }
}

void deleteEmployee(PayrollSystem& payroll) 
{
    int id;
    cout << "Enter employee ID to delete: ";
    cin >> id;
    if (payroll.deleteEmployee(id)) 
    {
        cout << "Employee deleted successfully." << endl;
    } 
    else 
    {
        cout << "Employee not found!" << endl;
    }
}

void displayAllEmployees(const PayrollSystem& payroll) 
{
    payroll.displayAllEmployees();
}

int main() 
{
    PayrollSystem payroll;
    int choice;

    while (true) 
    {
        try 
        {
            choice = displayMenu();

            switch (choice) 
            {
            case 1:
                addEmployee(payroll);
                break;
            case 2:
                enterWorkDetails(payroll);
                break;
            case 3:
                payroll.generateAllPayStubs();
                payroll.resetAllEmployees();
                break;
            case 4:
                deleteEmployee(payroll);
                break;
            case 5:
                displayAllEmployees(payroll);
                break;
            case 6:
                return 0;
            default:
                throw PayrollException("Invalid choice, please try again.");
            }
        } 
        catch (const PayrollException& e) 
        {
            cout << e.what() << endl;
        }

        clearInputBuffer(); 
    }

    return 0;
}
