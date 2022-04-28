# Description
Here is the code part of my graduation thesis "Optimization methods of usage of genetic algorithms on JSP".<br>
You can learn more about JSP on this [link](https://en.wikipedia.org/wiki/Job-shop_scheduling).<br/>

Repository contains implementation of EGA (Evolution Genetic Algorithm) splited on several headers describe each part of EGA.<br>
EGA contains following components:
* Selection strategy
* Crossover algorithm
* Mutation algorithm
* Reproduction strategy
* Search ending condition
<br/>

# Usage
I've implemented EGA in form of constructor. We need to construct object of class algorithm_configuration passing components we need:
```cpp
algorithm_configuration<crossover<unsigned int, decltype(target_adaptation_function)>,
                                    mutation<unsigned int>,
                                    reproductive_strategy<unsigned int>,
                                    selection_strategy<unsigned int,decltype(decider)>,
                                    end_condition<unsigned int>
                                   >evo(new npoint_ordered_crossover<unsigned int, decltype(target_adaptation_function)>(2),
                                        new saltation_mut<unsigned int>(),
                                        new positive_assotiative_reproductive_sterategy<unsigned int>(),
                                        new beta_tournament<unsigned int,decltype(decider)>(5),
                                        new min_adaptation_cond<unsigned int>(50)
                                       );
```
* Template will be common for different combinations of components, we need to specify argument list for constructor.
* "decider" here is a comparator. It's used to decide wich permutation is better: with greater value of adaptation or lesser.
* "target_adaptation_function" is used to calculate adaptation value of permutation. Example [here](https://github.com/DenisParal/Graduation-thesis/blob/91d40d13f8f0befb15c284607bbec7ac0cac3f1c/include/core.hpp#L60)

To run the configured algorithm we need to pass to it starting population (list of pregenerated permutations), target adaptation function, comparator for adaptation values and value of mutation chance.
```cpp
std::shared_ptr<individual<unsigned int>> res = evo(pool, calc, decider, 30);
```

You can find example of usage EGA [here](https://github.com/DenisParal/Graduation-thesis/blob/91d40d13f8f0befb15c284607bbec7ac0cac3f1c/src/single_run.cpp).
