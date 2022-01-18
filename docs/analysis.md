## Noise Analysis

The environment developed in this project is highly noisy for any evolutionary algorithm, because the fitness score of a specie doesn't depend exclusively on its genes (```alignmentWeigh```, ```cohesionWeigh```, ```separationWeigh``` and ```avoidSharkWeigh```), but its surroundings also influence on its performace. For instance, if a specie happen to be near many sharks and attract them by chance, the environment will have a major influence on its fitness score.

To observe this noise, some simulations were executed with boids that had fixed genes and, therefore, weren't evolving. For example, in a simulation that 20 boids of 20 fish interact with 5 sharks during many generations and every boid were defined to have the following fixed genes : 
```
alignmentWeigh = 50
cohesionWeigh = 50
separationWeigh = 50
avoidSharkWeigh = 50
```
Resulted in the graph below :

<p align = "center">
  <img width = "1000" src = "https://github.com/kenzonobre/Evolutionary-fish/blob/main/img/noise_evidence_1_generation_interval.png">
</p>

Here is the same graph with smoother lines (using a moving average of 10 generation interval) : 

<p align = "center">
  <img width = "1000" src = "https://github.com/kenzonobre/Evolutionary-fish/blob/main/img/noise_evidence_10_generation_interval.png">
</p>

The fluctuation observed in the graph evidences that even though all species had the same genes during all generations (and therefore they had the same behavior), the noisy environment significantly influence on the species' scores. In other words, a specie with the same behaviour can have quite different performaces. This  creates a serious problem for an evolutionary algorithm (since it evalutes an individual by its fitness score), making it more susceptible to luck. In order to reduce the noise influence, some evolutionary strategies were adopted in the algorithm.

## Evolutionary Strategies

### Elitism

As said previously, the environment of the project is highly noisy, so it is difficult to analyse if a boid with a good score is an unfit specie that got lucky, or it is actually an apt specie. So, the elitism strategy were implemented in a way to avoid this uncertainty. 

First, it is stored in a vector called ```boidsFitness[][i]``` the fitness score of each specie _i_ and the fitness score of its latest 9 ancestors. The function ```getAverageFitness(i)``` calculates the average score of the vector ```boidsFitness[][i]``` without the maximum and minumum value. Next, the best boid of a generation is selected based on this ```getAverageFitness()``` function. Therefore, it is possible to avoid that an unfit specie that got lucky become the best individual of a generation, since not only its performace is being analysed, but also the performace of its ancestors (in other words, the history of this specie is being examined).

Besides that, to create the next generation, the best boid only crossover with those boids that have lower fitness score than itself, while boids with greater scores are cloned. In this way, species that have a bad history but achieve somehow a good score (greater than the best boid) by aptness, can have a chance to show their potential.   

### Variable Mutation

The elitism strategy is very good to evolve population that has few individuals (such as those in this project, in which most of the simulations are based on 20 fish species), however, it converges the individuals' genes very fast, reducing the diversity of the population. In order to solve this, the variable mutation method was used. 

A variable called ```TAX_OF_MUTATION``` is responsible to store a value that is added on or subtracted from the genes of an individual that is being mutated. This variable ranges from 1 to 5 and it is initialized as 1. The idea behind it is that when there is no significant change on the individuals evolution, the ```TAX_OF_MUTATION``` should be increased to diversify more the population. In this case, the ```TAX_OF_MUTATION``` is increased when a specie becomes the best boid multiple times.

### Random Predation

Periodically, after 10 generations, the worst boid is selected (using the ```getAverageFitness()``` function) to be replaced by a completely random specie. This strategy was implemented to diversify even more the population and reduce the elitism convergence.

## Evolution Analysis

After many simulations, it was possible to observe the following pattern :

<p align = "center">
  <img width = "1000" src = "https://github.com/kenzonobre/Evolutionary-fish/blob/main/img/evolution_1_generation_interval.png">
</p>

Here is the same graph with smoother lines (using a moving average of 10 generation interval) : 

<p align = "center">
  <img width = "1000" src = "https://github.com/kenzonobre/Evolutionary-fish/blob/main/img/evolution_10_generation_interval.png">
</p>

In both graphs, it can be seen that there are two downslopes near the 400-th and 700-th generation. Ideally, the mean fitness should increase gradually until reach a point without significant changes, allowing minor fluctuations from the noisy environment. However, these declines were present in a significant number of simulations. After investigating these situations, it was evidenced that this happened due to a combination of factors between the high noise and the random predation technique.

More specifically, when the population of boids reached a stagnation point with a high mean fitness, the species usually had high value for the ```avoidSharkWeigh``` gene, mid-high value for the ```alignmentWeigh``` and ```separationWeigh``` and low value for the ```cohesionWeigh```. These characteristics make sense, because :
- a high ```avoidSharkWeigh``` prioritizes running away from sharks.
- a mid-high ```separationWeigh``` with a low ```cohesionWeigh``` results in a less dense boid, making fewer fish die when a shark approaches the entire school.
- a mid-high ```alignmentWeigh``` turns the specie's movement more organized.

Unluckily, during this stagnation point, the random predation could generate an unfit boid with high ```cohesionWeigh``` that gets a good score by accident. Furthermore, since this unfit boid is new, it doesn't have ancestors and a score history to calculate its average fitness, so it might become the best boid of the generation and, later, crossover itself with many other species, worsening a great part of the population.

The following gif shows a representation of this decline situation : 

<p align = "center">
  <img width = "500" src = "https://github.com/kenzonobre/Evolutionary-fish/blob/main/img/decline_situation.gif">
</p>

The boids with a red tone (they all have similar color due to the elitism convergence) indicates the apt species. In this simulation, they have a set of genes aproximatelly to : 
```
alignmentWeigh = 60
cohesionWeigh = 10
separationWeigh = 60
avoidSharkWeigh = 90
```

The blue boid indicates the unfit new random boid. In this case, it has the following set of genes : 
```
alignmentWeigh = 60
cohesionWeigh = 90
separationWeigh = 60
avoidSharkWeigh = 10
```

If the blue boid become the best boid of the generation, it will pass its unfit genes, such as the low ```avoidSharkWeigh``` (giving less priority to avoid sharks), worsening and declining the mean fitness of the population. In conlusion, even though some evolutionary strategies were used to reduce the environment high noise, it still had a significant influence on the simulation in some cases.
