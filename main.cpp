#include<bits/stdc++.h>

#define WRONG_LINE_IN_INPUT_FILE 0
#define WRONG_FILE 1
#define WRONG_NUMBER_OF_PARAMETERS 2
#define WRONG_ID 3
#define WRONG_GROUP_DESCRIPTION 4
#define ID_DUPLICATE 5
#define MAX_GROUP_SIZE 3
#define MIN_GROUP_SIZE 2

using namespace std;

void handleError(int errorType, int lineNumber, string message = "MISSING_ERROR_PARAMETER", string message2 = "MISSING_ERROR_PARAMETER2"){
//    ostream errorOutput = cout;
    if(errorType == WRONG_LINE_IN_INPUT_FILE)
        cout << "Error in " + message + ", line " << lineNumber << ": " + message2 + "\n";
    else if(errorType == WRONG_FILE)
        cout << "Error: problem with file " + message + "\n";
    else if(errorType == WRONG_NUMBER_OF_PARAMETERS)
        cout << "Usage: " + message + " file\n";
    else if(errorType == ID_DUPLICATE || errorType == WRONG_ID)
        cout << "Error in " + message + ", line " << lineNumber << ": " + message2 + "\n";
    else if(errorType == WRONG_GROUP_DESCRIPTION)
        cout << "Error in cin, line " << lineNumber << ": " + message + "\n";
    else
        cout << "Unknown error happened in line " << lineNumber << " error id: "
                    << errorType << " error massages: " + message + " " +message2 + "\n";
}

bool isIdCorrect(string studentId){
    regex idRegex("([a-z]{2}[0-9]{6})|([a-z]{2}m-[0-9]{4})");
    return regex_match(studentId,idRegex);
}

list<string> splitString(string& str, int lineNumber){
    list<string> result;
    while(str.length() >= 8){
        result.push_back(str.substr(0,8));
        if(str[8] != '+' && str.length() > 8)
            handleError(WRONG_GROUP_DESCRIPTION,lineNumber);
        str = str.substr(8 + (str.length() > 10));
    }
    if(str.length() != 0 ){
        handleError(WRONG_GROUP_DESCRIPTION,lineNumber);
        result.clear();
    }
    return result;
}
//to jest trochę za długie chyba
list<string> ExtractStudentsFromGroup(string groupDescription, int lineNumber, map<string,vector<string>>& existingGroups) {
    regex prefixRegex("grupa[1-8]/zadanie[1-6]/");
    list<string> result;
    string groupDescriptionCopy = groupDescription;
    set<string> idsInGroup;
    list<list<string>::iterator> stringToDelete;
    if(regex_search(groupDescription,prefixRegex)){
         groupDescription = regex_replace(groupDescription,prefixRegex,"");
    } else {
        handleError(WRONG_GROUP_DESCRIPTION,lineNumber,groupDescriptionCopy);
        result.clear();
        return result;
    }

    result = splitString(groupDescription,lineNumber);
    if(result.size() == 0)
        return result;

    if(result.size() < MIN_GROUP_SIZE || result.size() > MAX_GROUP_SIZE){
        handleError(WRONG_GROUP_DESCRIPTION,lineNumber,groupDescriptionCopy);
        result.clear();
        return result;
    }

    for (list<string>::iterator it = result.begin(); it != result.end(); it++) {
        if (isIdCorrect(*it) == false) {
            handleError(WRONG_GROUP_DESCRIPTION,lineNumber,groupDescriptionCopy);
            result.clear();
            return result;
        }
    }

    for (list<string>::iterator it = result.begin(); it != result.end(); it++){
        if(existingGroups.find(*it) == existingGroups.end()){
            handleError(WRONG_ID,lineNumber,*it);
            stringToDelete.push_back(it);
        }
        else {
            if(idsInGroup.find(*it) == idsInGroup.end()){
                idsInGroup.insert(*it);
            } else{
                handleError(WRONG_ID,lineNumber,*it);
                stringToDelete.push_back(it);
            }
        }
    }

    for(list<list<string>::iterator>::iterator it  = stringToDelete.begin(); it != stringToDelete.end(); it++){
        result.erase(*it);
    }

    return result;
}

pair<vector<string>, bool> readStudentsIdsFromFile(string fileName, map<string,vector<string>>& existingGroups){
    vector<string> input;
    pair<vector<string>,bool> result;
    result.second = true;
    string bufor;
    ifstream inputStream(fileName);
    int lineNumber = 1;

    if(inputStream.is_open()){
        getline(cin,bufor);
        while(bufor != ""){
            if(isIdCorrect(bufor) && existingGroups.find(bufor) == existingGroups.end()) {
                result.first.push_back(bufor);
                pair<string,vector<string>> help;
                help.first = bufor;
                vector<string> emptyVector;
                emptyVector.clear();
                help.second = emptyVector;
                existingGroups.insert(help);
            } else {
                handleError(WRONG_LINE_IN_INPUT_FILE,lineNumber,fileName,bufor);
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
        pair<vector<string>,bool> studentsIds = readStudentsIdsFromFile(argv[1],existingGroups);
        if(studentsIds.second == false){
            return 1;
        } else {
            list<list<string>> groups = readGroupDescriptionsFromStdin(existingGroups);
        }
    }

    return 0;
}