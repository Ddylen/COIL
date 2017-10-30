#include "header.h"
#include <iostream>
#include <robot_instr.h>
#include <robot_link.h>
#include <stopwatch.h>
#include <unistd.h>

vector<ball> load(robot_link& rlink){
	//contains code for loading section of routine, returns list of balls for use in deliver

	vector<ball> balllist; //stores list of balls
	
	
	
	
	//movement program top get to first pickup location
	rlink.command(MOTOR_3_GO, 110); //close arms
	usleep(1000000*3); // 1 million * number of seconds, for arms to close
	
	usleep(1000000*2.5); //time for ball to enter colour sensor
	
	colour col = measurecolour(rlink);

	
	set_actuator(rlink,1,1); //kick out ball
	bool weightbool = measure_weight(rlink);
	
	set_actuator(rlink,1,0);// returns 'kicker' to default down position
	
	rlink.command(MOTOR_3_GO, 237); //open arms
	usleep(1000000*3);// time for arms to open, might be some space to merge this time with waits above
	rlink.command(MOTOR_3_GO, 0); //shut it off
	
	
	
	balllist.push_back(ball(col, weightbool));
	
	return balllist;
}

/*
void deliver(robot_link& rlink, vector<ball> balllist, stopwatch& watch){
	//takes timer started in start()
	move_from(rlink, ball6_to_ref);
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
		
		if((balllist[0].ball_colour == white && balllist[0].weight == true) || (balllist[0].ball_colour ==  multicolour && balllist[0].weight == false)){
			cout << "Delivering ball 2/ ball 5" << endl;
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
		
		if(balllist[0].ball_colour == yellow && balllist[0].weight == true){
			cout << "Delivering ball 4" << endl;
			move_from(rlink, ref_to_D3);
			dropball(rlink);
			move_from(rlink, D3_to_ref);
			
			
		}
		balllist.erase(balllist.begin());
	
}
	

	
	
}
*/
	
	
	
	
	
	
	
	
