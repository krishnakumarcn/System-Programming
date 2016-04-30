/*
macroProcessor
krishnakumarcn
*/
#include<iostream>
#include<fstream>
#include<map>
#include<list>
#include<sstream>
using namespace std;

class Line{
public:
	string label,opcode,operand;
	Line(){

	}
	Line(string l,string opc,string opr){
		label=l;
		operand=opr;
		opcode=opc;
	}
	string toString(){
		return label+" "+opcode+" "+operand;
	}
};

void expand(Line);
void define(Line);
map<string,list<Line> > defTab;
ifstream input("input.txt");
ofstream output("output.txt");

Line getLine(){
	string buffer,lbl,opc,opr="";
	getline(input,buffer,'\n');
	stringstream ss(buffer);
	ss>>lbl>>opc>>opr;
	if(opr==""){
		opr=opc;
		opc=lbl;
		lbl="";
	}
//	cout<<endl<<lbl<<"\t|"<<opc<<"\t|"<<opr<<"|";

	return Line(lbl,opc,opr);
}

void processLine(Line line){
	//macro call occur,then expand macro
	if(defTab.find(line.opcode)!=defTab.end()){
		expand(line);
	}
	//MACRO keyword => a new macro is going to define
	else if(line.opcode=="MACRO"){
		define(line);
	}
	//other line output as such
	else{
		output<<line.toString()<<endl;
	}
}

void define(Line line){
	list<string> body;
	int level=1;
	//RDBUFF  ||    RDBUFF MACRO &INDEV,&BUFADR,&RECLTH  is pushed
	defTab[line.label].push_back(line);
	while(level>0){
		Line defLine=getLine();
		if(defLine.opcode=="MACRO"){
			level++;
			//define(line);
		}
		else if(defLine.opcode=="MEND"){
			level--;
		}
		else{
			defTab[line.label].push_back(defLine);
		}
	}
}


void expand(Line line){
	output<<" ."<<line.toString()<<endl;

	//caller parameter value stream
	stringstream paramVal(line.operand);

	//callee stream
	stringstream paramList(defTab[line.opcode].front().operand);

	string pval,plist;
	map<string,string> paramMap;

	while(getline(paramVal,pval,',') && getline(paramList,plist,',')){
		paramMap[plist]=pval;
	}

	auto itr=defTab[line.opcode].begin();
	itr++;	//macro 1st line already expanded
	for(;itr!=defTab[line.opcode].end();itr++){

		string expandLine=(*itr).toString();
		for(auto mitr=paramMap.begin();mitr!=paramMap.end();mitr++){
			int replacePos=expandLine.find(mitr->first);	//get the position where pval is placed instead of plist
			while(replacePos!=-1){
				expandLine.replace(replacePos,( mitr->first ).length(), mitr->second );
				replacePos=expandLine.find(mitr->first);
			}
		}
		output<<expandLine<<endl;
	}
}



int main(){
	Line mline=getLine();
	while(mline.opcode!="END"){
		processLine(mline);
		mline=getLine();
	}
	output.close();
	input.close();
	cout<<"Input:\n";
	system("cat input.txt");
	cout<<"\nOutput:\n";
	system("cat output.txt");
	
}