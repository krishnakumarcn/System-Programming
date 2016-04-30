#include<iostream>
#include<fstream>
#include<map>
#include<list>
#include<set>
#include<sstream>
using namespace std;

class FnF{
public:

	set<string>first;
	set<string>follow;
	FnF(){

	}
	void printFirst(){
		for(set<string>::iterator itr=first.begin();itr!=first.end();itr++)
			cout<<*itr<<" ";
		cout<<endl;
		
	}
	void printFollow(){
		for(set<string>::iterator itr=follow.begin();itr!=follow.end();itr++){
			cout<<*itr<<" ";
		cout<<endl;
		}
	}
};

map<string,FnF>element;
map<string, list< list<string> > > productions;

list<list<string> > getProduction(string line){
	list<list<string> > prodxn;
	int pos=line.find("-->")+3;
	stringstream ss(line.substr(pos,line.length()-pos));
	string p;
	while(getline(ss,p,'|')){
		list<string> prod;
		stringstream pstream(p);
		string elt;
		while(getline(pstream,elt,' ')){
			if(elt!=""){
				prod.push_back(elt);
			}
		}
		prodxn.push_back(prod);
	}
	return prodxn;
}


int isTerminal(string t){
	if(!isupper(t.at(0)))
		return 1;
	return 0;
}

void addFirst(string lhs){
	list< list<string> > prods=productions[lhs];
	for(auto i=prods.begin();i!=prods.end();i++){
		for(auto j=i->begin();j!=i->end();j++){
			if(isTerminal( (*j)) ){
				element[lhs].first.insert(*j);
				break;
			}
			else{
				addFirst(*j);
				bool repeat=false;
				for(auto k=element[*j].first.begin();k!=element[*j].first.end();k++){
					if( (*k)=="e"){
						repeat=true;
					}
					else{
						element[lhs].first.insert(*k);
					}
				}
				if(!repeat){
					break;
				}
			}

		}
	}

}



void createFirst(){
	for(auto itr=productions.begin();itr!=productions.end();itr++){
		addFirst(itr->first);
	}
}

string getElement(string line){
	stringstream s(line);
	string el;
	getline(s,el,' ');
	return el;
}



int main(){
	ifstream fin("input.txt");

	string line,lhs;
	getline(fin,line);
	stringstream getLhs(line);
	getLhs>>lhs;
	productions[lhs]=getProduction(line);
	element[lhs]=FnF();
	element[lhs].follow.insert("$");

	while(getline(fin,line,'\n')){
		stringstream lhsGet(line);
		lhsGet>>lhs;
		productions[lhs]=getProduction(line);
		element[lhs]=FnF();
	}
	createFirst();
	
	for(auto itr=element.begin();itr!=element.end();itr++){
		cout<<"\n"<< itr->first <<" :"<<endl;
		cout<<"\tFirst: ";
		(itr->second).printFirst();
	}



}