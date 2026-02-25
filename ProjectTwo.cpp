#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <limits>

using namespace std;

// Represents one course
struct Course {
    string courseId;
    string title;
    vector<string> prerequisites;
};

// Removes extra spaces from beginning and end
string trim(const string& str) {
    size_t first = str.find_first_not_of(" \t\r\n");
    if (first == string::npos) return "";
    size_t last = str.find_last_not_of(" \t\r\n");
    return str.substr(first, (last - first + 1));
}

// Converts text to uppercase
string toUpper(string str) {
    transform(str.begin(), str.end(), str.begin(),
        [](unsigned char c) { return toupper(c); });
    return str;
}

// Splits CSV line by comma
vector<string> splitLine(const string& line) {
    vector<string> tokens;
    string token;
    stringstream ss(line);

    while (getline(ss, token, ',')) {
        tokens.push_back(trim(token));
    }

    return tokens;
}

// Loads courses into the map
bool loadCourses(const string& fileName, map<string, Course>& courseMap) {
    ifstream file(fileName);

    if (!file.is_open()) {
        cout << "Error: Could not open file." << endl;
        return false;
    }

    courseMap.clear();
    string line;

    while (getline(file, line)) {
        vector<string> fields = splitLine(line);

        if (fields.size() < 2)
            continue;

        Course course;
        course.courseId = toUpper(fields[0]);
        course.title = fields[1];

        for (size_t i = 2; i < fields.size(); i++) {
            course.prerequisites.push_back(toUpper(fields[i]));
        }

        courseMap[course.courseId] = course;
    }

    file.close();
    return true;
}

// Prints all courses in order
void printCourseList(const map<string, Course>& courseMap) {
    cout << "Here is a sample schedule:" << endl;

    for (const auto& pair : courseMap) {
        cout << pair.second.courseId << ", " << pair.second.title << endl;
    }
}

// Prints one course
void printCourse(const map<string, Course>& courseMap, const string& courseIdInput) {
    string courseId = toUpper(courseIdInput);
    auto it = courseMap.find(courseId);

    if (it == courseMap.end()) {
        cout << "Course not found." << endl;
        return;
    }

    const Course& course = it->second;

    cout << course.courseId << ", " << course.title << endl;

    if (course.prerequisites.empty()) {
        cout << "Prerequisites: None" << endl;
        return;
    }

    cout << "Prerequisites: ";

    for (size_t i = 0; i < course.prerequisites.size(); i++) {
        cout << course.prerequisites[i];
        if (i < course.prerequisites.size() - 1)
            cout << ", ";
    }

    cout << endl;
}

// Displays menu
void displayMenu() {
    cout << endl;
    cout << "1. Load Data Structure." << endl;
    cout << "2. Print Course List." << endl;
    cout << "3. Print Course." << endl;
    cout << "9. Exit" << endl;
    cout << "What would you like to do? ";
}

int main() {

    map<string, Course> courseMap;
    bool dataLoaded = false;
    int choice = 0;

    cout << "Welcome to the course planner." << endl;

    while (choice != 9) {

        displayMenu();

        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input." << endl;
            continue;
        }

        switch (choice) {

        case 1: {
            cout << "Enter file name: ";
            string fileName;

            // Read entire line including spaces
            getline(cin >> ws, fileName);

            // Remove quotes if user pasted path with quotes
            if (!fileName.empty() && fileName.front() == '"' && fileName.back() == '"') {
                fileName = fileName.substr(1, fileName.size() - 2);
            }

            dataLoaded = loadCourses(fileName, courseMap);

            if (dataLoaded)
                cout << "Data loaded successfully." << endl;

            break;
        }

        case 2:
            if (!dataLoaded) {
                cout << "Please load data first." << endl;
            }
            else {
                printCourseList(courseMap);
            }
            break;

        case 3:
            if (!dataLoaded) {
                cout << "Please load data first." << endl;
            }
            else {
                cout << "What course do you want to know about? ";
                string courseId;
                cin >> courseId;
                printCourse(courseMap, courseId);
            }
            break;

        case 9:
            cout << "Thank you for using the course planner!" << endl;
            break;

        default:
            cout << choice << " is not a valid option." << endl;
            break;
        }
    }

    return 0;
}
