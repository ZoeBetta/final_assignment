/* author: Zoe Betta 5063114
 * Date of creation 02/01/2021
 * 
 * FINAL ASSIGNMENT
 * Research track 1
 *
 * server that implements the follow the wall behaviour
 *
*/

#include "ros/ros.h"
#include "std_msgs/String.h"
#include "final_assignment/wall_follow.h" 
#include "nav_msgs/Odometry.h"
#include <string>
#include "sensor_msgs/LaserScan.h"

// definition of structure
struct regions{
		 float right;
		 float fright;
		 float front;
		 float fleft;
		 float left;
		};

// definitions of global variables
using namespace nav_msgs;
int active;
ros::Publisher pub;
geometry_msgs::Twist msg;
int state=0;
int STOP=0;
regions regions_;

// function declaration
void take_action(int i);
bool wall_follow_switch(final_assignment::wall_follow::Request &req, final_assignment::wall_follow::Response &res);
float CalculateMinimum(float max, sensor_msgs::LaserScan array, int indexMin, int indexMax);
void clbk_laser( const sensor_msgs::LaserScan:: Ptr& x);
void change_state(int state);
void find_wall();
void turn_left();
void follow_the_wall();
void stop();

// main function 
int main(int argc, char **argv)
{	
	// inizializaion of:
	// subscriber on the topic /scan
	// publisher on the topic cmd_vel
	// server /wall_follow
	ros::init(argc, argv, "wall_follower");
	ros:: NodeHandle n1;
	ros:: NodeHandle n2;
	ros:: NodeHandle n3;
	ros:: Subscriber robot_position= n1.subscribe("/scan", 1000, clbk_laser);
	pub= n2.advertise<geometry_msgs::Twist>("cmd_vel", 1000);
	ros::ServiceServer service=n3.advertiseService("/wall_follow", wall_follow_switch); 
  	ros::Rate loop_rate(10);
	// loop that implements the action requested during the follow the wall behaviour
	while (ros::ok())
		{	
			// if I received the request of following the wall
			if(active == 1)
				{
					if (state==0)
						{							
							find_wall();
						}
					if (state==1)
						{
							turn_left();
						}
					if (state==2)
						{
							follow_the_wall();
						}					

				}
			// if I don't have to follow the wall I stop the robot
			else 
				{
					if (STOP==1)
						{
							stop();
							STOP=0;
						}					
					loop_rate.sleep();
				}
	ros::spinOnce();
		}

 	return 0;

}

/**
 *@brief this function reads from a global variable the elaborated input from the laser sensor and decides what the next state will be
 *@param none
 *@retval none
 */
void take_action()
{ 	
	// definition of local variables
	regions regionTA;
	regionTA=regions_;
	msg.linear.x=0;
	msg.angular.z=0;
	int d0=1;
	int d=1.5;

	//depending on the input received from the sensors and elaborated in the function clbk_laser I decide the state
	if ((regions_.front>d0) & ( regions_.fleft>d) & (regions_.fright>d))
		{
			//nothing
			change_state(0);
		}
	else if ((regions_.front<d0) & (regions_.fleft>d) & (regions_.fright>d))
		{
			//front
			change_state(1);
		}
	else if ((regions_.front>d0) & ( regions_.fleft>d)& (regions_.fright<d))
		{
			//fright
			change_state(2);
		}		
	else if ((regions_.front>d0) & (regions_.fleft<d) & (regions_.fright>d))
		{
			//fleft
			change_state(0);
		}
	else if ((regions_.front<d0) & (regions_.fleft>d) & (regions_.fright<d))
		{
			//front and fright
			change_state(1);
		}
	else if ((regions_.front<d0) & (regions_.fleft<d) & (regions_.fright>d))
		{
			//front and fleft
			change_state(1);
		}
	else if ((regions_.front<d0) & (regions_.fleft<d) & (regions_.fright<d))
		{
			//front and fleft and fright
			change_state(1);
		}
	else if ((regions_.front>d0) & (regions_.fleft<d) & (regions_.fright<d))
		{
			//fleft and fright
			change_state(1);
		}
	else 
		{
			//unknown
		}
	
}

