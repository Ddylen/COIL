#include "header.h"
#include <iostream>
#include <robot_instr.h>
#include <robot_link.h>
#include <stopwatch.h>
#include <unistd.h>
#include <string>

using namespace std;

const int wait_time_ball_roll_over_weight_switch = 3000;
const int release_color_box_actuator = 0x00;
const int weight_switch_mask = 0x10; //00010000

void set_led_on(robot_link& rlink, int led_number) {
	//This function turns off all other ball type LEDs and turns on the specified by led_number
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
	//sets a specific actuator to low or high
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

bool get_weight(robot_link& rlink) { //NEEDS TO BE CHANGED
	// releaes ball from light sensing area to allow it to rolls over weight switch
	
	//Release the actuator that is holding the ball in the LDR sensing box
	
	
	//Poll the weight switch to see if is ever pressed during the waiting period
	stopwatch watch;
	watch.start();
	
	bool was_weight_switch_pressed = false;

		
	if (readbit(0, 3, rlink) == 1) { //will be a different bit in practice
		was_weight_switch_pressed = true;
		}
	
	
	
	return was_weight_switch_pressed;
}

colour measure_colour(robot_link& rlink){
	//measures ADC reading of colour

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
		return outofbounds; // new addition to get rid of that warning
			
}

void dropball(robot_link& rlink){

	set_actuator(rlink, 1, 1);
	stopwatch watch;
	watch.start();
	while(watch.read()<3000) {
		/*
		set_actuator(rlink, 2, 1);// shaking motion
		usleep(1000000*0.25);
		set_actuator(rlink, 2, 0);
		usleep(1000000*0.25);
		*/
		
	}
	
	set_actuator(rlink, 1, 0);
	usleep(1000000*1);
	
}

void dropball_with_ram(robot_link& rlink, int num_of_rams) {
	set_actuator(rlink, 1, 1);
	stopwatch watch;
	watch.start();
	usleep(3*1000000);
	for(int i = 0; i < num_of_rams; i++) {
		backwardspedal(rlink, 100);
		forwards_untill_impact(rlink);
	}
	set_actuator(rlink, 1, 0);
	//there was a 1 second sleep here
}

ostream& operator <<(ostream& os, const ball& inp_ball) {
	string ball_colour = "";
	if (inp_ball.ball_colour == white) {
		ball_colour = "white";
	} else if (inp_ball.ball_colour == yellow) {
		ball_colour = "yellow";
	} else if (inp_ball.ball_colour == multicolour) {
		ball_colour = "multi-colour";
	} else {
		ball_colour = "colour unknown";
	}
	string ball_weight = "";
	if (inp_ball.weight) {
		ball_weight = "Heavy";
	} else {
		ball_weight = "Light";
	}
	os << "Ball colour: " << ball_colour << " Ball weight: " << ball_weight;
	return os;
}


