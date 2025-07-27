/*
TASK 1: CGPA Calculator
--> This program calculates the GPA for each semester and the overall CGPA for a student.
--> It takes letter grades and credit hours for each course, computes grade points and displays results in a formatted table.
*/

#include <iostream>
#include <iomanip>
using namespace std;

// Converts a letter grade to its corresponding grade point value
double letterGradeToPoint(const string &grade)
{
    if (grade == "A")
        return 4.00;
    if (grade == "B+")
        return 3.50;
    if (grade == "B")
        return 3.00;
    if (grade == "C+")
        return 2.50;
    if (grade == "C")
        return 2.00;
    if (grade == "D+")
        return 1.50;
    if (grade == "D")
        return 1.00;
    if (grade == "F")
        return 0.00;
    return -1.0; // Invalid grade
}

int main()
{
    int semesters;
    cout << "Enter the number of semesters: "; // Prompt for number of semesters
    cin >> semesters;
    double overallGradePoints = 0.0, overallCredits = 0.0;
    for (int sem = 1; sem <= semesters; ++sem)
    {
        cout << "\n--- Semester " << sem << " ---\n";
        int n;
        cout << "Enter the number of courses in semester " << sem << ": "; // Prompt for courses in this semester
        cin >> n;
        double totalGradePoints = 0.0, totalCredits = 0.0;
        string *grades = new string[n];      // Array to store letter grades
        double *credits = new double[n];     // Array to store credit hours
        double *gradePoints = new double[n]; // Array to store grade points
        cout << "Enter letter grade (A, B+, B, C+, etc.) and credit hours for each course:\n";
        for (int i = 0; i < n; ++i)
        {
            cout << "Course " << (i + 1) << " - Letter Grade: ";
            cin >> grades[i];                               // Input letter grade
            gradePoints[i] = letterGradeToPoint(grades[i]); // Convert to grade point
            while (gradePoints[i] < 0)
            {
                cout << "Invalid grade. Please enter a valid letter grade (A, B+, B, C+, etc.): ";
                cin >> grades[i];
                gradePoints[i] = letterGradeToPoint(grades[i]);
            }
            cout << "Course " << (i + 1) << " - Credit Hours: ";
            cin >> credits[i];                               // Input credit hours
            totalGradePoints += gradePoints[i] * credits[i]; // Accumulate grade points
            totalCredits += credits[i];                      // Accumulate credits
        }
        double gpa = (totalCredits > 0) ? (totalGradePoints / totalCredits) : 0.0; // GPA for this semester
        // Print semester summary table
        cout << "\n=====================================\n";
        cout << "|    Semester " << sem << " - Course Summary    |" << endl;
        cout << "|===================================|\n";
        cout << "| Course | Grade | Points | Credits |" << endl;
        cout << "|-----------------------------------|" << endl;
        for (int i = 0; i < n; ++i)
        {
            cout << "| " << setw(6) << left << (i + 1)
                 << " | " << setw(5) << left << grades[i]
                 << " | " << setw(6) << left << fixed << setprecision(2) << gradePoints[i]
                 << " | " << setw(7) << left << credits[i] << " |" << endl;
        }
        cout << "|-----------------------------------|" << endl;
        cout << "| Total Credits:      " << setw(7) << left << totalCredits << "       |" << endl;
        cout << "| Total Grade Points: " << setw(7) << left << totalGradePoints << "       |" << endl;
        cout << "| GPA for Semester:   " << setw(7) << left << gpa << "       |" << endl;
        cout << "=====================================\n";
        overallGradePoints += totalGradePoints; // Add to overall totals
        overallCredits += totalCredits;
        delete[] grades;
        delete[] credits;
        delete[] gradePoints;
    }
    // Calculate and print overall CGPA summary
    double cgpa = (overallCredits > 0) ? (overallGradePoints / overallCredits) : 0.0;
    cout << "\n======================================\n";
    cout << "|           CGPA SUMMARY             |" << endl;
    cout << "======================================\n";
    cout << "| Overall Total Credits:      " << setw(7) << left << overallCredits << "|" << endl;
    cout << "| Overall Total Grade Points: " << setw(7) << left << overallGradePoints << "|" << endl;
    cout << "| Final CGPA (All Semesters): " << setw(7) << left << fixed << setprecision(2) << cgpa << "|" << endl;
    cout << "======================================\n";
    return 0;
}