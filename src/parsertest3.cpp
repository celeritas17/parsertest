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

string eval(const vector<string>& input);

string to_string(const vector<string>& input, string sep, int start, int end) {
    string output;
    for (int i = start; i < end; i++) {
        output += input[i] + sep;
    }
    return output;
}

string to_string(const vector<string>& input, int start, int end) {
  return to_string(input, "", start, end);
}

string to_string(const vector<string>& input, string sep) {
  return to_string(input, sep, 0, input.size());
}

string to_string(const vector<string>& input) {
  return to_string(input, "", 0, input.size());
}

string strip_outer_parens(const string& expr){
    string output = "";
    
    if (expr[0] == '(' && expr[expr.size() - 2] == ')')
        output = expr.substr(1, expr.size() - 3);
        
    else
        output = expr;
    
    return output;
}


vector<string> parse(const string& input){
    
    vector<string> tokens;
    
    int i = 0;
    
    string next_token = "";
    
    string buffer = "";
    
    while (i < input.size()) {
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
        i++;
    }
    
    return tokens;
}

string find_arg_expr(const vector<string>& input, int embed_level) {

    int forward_counter = 1;
    string arg_expr = "";

    if (input[0] == "(") {
        embed_level++;
        while(embed_level != 0){
            
            if (input[forward_counter] == "(")
                embed_level++;
            else if (input[forward_counter] == ")")
                embed_level--;
            
            if (forward_counter > input.size() - 1){
                break;
            }
            else
                forward_counter++;
            
        } // end while
        
        while (forward_counter < input.size())
            arg_expr += input[forward_counter++] + " ";
    }
    return arg_expr;
}

string eval_arg(const vector<string>& input) {
    
    string arg, arg_expr;
    int forward_counter = 1;

    arg_expr = find_arg_expr(input, 0);

    if (arg_expr != "")
        arg = eval(parse(arg_expr)); // eval arg_expr first
    else
        arg = arg_expr;
    
    cout << "ARG:" << arg << "\n";

    return arg;
}

string eval(const vector<string>& input){
    
    int embed_level = 0;    // Balance check for parentheses.
    string bound_var, body_expr, arg_expr, arg;
    string output = "";
    const int NUM_TOKENS = input.size();

    arg = eval_arg(input);
    
    int forward_counter = 1;
        
    if (input[0] == "("){
        
        embed_level++;
        
        while(input[forward_counter] == "("){
            
            embed_level++;
            forward_counter++;
        }
                
        if (input[forward_counter] == "lambda"){// The bound variable is always the next token after the (first) lambda
            
            bound_var = input[forward_counter + 1];
            
            cout << bound_var<< "\n" << forward_counter << "\n";
            
            if (input[forward_counter + 2] == "("){
                
                embed_level++;
                
                if (arg != ""){
                
                    forward_counter += 2;
                
                    //embed_level++;
                
                    body_expr += input[forward_counter++] + " "; // Advance counter to start of body expression and start
                    // building body expression.
                
                    while(embed_level > 1 || input[forward_counter] == "("){
                    
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
                                        
                }
            }
            else if (input[forward_counter + 2] == bound_var){
                if (arg != "")
                    output = arg;
            }
            else {  // this means that the body is not a function application or another abstraction
                if (arg != "")
                    output = input[forward_counter + 2];
            }
        } 
    }

    if (output == "")
      output = to_string(input, " ");

    return output;
}


int main(int argc, const char * argv[])
{
    string test_string;
    vector<string> tokens;
    
    //string test = "(test)";
 
    //if (test[0] == '(' && test[test.size() - 1] == ')')
      //  cout << strip_outer_parens(test) << "\n";
    
    
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

