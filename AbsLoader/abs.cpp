#include<iostream>
#include<map>
#include<sstream>
#include<fstream>
#include<algorithm>
using namespace std;

int main(){
  map<int,string> memory;
  string line,record,pgmName;
  int startAdd,pgmstartAdd,pgmLen,recLen;
  ifstream fin("inputAbs.txt");
  while(getline(fin,line,'\n')){
    replace(line.begin(),line.end(),'^',' ');
    if(line[0]=='H'){
      stringstream ss(line.substr(2,line.length()-2));
      ss>>hex>>pgmName>>pgmstartAdd>>pgmLen;     
    }
    else if(line[0]=='T'){
      stringstream ss(line);
      string temp;
      ss>>temp;
      ss>>hex>>startAdd>>recLen;
      stringstream recStream(line.substr(12,line.length()-12));
      while(getline(recStream,record,' ')){
	for(int j=0;j<record.length();j=j+2){
	  memory[startAdd]=record.substr(j,2);
	  startAdd++;
	}
      }
    }
    else if(line[0]=='E')
	  break;
  }
  for(auto itr=memory.begin();itr!=memory.end();itr++){
    cout<<hex<< itr->first <<" : "<< itr->second <<endl;
  }

  return 0;
}
