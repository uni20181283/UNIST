///DayOfYear
#include <iostream>
#include <fstream>
using namespace std;

class DayOfYear {
    private:
        string monthOfYear;
        int daysInYear;
    public:
        DayOfYear(int userInputDay)
        {
            if(userInputDay<=31)
            {
                monthOfYear="January";
                daysInYear=userInputDay;
            }
            else if(userInputDay<=59)
            {
                monthOfYear="February";
                daysInYear=userInputDay-31;
            }
            else if(userInputDay<=90)
            {
                monthOfYear="March";
                daysInYear=userInputDay-59;            
            }
            else if(userInputDay<=120)
            {
                monthOfYear="April";
                daysInYear=userInputDay-90;
            }
            else if(userInputDay<=151)
            {
                monthOfYear="May";
                daysInYear=userInputDay-120;
            }
            else if(userInputDay<=181)
            {
                monthOfYear="June";
                daysInYear=userInputDay-151;           
            }
            else if(userInputDay<=212)
            {
                monthOfYear="July";
                daysInYear=userInputDay-181;
            }
            else if(userInputDay<=243)
            {
                monthOfYear="August";
                daysInYear=userInputDay-212;
            }
            else if(userInputDay<=273)
            {
                monthOfYear="September";
                daysInYear=userInputDay-243;
            }
            else if(userInputDay<=304)
            {
                monthOfYear="October";
                daysInYear=userInputDay-273;

            }
            else if(userInputDay<=334)
            {
                monthOfYear="November";
                daysInYear=userInputDay-304;
            }
            else
            {
                monthOfYear="December";
                daysInYear=userInputDay-334;
            }
        }
        void print(int userInputDay, int x)
        {
            ofstream outFile("output_1.txt",ios::app);
            if(userInputDay<1 ||userInputDay>365)
            {
            outFile << "#" << x << " -1" << "\n";
            }
            else
            {
            outFile << "#" << x << " Day " << userInputDay << " would be " << monthOfYear << " " << daysInYear<< "." << "\n";
            }
        }
};

int main()
{
    int classnumber=1;
    int numberofcase;
    int num;
    
    ifstream inFile;
    inFile.open("input_1.txt");
    ofstream outFile;
    outFile.open("output_1.txt");

    inFile >> numberofcase;
    while(inFile >> num)
    {
        DayOfYear result(num);
        result.print(num, classnumber);
        classnumber++;
    }
    inFile.close();
    outFile.close();
    return 0;
}