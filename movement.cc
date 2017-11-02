#include <iostream>
#include "header.h"
#include <robot_instr.h>
#include <robot_link.h>
#include <stopwatch.h>
#include <vector>
#include <unistd.h>
using namespace std;
//this is untested prototype code

static const float right_wheel_correction_factor = 1.0;
static const float left_wheel_correction_factor = 1.0; //0.999
	
void changespeed(robot_link& rlink, int motor1speed, int motor2speed){
	
	//cout << "movement.cc changespeed motor1speed: " << motor1speed << " motor2speed: " << motor2speed << endl;
	//motor1 = left motor2 = right
	static int prev_motor1speed = -1;
	static int prev_motor2speed = -1;
	static bool is_first_call = true;
	
	if (is_first_call) {
		prev_motor1speed = motor1speed;
		prev_motor2speed = motor2speed;
		is_first_call = false;
		
		rlink.command(MOTOR_4_GO, motor1speed);
		rlink.command(MOTOR_2_GO, motor2speed);		
	} else {
		if(motor1speed != -1 && motor1speed != prev_motor1speed) {
			//cout << "movement.cc changespeed calling rlink for motor1" << endl;
			rlink.command(MOTOR_4_GO, motor1speed);
			
		}
		if (motor2speed != -1 && motor2speed != prev_motor2speed) {
			//cout << "movement.cc changespeed calling rlink for motor2" << endl;
			rlink.command(MOTOR_2_GO, motor2speed);
		}
		prev_motor1speed = motor1speed;
		prev_motor2speed = motor2speed;
	}
}
	
void forwards(robot_link& rlink, int stopatthisnumofintersections){
	
	int left, middle, right, threshold, n;
	int cruise, slow, veryslow, stop; //speeds
	cruise = 110;
	slow =  60;
	veryslow = 20;
	stop = 0;
	threshold = 0; 
	n= 1; // for infinite loop
	
	stopwatch watch;
	watch.start();
	
	bool onintersection = false; // ensures 1 intersection does not get logged as multiple
	int intersectioncount = 0; //counts number of intersections passed
	while(n>0){
		left =  readbit(0, 6, rlink);
		middle = readbit(0, 5, rlink);
		right = readbit(0, 4, rlink); // needs to be updated with final sensor positions

		//cout <<"movement.cc forwards() " <<  left<< " " <<  middle<< " " << right << " " << endl;
		if(middle > threshold && left<=threshold && right <= threshold){
			// well aligned
			//cout << "Aligned" << endl;
			changespeed(rlink,cruise,cruise);
		}
		if(middle > threshold && left>threshold && right > threshold){
			// on intersection
			cout << "movement.cc forwards() Intersection. Time: " << watch.read() << endl;
			changespeed(rlink,cruise,cruise);
			if (onintersection == false) {
				cout << "movement.cc forwards() incrementing intersection count" << endl;
				intersectioncount++;
			}
			onintersection = true;
			
			
		} else {
			onintersection = false;
		}
		if(middle > threshold && left > threshold && right <= threshold){
			// little too left
			//cout << "Little too right" << endl;
			changespeed(rlink,slow, cruise);
		}
		if(middle > threshold && left <= threshold && right > threshold){
			// little too right
			//cout << "little too left" << endl;
			changespeed(rlink,cruise,slow);
		}
		if(middle <= threshold && left > threshold && right <= threshold){
			// far too left
			//cout << "far too right" << endl;
			changespeed(rlink,veryslow,cruise);
		}
		if(middle <= threshold && left <= threshold && right > threshold){
			// far too right
			//cout << "far too left" << endl;
			changespeed(rlink,cruise,veryslow);
		}
		if(middle <= threshold && left <= threshold && right <= threshold){
			// lost
			//cout << "lost" << endl;
			changespeed(rlink,stop,stop);
			sweepforline(rlink);

		}
		if(intersectioncount == stopatthisnumofintersections){
			n=0;			
		}

}
}

