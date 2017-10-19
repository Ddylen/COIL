#include <iostream>
#include "header.h"
#include <robot_instr.h>
#include <robot_link.h>
#include <stopwatch.h>
#include <vector>
#include <unistd.h>
using namespace std;
//this is untested prototype code

void forwards(robot_link& rlink, int stopatthisnumofintersections){
	
	int left, middle, right, threshold, n;
	int cruise, slow, veryslow, stop; //speeds
	cruise = 127;
	slow =  80;
	veryslow = 20;
	stop = 0;
	threshold = 0; 
	n= 1; // for infinite loop
	
	int onintersection = 0; // 0 for off, 1 for on, ensures 1 intersection does not get logged as multiple
	int intersectioncount = 0; //counts number of intersections passed
	stopwatch watch;
	watch.start();
	while(n>0){
		left =  readbit(0, 6, rlink);
		middle = readbit(0, 5, rlink);
		right = readbit(0, 4, rlink); // needs to be updated with final sensor positions
		/*
		left =  readbit(0, 0, rlink);
		middle = readbit(0, 1, rlink);
		right = readbit(0, 2, rlink); // needs to be updated with final sensor positions
		*/
		cout <<  left<< " " <<  middle<< " " << right << " " << endl;
		if(middle > threshold && left<=threshold && right <= threshold){
			// well aligned
			cout << "Aligned" << endl;
			rlink.command(MOTOR_1_GO, cruise);
			rlink.command(MOTOR_2_GO, 0.9*cruise);
			if(onintersection==1){
				onintersection =0;
				intersectioncount = intersectioncount + 1; // increment intersection counter
			}
		}
		if(middle > threshold && left>threshold && right > threshold){
			// on intersection
			cout << "Intersection" << endl;
			rlink.command(MOTOR_1_GO, cruise);
			rlink.command(MOTOR_2_GO, 0.9*cruise);
			if(onintersection==0){
				onintersection =1; // ready intersection counter to be incremented
			}
			
		}
		if(middle > threshold && left > threshold && right <= threshold){
			// little too left
			cout << "Little too right" << endl;
			rlink.command(MOTOR_1_GO, slow);
			rlink.command(MOTOR_2_GO, cruise);
		}
		if(middle > threshold && left <= threshold && right > threshold){
			// little too right
			cout << "little too left" << endl;
			rlink.command(MOTOR_1_GO, cruise);
			rlink.command(MOTOR_2_GO, slow);
		}
		if(middle <= threshold && left > threshold && right <= threshold){
			// far too left
			cout << "far too right" << endl;
			rlink.command(MOTOR_1_GO, veryslow);
			rlink.command(MOTOR_2_GO, cruise);
		}
		if(middle <= threshold && left <= threshold && right > threshold){
			// far too right
			cout << "far too left" << endl;
			rlink.command(MOTOR_1_GO, cruise);
			rlink.command(MOTOR_2_GO, veryslow);
		}
		if(middle <= threshold && left <= threshold && right <= threshold){
			// lost
			cout << "lost" << endl;
			rlink.command(MOTOR_1_GO, stop);
			rlink.command(MOTOR_2_GO, stop);
			n=0;
			
			// run some kind of lost function

		}
		if(watch.read()>10000){
			n= 0;
			cout << "Time out of forwards" << endl;
			// replace this with a 'number of intersections crossed' condition to end, will require new input
		}
		if(intersectioncount == stopatthisnumofintersections){
			n=0;
			
	}

}
}




void move_from(robot_link& rlink, enum_matchings input){
	// stores hardcoded routes between different points, these are estimates and have not been tested
		if (input == start_to_ball1 ) {
			forwards(rlink,2);
			right(rlink);
			forwards_untill_impact(rlink);
		}
		if (input == ball1_to_ball2 || input == ball2_to_ball3 || input == ball3_to_ball4 || input == ball4_to_ball5 || input == ball5_to_ball6) {
			back_to_line(rlink);		//reverse untill wheelbase reaches line
			left(rlink);
			backwards(rlink, 1);
			right(rlink);
			forwards_untill_impact(rlink);
		}
		if (input == ball6_to_ref) {
			back_to_line(rlink);		//reverse untill wheelbase reaches line
			right(rlink);
			forwards(rlink, 6);
			right(rlink);
			forwards(rlink, 3);
			right(rlink);
			forwards(rlink, 2);
		}
		if (input == ref_to_D1) {
			right(rlink);
			forwards_untill_impact(rlink);
		}
		
		if (input == D1_to_ref) {
			backwardspedal(rlink, 0.5);
			spin(rlink);
			right(rlink);
		}
		if (input == ref_to_D2) {
			forwardspedal(rlink, 0.2);
			right(rlink);
			forwards_untill_impact(rlink);
		}
		if (input == D2_to_ref) {
			backwardspedal(rlink, 0.5);
			spin(rlink);
			left(rlink);
			forwardspedal(rlink, 2);
			spin(rlink);
			forwards(rlink, 1);
		}
		
		if (input == ref_to_D3) {
			forwards(rlink, 1);
			right(rlink);
			forwards_untill_impact(rlink);
		}
		
		if (input == D3_to_ref) {
			backwardspedal(rlink, 0.5);
			spin(rlink);
			left(rlink);
			forwardspedal(rlink, 4);
			spin(rlink);
			forwards(rlink, 2);
		}
		
		if (input == ref_to_DR) {
			forwards_untill_impact(rlink);
			backwardspedal(rlink, 0.5);
		}
		
		if (input == DR_to_ref) {
			backwardspedal(rlink, 0.5);
			spin(rlink);
			forwards(rlink, 3);
			forwardspedal(rlink, 1);
			spin(rlink);
			forwards(rlink, 2);
		}
		
		if (input == ref_to_start) {
			spin(rlink);
			forwards(rlink, 1);
			left(rlink);
			forwards(rlink, 4);
			left(rlink);
		}
		
			
}

void sweepforline(robot_link& rlink){
	bool foundline = false;
	int sweeptime = 0.25;
	int sweepnumber = 1;
	int leftmotorspeed = 127;
	int rightmotorspeed = 255;
	stopwatch watch;
	while(foundline == false){
		watch.start();
		int left =  readbit(0, 0, rlink);
		int middle = readbit(0, 1, rlink);
		int right = readbit(0, 2, rlink); // needs to be updated with final sensor positions
		while(watch.read()<sweeptime){
			rlink.command(MOTOR_1_GO, leftmotorspeed);
			rlink.command(MOTOR_2_GO, rightmotorspeed);
			if(middle == 1){
				foundline = true;
			}
		sweepnumber++;
		sweeptime = sweeptime*sweepnumber;
		int lastleftmotorspeed = leftmotorspeed;
		int lastrightmotorspeed =  rightmotorspeed;
		leftmotorspeed = lastrightmotorspeed;
		rightmotorspeed = lastleftmotorspeed;
		if(watch.read()> 10000){
			foundline = true;
			cout << "Time out as arc took over 10 seconds, robot is lost" << endl;
			throw "Time out as arc took over 10 seconds, robot is lost" ;
		}
}
}
}
