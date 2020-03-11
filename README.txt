/***************** INSTRUCTIONS *******************/
-----Task1-----
1. Compile all the programs in required number of terminals(i.e. minions+2). Compile gru:= g++ Gru.cpp -o Gru -lpthread
2. Execute Gru as ./<app_name_Gru> <PORTNO>
3. Then Connect the Client by executing it as ./<app_name_Client> <hostname> <PORTNO>
4. Send the numbers via the Client separated by commas (eg: 1,2,3,4,5,) ending with a comma.
5. Execute the minions as ./<app_name_Minion> <PORTNO>
6. As the minions are executed they return the sums of the subarrays 
7. Finally, the Client receives the total sum 

-----Task2-----
Part1:-
1. Compile and Execute the program
2. Enter the Required data as per the system
3. Now enter the data as: '2 U 3' (without inverted commas) and press enter
4. enter the multiple duties and enter '0 0 0' TO STOP
5. Paths of both the lifts will now be visible

-----Task3-----
1. Compile opencv.cpp and run it. Copy the file points.txt into task3/src Compile:  g++ opencv.cpp -o app `pkg-config --cflags --libs opencv`
2. Now run roscore master node in a terminal
3. catkin_make in the ~/catkin_ws 
4. source devel/setup.bash
5. roscd task3
6. rosrun task3 task3
7. rosrun turtlesim turtlesim_node in a new terminal. (Spawn the turtle at x=  , y=   , theta=) 
8. Turtle will now execute the Path

-----Task4-----
1. Copy the file ppo.py into the spinningup/spinup/algos/ppo
2. Run the Pendulum-v0 environment via the ppo algorithm : python ppo.py --env Pendulum-v0
3. The modified ppo file has been attached
4. Pendulum.py is also attached with a damping factor added to the costs calculation
5. Multiple graphs have also been attached to highlight the effect of change in factors of the 'costs'
6. 

-----Task5-----
1. Read and understood via the UTAustinvilla code base. STATUS: Incomplete

