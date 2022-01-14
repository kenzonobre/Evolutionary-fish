# Installation

## Requirements

- Make
- GCC C++11
- Glut/OpenGL

## Instructions 
Go to the file that you want to store the repository and type in the terminal : 
```
git clone https://github.com/kenzonobre/Evolutionary-fish.git
```
Next, to go to the source code file, type : 
```
cd Evolutionary-fish/src
```
Then, to compile the program :
```
make all
```
and to run the program :
```
make run
```
If you want to make changes on the source code, such as changing some constant values, remember to save the file and write in the terminal the last two commands to recompile and run the program. 

## Menu

The interface has a simple menu where you can see some important information of the simulation. You can also press :
- key **i** to increase the simulation speed.
- key **d** to decrease the simulation speed.
- key **e** to evolve the generation before the timer go to 0.
- key **r** to reset the whole simulation.

Obervation : The simulation speed only ranges from 1 to 4.


<p align = "center">
  <img width = "600" src = "https://github.com/kenzonobre/Evolutionary-fish/blob/main/img/simulation_with_menu.gif">
</p>

