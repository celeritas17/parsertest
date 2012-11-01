//
//  parsertest3.cpp
//  parsertest3
//
//  Created by Ryan Koven on 10/30/12.
//  Copyright (c) 2012 Ryan Koven. All rights reserved.
//

#include <iostream>
#include <string>
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
    
    
    int embed_level = 0;    // Balance check for parentheses.
    string bound_var, body, body_expr, arg_expr, arg, inner_arg, next_lambda, output;
    const int NUM_TOKENS = input.size();
    
    int forward_counter = 1;
    
    int backward_counter = NUM_TOKENS - 1;
    
    
    // Extract the argument expression, then simplify it.
    ///////////////////////////////////////
    
    if (input[0] == "("){               // the first token in the input must be an opening paren, or else it's invalid.
        
        embed_level++;
    
        while(embed_level != 0){
            
            if (input[forward_counter] == "(")
                embed_level++;
            else if (input[forward_counter] == ")")
                embed_level--;
            
            if (forward_counter > NUM_TOKENS - 1){
                output = "Error: Unbalanced parentheses";  //handle differently
                break;
            }
            else
                forward_counter++;
            
        } // end while
        
        while (forward_counter < NUM_TOKENS)
            arg_expr += input[forward_counter++] + " ";
            
        if (arg_expr != "")
            arg = eval(parse(arg_expr));
        else
            arg = arg_expr;
        cout << arg << "\n";
        
    } // end if
    
    else{
        output = "Error: No opening paren.";
    }
    
    ///////////////////////////////////////
    
    
    // Extract bound variable and body expression.
    //////////////////////////////////////
    
    //forward_counter = 1;
    embed_level = 0;
    
  
    
    
        
    if (input[0] == "("){ 
        
    
            embed_level++;
        if (input[1] == "lambda"){// The bound variable is always the next token after the (first) lambda
            
            
            
            bound_var = input[2];
            
            
            cout << bound_var<< "\n";
            
            if (input[3] == "("){
                
                if (arg != ""){
                
                forward_counter = 3;
                
                embed_level++;
                
                body_expr += input[forward_counter++] + " "; // Advance counter to start of body expression and start
                // building body expression.
                
                while(embed_level > 1){
                    
                    if (input[forward_counter] == "(")
                        embed_level++;
                    
                    if (input[forward_counter] == ")")
                        embed_level--;
                    
                    if (input[forward_counter] == bound_var){
                        body_expr += arg + " ";          //////////Danger!
                        forward_counter++;
                    }
                    
                    else
                        body_expr += input[forward_counter++] + " ";
                    
                    
                    //cout << "still going\n";
                    
                }  //end while
                
                if (input[forward_counter] == bound_var)
                    body_expr += arg + " ";
                else if (input[forward_counter] != ")")
                    body_expr += input[forward_counter] + " "; /////////test!
                
                cout << body_expr << "\n";
                
                //output = eval(parse("(lambda " + bound_var + " " + eval(parse(body_expr)) + " " + arg_expr + ")"));
                
                    output = eval(parse(body_expr));   ///////////Test!
                    
                }// end if
                
                else {
                    for (int k = 0; k < NUM_TOKENS; k++)
                        output += input[k] + " ";   /////////////////////////Danger!
                }
                
            }  //end if
            
            else if (input[3] == bound_var){
                //cout << "input[3] == bound_var\n";
                if (arg != ""){
                    output = arg;
                    //cout << "arg_expr not null\n";
                }
                else{
                    for (int k = 0; k < NUM_TOKENS; k++)
                        output += input[k] + " ";
                }
            }
            
            else {  // this means that the body is not a function application or another abstraction
                
                output = input[3];
                //body_expr = bound_var;
                //cout << "Failed\n";
                
            }
            
            //output = body_expr;
            
        }
        
        else{
            
            for (int j = 0; j < NUM_TOKENS; j++)
                output += input[j] + " ";
        
        }
        //output = body_expr;
        
    }
        else
            output = input[0];


    //////////////////////////////////////
    
    //output += body_expr;
    
    return output;
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
          //cout << tokens[i] << "\n";
        
       cout << eval(tokens) << "\n";
        
    }


        return 0;
}

