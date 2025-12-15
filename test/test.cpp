
#include <catch2/catch_test_macros.hpp>
#include <iostream>

#include "CampusCompass.h"

using namespace std;


TEST_CASE("Test invalid commands") {
    CampusCompass cc;
    cc.ParseCSV("data/edges.csv", "data/classes.csv");

    vector<string> testResults;
    vector<string> expectedResults = {"unsuccessful", "unsuccessful", "unsuccessful", "unsuccessful",
                                      "unsuccessful","unsuccessful","unsuccessful","unsuccessful",
                                      "unsuccessful","unsuccessful","unsuccessful","unsuccessful",
                                      "unsuccessful","unsuccessful","unsuccessful"};
    cc.executeValidCommand("insert \"Ethan\" 0000000 1 1 COP3530", testResults); // 7-digit ufid
    cc.executeValidCommand("insert \"Ethan\" 000000000 1 1 COP3530", testResults); // 9-digit ufid
    cc.executeValidCommand("insert \"Ethan\" 000$0000 1 1 COP3530", testResults); // invalid ufid
    cc.executeValidCommand("insert \"Ethan\" 0000a000 1 1 COP3530", testResults); // invalid ufid
    cc.executeValidCommand("insert \"Ethan$\" 00000000 1 1 COP3530", testResults); // invalid name
    cc.executeValidCommand("insert \"Ethan2\" 00000000 1 1 COP3530", testResults); // invalid name
    cc.executeValidCommand("insert \"Ethan_\" 00000000 1 1 COP3530", testResults); // invalid name
    cc.executeValidCommand("insert \"Ethan_\"", testResults); // missing arguments
    cc.executeValidCommand("insert \"Ethan_\"", testResults); // missing arguments
    cc.executeValidCommand("insert", testResults); // missing arguments
    cc.executeValidCommand("Insert \"Ethan\" 00000000 1 1 COP3530", testResults); // invalid command
    cc.executeValidCommand("insertt \"Ethan\" 00000000 1 1 COP3530", testResults); // invalid command
    cc.executeValidCommand("insert \"Ethan\" 00000000 1 2 COP3530", testResults); // invalid command
    cc.executeValidCommand("insert \"Ethan\" 00000000 1 2 COP3530 COP3502 COP3503", testResults); // invalid command
    cc.executeValidCommand("insert \"Ethan\" 00000000 1 3 COP3530 COP3502", testResults); // invalid command


    REQUIRE(expectedResults == testResults);
}

TEST_CASE("Edge Cases") {
    CampusCompass cc;
    cc.ParseCSV("data/edges.csv", "data/classes.csv");

    vector<string> testResults;
    vector<string> expectedResults = {"successful", "unsuccessful", "unsuccessful", "unsuccessful",
                                      "unsuccessful","unsuccessful","unsuccessful","unsuccessful",
                                      };
    cc.executeValidCommand("insert \"Ethan\" 00000000 1 1 COP3530", testResults);
    cc.executeValidCommand("insert \"EthanDupe\" 00000000 1 1 COP3530", testResults); // insert duplicate id
    cc.executeValidCommand("remove 11111111", testResults); // remove id that doesn't exist
    cc.executeValidCommand("dropClass 11111111 COP3530", testResults); // drop class for id that doesn't exist
    cc.executeValidCommand("dropClass 00000000 COP3503", testResults); // drop class that student doesn't have
    cc.executeValidCommand("replaceClass 11111111 COP3530 COP3503", testResults); // id doesn't exist
    cc.executeValidCommand("replaceClass 00000000 COP3503 COP3502", testResults); // student doesn't have class code 1
    cc.executeValidCommand("replaceClass 00000000 COP3530 COP3530", testResults); // student already has class code 2
    REQUIRE(expectedResults == testResults);
}

TEST_CASE("dropClass, replaceClass, remove, removeClass") {
    CampusCompass cc;
    cc.ParseCSV("data/edges.csv", "data/classes.csv");

    vector<string> testResults;
    vector<string> expectedResults = {"successful", "successful", "successful", "successful",
                                      "successful","successful","2","2","successful", "1"
    };
    cc.executeValidCommand("insert \"Ethan\" 00000000 1 6 COP3530 MAC2311 CDA3101 COT3100 COP3502 COP3503", testResults);
    cc.executeValidCommand("insert \"Ethan Again\" 11111111 10 6 MAC2312 MAC2311 MAC2313 COT3100 COP3502 COP3503", testResults);
    cc.executeValidCommand("dropClass 00000000 COP3530", testResults);
    cc.executeValidCommand("dropClass 00000000 COP3502", testResults);
    cc.executeValidCommand("replaceClass 00000000 MAC2311 COP3530", testResults);
    cc.executeValidCommand("replaceClass 00000000 COP3530 MAC2311", testResults);
    cc.executeValidCommand("removeClass MAC2311", testResults);
    cc.executeValidCommand("removeClass COT3100", testResults);
    cc.executeValidCommand("remove 11111111", testResults);
    cc.executeValidCommand("removeClass COP3503", testResults);
    REQUIRE(expectedResults == testResults);
}

TEST_CASE("printShortestEdges with toggling edges") {
    CampusCompass cc;
    cc.ParseCSV("data/edges.csv", "data/classes.csv");

    vector<string> testResults;
    cc.executeValidCommand("insert \"Ethan\" 00000000 7 1 CEN4907", testResults);
    cc.executeValidCommand("printShortestEdges 00000000", testResults);
    vector<int> v1 = cc.printShortestEdges("00000000");
    cc.executeValidCommand("toggleEdgesClosure 1 45 47", testResults); // still reachable, longer path
    cc.executeValidCommand("printShortestEdges 00000000", testResults);
    vector<int> v2 = cc.printShortestEdges("00000000");
    cc.executeValidCommand("toggleEdgesClosure 1 45 46", testResults); // unreachable
    cc.executeValidCommand("printShortestEdges 00000000", testResults);
    vector<int> v3 = cc.printShortestEdges("00000000");
    vector<int> v11 = {6};
    vector<int> v22 = {8};
    vector<int> v33 = {-1};
    REQUIRE(v1 == v11);
    REQUIRE(v2 == v22);
    REQUIRE(v3 == v33);
}
