#include <iostream>
#include <fstream>
#include "CampusCompass.h"

using namespace std;

int main() {
    CampusCompass compass;

    compass.ParseCSV("data/edges.csv", "data/classes.csv");

    vector<string> testing;
    string command;
    getline(cin, command);
    int num_of_lines = stoi(command);
    for (int i = 0; i < num_of_lines; i++) {
        getline(cin, command);
        compass.executeValidCommand(command, testing);
    }
}
