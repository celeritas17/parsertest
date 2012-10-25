//
//  main.cpp
//  parsertest2
//
//  Created by Ryan Koven on 10/24/12.
//  Copyright (c) 2012 Ryan Koven. All rights reserved.
//

#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <vector>

using namespace std;

vector<string> parse(const string& input){
    
    vector<string> tokens;
    
    int i = 0;
    
    string next_token = "";
    
    string buffer = "";
    
    while (i < input.size()) {
        if (input[i] != ' ' && input[i] != '\t' && input[i] != '\n'){
            while (input[i] != ' ' && input[i] != '\t' && input[i] != '\n' && i < input.size()){
                if ((input[i] == '(' || input[i] == ')') && next_token != ""){
                    tokens.push_back(next_token);
                    tokens.push_back(buffer += input[i++]);
                    buffer = "";
                    next_token = "";
                }
                else if ((input[i] == '(' || input[i] == ')') && next_token == ""){
                    tokens.push_back(buffer += input[i++]);
                    buffer = "";
                }
                else
                    next_token += input[i++];
            }
            if (next_token != ""){
                tokens.push_back(next_token);
                next_token = "";
            }
        }
        i++;
    }
    
    return tokens;
}


string eval(const vector<string>& input){
    
    //vector<string> tokenized;
    int embed_level = 0;
    string bound_var = "";
    string body = "";
    string body_expr = "";
    string arg_expr = "";
    string arg = "";
    string inner_arg = "";
    string next_lambda = "";
    string output = "";
    
    
    int i = 0;
    
    while (i < input.size()){
        
        if (input[i] == "(" && input[i + 1] == "lambda"){
            bound_var = input[i + 2];
            if (input[i + 3] == "("){
                embed_level++;
                i += 3;
                for (int j = i; embed_level > 0; j++){
                    body_expr += input[j] + " ";
                    i++;
                    if (input[j] == "(")
                        embed_level++;
                    else if (input[j] == ")")
                        embed_level--;
                }
                
                //body_expr += input[i++];
            
                if (input[i] != ")"){
                    inner_arg = input[i++];
                    body_expr += " " + inner_arg;
                }
                
               // cout << body_expr << "\n";
                
                body = eval(parse(body_expr));
                
                //cout << body << "\n";
                
                for (int k = i; input[k] == ")"; k++)
                    i++;
                for (int l = i; l < input.size(); l++){
                    arg_expr += input[l] + " ";
                    i++;
                }
                
                arg = eval(parse(arg_expr));
                
                //cout << arg << "\n";
                
                next_lambda = "(lambda " + bound_var + " " + body + ")" + arg;
                
                //cout << next_lambda  << "\n";
                
                output = eval(parse(next_lambda));
            }
            
            else {
                
                                
                bound_var = input[i + 2];
                body = input[i + 3];//while (input[i] != ")")
                i += 3;
                for (int k = i; input[k] != ")"; k++)
                        i++;
                for (int l = i + 1; l < input.size(); l++){
                    arg_expr += input[l] + " ";
                    i++;
                }
                
                if (arg_expr == ""){
                    string input_string;
                    
                    for (int a = 0; a < input.size(); a++)
                        input_string += input[a] + " ";
                    
                    output = input_string;
                
                }
                else if (bound_var == body)
                    output = eval(parse(arg_expr));
                else
                    output = body;
                
                
            //i++;
            
            }
        }
        
        else {
            for (int b = 0; b < input.size(); b++)
                output += input[b] + " ";
            break;
        }
        
        
        
        i++;
        
    }
    
    
    return output;
}

vector<string> tokenize(const string& input){
    
    vector<string> tokens;
    istringstream stream(input);
    
    copy(istream_iterator<string>(stream), istream_iterator<string>(), back_inserter(tokens));

    return tokens;
}

int main(int argc, const char * argv[])
{
    
    string test_string;
    vector<string> tokens;
    
    while (test_string.find(EOF) == test_string.npos){
        
        cout << "lambda_fun> ";
        
        getline(cin, test_string);

        tokens = parse(test_string);
        
        //for (int i = 0; i < tokens.size(); i++)
          //  cout << tokens[i] << "\n";
        
        cout << eval(tokens) << "\n";
    
    }
    return 0;
}

