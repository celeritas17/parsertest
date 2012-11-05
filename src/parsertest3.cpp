//
//  parsertest4.cpp
//  parsertest4
//
//  Created by Ryan Koven on 10/30/12.
//  Copyright (c) 2012 Ryan Koven. All rights reserved.
//

#include <iostream>
#include <string>
#include <vector>

using namespace std;

string eval(const vector<string>& input);

vector<string> split_arg_list(const string input){
    vector<string> args;
    string next_arg = "";
    int i = 0;
    
    while(i < input.size()){
        while(input[i] != '[' && input[i] != ']' && input[i] != ',' && i < input.size())
            next_arg += input[i++];
        
        if (next_arg != ""){
            args.push_back(next_arg);
            next_arg = "";
        }
        i++;
    }
    
    return args;
}

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

string eval_body(const vector<string>& input, string arg, string bound_var, int forward_counter, int embed_level) {
    string body_expr = input[forward_counter++] + " ";
    
    while(embed_level > 1 || input[forward_counter] == "("){
        
        if (input[forward_counter] == "(")
            embed_level++;
        
        if (input[forward_counter] == ")")
            embed_level--;
        
        if (input[forward_counter] == bound_var){
            body_expr += arg + " ";          //////////Danger!
            forward_counter++;
        }
        else {
            body_expr += input[forward_counter++] + " ";
        }
    }
    
    if (input[forward_counter] == bound_var)
        body_expr += arg + " ";
    else if (input[forward_counter] != ")")
        body_expr += input[forward_counter] + " "; /////////test!
    
    cout << body_expr << "\n";
    
    return eval(parse(body_expr));   ///////////Test!
}

string get_body_expr(const vector<string>& input){
    string output = "";
    
    int embed_level = 0;    // Balance check for parentheses.
    int forward_counter = 0;
    
    if (input[0] == "("){
        embed_level++;
        output += input[forward_counter++] + " ";
        
        while(embed_level > 0 && forward_counter < input.size()){
            if (input[forward_counter] == "(")
                embed_level++;
            else if (input[forward_counter] == ")")
                embed_level--;

            output += input[forward_counter++] + " ";
        }
    }
    else
        output = to_string(input, " ");
    
    return output;
}

string eval_args(const vector<string>& input){
    
    string next_arg;
    
    string next_fun = get_body_expr(input);
    
    cout << "next_fun: " << next_fun << "\n";
    
    vector<string> args = split_arg_list(find_arg_expr(input, 0));
    
    if (args.size() > 0){
    
        for (int i = 0; i < args.size(); i++){
            cout << "args[i] " << args[i] << "\n";
            next_fun = eval(parse(next_fun + args[i]));
        }
        
    }
    
    return next_fun;
}

string eval(const vector<string>& input){
    
    int embed_level = 0;    // Balance check for parentheses.
    string bound_var, body_expr, arg_expr, arg;
    string output = "";
    //const int NUM_TOKENS = input.size();
    
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
                    output = eval_body(input, arg, bound_var, forward_counter, embed_level);
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
    
    string test = "(lambda x x)";
    vector<string> args = split_arg_list(test);
    
    for (int i = 0; i < args.size(); i++)
        cout << args[i] << "\n";
    
    
    while (test_string.find(EOF) == test_string.npos){
        
        cout << "lambda_fun> ";
        
        getline(cin, test_string);
        
        tokens = parse(test_string);
        
        //for (int i = 0; i < tokens.size(); i++)
        //cout << tokens[i] << "\n";
        
        cout << eval_args(tokens) << "\n";
        
    }
    
    
    return 0;
}

