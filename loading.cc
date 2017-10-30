#include "header.h"
#include <iostream>
#include <robot_instr.h>
#include <robot_link.h>
#include <stopwatch.h>
#include <unistd.h>

using namespace std;

const int wait_time_ball_roll_over_weight_switch = 3000;
const int release_color_box_actuator = 0x00;
const int weight_switch_mask = 0x10; //00010000

void set_led_on(robot_link& rlink, int led_number) {
	//This function turns off all other ball type LEDs and turns on the
	//specified by led_number
	//set to 0 for all off
	// bitand clause keeps actuator bits low
	if (led_number == 1) {
		rlink.command(WRITE_PORT_1, 0xFB bitand 0xFC);
	} else if (led_number == 2) {
		rlink.command(WRITE_PORT_1, 0xF7 bitand 0xFC);
	} else if (led_number == 3) {
		rlink.command(WRITE_PORT_1, 0xEF bitand 0xFC);
	} else if (led_number == 4) {
		rlink.command(WRITE_PORT_1, 0xDF bitand 0xFC);
	} else if (led_number == 5) {
		rlink.command(WRITE_PORT_1, 0xBF bitand 0xFC);
	} else if (led_number == 0) {
		rlink.command(WRITE_PORT_1, 0xFF bitand 0xFC);
	} else {
		cout << "ERROR in loading.cc set_led_on. Number other than 0 to 5 given!" << endl;
	}
}

void set_actuator(robot_link& rlink, int number, int high){
	if(number == 1 && high ==1){
		rlink.command(WRITE_PORT_1, 0xFE);	
	}
	if(number ==2 && high ==1){
		rlink.command(WRITE_PORT_1, 0xFD);	
	}
	if(high ==0){
		rlink.command(WRITE_PORT_1,0xFC);
	}
}




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

colour measure_colour(robot_link& rlink){

		colour col = measurecolour(rlink);
		cout<< col << " " << rlink.request(ADC0) << endl;
		if(col  == yellow){
			//cout<< "Yellow " << rlink.request(ADC0) << endl;
			return yellow;
		}
		if(col  == white){
			//cout<< "White " << rlink.request(ADC0) << endl;
			return white;
		}
		if(col  == multicolour){
			//cout<< "Multicolour " << rlink.request(ADC0) << endl;
			return multicolour;
		}
		if(col  == ambient){
			//cout<< "Ambient " << rlink.request(ADC0) << endl;
			return ambient;
		}
		
			
}

bool measure_weight(robot_link& rlink){
	// untested
	int detecttime = 5; // time sensor waits for reading for
	stopwatch watch;
	watch.start();
	bool switchfired = false;
	while(watch.read()<detecttime*1000000){
		if(readbit(0, 3, rlink) == 1){
			switchfired = true;
			return switchfired;
		}
	return false; // if switch doesnt detect anything in detecttime
	
	}
}

