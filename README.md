# Evolutionary-fish

School of fish based on boids algorithm that evolve through a genetic algorithm

<p align = "center">
  <img width = "500" src = "https://github.com/kenzonobre/Evolutionary-fish/blob/main/assets/simulation.gif">
</p>

## Description

The goal of this project was to evolve species of fish in an hostile environment consisting of sharks. In order accomplish this evolution, it was developed an genetic algorithm that prioritizes the number of living individuals in a group of fish. 

## Boids

To simulate the behavior of a school of fish, each specie of fish was implemented based on [boids algorithm](http://www.red3d.com/cwr/boids/) developed by Craig Reynolds. 
In short, the boids algorithm is ruled by three aspects : alignment, cohesion and separation.

### Alignment

The alignment rule states that an individual should point to the average direction of its nearby neighbors. 
The following gif shows a school of fish that only has the alignment behavior. The the gray semi-circle represents the observable range of the black fish and the black lines at front of each fish represents the direction it is pointing towards.

<p align = "center">
  <img width = "500" src = "https://github.com/kenzonobre/Evolutionary-fish/blob/main/assets/alignment.gif">
</p>

<p align = "center">
  <img width = "500" src = "https://github.com/kenzonobre/Evolutionary-fish/blob/main/assets/cohesion.gif">
</p>

<p align = "center">
  <img width = "500" src = "https://github.com/kenzonobre/Evolutionary-fish/blob/main/assets/separation.gif">
</p>
