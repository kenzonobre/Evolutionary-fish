# Evolution Analysis

## Noise Analysis

The environment developed in this project is highly noisy for any evolutionary algorithm, because the fitness score of a specie doesn't depend exclusively on its genes (```alignmentWeigh```, ```cohesionWeigh```, ```separationWeigh``` and ```avoidSharkWeigh```), but its surroundings also influence on its performace. For instance, if a specie happen to be near many sharks and attract them by chance, the environment will have a major influence on its fitness score.

To observe this noise, some simulations were executed with boids that had fixed genes and, therefore, weren't evolving. For example, in a simulation that 20 boids of 20 fish interact with 5 sharks during many generations and every boid were defined to have the following genes : 
```
alignmentWeigh = 50
cohesionWeigh = 50
separationWeigh = 50
avoidSharkWeigh = 50
```
Resulted in the graph below :

(graph)

The fluctuation observed in the graph evidences that even though all species had the same genes during all generations (and therefore they had the same behavior at every moment), the noisy environment influence on each specie's performace. 
