#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>
#include <algorithm>
using namespace std;

#include "NumberWithUnits.hpp"
using namespace ariel;

    static vector<vector<string>> mesurementTypes;  // These two 2d vectors are helping for checking if the measuermnets are for the sane type  
    static vector<vector<pair<string, double>>> convertion; // and for normilization of the amount of each measure 

    vector<string> ariel::NumberWithUnits::file_to_vector(ifstream &myfile){ // Convert the file line into vectors of strings and replace the third line with fourth 
        vector<string> output;                                               // for the biggest measuremnt to the smallest of each type
        string line;
        string tmpLine;
        unsigned int counter = 0;
        while(getline(myfile,line)){
            if(counter == 3){ // temporary variable for the third line to insert after we insert the fourth line 
                tmpLine = line;
            }
            
            else if(counter == 4){ // Insert the fourth line and then the third 
                output.push_back(line);
                output.push_back(tmpLine);
            }

            else{    
                output.push_back(line); // else we just push the line to the vector 
            }
                
            counter++;
        
        }
        return output;
   }


    void ariel::NumberWithUnits::read_units(ifstream& myfile){
        string line;
        string left;
        string right;
        bool first = true;
        if (myfile.is_open()){
            vector<string> file =  file_to_vector(myfile); 
            vector<string> mesurements;// Temporary vector such that we will inesrt it to mesurementTypes when we finish to pass on every measurment of the same type
            vector<pair<string, double>> convert; // The same as mesurements but we insert it into convertion
            for(unsigned int i = 0; i < file.size(); i++){ // For each line we'll divide him to left and right and  for both sides we slice it to amount the measuremnt type 
                line = file.at(i);
                unsigned int divid = line.find('=');
                left = line.substr(0, divid-1);
                right = line.substr(divid+2 ,line.length()-1);
                
                unsigned int leftDivid = left.find(' ');
                unsigned int rightDivid = right.find(' ');

                double leftAmout = stod(left.substr(0, leftDivid)); // stod stand for convert strimg into double
                double rightAmout = stod(right.substr(0, rightDivid));
                string leftMeasure = left.substr(leftDivid+1, left.length()-1);
                string rightMeasure = right.substr(rightDivid+1, right.length()-1);

                pair<string, double> leftEntry(leftMeasure, leftAmout); 
                pair<string, double> rightEntry(rightMeasure, rightAmout);     

                double multi = 1;
                if(find(mesurements.begin(), mesurements.end(), leftMeasure)!= mesurements.end()){
                    mesurements.push_back(rightMeasure);
                    multi *= convert.at(convert.size()-1).second;
                    rightEntry.second *= multi;
                    convert.push_back(rightEntry);   
                    for(unsigned int j = 0; j < convert.size(); j++){ 
                        cout << convert.at(j).first << " " << convert.at(j).second << ", ";
                    }  
                }

                else{
                    mesurementTypes.push_back(mesurements);
                    convertion.push_back(convert);
                    mesurements.clear();
                    convert.clear();
                    first = true;
                }

                if(first){
                    first = false;
                    mesurements.push_back(leftMeasure);
                    mesurements.push_back(rightMeasure);
                    convert.push_back(leftEntry);
                    convert.push_back(rightEntry);
                }

                if(i+1 == file.size()){
                        mesurementTypes.push_back(mesurements);
                        convertion.push_back(convert);
                        mesurements.clear();
                        convert.clear();
                } 
            }
        }

        myfile.close();

    }     
    

    ariel::NumberWithUnits::NumberWithUnits(const double& givenamount, const string& mesurment):// Constructor
        amount(givenamount), measure(mesurment){}


    ostream& ariel::operator<< (ostream& os, const NumberWithUnits& myself){
        os << myself.amount << "["<< myself.measure << "]";
        return os;
    }

    istringstream& ariel::operator>> (istringstream& is, NumberWithUnits& myself){
        string amo;
        string meas;
        string tmp;
        is >> amo >> tmp >> meas; // Because the input is 700 [ kg ] we will insert [ into tmp
        myself.amount = stod(amo);
        myself.measure = meas;
        return is;
    }

    NumberWithUnits ariel::NumberWithUnits::operator+() const{ // Return out itself
        return NumberWithUnits(amount, measure);
    }

    unsigned int ariel::NumberWithUnits::searchSameMessure(const string& myMeasure, const string& otherMesure){ // Check if we can convert the two measuremnts 
        for(unsigned int i = 0; i < mesurementTypes.size(); i++){
            bool mine = find(mesurementTypes.at(i).begin(), mesurementTypes.at(i).end(), myMeasure) != mesurementTypes.at(i).end(); // Search mine measure 
            bool his = find(mesurementTypes.at(i).begin(), mesurementTypes.at(i).end(), otherMesure) != mesurementTypes.at(i).end(); // Search for his measure

            if(mine && his){ // If the computer found the measuremnts in the same vector than it will return the vector index in the 2d vector
                return i;
            }
            
            if(((mine && !his) || (!mine && his))){ // If he found one measurment but the second didn't find than return excption
                throw std::invalid_argument("Units do not match - [" + otherMesure + "] cannot be converted to [" + myMeasure + "]");
            }

            if(i+1 == mesurementTypes.size()){ // If we didn't find any measuremnt of the two that the function got
                throw std::invalid_argument("One of the two units are not valid at all");
                
            }
        }

        return 0;                      
    }

    double ariel::NumberWithUnits::convertWithoutChanging(const pair<string, double>& mine, const pair<string, double>& his, const unsigned int& lineNum){ // This function change his amount accordint to mine (this) and his measurmnets types 
        
        unsigned int myIndex = 0;
        unsigned int hisIndex = 0;
        double output = 0;

        for(unsigned int i = 0; i < convertion.at(lineNum).size(); i++){// search for our measurments in the index that searchSameMessure return 
            if(convertion.at(lineNum).at(i).first == mine.first){
               myIndex = i; 
            }

            if(convertion.at(lineNum).at(i).first == his.first){
               hisIndex = i; 
            }
        }
        double myMeasure = convertion.at(lineNum).at(myIndex).second;
        double hisMeasure = convertion.at(lineNum).at(hisIndex).second;
        output = (his.second * myMeasure) / hisMeasure; // Calculate the normalization

        return output;
    }

    NumberWithUnits& ariel::NumberWithUnits::operator+= (const NumberWithUnits& other){
        if(other.measure == this->measure){
            this->amount += other.amount; 
        }

        else{
            pair<string, double> mine(this->measure, this->amount);
            pair<string, double> his(other.measure, other.amount);
            unsigned int lineNum = searchSameMessure(mine.first, his.first);

            if(lineNum >= 0){
                this->amount += convertWithoutChanging(mine, his, lineNum);

            }
        }

        return *this;

    }

    NumberWithUnits ariel::NumberWithUnits::operator+ (const NumberWithUnits& other){
        double tmp = 0;
        bool same = false;
        if(this->measure == other.measure){
            same = true;
        }


        else{
            pair<string, double> mine(this->measure, this->amount);
            pair<string, double> his(other.measure, other.amount);
            unsigned int lineNum = searchSameMessure(mine.first, his.first);

            if(lineNum >= 0){
                tmp = convertWithoutChanging(mine, his, lineNum);
            }
        }
    
        if(same){
            return NumberWithUnits(this->amount + other.amount, measure);
        }
        
        return NumberWithUnits(this->amount+tmp, measure); 
    }

    NumberWithUnits ariel::NumberWithUnits::operator-() const{

        return NumberWithUnits(-amount, measure); 
    }


    NumberWithUnits& ariel::NumberWithUnits::operator -= (const NumberWithUnits& other){
        if(other.measure == this->measure){
            this->amount -= other.amount; 
        }

        else{
            pair<string, double> mine(this->measure, this->amount);
            pair<string, double> his(other.measure, other.amount);
            unsigned int lineNum = searchSameMessure(mine.first, his.first);

            if(lineNum >= 0){
                this->amount -= convertWithoutChanging(mine, his, lineNum);
            }
        }

        return *this;

    }
    

    NumberWithUnits ariel::NumberWithUnits::operator - (const NumberWithUnits& other){
        double tmp = 0;
        bool same = false;
        if(this->measure == other.measure){
            same = true;
        }


        else{
            pair<string, double> mine(this->measure, this->amount);
            pair<string, double> his(other.measure, other.amount);
            unsigned int lineNum = searchSameMessure(mine.first, his.first);

            if(lineNum >= 0){
                tmp = convertWithoutChanging(mine, his, lineNum);
            }
        }
    
        if(same){
            return NumberWithUnits(this->amount - other.amount, measure);
        }
        
        return NumberWithUnits(this->amount - tmp, measure); 
    }


    bool ariel::NumberWithUnits::operator == (const NumberWithUnits& other){
        if(other.measure == this->measure){
            return other.amount == this->amount;
        }

        else{
            pair<string, double> mine(this->measure, this->amount);
            pair<string, double> his(other.measure, other.amount);
            unsigned int lineNum = searchSameMessure(mine.first, his.first);
            if(lineNum >= 0){
                double checking = convertWithoutChanging(mine, his, lineNum);

                if(this->amount == checking){
                    return true;
                }

                else{
                    return false; 
                }
            }
        }

        return false;
    }
            
    bool ariel::NumberWithUnits::operator != (const NumberWithUnits& other){
        if(other.measure == this->measure){
            return other.amount != this->amount;
        }

        else{
            pair<string, double> mine(this->measure, this->amount);
            pair<string, double> his(other.measure, other.amount);
            unsigned int lineNum = searchSameMessure(mine.first, his.first);
            if(lineNum >= 0){
                double checking = convertWithoutChanging(mine, his, lineNum);

                if(this->amount != checking){
                    return true;
                }

                else{
                    return false; 
                }
            }
        }

        return false;
    }

    bool ariel::NumberWithUnits::operator >= (const NumberWithUnits& other){

        if(other.measure == this->measure){
            return this->amount >= other.amount;
        }

        else{
            pair<string, double> mine(this->measure, this->amount);
            pair<string, double> his(other.measure, other.amount);
            unsigned int lineNum = searchSameMessure(mine.first, his.first);
            if(lineNum >= 0){
                double checking = convertWithoutChanging(mine, his, lineNum);

                if(this->amount >= checking){
                    return true;
                }

                else{
                    return false; 
                }
            }
        }

        return false;
    }
            
    bool ariel::NumberWithUnits::operator <= (const NumberWithUnits& other){
        if(other.measure == this->measure){
            return this->amount <= other.amount;
        }

        else{
            pair<string, double> mine(this->measure, this->amount);
            pair<string, double> his(other.measure, other.amount);
            unsigned int lineNum = searchSameMessure(mine.first, his.first);
            if(lineNum >= 0){
                double checking = convertWithoutChanging(mine, his, lineNum);

                if(this->amount <= checking){
                    return true;
                }

                else{
                    return false; 
                }
            }
        }

        return false;
    }

    bool ariel::NumberWithUnits::operator > (const NumberWithUnits& other){

        if(other.measure == this->measure){
            return this->amount > other.amount;
        }

        else{
            pair<string, double> mine(this->measure, this->amount);
            pair<string, double> his(other.measure, other.amount);
            unsigned int lineNum = searchSameMessure(mine.first, his.first);
            if(lineNum >= 0){
                double checking = convertWithoutChanging(mine, his, lineNum);

                if(this->amount > checking){
                    return true;
                }

                else{
                    return false; 
                }
            }
        }

        return false;
    }
            
    bool ariel::NumberWithUnits::operator < (const NumberWithUnits& other){
        if(other.measure == this->measure){
            return this->amount < other.amount;
        }

        else{
            pair<string, double> mine(this->measure, this->amount);
            pair<string, double> his(other.measure, other.amount);
            unsigned int lineNum = searchSameMessure(mine.first, his.first);
            if(lineNum >= 0){
                double checking = convertWithoutChanging(mine, his, lineNum);

                if(this->amount < checking){
                    return true;
                }
            }
        }

        return false;
    }

    NumberWithUnits& ariel::NumberWithUnits::operator ++(){
        this->amount += 1;
        return *this;
    }

    const NumberWithUnits& ariel::NumberWithUnits::operator ++(int){
        this->amount += 1;
        return *this;
    }

    NumberWithUnits& ariel::NumberWithUnits::operator --(){
        this->amount -= 1;
        return *this;
    }

    const NumberWithUnits& ariel::NumberWithUnits::operator --(int){
        this->amount -= 1;
        return *this;
    }

    NumberWithUnits ariel::operator* (double mutiplyer, NumberWithUnits& myself){
        return NumberWithUnits(myself.amount*mutiplyer, myself.measure);
    }