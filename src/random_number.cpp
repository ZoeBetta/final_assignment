/* author: Zoe Betta 5063114
 * Date of creation 02/01/2021
 * 
 * FINAL ASSIGNMENT
 * Research track 1
 *
 * server that calculates the random position, chosen between a set of given ones
 *
*/

#include "ros/ros.h"
#include "std_msgs/String.h"
#include "final_assignment/random.h"

// function declaration
int randMToN(int M, int N);
bool myrandom(final_assignment::random::Request &req, final_assignment::random::Response &res);

int main(int argc, char **argv)
{	// initialization of:
	// the server random
 	ros::init(argc, argv, "random_number");
 	ros::NodeHandle n1;
 	ros::ServiceServer service=n1.advertiseService("/random", myrandom); 
 	ros::spin();

 	return 0;

}

/**
 *@brief this function give two inputs calculates one random number in the interval between them
 *@param M is the minimum number of the interval 
 *@param N is the maximum number of the interval
 *@retval the random number
 */
int randMToN(int M, int N)
{
	return M+(rand() / (RAND_MAX / (N-M) ) ) ;
}

/**
 *@brief this function is called when I have a request, it reads the request and output the coordinates
	 of a point randomly chosen in a set of acceptable ones.
 *@param final_assignment::random::Request &req the request received from the client
						the minimum and maximum of the interval
 *@param final_assignment::random::Response &res the response returned to the client
						 the coordinates randomly chosen in the set
 *@retval bool is always set to true
 */
bool myrandom(final_assignment::random::Request &req, final_assignment::random::Response &res)
{	int index;
	int position_x[6]={-4,-4,-4,5,5,5};
	int position_y[6]={7,2,-3,1,-3,-7 };
	index=randMToN(req.min, req.max);
	res.a=position_x[index];
	res.b=position_y[index];
	ROS_INFO("Random Target [%f, %f]", res.a, res.b);
	return true;
}
