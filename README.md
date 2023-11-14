# C-Coursework
This is the first C Coursework for COMP0002.

## Implement
- Display a 10*10 grid with several markers randomly positioned anywhere in the grid and the robot positioned at the selected home square, which can be anywhere except where the marker is.
- Enter the starting position for the robot (also the position of the selected home square) by using command line arguments as **Use command** shown.
- Then move the robot around the grid to find the marker, and record the movements the robot makes to find the marker, so that it can pick up and return the marker to the home square by following the moves in reverse.
- There are several blocks randomly positioned anywhere in the grid, such that robot need to collect all the markers and avoid all the blocks.

## Use command:
#### Download Drawapp-2.0.zip package
#### Complie the program
- `gcc -o filename coursework.c graphics.c`
#### Run the program with line command arguments
- 3 numbers for line command arguments
  - 1st number for initial x-coord
  - 2nd number for initial y-coord
  - 3rd number for initial direction (0 for north, 1 for east, 2 for south, 3 for west)
- e.g. `./filename 2 3 1 | java -jar drawapp-2.0.jar` meaning start the robot at position (2,3) in the grid, facing east.
- special case (if the command line arguments are not given): `./filename | java -jar drawapp-2.0.jar` meaning start the robot at position (6,5) in the grid, facing north. 
