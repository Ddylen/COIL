#include <iostream>
#include <robot_instr.h>
#include <robot_link.h>

#define ROBOT_NUM 2
//using namespace std;

void initialise(robot_link& rlink){
	//initialises the robot link
#ifdef __arm__
   if (!rlink.initialise ("127.0.0.1")) { 
	    std::cout << "Local Hardware Communication Error" << std::endl;
		rlink.print_errs("  ");        // setup for local hardware
	}
#else
   if (!rlink.initialise (ROBOT_NUM)) { // setup the link
		std::cout << "Cannot initialise link" << std::endl;
		rlink.print_errs("  ");
	}
	
	rlink.command (RAMP_TIME, 0);
#endif
	
	
}

//functions declared in reading.cc
int read(int input, robot_link& rlink); 
int readbit(int port, int bit, robot_link& rlink);

//functions declared in movement.cc
void forwards(robot_link& rlink, int stopatthisnumofintersections);

//functions declared in loading.cc
bool get_weight(robot_link& rlink);

enum enum_matchings {
	//Matches enums for use in move_from
	start_to_ball1 = 0,
	ball1_to_ball2 = 1,
	ball2_to_ball3 = 2,
	ball3_to_ball4 = 3,
	ball4_to_ball5 = 4,
	ball5_to_ball6 = 5,
	ball_6_to_ref = 9,
	ref_to_D1 = 10,
	D1_to_ref = 11,
	ref_to_D2 = 20,
	D2_to_ref = 21,
	ref_to_D3= 30,
	D3_to_ref = 31,
	ref_to_DR= 40,
	DR_to_ref = 41,
	ref_to_start = 55
};
