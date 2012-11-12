//
//  lambda.cpp
//  
//
//  Created by Ryan Koven on 10/30/12.
//  Copyright (c) 2012 Ryan Koven. All rights reserved.
//

#include <iostream>
#include <string>
#include <vector>
#include "lambda.h"

using namespace std;

int main(int argc, const char * argv[])
{
    string test_string;
    vector<string> tokens;
    
    while (test_string.find(EOF) == test_string.npos){
        
        cout << "lambda_fun> ";
        
        getline(cin, test_string);

        tokens = tokenize(test_string);
        
        cout << "\nNormal Form: " << eval_args(tokens) << "\n\n";
        
    }
    
    
    return 0;
}


