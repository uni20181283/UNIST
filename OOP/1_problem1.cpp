#include <iostream>
#include <fstream>
using namespace std;
int main()
{
    int case_number;
    int number1, number2, number3;
    ifstream inFile;
    inFile.open("input_1.txt");
    ofstream outFile;
    outFile.open("output_1.txt");
    
    inFile >> case_number;
    cout << case_number << endl;
    int num=1;
    while(num, inFile >> number1 >> number2 >> number3)
    {
        cout << number1 << " " << number2 << " " << number3 << endl;
        outFile << "#" << num << " " << number1 + number2 + number3 << endl;
        num +=1;
    }

    inFile.close();
    outFile.close();

    return 0;
}