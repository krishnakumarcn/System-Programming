 /*
 2passAssembler
 krishnakumar.c.n
 krishnacnskp@gmail.com
*/


#include<iostream>
#include<fstream>
#include<iomanip>
#include<sstream>
#include<cstdlib>
 #include<ctime>
using namespace std;
#define INPUT_FILE "input_file.txt"
#define OPTAB_FILE "optab_file.txt"
#define INTER_FILE "inter_file.txt"
#define OBJECT_FILE "obj_record.txt"
const int NOT_FOUND=-1;


//for storing the symbol table
class SymTab{
	string symbol[30];
	int value[30];
	int count;	//keep track the item which is added and total count
public:
		SymTab(){
			count=0;
		}

		int find(string key){
			for(int i=0;i<count;i++){
				if(!key.compare(symbol[i])){
					return value[i];
				}
			}
			return NOT_FOUND;
		}
		//inserting into SymTab
		void add(string sym,int loc){
			symbol[count]=sym;
			value[count]=loc;
			count++;
		}

		void print(){
			for(int i=0;i<count;i++)
				cout<<"\n"<<symbol[i]<<"\t"<<hex<<value[i];
		}
};

//saving the optab file into class OpTab
class OpTab{
	string opcode[30];
	string objcode[30];
	int count,location; //location will get the required index 
						//after calling find() thereby process getCode made easy
public:
		OpTab(){
			count=0;
			ifstream fop(OPTAB_FILE);
			while(!fop.eof()){
				fop>>opcode[count]>>objcode[count];
				count++;
			}
		}

		void print(){
			for(int i=0;i<count;i++)
				cout<<"\n"<<opcode[i]<<"\t"<<objcode[i];
		}

		string getCode(string key){
			if(find(key)){
				return objcode[location];
			}
			else{
				cerr<<"\nError:ObjectCodeException!!: "<<key;
				return NULL;
			}
		}

		int find(string key){
			for(int i=0;i<count;i++){
				if(!key.compare(opcode[i])){
					location=i;
					return 1;
				}
			}
			return 0;
		}


};


//for storing a line of file(input or intermediate)
class Line{
public:
	string label,opcode,operand,mlocctr;	//mlocctr for intermediate file only
	Line(){

	}	
	Line(string a,string b,string c){	
		label=a;
		opcode=b;
		operand=c;
	}
	Line(string mloc,string lab,string opc,string oper){
		mlocctr=mloc;
		label=lab;
		opcode=opc;
		operand=oper;
	}

	string getWhole(){		//for make printing easy
		return "\t"+label+"\t"+opcode+"\t"+operand+"\n";
	}
};


Line readLine(ifstream &fin){
	string buffer,label,opcode,operand="";
	getline(fin,buffer,'\n');
	if(buffer.at(0)=='.'){		// line starting with . are comments (Assume!!)
		getline(fin,buffer,'\n');
	}
	istringstream seperator(buffer);	
	seperator>>label>>opcode>>operand;
	if(operand==""){	// if label is empty , the original opcode will placed in label field,
						//inorder to avoid tht use the following code
		operand=opcode;
		opcode=label;
		label="";
	}
	return Line(label,opcode,operand);	//constructor of line class will called

}

int conv2operand(string val){	//string to integer in hex conversion
	int value;
	stringstream stream;
	stream<<hex<<val;
	stream>>value;
	return value;
}

string conv2Hex(int val){	//integer to integer in hex conversion
	string value;
	stringstream stream;
	stream<<hex<<setw(4)<<setfill('0')<<val;
	stream>>value;
	return value;
}

Line readInter(ifstream &fin){
	string label,opcode,operand="",buffer;
	string mlocctr;
	getline(fin,buffer,'\n');
	istringstream seperator(buffer);
	seperator>>mlocctr>>label>>opcode>>operand;
	if(operand==""){
		operand=opcode;
		opcode=label;
		label="";
	}
	return Line(mlocctr,label,opcode,operand);
}

class TextRecord{
public:
	int startAdd,unWantedLen;
	string objcode;

	void init(int locctr){	//used at the start
		startAdd=locctr;
		unWantedLen=0;
		objcode="";
	}

	void init(){
		startAdd+=(objcode.length()-unWantedLen)/2;
		unWantedLen=0;
		objcode="";
	}

