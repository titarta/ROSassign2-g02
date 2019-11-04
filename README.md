# ROSassign2-g02

 Pre-Requisites:
 * Install ROS
 * Install STDR
 * Copy all the contents of 'stdr_resources/maps' to the respective folder inside the stdr_resources package (use 'roscd stdr_resources/maps')
 * Copy all the contents of 'stdr_resources/resources/robots' to the respective folder in the stdr_resources package
         (use 'roscd stdr_resources/resources/robots')

 Compilation / Installation:
 * Execute 'catkin_make' in the root directory

To initialize the STDR GUI + Map + Robot execute one of the following commands:
   
 * 'roslaunch reactive map45rbt1.launch' for robot 1 in map with 45º
 * 'roslaunch reactive map45rbt2.launch' for robot 2 in map with 45º
 * 'roslaunch reactive map90rbt1.launch' for robot 1 in map with 90º
 * 'roslaunch reactive map90rbt2.launch' for robot 2 in map with 90º
 * 'roslaunch reactive map150rbt1.launch' for robot 1 in map with 150º
 * 'roslaunch reactive map150rbt2.launch' for robot 2 in map with 150º
 
 To initialize the code execute the following command on a new terminal:
 
 * 'rosrun reactive robot'
 
