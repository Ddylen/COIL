#include "header.h"
#include <iostream>
#include <robot_instr.h>
#include <robot_link.h>
#include <stopwatch.h>
#include <unistd.h>

vector<ball> load(robot_link& rlink){
	//contains code for loading section of routine, returns list of balls for use in deliver

	vector<ball> balllist; //stores list of balls
	

	set_actuator(rlink, 2, 0); //reset kicker
	move_from(rlink, start_to_ball6);
	
	
	
	for(int i=0;i<6;i++){
	
	
		int speed = 127;
		double time_for_close_arms = 1; //time for arms to close/open
		double time_to_wait_for_weight_switch = 6;
	

	
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
		usleep(1000000*2);
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
		
		if(i<5){
			move_from(rlink, ball_to_nearestrightball);
		}
		
	}
	return balllist;
	
}


void deliver(robot_link& rlink, vector<ball>& balllist, stopwatch& watch){
	//takes timer started in start()
	set_actuator(rlink, 1, 0); //put dropoff in delivery position
	move_from(rlink, ball1_to_ref);
	int running_out_of_time_time =240;
	// conditional delivery section of code
	while(balllist.size()>0){
		if(watch.read()>(1000*running_out_of_time_time)){
			move_from(rlink, ref_to_start);
			break;
		}
			
		if(balllist[0].ball_colour == white && balllist[0].weight == false){
			cout << "Delivering ball 1" << endl;
			move_from(rlink, ref_to_D1);
			dropball(rlink);
			move_from(rlink, D1_to_ref);
			
		}
		
		if((balllist[0].ball_colour == white && balllist[0].weight == true) || (balllist[0].ball_colour == yellow && balllist[0].weight == true)){
			cout << "Delivering ball 2/ ball 4" << endl;
			move_from(rlink, ref_to_DR);
			dropball(rlink);
			move_from(rlink, DR_to_ref);
			
			
		}
		
		if(balllist[0].ball_colour == yellow && balllist[0].weight == false){
			cout << "Delivering ball 3" << endl;
			move_from(rlink, ref_to_D2);
			dropball(rlink);
			move_from(rlink, D2_to_ref);
			
		}
		
		if(balllist[0].ball_colour ==  multicolour){
			cout << "Delivering ball 5" << endl;
			move_from(rlink, ref_to_D3);
			dropball(rlink);
			move_from(rlink, D3_to_ref);
			
			
		}
		balllist.erase(balllist.begin());
	
}
	

	
	
}

void main_code(robot_link& rlink, stopwatch& mainwatch) {
	vector<ball> balls;
	
	balls = load(rlink);

	cout << "Ball list: " << endl;
	for (unsigned int i = 0; i < balls.size(); i++) {
		cout << "Ball " << i << " is " << balls[i] << endl;
	}
	
	deliver(rlink, balls, mainwatch);
}


int main(){
	
	stopwatch mainwatch;
	mainwatch.start();
	cout << "program started"<< endl;
	robot_link rlink;
	initialise(rlink);
	
	while(true){
	
	dropball(rlink);
	usleep(1000000 * 2);
	}
	

	
	/*
	vector<ball> balllist = load(rlink);
	cout << "load time is " << mainwatch.read() << endl;
	
	deliver(rlink, balllist, mainwatch);
	cout << "Deliver time is " << mainwatch.read() << endl;
	
	move_from(rlink, ref_to_start);
	cout << "Total time is " << mainwatch.read() << endl;
	*/
	return 0;
	
}

	
	
	
	
	
	
	
	
