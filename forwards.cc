#include <iostream>
#include "header.h"
#include <robot_instr.h>
#include <robot_link.h>
#include <stopwatch.h>
#include <vector>
#include <unistd.h>
using namespace std;

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
	rlink.command (RAMP_TIME, 0);
	watch.start();
	while(n>0){
		left =  readbit(0, 6, rlink);
		middle = readbit(0, 5, rlink);
		right = readbit(0, 4, rlink); // needs to be updated with final sensor positions
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
