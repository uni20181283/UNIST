#ifndef TO_POSTFIX_H
#define TO_POSTFIX_H

#include <iostream>
#include <sstream>
#include "stack.h"

using namespace std;
string remake(string input){
    string midput;
    for(int i = 0 ; i < input.size() ; i++){
        if(input[i] == ' ')
            continue;
        else
            midput += input[i];
    }
    string output;
    output+=midput[0];
    for(int i =1 ; i < input.size();i++){
        if(midput[i] == '(' && midput[i-1] == '-' ){
            output += '1';
            output += '*';
        }
        output += midput[i];
    }
    return output;
}
string to_postfix(const string& infix) {
    string result = remake(infix);
    Stack<char> stack;
    string postfix;
    for(int i = 0; i < result.size() ; i++){
        if(i == 0){
            if(result[i] == '-'){
                postfix += 'k';
            }
            else if(result[i] == '('){
                stack.push(result[i]);
            }
            else{
                postfix += result[0];
            }
        }

        else if(result[i] >= '0' && result[i] <= '9'){
            if(result[i-1] >= '0' && result[i-1] <= '9'){
                postfix += result[i];
            }
            else if(postfix[postfix.size()-1] == 'k'){
                postfix[postfix.size()-1] = '-';
                postfix += result[i];
            }
            else{
                postfix += ' ';
                postfix += result[i];
            }
        }
        else{
            if( (result[i-1] == '+' || result[i-1] == '/' || result[i-1] == '*' || result[i-1] == '-' || result[i-1] == '(') && result[i] == '-' ){
                postfix += ' ';
                postfix += 'k';
                continue;
            }
            if(result[i] == ')'){
                while(stack.top() != '('){
                    postfix += ' ';
                    postfix += stack.top();
                    stack.pop();
                }
                stack.pop();
            }
            else if(stack.empty() || result[i] == '(')
                stack.push(result[i]);
            else if( result[i] == '+' || result[i] == '-'){
                while(!stack.empty() && stack.top() != '(' ){
                    postfix += ' ';
                    postfix += stack.top();
                    stack.pop();
                }
                stack.push(result[i]);
            }
            else if( result[i] == '*' || result[i] == '/'){
                while( !stack.empty() && stack.top() != '(' && stack.top() != '+' && stack.top() != '-'){
                    postfix += ' ';
                    postfix += stack.top();
                    stack.pop();
                }
                stack.push(result[i]);
            }
        }
    }
    while(!stack.empty()){
        postfix += ' ';
        postfix += stack.top();
        stack.pop();
    }
    if(postfix[0] == ' '){
        postfix = postfix.substr(1);
    }
    return postfix;
}


#endif //TO_POSTFIX_H
