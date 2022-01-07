# Evolutionary-fish

Fish schools based on the boids algorithm that evolve through an evolutionary algorithm.

<p align = "center">
  <img width = "500" src = "https://github.com/kenzonobre/Evolutionary-fish/blob/main/assets/simulation.gif">
</p>

## Description

The goal of this project was to evolve species of fish in a hostile environment consisting of sharks. In order accomplish this evolution, it was developed an evolutionary algorithm that prioritizes the number of living individuals in each group of fish. 

## Boids

To simulate the behavior of a fish school, each specie was implemented based on the [boids algorithm](http://www.red3d.com/cwr/boids/) developed by Craig Reynolds. 
In short, the boids algorithm is ruled by three conditions : alignment, cohesion and separation.

### Alignment

The alignment rule states that an individual should point to the average direction of its neighbors. 
The following gif shows a fish school that only has the alignment behavior. The gray semi-circle represents the observable range of the black fish and the black lines at front of each fish represents the direction it is pointing towards.

<p align = "center">
  <img width = "500" src = "https://github.com/kenzonobre/Evolutionary-fish/blob/main/assets/alignment.gif">
</p>

### Cohesion

The cohesion rule states that an individual should go towards the mass center of its neighbors. 
The following gif shows a fish school that only has the cohesion behavior and the red circle represents the mass center of the black fish's neighbors.

<p align = "center">
  <img width = "500" src = "https://github.com/kenzonobre/Evolutionary-fish/blob/main/assets/cohesion.gif">
</p>

### Separation

The separation rule states that an individual should avoid its neighbors when they are close.
The following gif shows a fish school that only has the separation behavior and the red area represents the avoidance range of the black fish. Observation : for this specific project, the avoidance range was developed to be small than the visual range of each fish.

<p align = "center">
  <img width = "500" src = "https://github.com/kenzonobre/Evolutionary-fish/blob/main/assets/separation.gif">
</p>

In this way, it is possible to simulate a similar behavior of fish schools using the alignment, cohesion and separation rules.

<p align = "center">
  <img width = "500" src = "https://github.com/kenzonobre/Evolutionary-fish/blob/main/assets/boid.gif">
</p>

## Evolutionary Algorithm

In order to develop a evolutionary algorithm, it is necessary to define what is the population of individuals, how to calculate the fitness score and which evolutionary strategies will be used.

### Population

The population is composed by species of fish. Each specie were represented by a class named **Boid**, which has the following main characterists :
- alignmentWeigh
- cohesionWeigh
- separationWeigh
- avoidSharkWeigh

These variables range from 1 to 100 and they represent the genes of a specie.
There are also other variables that define the color of the fish in the **Boid** class, but they are only for aesthetic purpose and don't influence on the boid's behavior.

### Fitness Score

The fitness score is the sum of seconds that each fish survived. More specifically, a boid fitness score is calculated by the following equation :

<img src="https://render.githubusercontent.com/render/math?math=FitnessScore = \Sigma secondsSurvived_i">

in which  <img src="https://render.githubusercontent.com/render/math?math=secondsSurvived_i">  is the number of seconds the i-th fish survived.

### Evolutionary strategies

For this project, it was used the **elitism**, **variable mutation** and **random predation** techniques to evolve the population.

#### Elitism

The elitism consists of taking the best individual (boid) of a generation and crossover it with every other individual. To perform the crossover between two individuals **A** and **B**, it is only necessary to calculate the mean values between each gene of **A** and **B**.

#### Variable Mutation

A normal mutation vary some or all genes of an individual by a fixed percentage over the total range value of the gene (in this case this range goes from 0 to 100).
Meanwhile, a variable mutation consists of a normal mutation that can change this percentage value depending on some situations.

#### Random Predation

The random predation consists of taking the worst individual (boid) of a generation and substitue it with a complete random individual.

#### Strategies details

These three strategies above were implemented in a slightly different way in the source code. Here is the documentation that talks about these differences and explains the evolutionary strategies in more detail : [link](). 




