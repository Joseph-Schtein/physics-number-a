#include "doctest.h"
#include "NumberWithUnits.hpp"
using namespace ariel;

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
using namespace std;

TEST_CASE("some normal test") {
    ifstream units_file{"units.txt"};
    NumberWithUnits::read_units(units_file);

    NumberWithUnits a{2, "km"};
    NumberWithUnits b{200, "cm"};
    NumberWithUnits c = (a+b);
    NumberWithUnits d = (a-b);
    NumberWithUnits e = (2.5*a);
    NumberWithUnits f = (b+a);
    NumberWithUnits g = (b-a);
    NumberWithUnits h = (0.5*a);
    NumberWithUnits i = (8.5*b);
    
    CHECK(e.getAmount() == 5);
    CHECK(h.getAmount() == 1);
    CHECK(c.getAmount() == 2.002);
    CHECK(d.getAmount() == 1.998);
    CHECK(f.getAmount() == 200200);
    CHECK(g.getAmount() == -199800);
    CHECK(i.getAmount() == 1700);
    CHECK(e.getMeasure() == "km");
    CHECK(f.getMeasure() == "cm");
    CHECK(g.getMeasure() == "cm");

    istringstream sample_input1{"500 [ USD ]"};
    istringstream sample_input2{"1000 [ ILS ]"};
    sample_input1 >> a;
    sample_input2 >> b;

    NumberWithUnits c2 = (a+b);
    NumberWithUnits d2 = (a-b);
    NumberWithUnits e2 = (2.5*a);
    NumberWithUnits f2 = (b+a);
    NumberWithUnits g2 = (b-a);
    NumberWithUnits h2 = (0.5*a);

    CHECK(c2.getAmount() == 500+1000/3.33);
    CHECK(d2.getAmount() == 500-1000/3.33);
    CHECK(e2.getAmount() == 1250);
    CHECK(f2.getAmount() == 2665.0);
    CHECK(g2.getAmount() == -665.0);
    CHECK(h2.getAmount() == 250);

    istringstream sample_input3{"4 [ hour ]"};
    istringstream sample_input4{"150 [ min ]"};
    sample_input3 >> a;
    sample_input4 >> b;

    NumberWithUnits c3 = (a+b);
    NumberWithUnits d3 = (a-b);
    NumberWithUnits e3 = (2.5*a);
    NumberWithUnits f3 = (b+a);
    NumberWithUnits g3 = (b-a);
    NumberWithUnits h3 = (0.5*a);

    CHECK(c3.getAmount() == 6.5);
    CHECK(d3.getAmount() == 1.5);
    CHECK(e3.getAmount() == 10);
    CHECK(f3.getAmount() == 390);
    CHECK(g3.getAmount() == -90.0);
    CHECK(h3.getAmount() == 2);
    
}