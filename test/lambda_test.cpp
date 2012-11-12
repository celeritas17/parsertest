#include <iostream>
#include "../src/lambda.h"
#include <assert.h>

using namespace std;

int main(int argc, char** argv){

	assert(eval_args(tokenize("(lambda x x)k")) == "k ");
	
	assert(eval_args(tokenize("k")) == "k ");
	
	assert(eval_args(tokenize("(k)")) == "( k ) ");

	assert(eval_args(tokenize("(lambda func (lambda arg (func)arg))(lambda x x)")) == "( lambda arg ( ( lambda x x ) ) arg ) ");

	assert(eval_args(tokenize("(lambda func (lambda arg (func)arg))[(lambda x x), (lambda s (s)s)]")) == "( lambda s ( s ) s ) ");

	assert(eval_args(tokenize("(lambda s (s)s)(lambda x x)")) == "( lambda x x ) ");

	assert(eval_args(tokenize("(lambda first (lambda second (lambda func (func)[first,second])))[(lambda x x), (lambda f (lambda a (f)a))]")) == 
		"( lambda func ( func ) [ ( lambda x x ) , ( lambda f ( lambda a ( f ) a ) ) ] ) ");

	assert(eval_args(tokenize("(((lambda x x)))k")) == "k ");

	assert(eval_args(tokenize("(lambda first (lambda second (lambda func (func)[first,second])))[(lambda x x), (lambda f (lambda a (f)a)), (lambda y y)]")) == 
		"( lambda f ( lambda a ( f ) a ) ) ");

	assert(eval_args(tokenize("(lambda first (lambda second (lambda func (func)[first,second])))[(lambda x x), (lambda f (lambda a (f)a)), (lambda one (lambda two one))]")) == 
		"( lambda x x ) ");

	assert(eval_args(tokenize("(lambda first (lambda second (lambda func (func)[first,second])))[(lambda x x), (lambda f (lambda a (f)a)), (lambda one (lambda two two))]")) == 
		"( lambda f ( lambda a ( f ) a ) ) ");

	return 0;
}
