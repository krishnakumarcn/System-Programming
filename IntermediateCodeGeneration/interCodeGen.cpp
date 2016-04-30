/*
intermediateCodeGenereation
krishnkumarcn
*/



#include<iostream>
#include<fstream>
#include<stack>
#include<sstream>
using namespace std;

//in stack priority
int isp(string e){
	if(e=="(")
		return 6;
	else if(e=="^")
		return 4;
	else if(e=="*" || e=="/")
		return 3;
	else if(e=="+" || e=="-")
		return 2;
	else if(e==")")
		return 1;
	else
		return 0;

}

//incoming priority
int icp(string e){
	if(e=="(")
		return 7;
	else if(e=="^")
		return 5;
	else if(e=="*" || e=="/")
		return 3;
	else if(e=="+" || e== "-")
		return 2;
	else if(e== ")")
		return 1;
	else
		return 0;
}


//check if an element taken is alpha numeric?
int isalphanum(string e){
	if(isalnum(e[0]))
		return 1;
	return 0;
}

int main(){
	string line;
	ifstream fin("input.txt");
	stack<string>st;
	while(getline(fin,line,'\n')){
		string postfix="";
		stringstream ss(line);
		string elt;
		while(getline(ss,elt,' ')){
			if(isalphanum(elt)){
				postfix+=elt+" ";
			}
			else{
				if( st.empty()|| isp(st.top()) < icp(elt) ){
					st.push(elt);
				}
				else{
					while(!st.empty()&&(isp(st.top())>=icp(elt))){
						if(st.top()!="(" && st.top()!=")")
							postfix+=st.top()+" ";
						st.pop();
					}
					st.push(elt);
				}
			}

		}
		while(!st.empty()){
			if(st.top()!="("&&st.top()!=")")
				postfix+=st.top()+" ";
			st.pop();
		}
		/*at this point we get the postfix expression corresponds to
		our line of code,now process the postfix */
		stringstream s(postfix);
		int i=0;
		while(getline(s,elt,' ')){
			if(isalphanum(elt)){
				st.push(elt);
			}
			else{
				string second=st.top();
				st.pop();
				string first=st.top();
				st.pop();
				string result="";
				if(elt=="="){
					cout<<first<<"="<<second<<endl;
				}
				//binary operations proceed the below if
				if(!st.empty()){
					result="t"+to_string(i);
					cout<<result<<"="<<first<<" "<<elt<<" "<<second<<endl;
					st.push(result);
				}
				i++;
			}
		}
	}
}
