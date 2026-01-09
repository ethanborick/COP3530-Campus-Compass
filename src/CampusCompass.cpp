#include "CampusCompass.h"
#include <fstream>
#include <string>
#include <sstream>
#include <iostream>
#include <vector>
#include <unordered_map>
#include <queue>
#include <unordered_set>
#include <algorithm>
#include <stack>

using namespace std;

CampusCompass::CampusCompass() {}

bool CampusCompass::ParseCSV(const string &edges_filepath, const string &classes_filepath) {
    ifstream edges(edges_filepath);
    ifstream classes(classes_filepath);
    string line;
    getline(edges, line);
    while (getline(edges, line)) {
        istringstream stream(line);
        string id1, id2, name1, name2, time;
        getline(stream, id1, ',');
        getline(stream, id2, ',');
        getline(stream, name1, ',');
        getline(stream, name2, ',');
        getline(stream, time, ',');
        edgeData ed(stoi(time));
        adjList[stoi(id1)].push_back({stoi(id2), ed});
        adjList[stoi(id2)].push_back({stoi(id1), ed});

    }
    getline(classes, line);
    while (getline(classes, line)) {
        istringstream stream(line);
        string code, id, start, end;
        getline(stream, code, ',');
        getline(stream, id, ',');
        getline(stream, start, ',');
        getline(stream, end, ',');
        classCodes[code] = stoi(id);
        classIDs[stoi(id)] = code;
    }
    return true;
}

bool CampusCompass::validName(string name) { // from project 1
    if (name[0] != '"' || name[name.length() - 1] != '"') { // make sure name is in quotes
        return false;
    }
    for (int i = 1; i < name.length() - 1; i++) {
        if (!isalpha(name[i]) && name[i] != ' ') { // make sure all characters are letters or spaces
            return false;
        }
    }
    return true;
}

bool CampusCompass::validID(string id) { // from project 1
    if (id.length() != 8) { // make sure id is of length 8
        return false;
    }
    for (int i = 0; i < id.length(); i++) {
        if (!isdigit(id[i])) { // make sure all characters are numbers
            return false;
        }
    }
    return true;
}

bool CampusCompass::validNumber(string num) { // from project 1
    for (int i = 0; i < num.length(); i++) {
        if (!isdigit(num[i])) {
            return false;
        }
    }
    return true;
}

string CampusCompass::formattedName(string name) { // from project 1
    string formatted = "";
    for (int i = 1; i < name.length() - 1; i++) { // only take characters inside quotes
        formatted += name[i];
    }
    return formatted;
}

bool CampusCompass::validCommandHelper(string command, vector<string>& commands, vector<string>& testing) { // from project 1
    commands.clear(); // make sure commands vector is empty
    istringstream stream(command); // create an istringstream to go through each token
    string token;
    bool separated_name = false;
    while (stream >> token) { // while there is still more characters to read
        if (token[0] == '"' &&
            !(token[token.length() - 1] == '"')) { // check if token is the beginning of a separated name
            commands.push_back(token);
            separated_name = true;
        } else if (token[token.length() - 1] == '"' &&
                   !(token[0] == '"')) { // check if token is the end of a separated name
            commands[commands.size() - 1] += " " + token;
            separated_name = false;
        } else if (separated_name) { // check if token is in the middle of a separated name
            commands[commands.size() - 1] += " " + token;
        } else {
            commands.push_back(token);
        }
    }
    if (commands[0] == "insert") {
        if (validName(commands[1]) && validID(commands[2]) && 1 <= stoi(commands[3]) && stoi(commands[3]) <= 56) {
            if (validNumber(commands[4]) && commands.size() == 5 + stoi(commands[4]) && (1 <= stoi(commands[4])) && (stoi(commands[4]) <= 6)) {
                for (int i = 5; i < 5 + stoi(commands[4]); i++) {
                    if (classCodes.count(commands[i]) == 0) {
                        cout << "unsuccessful" << endl;
                        testing.push_back("unsuccessful");
                        return false;
                    }
                }
                return true;
            }
        }
    }
    else if (commands[0] == "remove") {
        if (commands.size() == 2 && validID(commands[1])) {
            return true;
        }
    }
    else if (commands[0] == "dropClass") {
        if (commands.size() == 3 && validID(commands[1]) && classCodes.count(commands[2]) != 0) {
            return true;
        }
    }
    else if (commands[0] == "replaceClass") {
        if (commands.size() == 4 && validID(commands[1]) && classCodes.count(commands[2]) != 0 && classCodes.count(commands[3]) != 0) {
            return true;
        }
    }
    else if (commands[0] == "removeClass") {
        if (commands.size() == 2) {
            return true;
        }
    }
    else if (commands[0] == "toggleEdgesClosure") {
        if (validNumber(commands[1]) && commands.size() == 2 + 2 * stoi(commands[1])) {
            return true;
        }
    }
    else if (commands[0] == "checkEdgeStatus") {
        if (commands.size() == 3 && validNumber(commands[1]) && validNumber(commands[2])) {
            return true;
        }
    }
    else if (commands[0] == "isConnected") {
        if (commands.size() == 3 && validNumber(commands[1]) && validNumber(commands[2])) {
            return true;
        }
    }
    else if (commands[0] == "printShortestEdges") {
        if (commands.size() == 2 && validID(commands[1])) {
            return true;
        }
    }
    else if (commands[0] == "printStudentZone") {
        if (commands.size() == 2 && validID(commands[1])) {
            return true;
        }
    }
    cout << "unsuccessful" << endl;
    testing.push_back("unsuccessful");
    return false;
}

