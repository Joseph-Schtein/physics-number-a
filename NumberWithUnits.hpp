#ifndef HEADER_HPP
#define HEADER_HPP

#include <utility>
#include <string>
#include <iostream>
#include <vector>


using namespace std;

namespace ariel{
    class NumberWithUnits{
        private:
            double amount;
            string measure;
            

        public:
            
        NumberWithUnits(const double& givenamount, const string& mesurment);

        void setAmount(double newAmount){
            this->amount = newAmount;
        }

        void setMeasure(string newMeasure){
            this->measure = newMeasure;
        }

        static void read_units(ifstream &file);

        static vector<string> file_to_vector(ifstream &myfile);

        friend ostream& operator << (ostream& os, const NumberWithUnits& myself);

        friend istringstream& operator >> (istringstream& is, NumberWithUnits& myself);     

        NumberWithUnits operator+ () const;

        unsigned int searchSameMessure(string myMeasure, string otherMesure);

        double addAmount(pair<string, double> mine, pair<string, double> his, unsigned int lineNum);

        double subAmount(pair<string, double> mine, pair<string, double> his, unsigned int lineNum);

        NumberWithUnits& operator+=(const NumberWithUnits& other);

        NumberWithUnits operator+(const NumberWithUnits& other);

        NumberWithUnits operator-() const;

        NumberWithUnits& operator -= (const NumberWithUnits& other);

        NumberWithUnits operator - (const NumberWithUnits& other);

        bool operator == (const NumberWithUnits& other);
            
        bool operator != (const NumberWithUnits& other);

        bool operator >= (const NumberWithUnits& other);
            
        bool operator <= (const NumberWithUnits& other);

        bool operator > (const NumberWithUnits& other);
            
        bool operator < (const NumberWithUnits& other);

        NumberWithUnits& operator ++();

        const NumberWithUnits& operator ++(int);

        NumberWithUnits& operator --();

        const NumberWithUnits& operator --(int);

        friend NumberWithUnits operator* (int mutiplyer, NumberWithUnits& myself);

        double convertWithoutChanging(pair<string, double> mine, pair<string, double> his, unsigned int lineNum);
    };
}


#endif