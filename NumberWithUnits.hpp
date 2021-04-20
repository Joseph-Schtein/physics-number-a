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

        double getAmount(){
            return this->amount;
        }

        string getMeasure(){
            return this->measure;
        }

        static void read_units(ifstream &file);

        static vector<string> file_to_vector(ifstream &myfile);

        friend ostream& operator << (ostream& os, const NumberWithUnits& myself);

        friend istringstream& operator >> (istringstream& is, NumberWithUnits& myself);     

        NumberWithUnits operator+ () const;

        static unsigned int searchSameMessure(const string& myMeasure, const string&  otherMesure);

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

        friend NumberWithUnits operator* (double mutiplyer, NumberWithUnits& myself);

        static double convertWithoutChanging(const pair<string, double>& mine, const pair<string, double>& his, const unsigned int& lineNum);
    };
}


#endif