	void add(string code){
		if(objcode.length()+code.length()-unWantedLen<60){
			objcode+="^"+code;
			unWantedLen++;
		}
		else{
			write();
			init();
			objcode+=code;
		}
	}

	void write(){	//print a text record
		if(objcode.length()==0)
			return;
		else{
			cout<<"T^"<<setw(6)<<setfill('0')<<hex<<startAdd<<"^";
			cout<<setw(2)<<setfill('0')<<hex<<objcode.length()/2<<objcode<<"\n";
		}
	}
};


int main(){
	SymTab sym;
	OpTab opt;
	Line line;
	ifstream fin;
	fin.open(INPUT_FILE);
	string buffer;
	ofstream fout;
	fout.open(INTER_FILE);
	int locctr,startAdd,pgmLen;

	/*////////////////////////////////////////////////////////////

						PASS - ONE

	\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
	
	line=readLine(fin);

	if(line.opcode=="START"){
			startAdd=conv2operand(line.operand);
			locctr=startAdd;


			fout<<hex<<setw(4)<<setfill('0')<<locctr<<line.getWhole();
	}
	else{
		locctr=startAdd=0;
		fout<<hex<<setw(4)<<setfill('0')<<locctr<<line.getWhole();
	}


	line=readLine(fin);

	
	while(line.opcode!="END"){
		if(line.label!=""){
			if(sym.find(line.label)!=NOT_FOUND){
				cerr<<"\nError: DuplicateSymbolException!!: "<<line.label;
				sym.print();
			}
			else{
				sym.add(line.label,locctr);
				
			}
		}
		
		fout<<setw(4)<<setfill('0')<<locctr<<line.getWhole();
		if(opt.find(line.opcode)){
			locctr+=3;
		}
		else if(line.opcode=="RESW"){
			locctr+=3*conv2operand(line.operand);
		}
		else if(line.opcode=="RESB"){
			locctr+=conv2operand(line.operand);
			
		}
		else if(line.opcode=="WORD"){
			locctr+=3;
		}
		else if(line.opcode=="BYTE"){
			if(line.operand.at(0)=='X'){
				locctr+=(line.operand.length()-3)/2;
			}
			else if(line.operand.at(0)=='C'){
				locctr+=line.operand.length()-3;
			}
		}
		line=readLine(fin);
	}
	fout<<setw(4)<<setfill('0')<<locctr<<line.getWhole();
	pgmLen=locctr-startAdd;
	cout<<endl<<"\n________SymbolTable__________\n";
	sym.print();
	fin.close();
	fout.close();

	/*////////////////////////////////////////////////////////////

						PASS - TWO
						
	\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

	fin.open(INTER_FILE);
	fout.open(OBJECT_FILE);
	TextRecord record;
	string code="";


	line=readInter(fin);
	if(line.opcode=="START"){
		cout<<"\n\n________ObjectRecord_________\n\nH^";
		cout<<line.label<<"^"<<setw(6)<<setfill('0')<<hex<<startAdd<<"^"<<hex<<pgmLen<<"\n";
		record.init(startAdd);
		line=readInter(fin);
		while(line.opcode!="END"){
			bool b=false;
			code="";
			if(opt.find(line.opcode)){
				code+=opt.getCode(line.opcode);
				if(line.operand!=""){
					if(sym.find(line.operand)!=NOT_FOUND){
					code+=conv2Hex(sym.find(line.operand));
					record.add(code);
					}
					else{
						code+="0000";
						cerr<<"\nError :InvalidOperand/UndefinedSymbol!!";
					}
				}
				else{
				code+="0000";
				record.add(code);
				}
			}
			else if(line.opcode=="WORD"||line.opcode=="BYTE"){
				stringstream stream;
				string data;
				if(line.opcode=="BYTE"){
					code=line.operand.substr(2,line.operand.length()-3);
				}
				else{
					stream<<setw(6)<<setfill('0')<<hex<<line.operand;
					stream>>data;
					code=data;
				}
				record.add(code);
			}
			else if(line.opcode=="RESW" || line.opcode=="RESB"){
				b=true;
				
			}

			line=readInter(fin);
			if(b==true){
				record.write();
				int temp=conv2operand(line.mlocctr);
				record.init(temp);
			}
			
		}
		record.write();
		cout<<"E^"<<setw(6)<<setfill('0')<<hex<<startAdd;
		cout<<"\n------------------------------\n";

	}


return 0;
}