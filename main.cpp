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

typedef unordered_map<string, vector<string> > str_to_vstr_map;

void handleError(int errorType, int lineNumber, string message = "MISSING_ERROR_PARAMETER",
                 string message2 = "MISSING_ERROR_PARAMETER2") {
    if (errorType == WRONG_LINE_IN_INPUT_FILE)
        cout << "Error in " + message + ", line " << lineNumber << ": " + message2 + "\n";
    else if (errorType == WRONG_FILE)
        cout << "Error: problem with file " + message + "\n";
    else if (errorType == WRONG_NUMBER_OF_PARAMETERS)
        cout << "Usage: " + message + " file\n";
    else if (errorType == ID_DUPLICATE || errorType == WRONG_ID)
        cout << "Error in " + message + ", line " << lineNumber << ": " + message2 + "\n";
    else if (errorType == WRONG_GROUP_DESCRIPTION)
        cout << "Error in cin, line " << lineNumber << ": " + message + "\n";
    else
        cout << "Unknown error happened in line " << lineNumber << " error id: "
             << errorType << " error massages: " + message + " " + message2 + "\n";
}

bool isIdCorrect(string studentId) {
    regex idRegex("([a-z]{2}[0-9]{6})|([a-z]{2}m-[0-9]{4})");
    return regex_match(studentId, idRegex);
}

vector<string> splitString(string &str, int lineNumber) {
    vector<string> result;
    while (str.length() >= 8) {
        result.push_back(str.substr(0, 8));
        if (str[8] != '+' && str.length() > 8)
            handleError(WRONG_GROUP_DESCRIPTION, lineNumber);
        str = str.substr(8 + (str.length() > 10));
    }
    if (str.length() != 0) {
        handleError(WRONG_GROUP_DESCRIPTION, lineNumber);
        result.clear();
    }
    return result;
}

//to jest trochę za długie chyba
vector<string> ExtractStudentsFromGroup(string groupDescription, int lineNumber, str_to_vstr_map &idToPartnersMap) {
    regex prefixRegex("grupa[1-8]/zadanie[1-6]/");
    vector<string> result;
    string groupDescriptionCopy = groupDescription;
    set<string> idsInGroup;
    vector<vector<string>::iterator> stringToDelete;
    if (regex_search(groupDescription, prefixRegex)) {
        groupDescription = regex_replace(groupDescription, prefixRegex, "");
    } else {
        handleError(WRONG_GROUP_DESCRIPTION, lineNumber, groupDescriptionCopy);
        result.clear();
        return result;
    }

    result = splitString(groupDescription, lineNumber);
    if (result.size() == 0)
        return result;

    if (result.size() < MIN_GROUP_SIZE || result.size() > MAX_GROUP_SIZE) {
        handleError(WRONG_GROUP_DESCRIPTION, lineNumber, groupDescriptionCopy);
        result.clear();
        return result;
    }

    for (vector<string>::iterator it = result.begin(); it != result.end(); it++) {
        if (isIdCorrect(*it) == false) {
            handleError(WRONG_GROUP_DESCRIPTION, lineNumber, groupDescriptionCopy);
            result.clear();
            return result;
        }
    }

    for (vector<string>::iterator it = result.begin(); it != result.end(); it++) {
        if (idToPartnersMap.find(*it) == idToPartnersMap.end()) {
            handleError(WRONG_ID, lineNumber, *it);
            stringToDelete.push_back(it);
        } else {
            if (idsInGroup.find(*it) == idsInGroup.end()) {
                idsInGroup.insert(*it);
            } else {
                handleError(WRONG_ID, lineNumber, *it);
                stringToDelete.push_back(it);
            }
        }
    }

    for (vector<vector<string>::iterator>::iterator it = stringToDelete.begin(); it != stringToDelete.end(); it++) {
        result.erase(*it);
    }

    return result;
}

bool readStudentsIdsFromFile(string fileName, str_to_vstr_map &idToPartnersMap) {
    vector<string> input;
    bool result;
    result = true;
    string bufor;
    ifstream inputStream(fileName);
    int lineNumber = 1;

    if (inputStream.is_open()) {
        getline(cin, bufor);
        while (bufor != "") {
            if (isIdCorrect(bufor) && idToPartnersMap.find(bufor) == idToPartnersMap.end()) {
                pair<string, vector<string> > help;
                help.first = bufor;
                vector<string> emptyVector;
                emptyVector.clear();
                help.second = emptyVector;
                idToPartnersMap.insert(help);
            } else {
                handleError(WRONG_LINE_IN_INPUT_FILE, lineNumber, fileName, bufor);
            }
            lineNumber++;
            bufor.clear();
            getline(inputStream, bufor);
        }
    } else {
        handleError(WRONG_FILE, 0, fileName);
        result = false;
    }

    return result;
}

vector<vector<string> > readGroupDescriptionsFromStdin(str_to_vstr_map &idToPartnersMap) {
    vector<vector<string> > result;
    string bufor;
    vector<string> groupDescription;
    int lineNumber = 1;

    getline(cin, bufor);
    while (bufor != "") {
        groupDescription = ExtractStudentsFromGroup(bufor, lineNumber, idToPartnersMap);
        if (groupDescription.size() >= MIN_GROUP_SIZE && groupDescription.size() <= MAX_GROUP_SIZE) {
            result.push_back(groupDescription);
        }
        lineNumber++;
        bufor.clear();
        getline(cin, bufor);
    }

    return result;
}

void assignPartners(str_to_vstr_map &idToPartnersMap, const vector<vector<string> > &groups) {
    for (auto const &group : groups) {
        vector<string>::const_iterator it = group.begin();
        while (it != group.end()) {
            vector<string> &partners = idToPartnersMap[*it];
            partners.insert(partners.end(), group.begin(), it);
            partners.insert(partners.end(), ++it, group.end());
        }
    }
}

int countPenaltyPoints2(const vector<string> &partnersIds) {
    unordered_map<string, int> duplicatesCounter;
    int penaltyPoints = 0;
    for (auto const &id : partnersIds) {
        penaltyPoints += ++duplicatesCounter[id] - 1;
    }
    return penaltyPoints;
}

int countPenaltyPoints(vector<string> &partnersIds) {
    int penaltyPoints = 0;
    sort(partnersIds.begin(), partnersIds.end());
    vector<string>::iterator firstOccurIt = partnersIds.begin();
    vector<string>::iterator lastOccurIt;
    do {
        lastOccurIt = upper_bound(firstOccurIt, partnersIds.end(), *firstOccurIt);
        int n = lastOccurIt - firstOccurIt;
        penaltyPoints += n * (n - 1) / 2;
        firstOccurIt = lastOccurIt;
    } while (firstOccurIt != partnersIds.end());
    return penaltyPoints;
}

string getStudentsIndex(const string &id) {
    return string(id.end() - 6, id.end());
}

int main(int argc, char **argv) {
    str_to_vstr_map idToPartnersMap;

    if (argc != 2) {
        handleError(WRONG_NUMBER_OF_PARAMETERS, 0);
        return 1;
    } else {
        bool readWasSuccessful= readStudentsIdsFromFile(argv[1], idToPartnersMap);
        if (readWasSuccessful == false) {
            return 1;
        } else {
            vector<vector<string> > groups = readGroupDescriptionsFromStdin(idToPartnersMap);
            assignPartners(idToPartnersMap, groups);
            for (auto &kv : idToPartnersMap) {
                cout << getStudentsIndex(kv.first) << ' ' << countPenaltyPoints(kv.second) << '\n';
            }
        }
    }
    return 0;
}