void CampusCompass::executeCommandHelper(vector<string>& commands, vector<string>& testing) { // from project 1
    if (commands[0] == "insert") {
        vector<string> c;
        for (int i = 5; i < 5 + stoi(commands[4]); i++) {
            c.push_back(commands[i]);
        }
        insert(formattedName(commands[1]), commands[2], stoi(commands[3]), c, testing);
    }
    if (commands[0] == "remove") {
        remove(commands[1], testing);
    }
    if (commands[0] == "dropClass") {
        dropClass(commands[1], commands[2], testing);
    }
    if (commands[0] == "replaceClass") {
        replaceClass(commands[1], commands[2], commands[3], testing);
    }
    if (commands[0] == "removeClass") {
        removeClass(commands[1], testing);
    }
    if (commands[0] == "toggleEdgesClosure") {
        vector<int> edges;
        for (int i = 2; i < 2+ 2 * stoi(commands[1]); i++) {
            edges.push_back(stoi(commands[i]));
        }
        toggleEdgesClosure(edges, testing);
    }
    if (commands[0] == "checkEdgeStatus") {
        checkEdgeStatus(stoi(commands[1]), stoi(commands[2]), testing);
    }
    if (commands[0] == "isConnected") {
        isConnected(stoi(commands[1]), stoi(commands[2]), testing);
    }
    if (commands[0] == "printShortestEdges") {
        printShortestEdges(commands[1]);
    }
    if (commands[0] == "printStudentZone") {
        printStudentZone(commands[1]);
    }
}

void CampusCompass::executeValidCommand(string command, vector<string>& testing) { // from project 1
    vector<string> commands;
    if (validCommandHelper(command, commands, testing)) {
        executeCommandHelper(commands, testing);
    }
}

bool CampusCompass::insert(string name, string id, int residence, vector<string> classes, vector<string>& testing) {
    if (students.count(id) != 0) { // if id isn't unique
        cout << "unsuccessful" << endl;
        testing.push_back("unsuccessful");

        return false;
    }
    Student s(name, id, residence, classes); // create new student object
    students.emplace(id, s);
    for (int i = 0; i < classes.size(); i++) { // add student to each class
        classStudent[classes[i]].push_back(id);
    }
    cout << "successful" << endl;
    testing.push_back("successful");
    return true;
}