void forwardspedal(robot_link& rlink, int time){
	//time is in milliseconds
	cout << "movement.cc Forwardspedal() for " << time << " started" << endl;
	int left, middle, right, threshold, n;
	int cruise, slow, veryslow, stop; //speeds
	cruise = 127;
	slow =  80;
	veryslow = 20;
	stop = 0;
	threshold = 0; 
	n= 1; // for infinite loop
	
	stopwatch watch;
	watch.start();
	while(n>0){
		left =  readbit(0, 6, rlink);
		middle = readbit(0, 5, rlink);
		right = readbit(0, 4, rlink); // needs to be updated with final sensor positions

		//cout <<"movement.cc forwardspedal() " <<  left<< " " <<  middle<< " " << right << " " << endl;
		if(middle > threshold && left<=threshold && right <= threshold){
			// well aligned
			//cout << "Aligned" << endl;
			changespeed(rlink,cruise,cruise);
		}
		if(middle > threshold && left>threshold && right > threshold){
			// on intersection
			//cout << "Intersection" << endl;
			changespeed(rlink,cruise,cruise);		
			
		}
			
		if(middle > threshold && left > threshold && right <= threshold){
			// little too left
			//cout << "Little too right" << endl;
			changespeed(rlink,slow,cruise);
		}
		if(middle > threshold && left <= threshold && right > threshold){
			// little too right
			//cout << "little too left" << endl;
			changespeed(rlink,cruise,slow);
		}
		if(middle <= threshold && left > threshold && right <= threshold){
			// far too left
			//cout << "far too right" << endl;
			changespeed(rlink,veryslow,cruise);
		}
		if(middle <= threshold && left <= threshold && right > threshold){
			// far too right
			//cout << "far too left" << endl;
			changespeed(rlink,cruise,veryslow);
		}
		if(middle <= threshold && left <= threshold && right <= threshold){
			// lost
			//cout << "lost" << endl;
			changespeed(rlink,stop,stop);
			sweepforline(rlink);

		}
		if(watch.read()>time){
			n= 0;
			//cout << "movement.cc Forwardspedal() finished timeout of " << time << endl;
		}


}
cout << "movement.cc Forwardspedal() complete "<< endl;
}

void right(robot_link& rlink){
	cout << "movement.cc right()"<< endl;
	cout << " " << endl;
	
	int middle = readbit(0, 5, rlink);
	
	int wheelbasesensor= readbit(0, 7, rlink);
	
	stopwatch watch;
	rlink.command (RAMP_TIME, 0);
	
	int ignoretime = 1000;// in ms //Used to be 1200
	cout << "movement.cc right() Moving to intersection"<< endl;
	while (wheelbasesensor==0){
		forwardspedal(rlink, 0.01);
		wheelbasesensor= readbit(0, 7, rlink);
	}// loops exists when wheelbase sensor reaches white
	cout << "movement.cc right() Turning right"<< endl;
	changespeed(rlink,127,255);
	watch.start();
	while(true){
		
		if(watch.read()>ignoretime){
		changespeed(rlink,60,187);
			middle = readbit(0, 5, rlink);
			if(middle ==1){
				changespeed(rlink, 0,0);
				break;
				
			}
			

		}
	}
	cout << "movement.cc right() found right" << endl;	
}

void left(robot_link& rlink){
	cout << "movement.cc left() TURNING LEFT" << endl;
	cout << " " << endl;
	
	int middle = readbit(0, 5, rlink);
	
	
	int wheelbasesensor= readbit(0, 7, rlink);
	
	stopwatch watch;
	rlink.command (RAMP_TIME, 0);
	
	int ignoretime = 1000;// in ms // used to be 1200
	cout << "movement.cc left() Moving to intersection"<< endl;
	while (wheelbasesensor==0){
		forwardspedal(rlink, 0.01);
		wheelbasesensor= readbit(0, 7, rlink);
	}// loops exists when wheelbase sensor reaches white
	cout << "movement.cc left() Turning left"<< endl;
	changespeed(rlink,255,127);
	watch.start();
	while(true){
		
		if(watch.read()>ignoretime){
		changespeed(rlink,187,60);
			middle = readbit(0, 5, rlink);
			if(middle ==1){
				changespeed(rlink, 0,0);
				break;
				
			}
			

		}
	}
	cout << "movement.cc left() found left" << endl;	
}

void spin(robot_link& rlink){

	cout << "movement.cc spin() SPINNING" << endl;
	cout << " " << endl;
	
	int middle = readbit(0, 5, rlink);
	
	
	stopwatch watch;
	rlink.command (RAMP_TIME, 0);
	
	int ignoretime = 2100;// in ms
	
	changespeed(rlink,255,127);
	watch.start();
	while(true){
		
		if(watch.read()>ignoretime){
		changespeed(rlink,187,60);
			middle = readbit(0, 5, rlink);
			if(middle ==1){
				changespeed(rlink, 0,0);
				cout << "spin breaking at time: " << watch.read() << endl;
				break;
				
			}
			

		}
	}
	cout << "movement.cc spin() spin complete" << endl;	
	
}

