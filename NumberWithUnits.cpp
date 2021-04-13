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

    static vector<vector<string>> mesurementTypes;
    static vector<vector<pair<string, double>>> convertion;

    vector<string> ariel::NumberWithUnits::file_to_vector(ifstream &myfile){
        vector<string> output;
        string line;
        string tmpLine;
        unsigned int counter = 0;
        while(getline(myfile,line)){
            if(counter == 2){
                tmpLine = line;
            }
            
            else if(counter == 3){
                output.push_back(line);
                output.push_back(tmpLine);
            }

            else{    
                output.push_back(line);
            }
                
            counter++;
        
        }
        return output;
   }


    void ariel::NumberWithUnits::read_units(ifstream& myfile){
        string line;
        string left= "";
        string right = "";
        bool first = true;
        if (myfile.is_open()){
            vector<string> file =  file_to_vector(myfile);
            vector<string> mesurements;
            vector<pair<string, double>> convert;
            for(unsigned int i = 0; i < file.size(); i++){
                line = file.at(i);
                unsigned int divid = line.find('=');
                left = line.substr(0, divid-1);
                right = line.substr(divid+2 ,line.length()-1);
                
                unsigned int leftDivid = left.find(' ');
                unsigned int rightDivid = right.find(' ');

                double leftAmout = stod(left.substr(0, leftDivid));
                double rightAmout = stod(right.substr(0, rightDivid));
                string leftMeasure = left.substr(leftDivid+1, left.length()-1);
                string rightMeasure = right.substr(rightDivid+1, right.length()-1);

                pair<string, double> leftEntry(leftMeasure, leftAmout); 
                pair<string, double> rightEntry(rightMeasure, rightAmout);     

                double multi = 1;
                if(find(mesurements.begin(), mesurements.end(), leftMeasure)!= mesurements.end()){
                    mesurements.push_back(rightMeasure);
                    for(unsigned int j = 0; j < convert.size(); j++){
                        multi *= convert.at(j).second;
                    }
                    rightEntry.second *= multi;
                    convert.push_back(rightEntry);     
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
                    if(i+1 == file.size()){
                        mesurementTypes.push_back(mesurements);
                        convertion.push_back(convert);
                        mesurements.clear();
                        convert.clear();
                    }
                } 
            }
        }
        myfile.close();
    }

    ariel::NumberWithUnits::NumberWithUnits(const double& givenamount, const string& mesurment):
        amount(givenamount), measure(mesurment){}


    ostream& ariel::operator<< (ostream& os, const NumberWithUnits& myself){
        os << myself.amount << "["<< myself.measure << "]";
        return os;
    }

    istringstream& ariel::operator>> (istringstream& is, NumberWithUnits& myself){
        string amo;
        string meas;
        string tmp;
        is >> amo >> tmp >> meas;
        myself.amount = stod(amo);
        myself.measure = meas;
        return is;
    }

    NumberWithUnits ariel::NumberWithUnits::operator+() const{
        return NumberWithUnits(amount, measure);
    }

    unsigned int ariel::NumberWithUnits::searchSameMessure(string myMeasure, string otherMesure){
        for(unsigned int i = 0; i < mesurementTypes.size(); i++){
            bool mine = find(mesurementTypes.at(i).begin(), mesurementTypes.at(i).end(), myMeasure) != mesurementTypes.at(i).end();
            bool his = find(mesurementTypes.at(i).begin(), mesurementTypes.at(i).end(), otherMesure) != mesurementTypes.at(i).end();
                    
            if(mine && his){
                return i;
            }

            else if(((mine && !his) || (!mine && his))){
                throw std::invalid_argument("Units do not match - [" + otherMesure + "] cannot be converted to ["+myMeasure+"]");
            }

            else if(i+1 == mesurementTypes.size()){
                throw std::invalid_argument("One of the two units are not valid at all");
                
            }
        }

        return 0;                      
    }

    double ariel::NumberWithUnits::addAmount(pair<string, double> mine, pair<string, double> his, unsigned int lineNum){
        
        unsigned int myIndex;
        unsigned int hisIndex;

        for(unsigned int i = 0; i < convertion.at(lineNum).size(); i++){
            if(convertion.at(lineNum).at(i).first == mine.first){
               myIndex = i; 
            }

            if(convertion.at(lineNum).at(i).first == his.first){
               hisIndex = i; 
            }
        }

        mine.second += (his.second * convertion.at(lineNum).at(myIndex).second) / convertion.at(lineNum).at(hisIndex).second;
        

        return mine.second;
    }

    double ariel::NumberWithUnits::subAmount(pair<string, double> mine, pair<string, double> his, unsigned int lineNum){
        
        unsigned int myIndex;
        unsigned int hisIndex;

        for(unsigned int i = 0; i < convertion.at(lineNum).size(); i++){
            if(convertion.at(lineNum).at(i).first == mine.first){
               myIndex = i; 
            }

            if(convertion.at(lineNum).at(i).first == his.first){
               hisIndex = i; 
            }
        }
        
        mine.second -= (his.second * convertion.at(lineNum).at(myIndex).second) / convertion.at(lineNum).at(hisIndex).second;
        

        return mine.second;
    }


    double ariel::NumberWithUnits::convertWithoutChanging(pair<string, double> mine, pair<string, double> his, unsigned int lineNum){
        
        unsigned int myIndex;
        unsigned int hisIndex;
        double output;

        for(unsigned int i = 0; i < convertion.at(lineNum).size(); i++){
            if(convertion.at(lineNum).at(i).first == mine.first){
               myIndex = i; 
            }

            if(convertion.at(lineNum).at(i).first == his.first){
               hisIndex = i; 
            }
        }

        output = (his.second * convertion.at(lineNum).at(myIndex).second) / convertion.at(lineNum).at(hisIndex).second;

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
                this->amount = addAmount(mine, his, lineNum);

            }
        }

        return *this;

    }

    NumberWithUnits ariel::NumberWithUnits::operator+ (const NumberWithUnits& other){
        double tmp;
        if(this->measure == other.measure){
            return NumberWithUnits(this->amount + other.amount, measure); 
        }


        else{
            pair<string, double> mine(this->measure, this->amount);
            pair<string, double> his(other.measure, other.amount);
            unsigned int lineNum = searchSameMessure(mine.first, his.first);

            if(lineNum >= 0){
                tmp = addAmount(mine, his, lineNum);
            }
        }
    
        return NumberWithUnits(tmp, measure); 
    }

    NumberWithUnits ariel::NumberWithUnits::operator-() const{

        return NumberWithUnits(-amount, measure); 
    }


    NumberWithUnits& ariel::NumberWithUnits::operator -= (const NumberWithUnits& other){
        if(other.measure == this->measure){
            this->amount += other.amount; 
        }

        else{
            pair<string, double> mine(this->measure, this->amount);
            pair<string, double> his(other.measure, other.amount);
            unsigned int lineNum = searchSameMessure(mine.first, his.first);

            if(lineNum >= 0){
                this->amount = subAmount(mine, his, lineNum);
            }
        }

        return *this;

    }
    

    NumberWithUnits ariel::NumberWithUnits::operator - (const NumberWithUnits& other){
        double tmp;
        if(this->measure == other.measure){
            return NumberWithUnits(this->amount - other.amount, measure); 
        }


        else{
            pair<string, double> mine(this->measure, this->amount);
            pair<string, double> his(other.measure, other.amount);
            unsigned int lineNum = searchSameMessure(mine.first, his.first);

            if(lineNum >= 0){
                tmp = subAmount(mine, his, lineNum);
            }
        }
    
        return NumberWithUnits(tmp, measure); 
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

                else{
                    return false; 
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