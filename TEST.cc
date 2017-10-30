#include <iostream>
#include "header.h"
#include <robot_instr.h>
#include <robot_link.h>
#include <stopwatch.h>
#include <vector>
#include <unistd.h>
#include <unistd.h>

using namespace std;

void test_long_path(robot_link& rlink) {
	while(true) {
		cout << "TEST.cc main starting forwards(3)" << endl;
		forwards(rlink, 3);
		cout << "TEST.cc main starting turn right" << endl;
		right(rlink);
		cout << "TEST.cc main starting forwardspedal" << endl;
		forwardspedal(rlink, 3000);
		cout << "TEST.cc main starting spin" << endl;
		spin(rlink);
		cout << "TEST.cc main starting forwards(rlink, 1)" << endl;
		forwards(rlink, 1);
		cout << "TEST.cc main starting left()" << endl;
		left(rlink);
		cout << "TEST.cc main starting forwards(2)" << endl;
		forwards(rlink, 2);
		cout << "TEST.cc main starting forwardspedal(4000)" << endl;
		forwardspedal(rlink, 4000);
		cout << "TEST.cc main starting spin()" << endl;
		spin(rlink);
		cout << "TEST.cc main restarting loop" << endl;
	}
	//cout << "TEST.cc main starting turn left" << endl;
	//left(rlink);
	//cout << "TEST.cc main starting forwards" << endl;
	//forwards(rlink, 4);
	//spin(rlink);
	//forwards(rlink, 6);
	//right(rlink);
	//forwards(rlink,3);
	//right(rlink);
	//forwards(rlink,2);
	//right(rlink);
	//sweepforline(rlink);
}

void test_colour_sensor(robot_link& rlink) {

	while(true){
		colour col = measurecolour(rlink);
		if(col  == yellow){
			cout<< "Yelow " << rlink.request(ADC0) << endl;
		}
		if(col  == white){
			cout<< "White " << rlink.request(ADC0) << endl;
		}
		if(col  == multicolour){
			cout<< "Multicolour " << rlink.request(ADC0) << endl;
		}
		if(col  == ambient){
			cout<< "Ambient " << rlink.request(ADC0) << endl;
		}
		
		usleep(500000);
	}

}

void test_wheel_speeds(robot_link& rlink) {
	full_speed_forwards(rlink);
}

void test_angle_correction1(robot_link& rlink) {
	forwards(rlink, 1);
	left(rlink);
	forwardspedal(rlink, 500);
	spin(rlink);
	for(int i = 0; i < 1; i++) {
		forwards_untill_impact(rlink);
		backwardspedal(rlink, 1500);
		turn_left_until_first_line(rlink);
	}
	forwards_untill_impact(rlink);
	backwardspedal(rlink, 800);
	turn_left_until_first_line(rlink);
	//forwardspedal(rlink, 500);
	forwards(rlink, 2);
	left(rlink);
	forwardspedal(rlink, 500);
	spin(rlink);
	for(int i = 0; i < 1; i++) {
		forwards_untill_impact(rlink);
		backwardspedal(rlink, 1750);
		turn_left_until_first_line(rlink);
	}
	
}

void headbutt(robot_link& rlink) {
	while(true) {
		forwards_untill_impact(rlink);
		backwardspedal(rlink, 2000);
		cout << "Next headbutt loop" << endl;
	}
}

void linefollowdemo(robot_link& rlink){
	forwards(rlink,1);
	left(rlink);
	forwards_untill_impact(rlink);
}

void testcollisionbutton(robot_link& rlink){
	forwards_untill_impact(rlink);
}

void loadpositiontest(robot_link& rlink){
	forwards(rlink,3);
	forwardspedal(rlink,200);
	turn_left_until_first_line(rlink);
	forwardspedal(rlink,3250);
	spin(rlink);
	turn_right_until_first_line(rlink);
	forwardspedal(rlink, 3000);
	/*
	forwards_untill_impact(rlink);
	for(int i=0;i<5; i++){
		sreverese(rlink);
		forwardspedal(rlink,500);
		sweepforline(rlink);
		forwards_untill_impact(rlink);
		forwardspedal(rlink, 500);
	}
	*/
}

void sweeptest(robot_link& rlink){
	forwardspedal(rlink,500);
	turn_left_until_first_line(rlink);
	forwards(rlink,3);
}

void balldetectiontest(robot_link& rlink){
	vector<ball> balllist; //stores list of balls
	
	//movement program top get to first pickup location
	rlink.command(MOTOR_3_GO, 110); //close arms
	usleep(1000000*3); // 1 million * number of seconds, for arms to close
	
	usleep(1000000*2.5); //time for ball to enter colour sensor
	
	colour col = measurecolour(rlink);

	
	
	set_actuator(rlink,1,1); //kick out ball
	usleep(1000000*0.5); // time for ball to be kicked out
	
	set_actuator(rlink,1,0);// returns 'kicker' to default down position
	
	rlink.command(MOTOR_3_GO, 237); //open arms
	usleep(1000000*3);// time for arms to open, might be some space to merge this time with waits above
	rlink.command(MOTOR_3_GO, 0); //shut it off
	
	bool weightbool = measure_weight(rlink);
	
	balllist.push_back(ball(col, weightbool));
	
}
int main(){
	cout << "program started"<< endl;
	robot_link rlink;
	initialise(rlink);
	
	//loadpositiontest(rlink); //test1
	//sweeptest(rlink); //test2
	move_from(rlink, ref_to_D3); //test3
	
	/*
	forwards(rlink,3);
	forwardspedal(rlink,200);
	turn_left_until_first_line(rlink);
	forwardspedal(rlink,3250);
	spin(rlink);
	turn_right_until_first_line(rlink);
	forwardspedal(rlink, 3000);
	
	forwards_untill_impact(rlink);
	for(int i=0;i<5; i++){
		sreverese(rlink);
		forwardspedal(rlink,500);
		sweepforline(rlink);
		forwards_untill_impact(rlink);
		forwardspedal(rlink, 500);
	}
	*/
	
	
	return 0;


}
