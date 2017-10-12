#include<bits/stdc++.h>

//#define WRONG_ID "wrong_id"
//#define
using namespace std;


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
list<string> ExtractStudentsFromGroup(string groupDescription, int lineNumber, map<string,vector<string>>& existingGroups ) {
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
    vector<string> input;
    pair<vector<string>,bool> result;
    result.second = true;
    string bufor;
    ifstream inputFile;
    inputFile.open(fileName);
    int lineNumber = 1;
    if(inputFile.is_open()){
        while( getline(inputFile,bufor)){
            if(isIdCorrect(bufor)) {
                result.first.push_back(bufor);
            } else {
                //todo obsługa błędu
            }
            lineNumber++;
            bufor.clear();
        }
    } else {
        cout << "jakiś błąd";
        result.second = false;
    }
    return result;
}

pair<string,bool> readLineFromInput(){

}

int main(int argc,  char** argv ) {
    cout <<"hello\nargc:"<<argc<<"\n\n";
    if(argc != 2){
        //todo obsługa błędu
    } else {
        pair<vector<string>,bool> input = readStudentsIdsFromFile(argv[0]);
        if(input.second == false){
            //todo bląd
            cout<< "pliku brakuje\n";
            return 1;
        } else {
            for(int i=0;i<input.first.size();i++){
                cout << input.first[i];
            }

            //todo cały program
        }
    }


    return 0;
}