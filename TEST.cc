#include <iostream>
#include "header.h"
#include <robot_instr.h>
#include <robot_link.h>
#include <stopwatch.h>
#include <vector>
#include <unistd.h>
#include <unistd.h>

#include <stdlib.h> //For rand
#include <time.h>  

//bash-4.2$ scp a.out.arm team@wlan-robot2.private:a.out.arm
//bash-4.2$ slogin team@wlan-robot2.private



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
	
}
void continual_drop(robot_link& rlink){
	while(true){
		set_actuator(rlink, 1, 1);
		stopwatch watch;
		watch.start();
		usleep(3*1000000);
		set_actuator(rlink, 1, 0);
		usleep(1*1000000);
	}
	
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
	
	bool weightbool = get_weight(rlink);
	
	balllist.push_back(ball(col, weightbool));
	
}


void test_sreverse(robot_link& rlink) {
	
	forwards_untill_impact(rlink);
	backwardspedal(rlink, 500);
	turn_left_until_first_line(rlink);
	forwards(rlink,2);
	spin(rlink);
	forwards(rlink,2);	
	forwardspedal(rlink, 1000);	
	
	sreverese(rlink);	
	forwardspedal(rlink,5000);
	sweepforline(rlink);
	
	sreverese(rlink);
	forwardspedal(rlink,5000);
	sweepforline(rlink);
	
	sreverese(rlink);
	forwardspedal(rlink,5000);
	sweepforline(rlink);
	
	sreverese(rlink);
	forwardspedal(rlink,5000);
	sweepforline(rlink);
	
	sreverese(rlink);
	forwardspedal(rlink,5000);
	sweepforline(rlink);
	
}

void test_consecutive_ball_delivery(robot_link& rlink) {
	move_from(rlink, ball1_to_ref);
	move_from(rlink, ref_to_D1);
	move_from(rlink, D1_to_ref);
	move_from(rlink, ref_to_D2);
	move_from(rlink, D2_to_ref);
	move_from(rlink, ref_to_D3);
	move_from(rlink, D3_to_ref);
	move_from(rlink, ref_to_DR);
	move_from(rlink, DR_to_ref);
	move_from(rlink, ref_to_start);
}

void test_continual_ball_delivery(robot_link& rlink) {
	srand(time(NULL));
	while(true) {
		int random_num = rand() % 4; //0, 1, 2 or 3
		if (random_num == 0) {
			cout << "Delivering to DR" << endl;
			move_from(rlink, ref_to_DR);
			move_from(rlink, DR_to_ref);
		} else if (random_num == 1) {
			cout << "Delivering to D1" << endl;
			move_from(rlink, ref_to_D1);
			move_from(rlink, D1_to_ref);	
		} else if (random_num == 2) {
			cout << "Delivering to D2" << endl;
			move_from(rlink, ref_to_D2);
			move_from(rlink, D2_to_ref);
		} else {
			cout << "Delivering to D3" << endl;
			move_from(rlink, ref_to_D3);
			move_from(rlink, D3_to_ref);
		}
		
	}
}

void ballID(robot_link& rlink) {


	vector<ball> balllist; //stores list of balls
	

	set_actuator(rlink, 2, 0); //reset kicker
	
	
	
	for(int i=0;i<999;i++){
	
	
		int speed = 127;
		double time_for_close_arms = 0.9; //time for arms to close/open
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
		
		cout << "weightbool is "<< weightbool<< endl;
		cout << "colour is " << col << endl;
		
		set_actuator(rlink, 2, 1); //kick ball out
		usleep(1000000*2);
		set_actuator(rlink, 2, 0); //reset kicker
		
		
		
		rlink.command(MOTOR_3_GO, speed); //open arms
		usleep(1000000*time_for_close_arms);// time for arms to open, might be some space to merge this time with waits above
		rlink.command(MOTOR_3_GO, 0); //shut it off
		balllist.push_back(ball(col, weightbool));
		
		

		//code below chooses which LED to light
		cout << " ALERT " << endl;
		cout << " ALERT " << endl;
		cout << " ALERT " << endl;
		
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
		

		cout << " ALERT " << endl;
		cout << " ALERT " << endl;
		cout << " ALERT " << endl;
		usleep(5*1000000);
	}
	
	
	
}
/*
int main(){
	
	cout << "program started"<< endl;
	robot_link rlink;
	initialise(rlink);
	
	stopwatch watch;
	watch.start();
	vector<ball> balllist;
	//balllist.push_back(ball(white, 0));
	balllist.push_back(ball(white, 1));
	balllist.push_back(ball(yellow, 0));
	balllist.push_back(ball(yellow, 1));
	balllist.push_back(ball(multicolour, 0));
	
	//takes timer started in start()
	set_actuator(rlink, 2, 0); //put dropoff in delivery position
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
