
#include <iostream>
#include <fstream>
#include <cmath>
using namespace std;
int main()
{
int pb_num;
float base, power;
ifstream inFile;
inFile.open("input.txt");
ofstream outFile;
outFile.open("output.txt");
inFile >> pb_num;
cout << pb_num << endl;
int row=1;
while(pb_num, inFile >> base >> power)
{
cout << base << " " << power << endl;
outFile << "#" << row << " " << pow(base,power) << endl;
row +=1;
}
inFile.close();
outFile.close();
return 0;
}