# Research Track I - Final Assignment

# Functions file wall_follower.cpp

## void take_action()

Declared in the file wall_follow.cpp

 @brief this function reads from a global variable the elaborated input from the laser sensor and decides what the next state will be. This function decides the next state based on the input from the laser scans. In particular it decides with the regions front, front-left and front-right. Depending on their values the function change_state is called with different inputs.

 @param none

 @retval none

## bool wall_follow_switch(final_assignment::wall_follow::Request &req, final_assignment::wall_follow::Response &res)

Declared in the file wall_follow.cpp

 @brief this function is called when it receives a call from the client and it changes the global variable active if the global variable is 1 the robot should follow the wall, on the other hand if it is zero it should stop following the wall. It reads the requst, prints it on the screen and then it changes the global variable active. It also prints some information about the state. 

 @param final_assignment::wall_follow::Request &req

 @param final_assignment::wall_follow::Response &res

 @retval none

## float CalculateMinimum(float max, sensor_msgs::LaserScan array, int indexMin, int indexMax)

Declared in the file wall_follow.cpp

 *@brief It receives as inputs an array, an interval and an integer number. The goal is to find the minimum between the elements of the array in the given interval and the integer number. It achieves said task by initializing a temporary minimum to the integer value and then with a for loop by checking if the element of the array is less than the temporary minimum and only in that case updating the temporary minimum

 *@param max an integer that is the maximum accteptable number

 *@param sensor_msgs::LaserScan array the array I have to find the minimum of

 *@param indexMin the index of the the element I start calculating the minimum of 

 *@param indexMax the index of the last element I look for the minimum

 *@retval TempMin the minimum between the element of the array from indexMin and indexMax and the number max

## void clbk_laser( const sensor_msgs::LaserScan:: Ptr& x)

Declared in the file wall_follow.cpp

 @brief this function is called everytime something is published on the topic /scan. It elaborates the data by calling the function CalculateMinimum passing as input the entire array of the laser ouput, the interval, I want to look for the minimum in, decided depending on the region and the number 10. The results are saved in the global variable regions_. After elaborating all of the data the function take_action is called

 @param x a variable of type LaserScan that contains all of the data from the sensors

 @retval none

## void change_state(int stateN)

Declared in the file wall_follow.cpp

 @brief this function changes the state and if the new state is different from the one before prints on the screen the information about the current state

 @param stateN is the new state that I want to implement

 @retval none

## void find_wall()

Declared in the file wall_follow.cpp

 @brief this function implements the find wall behaviour. It sets both a linear velocity along x and an angualar velocity along z and publishes them on the topic /cmd_vel.

 @param none

 @retval none

## void turn_left()

Declared in the file wall_follow.cpp

 @brief this function implements the turn left behaviour. It only sets an angular velocity along z and publishes on the topic /cmd_vel.

 @param none

 @retval none

## void follow_the_wall()

Declared in the file wall_follow.cpp

 @brief this function implements the follow the wall behaviour. It only sets a linear velocity along x and publishes on the topic /cmd_vel.

 @param none

 @retval none

## void stop()

Declared in the file wall_follow.cpp

 @brief this function stops the robot sending a zero command to both the linear and angular velocity and publishes on the topic /cmd_vel.

 @param none

 @retval none