/**
 *@brief this function is called when it receives a call from the client and it changes the global variable active
	 if the global variable is 1 the robot should follow the wall, on the other hand if it is zero it should stop following the wall
 *@param final_assignment::wall_follow::Request &req
 *@param final_assignment::wall_follow::Response &res
 *@retval none
 */
bool wall_follow_switch(final_assignment::wall_follow::Request &req, final_assignment::wall_follow::Response &res)
{	
	// definition of variables
	bool r;
	// receive the request
	active=req.data;
	// display on the screen the request
	ROS_INFO("active: %d", active);
	res.success=1;
	ROS_INFO("switched state in follow the wall");
	STOP=1;
	return true;
}

/**
 *@brief this function calculates the minimum between max and some elements of an array
 *@param max an integer that is the maximum accteptable number
 *@param sensor_msgs::LaserScan array the array I have to find the minimum of+
 *@param indexMin the index of the the element I start calculating the minimum of 
 *@param indexMax the index of the last element I look for the minimum
 *@retval TempMin the minimum between the element of the array from indexMin and indexMax and the number max
 */
float CalculateMinimum(float max, sensor_msgs::LaserScan array, int indexMin, int indexMax)
{	
	float TempMin=max;
	int i;
	for ( i=indexMin; i<=indexMax; i++)
		{
			if (array.ranges[i]<=TempMin)
				{
					TempMin=array.ranges[i];
				}
		}
	return TempMin;
}

/**
 *@brief this function is called everytime something is published on the topic /scan
         it elaborates the data from the scan and puts them in a structue, from each section
         it calculates the minimum of all the data and then it calls the function take_action that
         decides the next state
 *@param x a variable of type LaserScan that contains all of the data from the sensors
 *@retval none
 */
void clbk_laser( const sensor_msgs::LaserScan:: Ptr& x)
{	
	sensor_msgs::LaserScan rangeTemporary;
	rangeTemporary.ranges=x->ranges;
	// for each region I read the inputs from the laser and assign the minimum of them
	regions_.right=CalculateMinimum(10, rangeTemporary,0,143);
	regions_.fright=CalculateMinimum(10, rangeTemporary,144,287);	
	regions_.front=CalculateMinimum(10, rangeTemporary,288,431);
	regions_.fleft=CalculateMinimum(10, rangeTemporary,432,575);		
	regions_.left=CalculateMinimum(10, rangeTemporary,576,713);		
	take_action();
		
}

/**
 *@brief this function changes the state and prints on the screen the information about the current state
 *@param stateN is the new state that I want to implement
 *@retval none
 */
void change_state(int stateN)
{	
	// I print on the screen the new state if it is different from the one before
	if (stateN != state)
		{ 
			if (stateN==0)
				{
					ROS_INFO("Wall follower - 0 - find the wall");
				}
			if (stateN==1)
				{
					ROS_INFO("Wall follower - 1 - turn left");
				}
			if (stateN==2)
				{
					ROS_INFO("Wall follower - 2 - follow the wall");
				}
					
		}
	//Update the state
	state=stateN;
	
}

/**
 *@brief this function implements the find wall behaviour, it goes forward and rotates to the right.
 *@param none
 *@retval none
 */
void find_wall()
{
	// I set the velocities and publish them
	msg.linear.x=0.3;
	msg.angular.z=-0.6;
	pub.publish(msg);
}

/**
 *@brief this function implements the turn left behaviour, it only rotates to the left
 *@param none
 *@retval none
 */
void turn_left()
{
	// I set the velocities and publish them
	msg.angular.z=0.8;
	pub.publish(msg);
}

/**
 *@brief this function implements the follow the wall behaviour, it goes forward
 *@param none
 *@retval none
 */
void follow_the_wall()
{
	// I set the velocities and publish them
	msg.linear.x=0.5;
	pub.publish(msg);
}

/**
 *@brief this function stops the robot sending a zero command to both the linear and angular velocity
 *@param none
 *@retval none
 */
void stop()
{
	// I set the velocities and publish them
	msg.linear.x=0;
	msg.angular.z=0;
	pub.publish(msg);
}
