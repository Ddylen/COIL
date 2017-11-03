#include "header.h"
#include <iostream>
#include <robot_instr.h>
#include <robot_link.h>
#include <stopwatch.h>
#include <unistd.h>
#include <stdlib.h>

vector<ball> load(robot_link& rlink, int num_balls_to_pickup){
	//contains code for loading section of routine, returns list of balls for use in deliver

	vector<ball> balllist; //stores list of balls
	

	set_actuator(rlink, 2, 0); //reset kicker
	move_from(rlink, start_to_ball6);
	
	
	
	for(int i=0;i<num_balls_to_pickup;i++){
	
	
		int speed = 127;
		double time_for_close_arms = 1; //time for arms to close/open
		double time_to_wait_for_weight_switch = 3.5;
	

	
		rlink.command(MOTOR_3_GO, (speed+128)); //close arms , we tested with +127
		stopwatch watch;
		watch.start();
		
		bool weightbool = false; 
		while(watch.read()< time_for_close_arms * 1000){
			if(get_weight(rlink) == true){
				weightbool = true;
				} 
		}
			
		rlink.command(MOTOR_3_GO, 0);
		
		while(watch.read() < time_to_wait_for_weight_switch * 1000) {
			if(get_weight(rlink) == true){
				weightbool = true;
			}
		}
		
		
		
		colour col = measurecolour(rlink);

		set_actuator(rlink, 2, 1); //kick ball out
		usleep(1000000*1);
		set_actuator(rlink, 2, 0); //reset kicker
		
		rlink.command(MOTOR_3_GO, speed); //open arms
		usleep(1000000*time_for_close_arms);// time for arms to open, might be some space to merge this time with waits above
		rlink.command(MOTOR_3_GO, 0); //shut it off
		if(col != ambient){
			balllist.push_back(ball(col, weightbool));
		}
		
		cout << "weight switch read " << weightbool << endl;
		//code below chooses which LED to light
		if(balllist.back().ball_colour == white && balllist.back().weight == false){
			cout << "Picked up ball 1" << endl;
			set_led_on(rlink,1);
			
		}
		
		if((balllist.back().ball_colour == white && balllist.back().weight == true)){
			cout << "Picked up ball 2" << endl;
			set_led_on(rlink,2);
			
			
		}
		
		if(balllist.back().ball_colour == yellow && balllist.back().weight == false){
			cout << "Picked up ball 3" << endl;
			set_led_on(rlink,3);
			
		}
		
		if(balllist.back().ball_colour == yellow && balllist.back().weight == true){
			cout << "Picked up ball 4" << endl;	
			set_led_on(rlink,4);
		}
		if(balllist.back().ball_colour ==  multicolour){
			cout << "Picked up ball 5" << endl;	
			set_led_on(rlink,5);
		}
		
		if(i<num_balls_to_pickup - 1){
			move_from(rlink, ball_to_nearestrightball);
		}
		
	}
	return balllist;
	
}


void deliver(robot_link& rlink, vector<ball>& balllist, stopwatch& watch){
	//takes timer started in start()
	set_actuator(rlink, 1, 0); //put dropoff in delivery position
	move_from(rlink, ball1_to_ref);
	int running_out_of_time_time =255;
	// conditional delivery section of code
	while(balllist.size()>0){
		if(watch.read()>(1000*running_out_of_time_time)){
			cout << "Returning to start due to running out of time" << endl;

			break;
		}
			
		if(balllist[0].ball_colour == white && balllist[0].weight == false){
			cout << "Delivering ball 1" << endl;
			move_from(rlink, ref_to_D1);
			dropball_with_ram(rlink, 2);
			move_from(rlink, D1_to_ref);
			
		}
		
		if((balllist[0].ball_colour == white && balllist[0].weight == true) || (balllist[0].ball_colour == yellow && balllist[0].weight == true)){
			cout << "Delivering ball 2/ ball 4" << endl;
			move_from(rlink, ref_to_DR);
			dropball_with_ram(rlink, 2);
			move_from(rlink, DR_to_ref);
			
			
		}
		
		if(balllist[0].ball_colour == yellow && balllist[0].weight == false){
			cout << "Delivering ball 3" << endl;
			move_from(rlink, ref_to_D2);
			dropball_with_ram(rlink, 5);
			move_from(rlink, D2_to_ref);
			
		}
		
		if(balllist[0].ball_colour ==  multicolour){
			cout << "Delivering ball 5" << endl;
			move_from(rlink, ref_to_D3);
			dropball_with_ram(rlink, 2);
			move_from(rlink, D3_to_ref);
			
			
		}
		balllist.erase(balllist.begin());
	
	}
	move_from(rlink, ref_to_start);
	

	
	
}

void main_code(robot_link& rlink, stopwatch& mainwatch, int num_balls_to_pickup) {
	vector<ball> balls;
	stopwatch wa;
	wa.start();
	
	balls = load(rlink, num_balls_to_pickup);
	cout << " Time at loading end is " << endl;
	cout << "Ball list: " << endl;
	for (unsigned int i = 0; i < balls.size(); i++) {
		cout << "Ball " << i << " is " << balls[i] << endl;
	}
	
	deliver(rlink, balls, mainwatch);
}

/*
	vector<ball> balllist;
	balllist.push_back(ball(white, 0));
	balllist.push_back(ball(white, 1));
	balllist.push_back(ball(yellow, 0));
	balllist.push_back(ball(yellow, 1));
	balllist.push_back(ball(multicolour, 0));
	balllist.push_back(ball(yellow, 1));
	
	*/
	
int main(int argc, const char* argv[]){
	
	int num_balls_to_pickup = 6;
	if (argc > 1) {
		num_balls_to_pickup = atoi(argv[1]);
	}
	
	
	stopwatch mainwatch;
	mainwatch.start();
	cout << "program started"<< endl;
	cout << "Will pickup " << num_balls_to_pickup << " balls" << endl;
	robot_link rlink;
	initialise(rlink);
	
	
	//main_code(rlink, mainwatch, num_balls_to_pickup);
	test_colour_sensor(rlink);
	
	
	
	return 0;
}

	
	
	

	
	
	
	
