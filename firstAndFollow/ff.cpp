/*
krishnakumarcn
*/
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
	void print(){
		cout<<"First:"<<endl<<"\t";
		for(set<string>::iterator itr=first.begin();itr!=first.end();itr++)
			cout<<*itr<<" ";
		cout<<endl<<"Follow:"<<endl<<"\t";
		for(set<string>::iterator itr=follow.begin();itr!=follow.end();itr++)
			cout<<*itr<<" ";
		cout<<endl;
		
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
		for(auto j=(*i).begin();j!=(*i).end();j++){
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

void createFollow(){
	bool changed=false;
	for(auto i=productions.begin();i!=productions.end();i++){
		string var=i->first;
		for(auto j=i->second.begin();j!=i->second.end();j++){
			for(auto k=j->begin();k!=j->end();k++){
				if(isTerminal(*k))
					continue;
				else{
					auto next=k;
					next++;
					bool repeat=true;//for the first case on while make it true here
					for(;next!=j->end()&&repeat;next++){
						repeat=false;
						if(isTerminal(*next)){
							repeat=false;
							//insertion on set returns <pointer,bool> bool=true if success
							if(element[*k].follow.insert(*next).second){//
								changed=true;
							}
						}
						else{
							for(auto l=element[*next].first.begin();l!=element[*next].first.end();l++){
								if(*l != "e"){
									repeat=false;
									if(element[*k].follow.insert(*l).second){//
										changed=true;
									}
								}
								else{
									repeat=true;
								}
							}
						}
					}
					//if the pointer reaches end and still repeat??
					if(repeat){
						for(auto m=element[var].follow.begin();m!=element[var].follow.end();m++){
							if(*m !="e"){
								if(element[*k].follow.insert(*m).second)//
									changed=true;
							}
						}

					}
				}
			}
		}
	}
	if(changed){
		createFollow();
	}
}


/*
void createFollow(){
	bool changed = false;
	for(map<string,list<list<string > > >::iterator i=productions.begin();i!=productions.end();i++){
		string var = i->first;
		for(list<list<string > >::iterator j=i->second.begin();j!=i->second.end();j++){
			for(list<string>::iterator k=j->begin();k!=j->end();k++){
				if(isTerminal(*k)) continue;
				list<string>::iterator next = k;
				next++;
				bool repeat = true;
				while(next!=j->end()&&repeat){
					repeat = false;
					if(isTerminal(*next)){
						repeat = false;
						if(element[*k].follow.insert(*next).second)
							changed = true;
					}
					else{
						for(set<string>::iterator l=element[*next].first.begin();l!=element[*next].first.end();l++){
							if(*l=="e")
								repeat = true;
							else{
								if(element[*k].follow.insert(*l).second)
									changed = true;
							}
						}
					}
					next++;
				}
				if(repeat){
					// cout<<"Repeat : "<<var<<" "<<*k<<endl;
					for(set<string>::iterator l=element[var].follow.begin();l!=element[var].follow.end();l++){
						if(*l!="e"){
							if(element[*k].follow.insert(*l).second)
								changed = true;
						}
					}
				}
			}
		}
	}
	if(changed){
		createFollow();
	}
}

*/

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
	createFollow();
	
	for(auto itr=element.begin();itr!=element.end();itr++){
		cout<<"\n"<< itr->first <<" :"<<endl;
		(itr->second).print();
	}



}