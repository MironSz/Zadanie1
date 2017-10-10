#include<bits/stdc++.h>

#define WRONG_ID "wrong_id"
//#define
using namespace std;

bool isIdCorrect(string studentId){
    regex idRegex("([a-z]{2}[0-9]{6})|([a-z]{3}-[0-9]{4})");
    return regex_match(studentId,idRegex);
}

list<string> ExtractStudentsFromGroup(string groupDescription) {
    regex prefixRegex("grupa[1-8]/zadanie[1-6]/");
    list<string> result;
    int i, inputLength, lastBegin = 0;

    if(regex_search(groupDescription,prefixRegex)){
        groupDescription=regex_replace(groupDescription,prefixRegex,"");
        cout <<"good prefix, now input is |"+groupDescription+"}\n";
    } else {
        cout << "WRONG PREFIX\n";
    }

    inputLength = groupDescription.length();

    for (i = 0; i < inputLength; i++) {
        if (groupDescription[i] == '+' || i+1 == inputLength) {
            result.push_back(groupDescription.substr(lastBegin, i));
            lastBegin = i + 1;
        }
    }

    cout << "size: " << result.size() <<"\n";
    for (list<string>::iterator it = result.begin(); it != result.end(); it++){
        cout << "testing "+*it+"\n";
        if(isIdCorrect(*it)){
            cout << *it+" matches!\n\n";
        } else {
            cout << *it+" does not match :(\n\n";
        }
    }
    return result;
}



int main() {


    ExtractStudentsFromGroup(string("grupa1/zadanie5/ms3839503+ts123456+tsd123456+mxk-1000"));
    return 0;
}