bool CampusCompass::remove(string id, vector<string>& testing) {
    if (students.count(id) == 0) { // if student id doesn't exist
        cout << "unsuccessful" << endl;
        testing.push_back("unsuccessful");
        return false;
    }
    for (auto it = classStudent.begin(); it != classStudent.end(); it++) { // loop through map and remove student from each class they are in
        for (int i = 0; i < it->second.size(); i++) {
            if (it->second[i] == id) {
                it->second.erase(it->second.begin() + i);
                break;
            }
        }
    }
    students.erase(id);
    cout << "successful" << endl;
    testing.push_back("successful");
    return true;
}

bool CampusCompass::dropClass(string id, string classCode, vector<string>& testing) {
    if (students.count(id) == 0) { // if student id doesn't exist
        cout << "unsuccessful" << endl;
        testing.push_back("unsuccessful");
        return false;
    }
    bool found = false;
    for (int i = 0; i < classStudent[classCode].size(); i++) { // check if they have the class
        if (classStudent[classCode][i] == id) {
            found = true;
        }
    }
    if (!found) { // if they don't have the class
        cout << "unsuccessful" << endl;
        testing.push_back("unsuccessful");
        return false;
    }
    bool studentHasOneClass = false;
    if (students.at(id).classes.size() == 1) studentHasOneClass = true;
    for (int i = 0; i < students.at(id).classes.size(); i++) { // remove class from student object
        if (students.at(id).classes[i] == classCode) {
            students.at(id).classes.erase(students.at(id).classes.begin() + i);
            break;
        }
    }
    for (int i = 0; i < classStudent[classCode].size(); i++) { // remove student from class in map
        if (classStudent[classCode][i] == id) {
            classStudent[classCode].erase(classStudent[classCode].begin() + i);
            break;
        }
    }
    if (studentHasOneClass) { // remove the student if they only had one class
        students.erase(id);
    }
    cout << "successful" << endl;
    testing.push_back("successful");
    return true;
}

bool CampusCompass::replaceClass(string id, string classCode1, string classCode2, vector<string>& testing) {
    if (students.count(id) == 0) { // if student id doesn't exist
        cout << "unsuccessful" << endl;
        testing.push_back("unsuccessful");
        return false;
    }

    vector<string> s1 = classStudent[classCode1];
    vector<string> s2 = classStudent[classCode2];
    bool foundClass1 = false;
    for (int i = 0; i < s1.size(); i++) { // check if they have the first class
        if (s1[i] == id) {
            foundClass1 = true;
        }
    }
    if (!foundClass1) { // if they don't have the first class
        cout << "unsuccessful" << endl;
        testing.push_back("unsuccessful");
        return false;
    }
    for (int i = 0; i < s2.size(); i++) { // make sure they don't have the second class
        if (s2[i] == id) {
            cout << "unsuccessful" << endl;
            testing.push_back("unsuccessful");
            return false;
        }
    }
    students.at(id).classes.push_back(classCode2);
    classStudent[classCode2].push_back(id);
    return dropClass(id, classCode1, testing);
}

int CampusCompass::removeClass(string classCode, vector<string> &testing) {
    int studentsDropped = 0;
    for (auto it = students.begin(); it != students.end(); ) { // remove class from each student that has it
        for (int i = 0; i < it->second.classes.size(); i++) {
            if (it->second.classes[i] == classCode) {
                it->second.classes.erase(it->second.classes.begin() + i);
                studentsDropped++;
                break;
            }
        }
        if (it->second.classes.size() == 0) { // if student only had one class, remove them
            it = students.erase(it); // https://en.cppreference.com/w/cpp/container/map/erase_if.html
        }
        else {
            ++it; // only increment iterator when student is not removed to avoid out of bounds check later
        }
    }
    classStudent.erase(classCode);
    cout << studentsDropped << endl;
    testing.push_back(to_string(studentsDropped));
    return studentsDropped;
}

bool CampusCompass::toggleEdgesClosure(vector<int> edgesToClose, vector<string>& testing) {
    for (int i = 0; i < edgesToClose.size(); i+=2) {
        int edge1 = edgesToClose[i];
        int edge2 = edgesToClose[i+1];
        for (int j = 0; j < adjList[edge1].size(); j++) {
            if (adjList[edge1][j].first == edge2) {
                adjList[edge1][j].second.isOpen = !adjList[edge1][j].second.isOpen;
            }
        }
        for (int j = 0; j < adjList[edge2].size(); j++) {
            if (adjList[edge2][j].first == edge1) {
                adjList[edge2][j].second.isOpen = !adjList[edge2][j].second.isOpen;
            }
        }
    }
    cout << "successful" << endl;
    testing.push_back("successful");
    return true;
}

