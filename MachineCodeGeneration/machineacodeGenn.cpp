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
			string third=expn.front();
			expn.pop_front();
			if(third=="+")
				cout<<"ADD ";
			else if(third=="-")
				cout<<"SUB ";
			else if(third=="*")
				cout<<"MUL ";
			else if(third=="/")
				cout<<"DIV ";
			string fourth=expn.front();
			expn.pop_front();
			cout<<first<<","<<fourth<<endl;
		}
	}
}
