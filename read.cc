#include <iostream>
#include "header.h"
#include <robot_instr.h>
#include <robot_link.h>
#include <stopwatch.h>
using namespace std;


int read (int port, robot_link& rlink) {

	int output;
	if(port ==0){
		output = rlink.request (READ_PORT_0);
	}
	if(port ==1){
		output = rlink.request (READ_PORT_1);
	}
	if(port ==2){
		output = rlink.request (READ_PORT_2);
	}
	if(port ==3){
		output = rlink.request (READ_PORT_3);
	}
	if(port ==4){
		output = rlink.request (READ_PORT_4);
	}
	if(port ==5){
		output = rlink.request (READ_PORT_5);
	}
	if(port ==6){
		output = rlink.request (READ_PORT_6);
	}
	if(port ==7){
		output = rlink.request (READ_PORT_7);
	}
	return output;

}



 // testing section
int main(){
	robot_link rlink;
	//initialise(rlink);
	int n = 200;
	stopwatch watch;
	watch.start();
	while(n>0){
		//cout << "Port 7 is " << read(0, rlink) << endl;
		read(0, rlink);
		n = n-1;
	}
	cout<< watch.read()/200 << endl;
	 
 }
 
 


