#include<iostream>
#include<fstream>
#include<map>
#include<list>
#include<sstream>
#include<iomanip>
using namespace std;


map<string,string>optab;
map<string,int> symtab;
map<string, list<int> >FR;
ofstream output("output.txt");

class Line{
public:
	string label,opcode,operand;
	Line(){

	}
	Line(string lbl,string opc,string opr){
		label=lbl;
		opcode=opc;
		operand=opr;
	}

};

Line getLine(ifstream &input){
	string buffer;
	getline(input,buffer,'\n');
	string lbl,opc,opr="";
	stringstream ss(buffer);
	ss>>lbl>>opc>>opr;
	if(opr==""){
		opr=opc;
		opc=lbl;
		lbl="";
	}
	return Line(lbl,opc,opr);
}

class TextRecord{
public:
	int startAdd,unwLen;
	string objcode;
	void init(int address){
		startAdd=address;
		unwLen=0;
		objcode="";
	}
	void init(){
		startAdd +=(objcode.length()-unwLen )/2;
		unwLen=0;
		objcode="";

	}
	
	void add(string record){
			if(objcode.length()+record.length()-unwLen <= 60){
				objcode+="^"+record;
				unwLen++;
			}
			else{
				write();
				init();
				objcode+="^"+record;
				unwLen++;
			}
	}
	void write(){
		output<<"T^"<<hex<<setw(6)<<setfill('0')<<startAdd<<"^"
			<<setw(2)<<setfill('0')<<objcode.length()/2
				<<objcode<<endl;
	}
	int isempty(){
		if(objcode=="")
			return 1;
		return 0;

	}
};

void handleFR(string label){

	auto itr=FR.find(label);
	if(itr!=FR.end()){
		int loc=symtab[label];
		stringstream ss;
		ss<<hex<<setw(6)<<setfill('0')<<loc;
		string locc;
		ss>>locc;
		auto listitr=itr->second.begin();
		while(listitr!=itr->second.end()){
			TextRecord record;
			record.init(*listitr);
			record.add(locc);
			record.write();
			listitr++;
		}


	}
	else{
		cout<<"\nlabel not found in forward reference";
	}
}

int main(){
	cout<<"input with locCtr: \n";
	system("cat inpLoc.txt");
	cout<<endl;


	optab["LDA"]="00";
	optab["STA"]="50";
	optab["ADD"]="6B";

	string pgmName;
	int startAdd,locCtr;
	TextRecord record;
	string code;
	
	ifstream fin("input.txt");
	Line line=getLine(fin);
	if(line.opcode=="START"){
		pgmName=line.label;
		stringstream ss;
		ss<<hex<<line.operand;
		ss>>startAdd;
		locCtr=startAdd;
	}
	else{
		startAdd=0;
		locCtr=startAdd;
	}
	record.init(locCtr);
	output<<hex<<"H^"<<setw(6)<<setfill(' ')<<pgmName<<"^"<<startAdd<<"^  "<<endl;
	line=getLine(fin);
	while(line.opcode!="END"){
		code="";
		if(line.label!=""){//label in label field
			auto symitr=symtab.find(line.label);
			if(symitr!=symtab.end()){//label found in symtab
				if(symitr->second == -1){//if symtab value -1 ==> forwd referenced  else  error
					symitr->second = locCtr;
					record.write();
					handleFR(line.label);
					record.init(locCtr);
				}
				else{
					cout<<"\nError:Duplicate symbol!";
				}
			}
			else{	//insert the label into symtab
				symtab[line.label]=locCtr;
				list<int> frList;//initialze a FR list
				FR[line.label]=frList;
			}
		}
		auto opitr=optab.find(line.opcode);
		if(opitr!=optab.end()){//opcode found on optab
			auto symitr=symtab.find(line.operand);
			if(symitr!=symtab.end()){//operand found on symtab
				if( symitr->second !=-1){ //is it contain any real values??
					stringstream ss;
					ss<<hex<<setw(4)<<setfill('0')<<symtab[line.operand];
					string temp;
					ss>>temp;
					code+=opitr->second+temp;
					record.add(code);
				}
				else{
					code+=opitr->second + "0000";
					record.add(code);
					auto FRitr=FR.find(line.operand);
					if(FRitr!=FR.end()){
						FRitr->second.push_back(locCtr+1);
					}
					else{
						cout<<"\nError: forwad reference can't be added!";
					}
					
				}
			}
			else{//operand not on the symtab --> add a forward reference
				symtab[line.operand]=-1;
				list<int> newList;
				FR[line.operand]=newList;
				FR[line.operand].push_back(locCtr+1);
				code=opitr->second + "0000";
				record.add(code);
			}
		locCtr=locCtr+3;

		}
		else if(line.opcode=="RESW"){
			stringstream ss;
			ss<<hex<<line.operand;
			int temp;
			ss>>temp;
			locCtr+=3*temp;
		}
		else if(line.opcode=="RESB"){
			stringstream ss;
			ss<<hex<<line.operand;
			int temp;
			ss>>temp;
			locCtr+=temp;
		}
		else if(line.opcode=="WORD"){
			stringstream ss;
			ss<<hex<<setw(6)<<setfill('0')<<line.operand;
			string temp;
			ss>>temp;
			code=temp;
			record.add(code);
			locCtr+=3;
		}
		else if(line.opcode=="BYTE"){
			if(line.operand[0]=='X'){
				stringstream ss;
				string val=line.operand.substr(2,line.operand.length()-3);
				ss<<setw(6)<<setfill(' ')<<val;
				ss>>val;
				record.add(val);
				locCtr+=(line.operand.length()-3)/2;
			}
			else{
				locCtr+=line.operand.length()-3;
				string val="";
				code="454f46";
				record.add(code);
			}
		}
		else{
			cout<<"\nError:OPcode not found!";
		}

		line=getLine(fin);
	}
	if(!record.isempty()){
		record.write();
	}
	int pgmLen=locCtr-startAdd;
	stringstream ss;
	ss<<hex<<setw(2)<<setfill('0')<<pgmLen;
	ss>>pgmLen;
	output.seekp(14,ios::beg);
	output<<hex<<pgmLen<<endl;
	for(auto itr=symtab.begin();itr!=symtab.end();itr++)
		cout<<hex<<"\n"<< itr->first <<"\t"<<itr->second;
	cerr<<"\noutput:::\n";
	system("cat output.txt");	
}