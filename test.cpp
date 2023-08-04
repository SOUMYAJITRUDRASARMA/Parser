#include "util.hpp"
using namespace std;

int main() 
{
    // Examples ...
    // a+bcd+e
    // a+b\+cd+e
    // a+b\+cd+ea+b\+cd+e
    // \\\\\\

    // (a+b\.\\cd+ef)*+xyz

    string s;
    char buffer[100];
    cin.getline(buffer, 100);
    s = buffer;

    cout << "Got: " << s;

    cout << "\nProcessed: ";
    vector<Character> v = addConcat(s);
    printCharacterVector(v, cout);
    cout << endl;

    cout << "Postfix: ";
    v = convertPostfix(v);
    printCharacterVector(v, cout);
    cout << endl;

    return 0;
}