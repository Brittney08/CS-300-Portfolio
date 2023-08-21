//============================================================================
// Name      : CS.cpp
// Author      : BrittneyMiller
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <fstream>
#include "CSVparser.hpp"

using namespace std;

/* Courses contains ID, Name, Vector and prereqs
 *
 */
struct Course {
	string courseNum;
	string courseName;
	vector<string> preReqs;

	course() {}
};

/*
 * Binary search tree contains course infor and left and right children all in nodes
 * constructors allow nodes iwth or without defined courses
 */
struct Node {
	Course course;
	Node* left;
	Node* right;

	Node() {
		left = nullptr;
		right = nullptr;
	}
	Node(Course aCourse) : Node() {
		this->course = aCourse;
	}
};

class CourseBST {
private:
	Node* root;

	void addNode(Node* node, Course course);
	void printSampleSchedule(Node* node);
	void printCourseInformation(Node* node, string courseNum);

public:
	CourseBST();
	virtual ~CourseBST();
	void DeleteRecursive(Node* node);
	void Insert(Course course);
	int NumPrerequisiteCourses(Course course);
	void PrintSampleSchedule();
	void PrintCourseInformation(string courseNum);
};

/*
 * Construct binary seart tree and root
 */

CourseBST::CourseBST() {
	root = nullptr;
}

/*
 * Deconstruct binary search tree passing root to function that recursively deletes nodes
 */
CourseBST::~CourseBST(){
	DeleteRecursive(root);
	}

/*
 * Delete binary search tree nodes recursively using deconstructor
 */
void CourseBST::DeleteRecursive(Node* node) {
	if (node) {
		DeleteRecursive(node->left);
		DeleteRecursive(node->right);
		delete node;
	}
}

/*
 * Inserting node into Binary search tree
 */

void CourseBST::Insert(Course course) {
	if (root == nullptr)
		root = new Node(course);
	else
		this->addNode(root, course);

}

/*
 * return number of prereqs
 *
 */

int CourseBST::NumPrerequisiteCourses(Course course) {
	int count = 0;
	for (unsigned int i = 0; i < course.preReqs.size(); i++) {
		if (course.preReqs.at(i).length() > 0)
			count++;

	}
	return count;
}

/*
 * pass the binary search tree root to private method
 */

void CourseBST::PrintSampleSchedule() {
	this->printSampleSchedule(root);
}
/*
 * pass binary search tree root and course ID to be search and dipslayed if found
 */
void CourseBST::PrintCourseInformation(string courseNum) {
	this->printCourseInformation(root, courseNum);
}
/*Use public insert method
 * Determin and set alphanumeric location
 * if current child is nullptr
 * add node to search tree
 * keep looking until nullptr is found
 */
void CourseBST::addNode(Node* node, Course course) {
	if (node->course.courseNum.compare(course.courseNum) > 0) {
		if (node->left == nullptr)
			node->left = new Node(course);
		else
			this->addNode(node->left, course);
	}
	else {
		if (node->right == nullptr)
			node->right = new Node(course);
		else
			this->addNode(node->right, course);
	}
}
/*
 * Recursively prints loaded course in order
 */
void CourseBST::printSampleSchedule(Node* node) {
	if (node != nullptr) {
		printSampleSchedule(node->left);
		cout << node->course.courseNum << ", " << node->course.courseName << endl;
		printSampleSchedule(node->right);
	}
	return;
}
/*
 * Display single course info including prereqs
 */
void CourseBST::printCourseInformation(Node* curr, string courseNum) {
	while (curr != nullptr) {
		if (curr->course.courseNum.compare(courseNum) == 0) {
			cout << endl << curr->course.courseNum << ", " << curr->course.courseName << endl;
			unsigned int size = NumPrerequisiteCourses(curr->course);
			cout << "Prerequisite(s): ";
			unsigned int i = 0;
			for (i = 0; i < size; i++) {
				cout << curr->course.preReqs.at(i);
				if (i != size - 1)
					cout << ", ";
			}
			if (i == 0)
				cout << "No prerequisites required.";
			cout << endl;
			return;
		}
		else if (courseNum.compare(curr->course.courseNum) < 0)
			curr = curr->left;
		else
			curr = curr->right;
	}
	cout << "Course " << courseNum << " not found." << endl;
}
bool loadCourses(string csvPath, CourseBST* coursesBST) {
	try {
		ifstream courseFile(csvPath);
		if (courseFile.is_open()) {
			while (!courseFile.eof()) {
				vector<string> courseInfo;
				string courseData;

				getline(courseFile, courseData);
				while (courseData.length() > 0) {
					unsigned int comma = courseData.find(',');
					if (comma < 100) {
						courseInfo.push_back(courseData.substr(0, comma));
						courseData.erase(0, comma + 1);
					}
					else {
						courseInfo.push_back(courseData.substr(0, courseData.length()));
						courseData = "";
					}
				}
				Course course;
				course.courseNum = courseInfo[0];
				course.courseName = courseInfo[1];
				for (unsigned int i = 2; i < courseInfo.size(); i++) {
					course.preReqs.puch_back(courseInfo[i]);
				}
				coursesBST->Insert(course);
			}
			courseFile.close();
			return true;
		}
	}
	catch (csv::Error& e) {
		cerr << e.what() << endl;
	}
	return false;
}
int main(int argc, char* argv[]) {
	string csvPath, courseId;
	switch (argc) {
	case 2:
		csvPath = argv[1];
		break;
	case 3:
		csvPath = argv[1];
		courseId  = argv[2];
		break;
	default:
		csvPath = "";
		break;

	}
	CourseBST* coursesBST = nullptr;
	cout << "\nWelcome to the course planner!\n" << endl;

	string choice = "0";
	int userChoice = choice[0] - '0';
	while (userChoice != 9) {

		cout << "    1. Load Data Structure" << endl;
		cout << "    2. Print Course List" << endl;
		cout << "    3. Print Course" << endl;
		cout << "    9. Exit" << endl;
		cout << "\nWhat would yhou like to do? ";
		cin >> choice;

		if (choice.length() == 1)
			userChoice = choice[0] - '0';
		else
			userChoice = 0;

		bool success = 1;

		switch (userChoice) {

		case 1:

			if (coursesBST == nullptr)
				coursesBST = new CourseBST();
			if (csvPath.length() ==0) {
				cout << "Enter the file nme that contains the course data: ";
				cin >> csvPath;
			}
			success = loadCourses(csvPath, coursesBST);
			if (success)
				cout << "File not found.\n" << endl;
			csvPath = "";
			break;

		case 2:

			if (coursesBST != nullptr && success) {
				cout << "Here is a sample schedule: \n" << endl;
				coursesBST->PrintSampleSchedule();
				cout << endl;
			}
			else
				cout << "Load courses first - option 1\n" << endl;
			break;

		case 3:

			if (coursesBST != nullptr && success) {
				if (courseId.length() == 0) {
					cout << "What course do you want to know about? ";
					cin >> courseId;
					for (auto& userChoice : courseId) userChoice = toupper(userChoice);
				}
				coursesBST->PrintCourseInformation(courseId);
				cout << endl;
			}
			else
				cout << "Load courses first - option 1\n" << endl;
			courseId = "";
			break;

		default:
			if (userChoice != 9)
				cout << choice << " is not a valid option\n" << endl;
			break;
		}
	}
	cout << "\nThank you for using the course planner!" << endl;

	return 0;
}
