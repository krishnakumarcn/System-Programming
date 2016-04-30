#include <iostream>
#include <string>
using namespace std;

/*

Removing left recursion:
	A->Aa|b
	A->bA'
	A'->aA'


E  --> TE'
E' --> +TE' | epsilon 
T  --> FT'
T' --> *FT' | epsilon
F  --> (E) | x
*/

void e();
void ebar();
void t();
void tbar();
void f();

int position = 0;
string input="(x*x+)x*x";

void e(){
	t();
	ebar();
}
void t(){
	f();
	tbar();
}
void f(){
	if( input[position]=='(' ){
		position++;
		e();
		if(input[position]==')')
			position++;
		else return;
	}
	else if(input[position]=='x'){
		position++;
	}
	else
		return;
}
void tbar(){
	if(input[position]=='*'){
		position++;
		f();
		tbar();
	}
}

void ebar(){
	if(input[position]=='+'){
		position++;
		t();
		ebar();
	}
}

/*
E  --> TE'
E' --> +TE' | epsilon 
T  --> FT'
T' --> *FT' | epsilon
F  --> (E) | x
*/

int main(){
	e();
	if(position==input.length())
		cout<<"Accepted!! "<<input;
	else
		cout<<"Not Accepted!! "<<input;

}