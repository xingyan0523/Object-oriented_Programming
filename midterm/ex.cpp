#include <iostream>
#include <vector>
#define BAND(x) (((x)>5 && (x)<10) ? (x):0)

using namespace std;

int main( ){
    
    int a = 4;
    cout << BAND(++a) << endl;
    cout << a << endl;
    cout << BAND(++a) << endl;
    cout << a << endl;
    return 0;
}