int CampusCompass::checkEdgeStatus(int location1, int location2, vector<string>& testing) {
    // returns 0 if edge is open, 1 if edge is closed, 2 if edge DNE
    if (adjList.count(location1) == 0) { // if location doesn't exist
        testing.push_back("DNE");
        cout << "DNE" << endl;
        return 2;
    }
    for (int i = 0; i < adjList[location1].size(); i++) { // loop through all connections to location 1
        if (adjList[location1][i].first == location2) {
            if (adjList[location1][i].second.isOpen) {
                testing.push_back("open");
                cout << "open" << endl;
                return 0;
            }
            else {
                testing.push_back("closed");
                cout << "closed" << endl;
                return 1;
            }
        }
    }
    // location dne if location 2 wasn't one of location 1's connections
    testing.push_back("DNE");
    cout << "DNE" << endl;
    return 2;
}

vector<int> CampusCompass::printShortestEdges(string id) { // Code design from module 8b, slide 38
    vector<int> returnRes; // for testing
    vector<string> classesToPrint = students.at(id).classes;
    int studentResidence = students.at(id).residence;
    unordered_set<int> s = {studentResidence}; // initialize two sets and two vectors for dijkstra's
    unordered_set<int> vMinusS;
    vector<int> dv(57, 10000); // arbitrarily large number
    vector<int> pv(57, -1);
    for (int i = 1; i < 57; i++) { // for all locations in V-S
        if (i != studentResidence) { // checking to make sure location is not the residence
            vMinusS.insert(i);
            pv[i] = studentResidence; // set p[v] to the source
            for (int j = 0; j < adjList[studentResidence].size(); j++) {
                if (adjList[studentResidence][j].first == i && adjList[studentResidence][j].second.isOpen) { // if there is an edge from residence to the location
                    dv[i] = adjList[studentResidence][j].second.weight; // set d[v] to the weight of the edge from residence to the location, d[v] is set to 10000 otherwise (in place of infinity)
                }
            }
        }
    }
    while (!vMinusS.empty()) {
        int u = -1;
        int dvMin = 10000; // arbitrarily large number
        for (int i: vMinusS) { // for all locations in V-S, find the location closest to the residence
            if (dv[i] >= 0 && dv[i] < dvMin) {
                dvMin = dv[i];
                u = i;
            }
        }
        if (u == -1) break; // if there are no more reachable locations from residence in V-S, we are done
        vMinusS.erase(u); // remove the location from V-S and put it in S
        s.insert(u);
        for (int j = 0; j < adjList[u].size(); j++) { // for all locations adjacent to the selected location
            if (vMinusS.count(adjList[u][j].first) != 0 && adjList[u][j].second.isOpen) { // if the adjacent location is in V-S and the edge is open
                if (dv[u] + adjList[u][j].second.weight < dv[adjList[u][j].first]) { // if the weight to the selected location plus the weight to the adjacent location is less than the current weight to the adjacent location
                    dv[adjList[u][j].first] = dv[u] + adjList[u][j].second.weight; // update d[v] and p[v]
                    pv[adjList[u][j].first] = u;
                }
            }
        }
    }

    sort(classesToPrint.begin(), classesToPrint.end());
    cout << "Name: " << students.at(id).name << endl;
    for (int i = 0; i < classesToPrint.size(); i++) {
        if (dv[classCodes[classesToPrint[i]]] == 10000) {
            cout << classesToPrint[i] << " | " << "Total Time: -1" << endl;
            returnRes.push_back(-1);
        }
        else {
            cout << classesToPrint[i] << " | " << "Total Time: " << dv[classCodes[classesToPrint[i]]] << endl;
            returnRes.push_back(dv[classCodes[classesToPrint[i]]]);
        }
    }
    return returnRes;
}

