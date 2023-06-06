#include <exception>
#include <string>
#include <iostream>

using namespace std;

#ifndef EXCEPTION_HPP
#define EXCEPTION_HPP

class Exception : public exception{
public:
    Exception(const string& msg) : m_msg(msg){
		cout<<endl<<"Assembler error: "<<m_msg<<endl;
      exit(-1);
	}

   virtual const char* what() const throw () 
   {
        return m_msg.c_str();
   }

private:
	string m_msg;
};

#endif