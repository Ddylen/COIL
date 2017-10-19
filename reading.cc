#include <iostream>
#include "header.h"
#include <robot_instr.h>
#include <robot_link.h>
#include <stopwatch.h>
#include <vector>
#include <unistd.h>
using namespace std;
// this is untested prototype code

int read(int input, robot_link& rlink) 
{
	// needs to be updates with the final chip adress of chip 1 which will need to be used as a function inpute.g. rlink.request (READ_PORT_0, chipadress);
	int output;
	if(input ==0){
		output = rlink.request (READ_PORT_0);
	}
	if(input ==1){
		output = rlink.request (READ_PORT_1);
	}
	if(input ==2){
		output = rlink.request (READ_PORT_2);
	}
	if(input ==3){
		output = rlink.request (READ_PORT_3);
	}
	if(input ==4){
		output = rlink.request (READ_PORT_4);
	}
	if(input ==5){
		output = rlink.request (READ_PORT_5);
	}
	if(input ==6){
		output = rlink.request (READ_PORT_6);
	}
	if(input ==7){
		output = rlink.request (READ_PORT_7);
	}	
	
		return output;
	
}

int readbit(int port, int bit, robot_link& rlink){
	// port from 0 to 7, bit from 0 to 7
	int registerval =read(port, rlink);
	bool output =false;
	const int bit0 = 0x80;
	const int bit1 = 0x40;
	const int bit2 = 0x20;
	const int bit3 = 0x10;
	const int bit4 = 0x08;
	const int bit5 = 0x04;
	const int bit6 = 0x02;
	const int bit7 = 0x01;
	if(bit ==0){
		output = (registerval bitand bit0)>0;
	}
	if(bit ==1){
		output = (registerval bitand bit1)>0;
	}
	if(bit ==2){
		output = (registerval bitand bit2)>0;
	}
	if(bit ==3){
		output = (registerval bitand bit3)>0;
	}
	if(bit ==4){
		output = (registerval bitand bit4)>0;
	}
	if(bit ==5){
		output = (registerval bitand bit5)>0;
	}
	if(bit ==6){
		output = (registerval bitand bit6)>0;
	}
	if(bit ==7){
		output = (registerval bitand bit7)>0;
	}
	//output section
	if(output == true){
		return 1;
	}
	if(output == false){
		return 0;
	}
	
}

void turnright(robot_link& rlink){
	//robot_link rlink;
	//initialise(rlink);
	int n= 0; // for infinite loop
	int left, middle, right;
	left =  readbit(0, 6, rlink);
	middle = readbit(0, 5, rlink);
	right = readbit(0, 4, rlink); 
	stopwatch watch;
	watch.start();
	while(n<1){
		rlink.command(MOTOR_1_GO, 127);
		rlink.command(MOTOR_2_GO, 0);
		// UPDATE THIS SO WE MOVE TO CENTRE OF INTERSECTION AND THEN SPIN BOTH WHEELS IN OPPOSITE DIRECTIONS 
		if(watch.read()>2000){
			middle = readbit(0, 5, rlink);
			if(middle ==1){
				n= 1;
				rlink.command(MOTOR_1_GO, 0);
			}
			
		}
	
			
	}
}
