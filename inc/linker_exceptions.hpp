#include <exception>
#include <string>
#include <iostream>

using namespace std;

#ifndef EXCEPTION_HPP
#define EXCEPTION_HPP

class Exception : public exception{
public:
    Exception(const string& msg) : m_msg(msg){
		cout<<endl<<"Error: "<<m_msg<<endl;
	}

   virtual const char* what() const throw () 
   {
        return m_msg.c_str();
   }

private:
	string m_msg;
};

#endif