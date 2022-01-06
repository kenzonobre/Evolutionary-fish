# Evolutionary-fish

Schools of fish based on boids algorithm that evolve through a evolutionary algorithm.

<p align = "center">
  <img width = "500" src = "https://github.com/kenzonobre/Evolutionary-fish/blob/main/assets/simulation.gif">
</p>

## Description

The goal of this project was to evolve species of fish in a hostile environment consisting of sharks. In order accomplish this evolution, it was developed an genetic algorithm that prioritizes the number of living individuals in each group of fish. 

## Boids

To simulate the behavior of a school of fish, each specie was implemented based on [boids algorithm](http://www.red3d.com/cwr/boids/) developed by Craig Reynolds. 
In short, the boids algorithm is ruled by three conditions : alignment, cohesion and separation.

### Alignment

The alignment rule states that an individual should point to the average direction of its neighbors. 
The following gif shows a school of fish that only has the alignment behavior. The gray semi-circle represents the observable range of the black fish and the black lines at front of each fish represents the direction it is pointing towards.

<p align = "center">
  <img width = "500" src = "https://github.com/kenzonobre/Evolutionary-fish/blob/main/assets/alignment.gif">
</p>

### Cohesion

The cohesion rule states that an individual should go towards the mass center of its neighbors. 
The following gif shows a school of fish that only has the cohesion behavior and the red circle represents the mass center of the black fish's neighbors.

<p align = "center">
  <img width = "500" src = "https://github.com/kenzonobre/Evolutionary-fish/blob/main/assets/cohesion.gif">
</p>

### Separation

The separation rule states that an individual should avoid its neighbors when they are close.
The following gif shows a school of fish that only has the separation behavior and the red area represents the avoidance range of the black fish. Observation : for this specific project, the avoidance range was developed to be small than the visual range of each fish.

<p align = "center">
  <img width = "500" src = "https://github.com/kenzonobre/Evolutionary-fish/blob/main/assets/separation.gif">
</p>

In this way, it is possible to simulate a similar behavior of schools of fish using the alignment, cohesion and separation rules.

<p align = "center">
  <img width = "500" src = "https://github.com/kenzonobre/Evolutionary-fish/blob/main/assets/boid.gif">
</p>

## Evolutionary algorithm

In order to create a evolutionary algorithm, it is necessary to define a population of individuals that evolve based on a fitness score.

### Individuals and Population

In this project, it was created a population of species of fish. Each specie were represented by a class named **Boid**, which has the following characterists :
- alignmentWeigh
- cohesionWeigh
- separationWeigh
- avoidSharkWeigh

These variables can be called as genes and a whole group of genes is a chromosome.
There are also variables that define the color of each specie, but they are just for aesthetic purpose and don't influence on the boids' behavior.

### Fitness score

The fitness score is defined by a simple calculation of how much time each fish of each specie survive. More specifically, a boid fitness score is calculated by :

<img src="https://render.githubusercontent.com/render/math?math=FitnessScore = \Sigma fish_i * secondsSurvived_i">

in which <img src="https://render.githubusercontent.com/render/math?math=\large secondsSurvived_i"> is the number of seconds the i-th fish survived.






