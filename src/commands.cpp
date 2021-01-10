/* author: Zoe Betta 5063114
 * Date of creation 02/01/2021
 * 
 * FINAL ASSIGNMENT
 * Research track 1
 *
 * program that implements the request for a new action:
 * it prints on the screen a request, wait for a standard input and then set a parameter 
 *
*/
#include "ros/ros.h"
#include "std_msgs/String.h"
#include "nav_msgs/Odometry.h"
#include "move_base_msgs/MoveBaseActionGoal.h"

// main function 
int main(int argc, char **argv)
{   
    	ros::init(argc, argv, "commands");
    	int state=0;
	// while loop to ask for a new action
    	while(1)
		{
    			printf("What should we do next? \n 1. Random position chosen \n 2. Manually chosing next position \n 3. start following the wall \n 4. stop in the last position\n");
   			std::cin >> state;
			// change the param /state
    			ros::param::set("/state", state);	
		}
	ros :: spin();
	return 0;
}

