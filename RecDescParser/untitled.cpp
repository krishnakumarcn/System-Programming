#include<iostream>
#include<string>


/*
E  --> TE'
E' --> +TE' | epsilon 
T  --> FT'
T' --> *FT' | epsilon
F  --> (E) | x
*/

string input;
int position=0;

void e(){
	t();
	ebar();

}

void t(){
	f();
	tbar();

}
void f(){
	if(input[position]=="(")
		position++;
	else if(input[position]=="x")
		position++;
	else{
		cout<<"Rejected!!";
		return;
	}
	e();
	if(input[position]==")")
		position++;
	else
		return;
}

void tbar(){
	if(input[position]=="*")
		position++;
	f();
	tbar();

}

void ebar(){
	if(input[position]=="+")
		position++;
	t();
	ebar();
}

int main(){
	input=""
}

/*
09266623333*/