#include <iostream>
#include <fstream>
using namespace std;
int count(unsigned long long int x)
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
    inFile.open("input_3.txt");
    ofstream outFile;
    outFile.open("output_3.txt");

    inFile >> case_number;
    while(problem_number, inFile >> num1 >> num2)
    {
        if(num1 <=0 || num2 <=0 || num2 < num1)
        {
            outFile << "#" << problem_number << " -1" << endl;

        }
        else
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

        }
        problem_number++;
    }
    return 0;
}