#DESCRIPTION OF THE CONTENENT OF THE PACKAGE

This package is composed of four nodes and two servers. The nodes are commands, random_number, robot_interface and wall_follower. The servers are random and wall_follow. The node random_number implements the service for the random server while the node wall_follower implements the service for the wall_follow server, on the other hand the node robot_interface implements the client for both servers. 
The node commands asks the user for the next state and sets the parmeter /state.
The node robot_interface reads the position of the robot and depending on the states implements the different behaviors of the robot. Every state has the same structure: it checks if it should do the initialization operations and in that case completes them, or it completes other operations that must be done every iteration.
State one should ask for a random target, chosen between a set of valid ones, and go there. The initialization consist in calling the server random and publishing the new goal on the topic /move_base/goal, it also updates the value of the variable Target of type odometry used to calculate the distance of the robot from the target. Normal operations for state 1 are calculating each time the distance, printing it every 500 times and checking if the distance is less then one. In that case the robot has reached the goal and I have to change the state, I then impose the state 4. 
Very similar is the state two where the only difference is in the initialization operation where instead of calling the server random I ask the user for a new position and check if it is a valid position. The other operations are the same for state one.
State three is the follow the wall behavior. During the initialization Operation the follow the wall server is called passing as a request 1 in order to start following the wall. I then look for the updated state because I want to be able to change behavior at any time, if the new state is different from the previous one I go to state 4 after calling the server again sending 0 as a request to the wall_follow server. 
State 4 as initialization sets the target position as the current one and publishes it on the topic /move_base/goal. As ordinary operations instead updates the state. 
The random_number service calls the function randMToN that returns an integer that is the index of two given vectors with elements the valid x and y coordinates that are returned as responses of the server. The server random has as a request two integer, the minimum and maximum random number I want and as a response the two coordinates.
The wall_follower service when called sets the variable active_ equal to the request. The state of the laser is updated each time data are published on the topic /scan and a new state is decided. In the main a while loop is implemented that checks if the variable active is equal to 1, in that case depending on the state calls a function that sets the velocity on the topic /cmd_vel. If instead active_ is equal to 0 the function stop is called and all velocities are set to zero.

# INSTRUCTION ABOUT HOW TO RUN THE CODE

Make sure you have installed in the same worksapace the package slam_gmapping
in case you need to install it run in the src folder of your workspace:

git clone https://github.com/CarmineD8/slam_gmapping.git

Open three linux shell
On the first shell run:
1. roslaunch final_assignment move_base.launch
On the second shell run:
2. rosrun final_assignment commands
on the third shell run:
3. rosrun final_assignment robot_interface

# DESCRIPTION OF THE ROBOT BEHAVIOR

When first started the robot is in state 4 and so it is waiting for the next command that should be typed in the shell where the node commands is running. When you type the first command the robot will start serving that request. If the command given is 1 the robot will start moving in the direction of the first target, when the robot will reach that position it will ask for a new random position if the user didn't send another request or it will serve the new request. If the state is 2 the robot_interface will ask for a position and it will wait for a valid position, it will then go to that position and only when it reaches the position it will check if the state was changed. For state 3 instead the robot will start following the wall immediately and it will check the state at any moment and so it can change behavior at any time, while for state 1 and 2 the robot will change state only when the target is reached. 
If the user sends more than one request while the robot is in state 1 or 2 and moving towards the goal only the last request will be served because asking for another state will overwrite all previous requests. If the robot doesn't receive any request while moving towards a goal in state 1 or 2 when the goal is reached the robot will immediately start another cycle by asking the server for a random goal or asking the user for a new position.

# SOFTWARE ARCHITECTURE AND ARCHITECTURAL CHOICES

For the structure of this assignment I decided to write the code in the C++ language. I decided to have all of the functions declared and defined in the same file I am using them in order to have a more compact and easier to understand code structure, everything that was used for a node is defined in the same node. This can also be seen as a negative aspect because the code seems heavier. 
I created 4 separate nodes and divided them depending on the task; the commands node has to ask and retrieve the state information, the random_number node has to provide a new random position, the wall_follower node has to implement the follow the external wall behavior when requested and the robot_interface node communicates with all of the other nodes in order to decide what to do and retrieve all informations to do so. 

# SYSTEM'S LIMITATIONS AND POSSIBLE IMPROVMENTS

This system requires the user to send commands in two different shells, one to receive the commands and one to receive the new position. This was done in order to be able to receive a new state in any moment, in fact in both functions the keyboard input is blocking and by putting both input in one shell I wouldn't be able to receive a new state while waiting for a position. A further implementation could be having the keyboard inputs on only one shell. 
Another problem that can be faced with this assignment is that the performances of the robot that are not always excellent, in particular sometimes it is slow, especially while running the state 3 code, you could have better performances by stopping all other calculations while in state 3.




