#include <iostream>
#include <list>
#include <iterator>
#include <cstring>
#include <cstdlib>
//#include <algorithm>
using namespace std;

bool is_operator(char c){
    return (c=='+' || c== '-' ||c== '*' || c== '/');
}

double result(double n1,char op,double n2){
//TO DO: check if op is an operator
  if(op== '+') return n1+n2;
  else if(op== '-') return n1-n2;
  else if(op== '*') return n1*n2;
  else  return n1/n2;  
}

bool is_allowed(char c){
	return (is_operator(c) ||
		    isdigit(c) || 
			c=='.' || 				
			 c=='('||
			c==')');
}

double get_number(){
  //reads chars and convert them into a number
  char ch;
  char a[11];
  int index=0;
  bool dot=false;
  cin>>ch;
  while(ch!='='&&
	ch!=')'
	&&!is_operator(ch)){
    if((isdigit(ch)||ch=='.')&&index<10){
      if(ch=='.'&& dot==false){
	if(index==0){
	  a[index]='0';
	  index++;
	}
	dot=true;
	a[index]=ch;
	index++;
      }
      if(isdigit(ch)){
	a[index]=ch;
	index++;
      }
   }
    cin>>ch;
 }
 a[index]='\0';
 cin.putback(ch); 
return (double)atof(a);
}
void read_and_fill(list<char>&ops,list<double>&numbers){
  cout<<"Enter an expression you want to calculate.\n"
	<<"At the end press '='\n";


  char next('\0'), previous('\0');
  bool begin=true;//is true in the beginning and when we open the bracket
  int open_brackets=0;
  cin>>next;
  while(next!='='){
    if(is_allowed(next)){
      if(isdigit(next)||next=='.'){
	if(previous!=')'){
	  cin.putback(next);
	  double no=get_number();
	  numbers.push_back(no);
	  begin=false;
	  previous='1';
	}
      }
      if(is_operator(next)){
	if(begin==true)
	  numbers.push_back(0);
	if(!is_operator(previous)){
	  ops.push_back(next);
	  previous=next;
	  begin=false;
	}
      }
      if(next=='('){
	if(!isdigit(previous)&& previous!=')'){
	  ops.push_back(next);
	  previous=next;
	  begin=true;
	  open_brackets++;
	}
      }
      if(next==')'&& open_brackets>0){
	if(is_operator(previous) || previous=='(')
	  numbers.push_back(0);//e.g. "(2-)"will become "(2-0)"
  
        ops.push_back(next);
	previous=next;
	begin=false;
	open_brackets--;
	
      }                  
    }
    cin>>next;
  }
  if(is_operator(previous)||previous=='(')
	numbers.push_back(0);
  //close all open brackets
  while(open_brackets>0){
    ops.push_back(')');
    open_brackets--;
  }


}

int priority(char c){
//TO DO: check if c is an operator
  if(c=='/'||c=='*') return 1;
  else /* if(c=='+'||c=='-')*/ return 0; 
}


double oper(list<double> &numbers, list<char>& ops){
  if(ops.empty())
    return *numbers.begin();
  
  else if(ops.size()==1){
    return result(* numbers.begin(),*ops.begin(),*(++numbers.begin()));}
  else { 
    
    list<double>::iterator it_no1, it_no2, it_no3;
    it_no1=numbers.begin(); //first number
    it_no2=++numbers.begin();//second number
    it_no3=++numbers.begin();
    it_no3++;//third number
    
    list<char>::iterator it_op1, it_op2;
    it_op1=ops.begin();    
     double l= 0.0;
    for(it_op2=++ops.begin();it_op2!=ops.end();){
        //if the 1st and 2nd signs have equal priorities,
        //do the first operation, assign the result to the second elem.
            
            
            
       if(priority(*it_op1)>=priority(*it_op2)){
          
            l= result(*it_no1,*it_op1,*it_no2);
            it_op2++;
	    it_op1=ops.erase(it_op1);
           
            
            it_no3++;
            it_no2++;
          
            it_no1++;
            *it_no1=l;

            
       }
       else{
          
           //else do the 2nd operation
           //
            l= result(*it_no2,*it_op2,*it_no3);
            it_op2=ops.erase(it_op2);
            
            
            it_no3++;
            it_no2++;
            *it_no2=l;
  
                   
        }
        
    }

   return result(*it_no1, *it_op1, *it_no2);
      
  }
}

double calculate(list<double> num, list<char> chars){
    list<double>::iterator it_no1, it_no2;
    list<char>::iterator it_ch1, it_ch2;
    it_ch1=chars.begin();
    it_no1=num.begin();
    it_no2=num.begin();
    
    double result=0.0;
    
    for(it_ch2=chars.begin();it_ch2!=chars.end();it_ch2++){
        if(is_operator(*it_ch2)) //if the character is an operator move to the next operand
            it_no2++;
        if(*it_ch2==')'){
            it_ch1=it_ch2;
            it_ch1--;
            it_no1=it_no2;
            while(*it_ch1!='('){
                it_ch1--;
                it_no1--;
            }
            
                
            if(distance(it_ch1,it_ch2)==1){ //no operation in the brackets
                it_ch1=chars.erase(it_ch1);
                chars.erase(it_ch2);
                it_ch2--;
                }
            else{//there is at least one operation in the brackets
                  list<double> l(it_no1,++it_no2); //create new list that contains the numbers in the brackets
		  //how to use move operator here?

                  
                  list<char> c(++it_ch1, it_ch2);//create new list that contains the operations in the brackets

                  result=oper(l,c);
                  
                  it_ch2=chars.erase(--it_ch1,++it_ch2);//remove the calculated operators

//cout<<"*it_no1 is: "<<*it_no1<<endl;
                  it_no2=num.erase(it_no1, it_no2);
                  *(it_no2)=result;
//cout<<"now *it_no1 is: "<<*it_no1<<endl;
/*
*it_no1֊ը չի ջնջվում, ինչու՞
*/
//cout<<"first no is: "<<*num.begin()<<endl;
//cout<<"last no is: "<<*(--num.end())<<endl<<endl;
//?????????output

                
}
            }             
       }
// int k=0;
//for(list<double>::iterator i=num.begin();i!=num.end();i++){k++;cout<<endl<<k<<"-th num is :"<<*i;}
//cout<<*(--num.end())<<endl<<endl;

//for(list<char>::iterator i=chars.begin();i!=chars.end();i++)cout<<"char is "<<*i<<endl;
//cout<<"first operator: "<<*chars.begin()<<endl;
//ինչու՞  for loop ֊ը չի աշխատում։ 
        return(oper(num,chars));
}
   
int main(){

    list<double>numbers;
    list<char>o;
   read_and_fill(o, numbers);
cout<<"\nNumbers are: ";
for(list<double>::iterator i=numbers.begin();i!=numbers.end();i++)cout<<*i<<" ";

cout<<"\nChars are: ";
for(list<char>::iterator i=o.begin();i!=o.end();i++)cout<<*i<<" ";
  
  cout<<"\ncalculated number is: "<<calculate(numbers,o)<<endl;
 

  return 0 ;

}
