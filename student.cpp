#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <cctype>
#include <limits>

using namespace std;

class Student
{
private:
    int rollNumber;
    string name;
    string city;
    int age;
    string mobile;
    double percentage;
    string branch;

public:
    void inputData()
    {
        cout << "\n===== ENTER STUDENT DETAILS =====\n";

        cout << "Enter Roll Number: ";
        while (!(cin >> rollNumber)) {
            cout << "Invalid input! Please enter a number: ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }

        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        cout << "Enter Name: ";
        getline(cin, name);

        cout << "Enter City: ";
        getline(cin, city);

        cout << "Enter Age: ";
        while (!(cin >> age)) {
            cout << "Invalid input! Please enter a number: ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }

        cout << "Enter Mobile Number: ";
        cin >> mobile;

        cout << "Enter 12th Percentage: ";
        while (!(cin >> percentage)) {
            cout << "Invalid input! Please enter a valid percentage: ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }

        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        cout << "Enter Branch: ";
        getline(cin, branch);
    }

    int getRollNumber() const
    {
        return rollNumber;
    }

    void displayAll() const
    {
        cout << "\n-----------------------------\n";
        cout << "Roll Number     : " << rollNumber << endl;
        cout << "Name            : " << name << endl;
        cout << "City            : " << city << endl;
        cout << "Age             : " << age << endl;
        cout << "Mobile          : " << mobile << endl;
        cout << "Percentage      : " << percentage << "%" << endl;
        cout << "Branch          : " << branch << endl;
        cout << "-----------------------------\n";
    }

    void principal() const
    {
        cout << "\n-----------------------------\n";
        cout << "Roll Number     : " << rollNumber << endl;
        cout << "Name            : " << name << endl;
        cout << "City            : " << city << endl;
        cout << "Mobile          : " << mobile << endl;
        cout << "Percentage      : " << percentage << "%" << endl;
        cout << "Branch          : " << branch << endl;
    }

    void hod() const
    {
        cout << "\n-----------------------------\n";
        cout << "Roll Number     : " << rollNumber << endl;
        cout << "Name            : " << name << endl;
        cout << "Mobile          : " << mobile << endl;
        cout << "Percentage      : " << percentage << "%" << endl;
        cout << "Branch          : " << branch << endl;
    }

    void mentor() const
    {
        cout << "\n-----------------------------\n";
        cout << "Roll Number     : " << rollNumber << endl;
        cout << "Name            : " << name << endl;
        cout << "Mobile          : " << mobile << endl;
    }

    void updateData()
    {
        cout << "\n===== UPDATE STUDENT =====\n";

        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        cout << "Enter New Name: ";
        getline(cin, name);

        cout << "Enter New City: ";
        getline(cin, city);

        cout << "Enter New Age: ";
        while (!(cin >> age)) {
            cout << "Invalid input! Please enter a number: ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }

        cout << "Enter New Mobile: ";
        cin >> mobile;

        cout << "Enter New Percentage: ";
        while (!(cin >> percentage)) {
            cout << "Invalid input! Please enter a valid percentage: ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }

        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        cout << "Enter New Branch: ";
        getline(cin, branch);

        cout << "\nStudent updated successfully!\n";
    }

    void saveToFile(ofstream& file) const
    {
        file << rollNumber << "|"
             << name << "|"
             << city << "|"
             << age << "|"
             << mobile << "|"
             << percentage << "|"
             << branch << endl;
    }

    bool loadFromFile(const string& line)
    {
        string temp;
        stringstream ss(line);

        if (!getline(ss, temp, '|')) return false;
        stringstream rollStream(temp);
        if (!(rollStream >> rollNumber)) return false;

        if (!getline(ss, name, '|')) return false;
        if (!getline(ss, city, '|')) return false;

        if (!getline(ss, temp, '|')) return false;
        stringstream ageStream(temp);
        if (!(ageStream >> age)) return false;

        if (!getline(ss, mobile, '|')) return false;

        if (!getline(ss, temp, '|')) return false;
        stringstream percentageStream(temp);
        if (!(percentageStream >> percentage)) return false;

        if (!getline(ss, branch)) return false;

        return true;
    }
};

// ======================================================
// SAVE & LOAD STUDENTS
// ======================================================

void saveAllStudents(const vector<Student>& students)
{
    ofstream file("students.txt");
    if (!file)
    {
        cout << "\nError: students.txt could not be opened!\n";
        return;
    }
    for (const Student& student : students)
    {
        student.saveToFile(file);
    }
    file.close();
}

void loadAllStudents(vector<Student>& students)
{
    ifstream file("students.txt");
    if (!file) return;

    string line;
    while (getline(file, line))
    {
        if (line.empty()) continue;
        Student student;
        if (student.loadFromFile(line))
        {
            students.push_back(student);
        }
    }
    file.close();
}

// ======================================================
// AUTHENTICATION SYSTEM
// ======================================================

bool authenticate(const string& correctPassword)
{
    string inputPassword;
    cout << "\nEnter Password: ";
    cin >> inputPassword;

    if (inputPassword == correctPassword)
    {
        cout << "Access Granted.\n";
        return true;
    }
    else
    {
        cout << "Access Denied: Incorrect Password!\n";
        return false;
    }
}

// ======================================================
// ADMIN MENU
// ======================================================

void adminMenu(vector<Student>& students)
{
    int choice;

    do
    {
        cout << "\n================================\n";
        cout << "           ADMIN MENU\n";
        cout << "================================\n";
        cout << "1. Add Student\n";
        cout << "2. Display All Students\n";
        cout << "3. Search Student\n";
        cout << "4. Update Student\n";
        cout << "5. Delete Student\n";
        cout << "6. Exit\n";
        cout << "\nEnter Choice: ";
        
        // Input validation to prevent infinite loop on string input
        while (!(cin >> choice)) {
            cout << "Invalid input! Please enter a number from 1 to 6: ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }

        if (choice == 1) // ADD
        {
            Student student;
            student.inputData();
            bool duplicate = false;

            for (const Student& existing : students)
            {
                if (existing.getRollNumber() == student.getRollNumber())
                {
                    duplicate = true;
                    break;
                }
            }

            if (duplicate) {
                cout << "\nRoll Number already exists!\n";
            } else {
                students.push_back(student);
                saveAllStudents(students);
                cout << "\nStudent added successfully!\n";
            }
        }
        else if (choice == 2) // DISPLAY ALL
        {
            if (students.empty()) {
                cout << "\nNo student records found!\n";
            } else {
                cout << "\n===== ALL STUDENTS =====\n";
                for (const Student& student : students) {
                    student.displayAll();
                }
            }
        }
        else if (choice == 3) // SEARCH
        {
            int roll;
            bool found = false;
            cout << "\nEnter Roll Number: ";
            cin >> roll;

            for (const Student& student : students)
            {
                if (student.getRollNumber() == roll) {
                    student.displayAll();
                    found = true;
                    break;
                }
            }
            if (!found) cout << "\nStudent not found!\n";
        }
        else if (choice == 4) // UPDATE
        {
            int roll;
            bool found = false;
            cout << "\nEnter Roll Number: ";
            cin >> roll;

            for (Student& student : students)
            {
                if (student.getRollNumber() == roll) {
                    student.updateData();
                    saveAllStudents(students);
                    found = true;
                    break;
                }
            }
            if (!found) cout << "\nStudent not found!\n";
        }
        else if (choice == 5) // DELETE
        {
            int roll;
            bool found = false;
            cout << "\nEnter Roll Number: ";
            cin >> roll;

            for (auto it = students.begin(); it != students.end(); ++it)
            {
                if (it->getRollNumber() == roll) {
                    students.erase(it);
                    saveAllStudents(students);
                    cout << "\nStudent deleted successfully!\n";
                    found = true;
                    break;
                }
            }
            if (!found) cout << "\nStudent not found!\n";
        }
        else if (choice == 6) // EXIT
        {
            cout << "\nExiting Admin Panel...\n";
        }
        else
        {
            cout << "\nInvalid choice! Please try again.\n";
        }

    } while (choice != 6);
}

// ======================================================
// MAIN FUNCTION
// ======================================================

int main()
{
    vector<Student> students;
    loadAllStudents(students);
    string department;

    cout << "\n========================================\n";
    cout << "       ARYA COLLEGE STUDENT SYSTEM\n";
    cout << "========================================\n";
    cout << "\nEnter Department (admin, principal, hod, mentor): ";
    cin >> department;

    // Convert to lowercase
    for (char& c : department)
    {
        c = static_cast<char>(tolower(static_cast<unsigned char>(c)));
    }

    // ADMIN (Password: admin123)
    if (department == "admin")
    {
        if (authenticate("admin123")) 
        {
            adminMenu(students);
        }
    }
    // PRINCIPAL (Password: boss123)
    else if (department == "principal")
    {
        if (authenticate("boss123")) 
        {
            if (students.empty()) {
                cout << "\nNo student records available!\n";
            } else {
                cout << "\n===== PRINCIPAL VIEW =====\n";
                for (const Student& student : students) {
                    student.principal();
                }
            }
        }
    }
    // HOD (No Password)
    else if (department == "hod")
    {
        if (students.empty()) {
            cout << "\nNo student records available!\n";
        } else {
            cout << "\n===== HOD VIEW =====\n";
            for (const Student& student : students) {
                student.hod();
            }
        }
    }
    // MENTOR (No Password)
    else if (department == "mentor")
    {
        if (students.empty()) {
            cout << "\nNo student records available!\n";
        } else {
            cout << "\n===== MENTOR VIEW =====\n";
            for (const Student& student : students) {
                student.mentor();
            }
        }
    }
    else
    {
        cout << "\nInvalid Department!\n";
        cout << "Please enter: admin, principal, hod or mentor.\n";
    }

    return 0;
}