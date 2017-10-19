#include "header.h"
#include <iostream>
#include <robot_instr.h>
#include <robot_link.h>
#include <stopwatch.h>

using namespace std;

const int wait_time_ball_roll_over_weight_switch = 3000;
const int release_color_box_actuator = 0x00;
const int weight_switch_mask = 0x10; //00010000


bool get_weight(robot_link& rlink) {
	//Release the actuator that is holding the ball in the LDR sensing box
	
	rlink.command(WRITE_PORT_1, release_color_box_actuator);
	
	//Poll the weight switch to see if is ever pressed during the waiting period
	stopwatch watch;
	watch.start();
	
	bool was_weight_switch_pressed = false;
	
	while(watch.read() < wait_time_ball_roll_over_weight_switch) {
		int value_read_from_PCF = -99999;
		value_read_from_PCF = rlink.request(READ_PORT_0);
		
		if (value_read_from_PCF & weight_switch_mask) {
			was_weight_switch_pressed = true;
		}
	}
	
	return was_weight_switch_pressed;
}
