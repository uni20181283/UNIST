#include <iostream>
#include <fstream>
#include <cmath>
using namespace std;
int main()
{
    int case_number;
    int problem_number=1;
    int num;
    ifstream inFile;
    inFile.open("input_2.txt");
    ofstream outFile;
    outFile.open("output_2.txt");
    inFile >> case_number;
    while(problem_number, inFile >> num)
    {
        if(num <0)
        {
            outFile << "#" << problem_number << " -1" << endl;
            problem_number ++;
        }
        else
        {
            unsigned long long int solution=0;
            for(int a=1; a<=num ; a++)
            {
            unsigned long long int result=0;
            unsigned long long int sum=0;
                for(int b=0; b<a ; b++)
                {
                    result = round(pow(10,b));
                    sum += result;
                }
            solution += sum;
            }
            outFile << "#" << problem_number << " " << solution << endl;
            problem_number++;
        }
    }
    inFile.close();
    outFile.close();
    return 0;
}