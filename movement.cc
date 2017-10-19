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
	
	onintersection = 0; // 0 for off, 1 for on, ensures 1 intersection does not get logged as multiple
	intersectioncount = 0; //counts number of intersections passed
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
				onintersection ==0;
				intersectioncount = intersectioncount + 1; // increment intersection counter
			}
		}
		if(middle > threshold && left>threshold && right > threshold){
			// on intersection
			cout << "Intersection" << endl;
			rlink.command(MOTOR_1_GO, cruise);
			rlink.command(MOTOR_2_GO, 0.9*cruise);
			if(onintersection==0){
				onintersection ==1; // ready intersection counter to be incremented
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




void move_from(robot_link& rlink, enum_matchings input){
	// stores hardcoded routes between different points, these are estimates and have not been tested
		if (input == start_to_ball1 ) {
			forwards(rlink,2);
			right();
			forwards_untill_impact();
		}
		if (input == ball1_to_ball2 || input == ball2_to_ball3 || input == ball3_to_ball4 || input == ball4_to_ball5 || input == ball5_to_ball6) {
			back_to_line();		//reverse untill wheelbase reaches line
			left();
			backwards(1);
			right()
			forwards_until_impact();
		}
		if (input == ball6_to_ref) {
			back_to_line();		//reverse untill wheelbase reaches line
			right();
			forwards(6);
			right();
			forwards(3);
			right;
			forwards(2);
		}
		if (input == ref_to_D1) {
			right();
			forwards_untill_impact();
		}
		
		if (input == D1_to_ref) {
			backpedal(0.5);
			spin();
			right();
		}
		if (input == ref_to_D2) {
			forwardspedal(0.2);
			right();
			forwards_until_impact();
		}
		if (input == D2_to_ref) {
			backpedal(0.5);
			spin();
			left();
			forwardspedal(2);
			spin();
			forwards(1);
		}
		
		if (input == ref_to_D3) {
			forwards(1);
			right();
			forwards_until_impact(2);
		}
		
		if (input == D3_to_ref) {
			backpedal(0.5);
			spin();
			left();
			forwardspedal(4);
			spin();
			forwards(2);
		}
		
		if (input == ref_to_DR) {
			forwards_until_impact();
			backwardspedal(0.5);
		}
		
		if (input == DR_to_ref) {
			backpedal(0.5);
			spin();
			forwards(3);
			forwardspedal(1);
			spin;
			forwards(2);
		}
		
		if (input == ref_to_start) {
			spin();
			forwards(1);
			left();
			forwards(4);
			left();
		}
		
			
}
