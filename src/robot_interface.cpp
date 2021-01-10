/* author: Zoe Betta 5063114
 * Date of creation 02/01/2021
 * 
 * FINAL ASSIGNMENT
 * Research track 1
 *
 * program that implements the robot behavior
 *
*/

#include "ros/ros.h"
#include "std_msgs/String.h"
#include "nav_msgs/Odometry.h"
#include "move_base_msgs/MoveBaseActionGoal.h"
#include "move_base_msgs/MoveBaseActionResult.h"
#include "final_assignment/random.h"
#include "final_assignment/wall_follow.h"

// definitions of global variables
	using namespace nav_msgs;
	ros::Publisher pub;
	ros::ServiceClient client;
	ros::ServiceClient client1;
	move_base_msgs::MoveBaseActionGoal Goal;	
	nav_msgs::Odometry Position;
	nav_msgs::Odometry Target;
	float distanceX=0;
	float distanceY=0;
	int previous_state=0;
	int current_state=0;
	int InitOperations;
	int count=1;

// function declaration 	
int update_state(void);
void chatterCallback ( const Odometry:: Ptr& x);

// main function 
int main(int argc, char **argv)
{	
	// inizializaion of:
	// publisher on the /move_base/goal topic
	// client for the random server
	// client for the wall_follow server
	// subscriber for the current position of the robot
	ros::init(argc, argv, "robot_interface");
	ros:: NodeHandle n1;
	ros:: NodeHandle n2;
	ros:: NodeHandle n3;
	ros:: NodeHandle n4;

	pub= n1.advertise<move_base_msgs::MoveBaseActionGoal>("move_base/goal", 1);
	client= n2.serviceClient<final_assignment::random>("random");
	client1= n3.serviceClient<final_assignment::wall_follow>("wall_follow");
	ros:: Subscriber robot_position= n4.subscribe("odom", 1000, chatterCallback);
	
ros :: spin();
return 0;
	
}

/**
 *@brief this function reads the parameter /state from the parameter server and returns it in a integer variable
 *@retval newstate the new state
 */
int update_state(void)
{
	int newstate;
	ros::param::get("/state", newstate);
	return newstate;
}

/**
 *@brief this function is called everytime something is read on the topic /odom and decides what the robot should do
 *@param x it is a variable of type Odometry that has informations about the position of the robot 
 *@retval none
 */
