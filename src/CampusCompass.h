#pragma once
#include <string>
#include <map>
#include <vector>
#include <unordered_map>
using namespace std;

struct Student {
    string name;
    string id;
    int residence;
    vector<string> classes;
    Student(string n, string i, int r, vector<string> c) {
        name = n;
        id = i;
        residence = r;
        classes = c;
    }
};

struct edgeData {
    int weight;
    bool isOpen;
    edgeData();
    edgeData(int w) {
        weight = w;
        isOpen = true;
    }
};

class CampusCompass {
private:
    // Think about what member variables you need to initialize
    // perhaps some graph representation?
    unordered_map<int, vector<pair<int, edgeData>>> adjList; // maps location start to pairs with 1. location end, 2. time/openness
    unordered_map<string, int> classCodes;
    unordered_map<int, string> classIDs;
    unordered_map<string, Student> students;
    unordered_map<string, vector<string>> classStudent;
public:
    // Think about what helper functions you will need in the algorithm
    CampusCompass(); // constructor
    bool ParseCSV(const string &edges_filepath, const string &classes_filepath);
    void executeValidCommand(string command, vector<string>& testing);
    bool validCommandHelper(string command, vector<string>& commands, vector<string>& testing);
    void executeCommandHelper(vector<string>& commands, vector<string>& testing);
    bool validID(string id);
    bool validName(string name);
    bool validNumber(string num);
    string formattedName(string name);
    bool insert(string name, string id, int residence, vector<string> classes, vector<string>& testing);
    bool remove(string id, vector<string>& testing);
    bool dropClass(string id, string classCode, vector<string>& testing);
    bool replaceClass(string id, string classCode1, string classCode2, vector<string>& testing);
    int removeClass(string classCode, vector<string>& testing);
    bool toggleEdgesClosure(vector<int> edgesToClose, vector<string>& testing);
    int checkEdgeStatus(int location1, int location2, vector<string>& testing);
    bool isConnected(int location1, int location2, vector<string>& testing);
    vector<int> printShortestEdges(string id);
};
