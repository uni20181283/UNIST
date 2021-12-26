// 2. Find the missing number
#include <iostream>
#include <fstream>
using namespace std;


int main()
{
    int classnumber=1;
    int numberofcase;
    int num;

    ifstream inFile;
    inFile.open("input_2.txt");
    ofstream outFile;
    outFile.open("output_2.txt");

    inFile >> numberofcase;
    while(inFile >> num)
    {
        if(num>1&&num<10000001)
        {
            int i=0;
            int arr[i];
            for(i=0;i<num;i++)
            {
                inFile >> arr[i];
            }
            int temp;
            for(i=0;i<num;i++)
            {
                for(int j=0;j<num-i-1;j++)
                {
                if(arr[j]>arr[j+1])
                    {
                        temp=arr[j];
                        arr[j]=arr[j+1];
                        arr[j+1]=temp;
                    }
                }
            }
            int result=0;
            for(i=0;i<num-1;i++)
            {
                if((arr[i]+1)!=arr[i+1])
                {
                    result =  arr[i]+1;
                }
            }
            if(result==0)
            result=arr[0]-1;
            
            outFile << "#" << classnumber << " " << result << "\n";
        }
        else
        {
            for(int i=0;i<num;i++)
            inFile>>num;
            cout << num;
            outFile << "#" << classnumber << " -1\n";
        }

        classnumber++;
    }
    inFile.close();
    outFile.close();
    return 0;
}
// int arr로 한줄로 세우고 
// arr를 크기 순으로 정렬
// 행과 행 사이의 크기가 1이 아니면 행값+1