void forwards_untill_impact(robot_link& rlink){
	int left, middle, right, threshold;
	int cruise, slow, veryslow, stop; //speeds
	cruise = 127;
	slow =  80;
	veryslow = 20;
	stop = 0;
	threshold = 0; 
	cout << "movement.cc forwards_untill_impact() started " << endl;
	
	
	stopwatch watch;
	watch.start();
	int impactsensor = readbit(0, 2, rlink); 
	while(impactsensor == 0){
		impactsensor = readbit(0, 2, rlink);
		left =  readbit(0, 6, rlink);
		middle = readbit(0, 5, rlink);
		right = readbit(0, 4, rlink); // needs to be updated with final sensor positions

		//cout <<"movement.cc forwards_untill_impact() " <<  left<< " " <<  middle<< " " << right << " " << endl;
		if(middle > threshold && left<=threshold && right <= threshold){
			// well aligned
			//cout << "Aligned" << endl;
			changespeed(rlink,cruise,cruise);
		}
		if(middle > threshold && left>threshold && right > threshold){
			// on intersection
			//cout << "Intersection" << endl;
			changespeed(rlink,cruise,cruise);
			
			
		}
		if(middle > threshold && left > threshold && right <= threshold){
			// little too left
			//cout << "Little too right" << endl;
			changespeed(rlink,slow,cruise);
		}
		if(middle > threshold && left <= threshold && right > threshold){
			// little too right
			//cout << "little too left" << endl;
			changespeed(rlink,cruise,slow);
		}
		if(middle <= threshold && left > threshold && right <= threshold){
			// far too left
			//cout << "far too right" << endl;
			changespeed(rlink,veryslow,cruise);
		}
		if(middle <= threshold && left <= threshold && right > threshold){
			// far too right
			//cout << "far too left" << endl;
			changespeed(rlink,cruise,veryslow);
		}
		if(middle <= threshold && left <= threshold && right <= threshold){
			// lost
			//cout << "lost" << endl;
			changespeed(rlink,stop,stop);
			sweepforline(rlink);

		}
		


	}
	if(impactsensor == 1){
		changespeed(rlink,stop,stop);
		cout << "WE HAVE IMPACT" << endl;
	}
}
	
void full_speed_forwards(robot_link& rlink) {
		//For debugging
	changespeed(rlink, (int)(left_wheel_correction_factor * 127.f), (int)(right_wheel_correction_factor * 127.f));
	usleep(100000000);
}


void sweepforline(robot_link& rlink){ 
	cout << "Sweep for line started" << endl;
	bool foundline = false;
	int sweeptime = 500; // in milliseconds
	int sweepnumber = 1;
	int leftmotorspeed = 177;
	int rightmotorspeed = 50;
	int middle = readbit(0, 5, rlink);
	stopwatch watch;
	
	while(foundline == false){
		//cout << "starting sweep number " << sweepnumber << endl;
		watch.start();
		cout << watch.read() << 	endl;
		while(watch.read()<sweeptime && foundline == false){
			middle = readbit(0, 5, rlink);
			//cout<< "time is " << watch.read()<< "and middle sensor reads " << middle << endl;
			changespeed(rlink, leftmotorspeed, rightmotorspeed);
			
			if(middle == 1){
				cout << "LINE FOUND" << endl;
				foundline = true;
				changespeed(rlink, 0, 0);
			
			}
		}
		changespeed(rlink, 0, 0);
		sweepnumber = sweepnumber +1;
		sweeptime = sweeptime*sweepnumber;
		int lastleftmotorspeed = leftmotorspeed;
		int lastrightmotorspeed =  rightmotorspeed;
		leftmotorspeed = lastrightmotorspeed;
		rightmotorspeed = lastleftmotorspeed;
		if(watch.read()> 5000){
			foundline = true;
			cout << "Time out as arc took over 10 seconds, robot is lost" << endl;
			throw "Time out as arc took over 10 seconds, robot is lost" ;
		}
	
	}
	changespeed(rlink, 0, 0);
	}
	
void backwardspedal(robot_link& rlink, int timeout) {
	//Timeout in milliseconds
	cout << "movement.cc backwardspedal()" << endl;
	stopwatch watch;
	watch.start();
	changespeed(rlink, 230, 230);
	while(watch.read() < timeout) {		
		//changespeed(rlink, 250, 250);
	}
	//changespeed(rlink, 0, 0);
	cout << "movement.cc backwardspedal() finished()" << endl;
}

void turn_left_until_first_line(robot_link& rlink) {
	cout << "movement.cc left() turn_left_until_first_line" << endl;
	cout << " " << endl;
	
	int middle = readbit(0, 5, rlink);
	
	rlink.command (RAMP_TIME, 0);
	changespeed(rlink, 187, 60);
	usleep(500000);
	
	
	while(true){
		changespeed(rlink,187,60);
		middle = readbit(0, 5, rlink);
		if(middle ==1){
			changespeed(rlink, 0,0);
			break;			
		}
		
	}
	cout << "movement.cc leftuntillline() found left" << endl;	
}

