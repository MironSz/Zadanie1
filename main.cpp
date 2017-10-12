#include<bits/stdc++.h>

#define WRONG_LINE_IN_INPUT_FILE 0
#define WRONG_FILE 1
#define WRONG_NUMBER_OF_PARAMETERS 2
#define WRONG_ID 3
#define MAX_GROUP_SIZE 3
#define MIN_GROUP_SIZE 2
//#define
using namespace std;


void handleError(int errorType, int inLine, string fileName = ""){

}

bool isIdCorrect(string studentId){
    regex idRegex("([a-z]{2}[0-9]{6})|([a-z]{3}-[0-9]{4})");
    return regex_match(studentId,idRegex);
}

list<string> splitString(const string& str, const string& separator){
    list<string> result;
    //todo
    return result;
}
//poprawne istniejące id, bez powtórzeń
list<string> ExtractStudentsFromGroup(string groupDescription, int lineNumber, map<string,vector<string>>& existingGroups) {
    regex prefixRegex("grupa[1-8]/zadanie[1-6]/");
    list<string> result;

    if(regex_search(groupDescription,prefixRegex)){
        groupDescription=regex_replace(groupDescription,prefixRegex,"");
    } else {
        //TODO obsługa błędu wejścia
    }
    result = splitString(groupDescription,"+");

    for (list<string>::iterator it = result.begin(); it != result.end(); it++){
        if(isIdCorrect(*it)){
        } else {
            //TODO obsługa błędu wejścia
        }
    }

    return result;
}

pair<vector<string>, bool> readStudentsIdsFromFile(string fileName){
    set<string> existingIds;
    vector<string> input;
    pair<vector<string>,bool> result;
    result.second = true;
    string bufor;
    ifstream inputStream(fileName);
    int lineNumber = 1;

    if(inputStream.is_open()){
        getline(inputStream,bufor);
        while(bufor != ""){
            if(isIdCorrect(bufor) && existingIds.find(bufor) != existingIds.end()) {
                result.first.push_back(bufor);
                existingIds.insert(bufor);
            } else {
                handleError(WRONG_LINE_IN_INPUT_FILE,lineNumber,fileName);
            }
            lineNumber++;
            bufor.clear();
            getline(inputStream,bufor);
        }
    } else {
        handleError(WRONG_FILE,0,fileName);
        result.second = false;
    }
    return result;
}

list<list<string>> readGroupDescriptionsFromStdin(map<string,vector<string>>& existingGroups){
    list<list<string>> result;
    string bufor;
    list<string> groupDescription;
    int lineNumber = 1;

    getline(cin,bufor);
    while(bufor != ""){
        groupDescription = ExtractStudentsFromGroup(bufor,lineNumber,existingGroups);
        if(groupDescription.size() >= MIN_GROUP_SIZE && groupDescription.size() <= MAX_GROUP_SIZE){
            result.push_back(groupDescription);
        }
        lineNumber++;
        bufor.clear();
        getline(cin,bufor);
    }

    return result;
}

int main(int argc,  char** argv ) {
    map<string,vector<string>> existingGroups;

    if(argc != 2){
        handleError(WRONG_NUMBER_OF_PARAMETERS,0);
        return 1;
    } else {
        pair<vector<string>,bool> studentsIds = readStudentsIdsFromFile(argv[1]);
        if(studentsIds.second == false){
            return 1;
        } else {
            list<list<string>> groups = readGroupDescriptionsFromStdin(existingGroups);
        }
    }


    return 0;
}