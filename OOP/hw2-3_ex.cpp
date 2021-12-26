#include <iostream>
#include <fstream>
using namespace std;
int count(int x)
{
    int count =0;
    while(x>1)
    {
        if(x%2==0)
        {
            x=x/2;
            count++;
        }
        else if(x%2 != 0 )
        {
            x= (x*3)+1;
            count++;
        }
    }
    int result = count+1;
    return result;
}

int main()
{
    int case_number;
    int num1, num2;
    unsigned long long int result;
    int problem_number=1;
    ifstream inFile;
    inFile.open("input.txt");
    ofstream outFile;
    outFile.open("output.txt");

    inFile >> case_number;
    while(problem_number, inFile >> num1 >> num2)
    {
        result=count(num1);
        for(num1+1; num1<num2; num1++)
        {
            if(count(num1) > result)
            {
                result = count(num1);
            }
        }
        outFile << "#" << problem_number << " " << result << endl;
        problem_number++;
    }
    return 0;
}