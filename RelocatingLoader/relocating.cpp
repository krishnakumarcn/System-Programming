#include<iostream>
#include<sstream>
#include<fstream>
#include<map>
#include<algorithm>
using namespace std;

int main(){
	string line,pgmName,temp;
	string record;
	string startAdd;
	int startAddInt;

		cout<<"\nEnter the relocating address:";
	cin>>startAdd;
	stringstream intStram;
	intStram<<hex<<startAdd;
	intStram>>startAddInt;
	map<int,string> memory;

	ifstream fin("INPUT_RL.DAT");
	while(getline(fin,line,'\n')){
		if(line[0]=='T'){
			replace(line.begin(),line.end(),'^',' ');
			stringstream s(line);
			s>>temp;
			int address,length,mask;
			s>>hex>>address>>length>>mask;
			stringstream ss(line.substr(16,line.length()-16));
			while(getline(ss,record,' ')){
				if(( mask & 0x800 ) == 0x800){
					cerr<<"Debug: "<<record;	
					stringstream recStream;
					recStream<<hex<<record;
					int tempVal;
					recStream>>tempVal;
					tempVal+=startAddInt;
					record="";
					stringstream revStream;
					revStream<<hex<<tempVal;
					revStream>>record;
				}
				mask=mask<<1;
				for(int j=0;j<record.length();j=j+2){
					memory[address+startAddInt]=record.substr(j,2);
					address++;
				}


			}
		}
		else if(line[0]=='E'){
			break;
		}
		else if(line[0]=='H'){
			pgmName=line.substr(2,6);
		}

	}
	cout<<"\n Program Name: "<<pgmName<<endl;
	for(auto itr=memory.begin();itr!=memory.end();itr++){
		cout<<hex<< itr->first<<":\t"<<itr->second<<endl;
	}
}