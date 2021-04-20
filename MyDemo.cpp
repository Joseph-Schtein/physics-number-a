#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>
using namespace std;

#include "NumberWithUnits.hpp"
using namespace ariel;

int main(){

    ifstream units_file{"units.txt"};
    NumberWithUnits::read_units(units_file);

    NumberWithUnits a{10, "hour"};
    cout << a << endl;           // "10[hour]".
    cout << (2.4*a) << endl;    // "24[hour]"
    
    NumberWithUnits b{3, "day"};  // 3 days
    cout << (a+b) << endl;   // "82[hour]".
    cout << (b-a) << endl;   // "2.58333[day]".

    cout << boolalpha;
    cout << (a>b) << endl;  // "false"
    cout << (a<=b) << endl;  // "false"
    cout << (a==NumberWithUnits{600, "min"}) << endl;  // "true"
  
    istringstream sample_input1{"700 [ dm ]"};
    sample_input1 >> a;
    istringstream sample_input2{"400 [ m ]"};
    sample_input2 >> b;

    cout << (a+b) << endl;   // "4700[dm]"
    cout << (a += NumberWithUnits{700, "cm"}) << endl;  // "770[dm]"
    cout << (a+b) << endl;   // "4770[dm]".


    istringstream sample_input3{"100 [ POUND ]"};
    sample_input3 >> a;
    istringstream sample_input4{"253 [ USD ]"};
    sample_input4 >> b;

    cout << a << endl;  // "100[POUND]".
    cout << (2*b) << endl; // 506[USD]
    cout << (a+b) << endl; // 281.753[POUND]
    cout << (b+a) << endl; // 392.2[USD]

    istringstream sample_input5{"100 [ ILS ]"};
    sample_input5 >> b;

    cout << (a+b) << endl; // 121.573[POUND]
    cout << (b+a) << endl; // 563.536[USD]
    cout << (a-b) << endl; // 78.4267[POUND]
    cout << (b-a) << endl; // -363.536[USD]
    
  return 0;
}