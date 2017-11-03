#include <iostream>
#include <robot_instr.h>
#include <robot_link.h>
#include <vector>
#include <stopwatch.h>
#define ROBOT_NUM 2
using namespace std;
void initialise(robot_link& rlink);


enum colour {
	//Matches enums for use in colour sensing
	ambient = -1,
	white = 1,
	yellow = 2,
	multicolour = 3,
	outofbounds = 4
	
};

enum enum_matchings {
	//Matches enums for use in move_from
	start_to_ball6 = 0,
	ball_to_nearestrightball = 1,
	ball1_to_ref = 9,
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




struct ball {
	//for storing data on the balls currently in the robot
	
	ball(colour input_colour, bool input_weight) {
		ball_colour = input_colour;
		weight = input_weight;
	}
	
	colour ball_colour;
	bool weight; //True for heavy
	
	friend ostream& operator<<(ostream& os, const ball& inp_ball);
	
};

//functions declared in reading.cc
int read(int input, robot_link& rlink); 
int readbit(int port, int bit, robot_link& rlink);
colour measurecolour(robot_link& rlink);
bool measure_weight(robot_link& rlink);

//functions declared in movement.cc
void forwards(robot_link& rlink, int stopatthisnumofintersections);
void sweepforline(robot_link& rlink);
void left(robot_link& rlink);
void right(robot_link& rlink);
void spin(robot_link& rlink);
void forwards_untill_impact(robot_link& rlink); 
void changespeed(robot_link& rlink, int motor1speed, int motor2speed);

void forwardspedal(robot_link& rlink, int time);
void full_speed_forwards(robot_link& rlink); //For debugging the wheel rpm differences
void backwardspedal(robot_link& rlink, int timeout);
void turn_left_until_first_line(robot_link& rlink);
void turn_right_until_first_line(robot_link& rlink);
void sreverese(robot_link& rlink);
void move_from(robot_link& rlink, enum_matchings input);// NOT FINISHED

//functions declared in loading.cc
bool get_weight(robot_link& rlink);
void set_led_on(robot_link& rlink, int led_number);
void set_actuator(robot_link& rlink, int number, int high);
void dropball(robot_link& rlink);
void dropball_with_ram(robot_link& rlink, int num_of_rams);
ostream& operator<<(ostream& os, const ball& inp_ball);

//functions declared in upperlevel.cc
vector<ball> load(robot_link& rlink);
void deliver(robot_link& rlink, vector<ball>& balllist, stopwatch& watch);

// declared is test.cc
void ballID(robot_link& rlink);
void test_colour_sensor(robot_link& rlink);
void continual_drop(robot_link& rlink);
void balldetectiontest(robot_link& rlink);