void turn_right_until_first_line(robot_link& rlink) {
	cout << "movement.cc right() turn_right_until_first_line" << endl;
	cout << " " << endl;
	
	int middle = readbit(0, 5, rlink);
	
	rlink.command (RAMP_TIME, 0);
	changespeed(rlink, 60, 187);
	usleep(500000);
	
	
	while(true){
		changespeed(rlink,60,187);
		middle = readbit(0, 5, rlink);
		if(middle ==1){
			changespeed(rlink, 0,0);
			break;			
		}
		
	}
	cout << "movement.cc rightuntillline() found right" << endl;	
}

void sreverese(robot_link& rlink){
	cout << "movement.cc Sreverse() started" << endl;
	
	int slow = 180;
	int fast = 250;
	int timepersection = 1.95*1000000; 
	backwardspedal(rlink, 250);

	changespeed(rlink, fast, slow);
	usleep(timepersection);
	changespeed(rlink, slow, fast);
	usleep(timepersection);
	cout << "movement.cc Sreverse() complete" << endl;
	
	
}


void move_from(robot_link& rlink, enum_matchings input){
	// stores hardcoded routes between different points, these are estimates and have not been tested
	//ball 6 to ref and most other locatiosn need to be updated
		if (input == start_to_ball6 ) {
			forwards_untill_impact(rlink);
			backwardspedal(rlink, 500);
			turn_left_until_first_line(rlink);
			forwards(rlink,2);
			forwardspedal(rlink, 500);
			spin(rlink);
			forwards(rlink,3);	
			forwardspedal(rlink, 1000);	
			rlink.command(MOTOR_2_GO, 0);
			rlink.command(MOTOR_4_GO, 0);
		}
	
		if (input == ball_to_nearestrightball) {
			// should work, has NOT been tested
			cout<< "about to sreverse"  << endl;
			sreverese(rlink);	
			cout << "sreverse done " << endl;
			sweepforline(rlink); 
			forwardspedal(rlink,3500);
			rlink.command(MOTOR_2_GO, 0);
			rlink.command(MOTOR_4_GO, 0);
		}
		
		
		if (input == ball1_to_ref) {
			backwardspedal(rlink, 1000);
			turn_right_until_first_line(rlink);
			forwards_untill_impact(rlink);
			backwardspedal(rlink, 1500);
			right(rlink);
			forwards(rlink, 3);
			right(rlink);
			forwards(rlink, 2);
			forwardspedal(rlink, 100);
			changespeed(rlink,0,0);
		}
		
		if (input == ref_to_D3) {
			backwardspedal(rlink,500);
			forwards(rlink,1);
			right(rlink);
			forwards_untill_impact(rlink);
			changespeed(rlink,0,0);
		}

		if (input == D3_to_ref) {
			backwardspedal(rlink,200);
			spin(rlink);
			right(rlink);
			backwardspedal(rlink, 1000);
			forwards(rlink,1);
			changespeed(rlink,0,0);
		}

		if (input == ref_to_D2) {
			forwardspedal(rlink, 200);// so wheelbase sensor doesnt hit D3 line
			forwards(rlink, 1);
			right(rlink);
			forwards_untill_impact(rlink);
			changespeed(rlink,0,0);
		}
		if (input == D2_to_ref) {
			backwardspedal(rlink, 200);
			spin(rlink);
			left(rlink);
			forwards(rlink, 1);
			spin(rlink);
			forwards(rlink, 1);
			forwardspedal(rlink, 100); // to get just ahead of the intersection
			changespeed(rlink,0,0);
		}
		
		if (input == ref_to_D1) {
			forwards(rlink, 1);
			forwardspedal(rlink, 700);
			right(rlink);
			forwards_untill_impact(rlink);
			changespeed(rlink,0,0);
		}
		
		if (input == D1_to_ref) {
			backwardspedal(rlink, 200);
			spin(rlink);
			left(rlink);
			forwards(rlink, 2);
			spin(rlink);
			forwards(rlink, 1);
			forwardspedal(rlink, 100); // to get just ahead of the intersection
			changespeed(rlink,0,0);
		}
		
		if (input == ref_to_DR) {
			forwards_untill_impact(rlink);
			changespeed(rlink,0,0);
		}
		
		if (input == DR_to_ref) {
			backwardspedal(rlink, 400);
			spin(rlink);
			forwards(rlink, 3); 
			spin(rlink);
			forwards(rlink, 1);
			forwardspedal(rlink, 100); // to get just ahead of the intersection
			changespeed(rlink,0,0);
		}
		
		if (input == ref_to_start) {
			spin(rlink);
			left(rlink);
			forwards(rlink, 6);
			left(rlink);
			forwards(rlink, 1);
			spin(rlink);
			forwards(rlink, 3);
			changespeed(rlink,0,0);
		}
		
			
}

