#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <unordered_map>

using namespace std;

//building course structure 
struct Course {
	string courseName;
	string courseNum;
	vector <string> prereqs;
};

//function to load the csv file
void LoadCourses_Hash(const string& csvFile, unordered_map<string, Course>& courses) {

	ifstream file(csvFile);

	//error message if file does not open
	if (!file.is_open()) {
		cout << "Error cannot open file." << endl;
		return;
	}

	string fileLine;

	//loop to read through the file until the end
	while (getline(file, fileLine)) {
		stringstream ss(fileLine);
		string section;
		vector <string> parts;

		//loop to split line by commas into sections
		while (getline(ss, section, ',')) {
			parts.push_back(section);
		}

		//making sure there is at least a course number and name on the line
		if (parts.size() < 2) {
			cout << "Missing course information." << endl;
			continue;
		}

		//Creating a new course object
		Course c;
		c.courseNum = parts[0];
		c.courseName = parts[1];

		//if there is info after the second comma add it to prereqs
		if (parts.size() > 2) {
			for (size_t i = 2; i < parts.size(); i++) {
				c.prereqs.push_back(parts[i]);
			}
		}

		//insert course object into hash table
		courses[c.courseNum] = c;
	}
	
	file.close();
}

//function to print one input course's info
void PrintCourse_Hash(const unordered_map<string, Course>& courses, const string& courseNum) {

	//to look up a course in the hash table using iterator
	auto it = courses.find(courseNum);

	//error if course is not found
	if (it == courses.end()) {
		cout << "Error: course not found." << endl;
		return;
	}

	const Course& c = it->second;

	//Prints course number and course name
	cout << c.courseNum << ", " << c.courseName << endl;

	//Print prereq if appropriate
	if (!c.prereqs.empty()) {
		cout << "Prerequisites: ";

		for (const string& p : c.prereqs) {
			cout << p;
			//also printing the title for the prereq
			if (courses.find(p) != courses.end()) {
				cout << " (" << courses.at(p).courseName << ")";
			}

			cout << " ";
		}

		cout << endl;
	}

}

//function to print all courses in alphanumeric order
void PrintAllCourses_Hash(const unordered_map<string, Course>& courses) {

	vector<string> keys;

	//extract all course numbers for sorting
	for (const auto& pair : courses) {
		keys.push_back(pair.first);
	}

	//sort course numbers
	sort(keys.begin(), keys.end());

	//Print each course in order
	for (const string& key : keys) {
		const Course& c = courses.at(key);

		cout << c.courseNum << ", " << c.courseName << endl;

		if (!c.prereqs.empty()) {
			cout << "Prerequisites: ";

			for (const string& p : c.prereqs) {
				cout << p;
				//also printing the title for the prereq
				if (courses.find(p) != courses.end()) {
					cout << " (" << courses.at(p).courseName << ")";
				}

				cout << " ";
			}

			cout << endl;
		}

	}
}

//main program contains a menu that calls to the functions
int main() {
	
	//hash table that contains course info
	unordered_map<string, Course> courses;

	//variable for user input into menu
	int inputChoice = 0;

	//loop to repeat unless user chooses to stop
	while (inputChoice != 9) {

		//Print menu
		cout << "\n1. Load file." << endl;
		cout << "2. Print all courses." << endl;
		cout << "3. Print chosen course." << endl;
		cout << "9. Exit program." << endl;
		cin >> inputChoice;

		//switch case for user input
		switch (inputChoice) {

		//Load csv file
		case 1: {
			string filename;
			cout << "Enter file name: ";
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			getline(cin, filename);
			LoadCourses_Hash(filename, courses);
			break;
		}

		//Print all courses in order
		case 2: {
			PrintAllCourses_Hash(courses);
			break;
		}

		//Print chosen input course info
		case 3: {
			string courseNum;
			cout << "Enter course number: ";
			cin >> courseNum;
			//Converts input course number to upper case 
			transform(courseNum.begin(), courseNum.end(), courseNum.begin(), ::toupper);
			PrintCourse_Hash(courses, courseNum);
			break;
			}

		//Leaving the program
		case 9:
			cout << "Goodbye." << endl;
			break;

		//Input validation
		default:
			cout << "Invalid input." << endl;
			break;

		}
	}
	return 0;
}