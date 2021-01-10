# Research Track I - Final Assignment

# Functions file robot_interface.cpp

## int update_state(void)

Declared in the file robot_interface.cpp

@brief this function reads the parameter /state from the parameter server and returns it in a integer variable. In this function it is called the ros function ros::param::get that reads the parameter specfied in the first argument and saves the value read in the second parameter.

@param none

@retval newstate the new state read from the ros param "/state"

## void chatterCallback( const Odometry:: Ptr& x)

Declared in the file robot_interface.cpp

 @brief this function is called everytime something is read on the topic /odom and decides what the robot should do.The robot can be in 4 states: state 1 the robot asks for a random position, state 2 the robot asks the user for a new position, state 3 the robot should follow the wall of the map, state 4 the robot should stop. As a first operation the values of the current position of the robot are updated and also it is incremented the variable count. Count is only used to print every 500 times the distance from the target and the robot, this last one should be in state 1 or 2 in order to print the distance from a given target.
In state 1 I check if I have to do some initialization operations, if that's the case I print on the screen an update on the state of the robot and then ask for a new random position by calling the server random. I then update the values of the new target in a variable of type MoveBaseActionGoal and a variable of type Odometry. The first one is sent by a publisher on the topic /move_base/goal while the other one is used to calculate the distance. I then calculate said distance and if it is less than 1 for both x and y I change state, going temporarily in 4 and print on the screen informations about the state. If the robot is in state 2, similarly to what I have done for state 1 I check if some initialization operations are needeed; if that's the case I proceed similarly to case 1, the only difference is that I ask the user to insert from standard input a new Target position, between the available ones, it is then checked if the position inserted is valid, and if that is the case I perform the same updating operations of case 1. If the position is not valid an error message is printed on the screen and next iteration it will be asked again to insert a valid position. I then compute and check the distance and if that is less than the threshold, like in state 1, I switch to state 4. 
In state 3 if initialization operations are needed the server wall_follow is called and it is passed as a request the integer value 1, in order to start the follow the wall behavior. If I don't have to perform the init operations I update the current state by calling update_state and if the new state is different from the previous one I go in state 4 and call the server with a request of 0. 
In state 4, if I have to perform initialization operations I update the target point to the current position of the robot, or if I don't have to do any initialization I just update the current state

 @param x it is a variable of type Odometry that has informations about the position of the robot 

 @retval none