void chatterCallback( const Odometry:: Ptr& x)
{ 	
	// definition of variables
	float TargetX;
	float TargetY;
	// every time I call the callback of the subscriber I read the current position of the robot
	Position.pose.pose.position.x=x->pose.pose.position.x;
	Position.pose.pose.position.y=x->pose.pose.position.y;
	count++;
	// If the robot is in the first state, go to a position randomly chosen
	if (previous_state==1)
		{
			// If I have to perform the initialization operations, such as asking for a new position
			if (InitOperations==1)
				{	
					// print on the screen the information about the current state of the robot
					ROS_INFO("Init State 1-Random position");
					// I want to do these operations only once so I put init to 0					
					InitOperations=0;
					// I call the server for the random number
					final_assignment::random srv;
					srv.request.min= 0;
					srv.request.max= 5;
					client.call(srv);
					// Initialization of the Goal for the move_base algorithm
					Goal.goal.target_pose.header.frame_id="map";
					Goal.goal.target_pose.pose.orientation.w=1;
					Goal.goal.target_pose.pose.position.x=srv.response.a;
					Goal.goal.target_pose.pose.position.y=srv.response.b;
					// I save the target also in a variable of type odometry in order to be able to calculate the distance
					Target.pose.pose.position.x=Goal.goal.target_pose.pose.position.x;
					Target.pose.pose.position.y=Goal.goal.target_pose.pose.position.y;
					// publish the Target on the move_base/goal topic
					pub.publish(Goal);
					
				}
			//I compute the distance from the Target point
			distanceX=Target.pose.pose.position.x-Position.pose.pose.position.x;
			distanceY=Target.pose.pose.position.y-Position.pose.pose.position.y;
			// I print the distance from the target once every 500 loop iteration in order to have better performances
			if ((count%500)==0)
				{
					ROS_INFO("distances: %f \t %f", distanceX,distanceY);
				}
			// I check if the distances along x and along y are both in the interval -1 and 1
			// if that's the case I look for another state
			// if that's not the case I keep going there
			if((distanceX>=-1 && distanceX<=1) && (distanceY>=-1 && distanceY<=1))
				{
					// go to state 4
					previous_state=4;
					// put init equal to 1
					InitOperations=1;
					// print on the screen the information about the current state of the robot
					ROS_INFO("Exiting State One");	
				}
			else
				{
				}
		}
	// If the robot is in the second state, ask for a new position and check if it is doable
	else if (previous_state==2)
		{
			// If I have to perform the initialization operations, such as asking for a new position
			if(InitOperations==1)
				{	
					// print on the screen the information about the current state of the robot
					ROS_INFO("Init State 2-Ask for position");
					InitOperations=0;
					// ask for the new position
					printf("Insert new position\nYou may choose between:\n(-4,-3)\t(-4,2)\t(-4,7)\t(5,-7)\t(5,-3)\t(5,-1)\nx:");
    					std::cin >> TargetX;
					printf("\ny:");
					std::cin >> TargetY;
					// check if the position inserted by the user is one of the correct ones
					if (TargetX==(-4))
						{
							if ((TargetY==-3) || (TargetY==2)||(TargetY==7))
								{	
									// if the position is correct I update the Goal on the topic /move_base/goal
				 					printf("Valid Position");
									Goal.goal.target_pose.header.frame_id="map";
									Goal.goal.target_pose.pose.orientation.w=1;
									Goal.goal.target_pose.pose.position.x=TargetX;
									Goal.goal.target_pose.pose.position.y=TargetY;
									// I save the target also in a variable of type odometry in order to be able to calculate the distance
									Target.pose.pose.position.x=Goal.goal.target_pose.pose.position.x;
									Target.pose.pose.position.y=Goal.goal.target_pose.pose.position.y;
									// publish the Target on the move_base/goal topic
									pub.publish(Goal);
									// I want to do these operations only once so I put init to 0
									InitOperations=0;
								}
							else 
								{
									// if the position is not correct I print an error message
									printf("\n Position not valid: chose between:\n(-4,-3)\t(-4,2)\t(-4,7)\t(5,-7)\t(5,-3)\t(5,-1)\n");
								}	
							
						}
					if(TargetX==5)
						{
							if ((TargetY==-7)||(TargetY==-3)||(TargetY==-1))
								{
									// if the position is correct I update the Goal on the topic /move_base/goal
									printf("Valid Position");
									Goal.goal.target_pose.header.frame_id="map";
									Goal.goal.target_pose.pose.orientation.w=1;
									Goal.goal.target_pose.pose.position.x=TargetX;
									Goal.goal.target_pose.pose.position.y=TargetY;
									// I save the target also in a variable of type odometry in order to be able to calculate the distance
									Target.pose.pose.position.x=Goal.goal.target_pose.pose.position.x;
									Target.pose.pose.position.y=Goal.goal.target_pose.pose.position.y;
									// publish the Target on the move_base/goal topic
									pub.publish(Goal);
									// I want to do these operations only once so I put init to 0
									InitOperations=0;
								}
							else
								{
									// if the position is not correct I print an error message
									printf("Position not valid: chose between:\n(-4,-3)\t(-4,2)\t(-4,7)\t(5,-7)\t(5,-3)\t(5,-1)\n");
								}
						}
				}

			// I compute the distance from the Target point
			distanceX=Target.pose.pose.position.x-Position.pose.pose.position.x;
			distanceY=Target.pose.pose.position.y-Position.pose.pose.position.y;
			// I print the distance from the target once every 500 loop iteration in order to have better performances
			if( (count%500)==0)
				{
					ROS_INFO("distances: %f \t %f", distanceX,distanceY);
				}
			// I check if the distances along x and along y are both in the interval -0.1 and 0.1
			// if that's the case I look for another state
			// if that's not the case I keep going there
			if((distanceX>=(-1.0) && distanceX<=1.0) && (distanceY>=-1.0 && distanceY<=1))
				{
					// go to state 4
					previous_state=4;
					// put init equal to 1
					InitOperations=1;
					// print on the screen the information about the current state of the robot
					ROS_INFO("Exiting State Two");	
				}
			else
				{
				}
		}
	// If the robot is in the third state, start following the wall
	else if (previous_state==3)
		{	
			// If I have to perform the initialization operations, such as calling the server to start following the wall
			if(InitOperations==1)
				{ 	
					// print on the screen the information about the current state of the robot
					ROS_INFO("Init State 3-Follow the wall");					
					int RESF;	
					// call the server to start following the wall		
					final_assignment::wall_follow srvF;	
					srvF.request.data=1;
					client1.call(srvF);
					RESF=srvF.response.success;
					// I want to do these operations only once so I put init to 0
					InitOperations=0;

				}
			else
				{
					// update the state
					current_state=update_state();
					// if the state is the same as before do nothing
					if (current_state==previous_state)
						{
						}
					// if the state is different from before
					else 
						{
							// go to state 4
							previous_state=4;
							// put init equal to 1
							InitOperations=1;
							// call the serverto stop following the wall
							final_assignment::wall_follow srvFF;
							srvFF.request.data=0;
							client1.call(srvFF);
							// print on the screen the information about the current state of the robot
							ROS_INFO("Exiting State Three");
						}

				}
		}
	// If the robot is in the fourth state, stop in this position
	else if (previous_state==4)
		{	
			// If I have to perform the initialization operations, such as calling the server to start following the wall
			if(InitOperations==1)
				{
					// print on the screen the information about the current state of the robot
					ROS_INFO("Init State 4-Stop in last position");
					// inizilization of the goal in the last position of the robot
					Goal.goal.target_pose.header.frame_id="map";
					Goal.goal.target_pose.pose.orientation.w=1;
					Goal.goal.target_pose.pose.position.x=Position.pose.pose.position.x;
					Goal.goal.target_pose.pose.position.y=Position.pose.pose.position.y;
					// publish the Target on the move_base/goal topic
					pub.publish(Goal);
					// I want to do these operations only once so I put init to 0
					InitOperations=0;
				}
			else
				{
					// update the state
					current_state=update_state();
					// if the state is the same as before do nothing
					if (current_state==previous_state)
						{
						}
					// if the state is different from before
					else 
						{
							// update the previous state of the robot
							previous_state=current_state;
							// put init equal to one
							InitOperations=1;
						}
				}
		}
	// if previous state is not any of the states before update the state
	else 
		{
			current_state=update_state();
			previous_state=current_state;
		}

}
