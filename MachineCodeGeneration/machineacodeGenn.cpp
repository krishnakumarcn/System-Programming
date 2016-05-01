/*
krishnakumarcn
*/
#include<iostream>
#include<sstream>
#include<fstream>
#include<list>
using namespace std;

int main(){
	list<string> expn;
	string line,elt;
	ifstream fin("input.txt");
	while(getline(fin,line)){
		stringstream ss(line);
		while(getline(ss,elt,' ')){
			expn.push_back(elt);
		}
		string first=expn.front();
		expn.pop_front();
		expn.pop_front();	//=
		string second=expn.front();
		expn.pop_front();
		cout<<"MOV "<<first<<","<<second<<endl;
		if(!expn.empty()){
			string op=expn.front();
			expn.pop_front();
			if(op=="+")
				cout<<"ADD ";
			else if(op=="-")
				cout<<"SUB ";
			else if(op=="*")
				cout<<"MUL ";
			else if(op=="/")
				cout<<"DIV ";
			second=expn.front();
			expn.pop_front();
			cout<<first<<","<<second<<endl;
		}
	}
}
