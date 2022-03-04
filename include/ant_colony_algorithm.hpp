#pragma once
#include "core.hpp"
#include <map>

class ant_colony_algorithm
{
private:
    float eps;
    unsigned int target_population;
    unsigned int population_size;
    float evaporation_degree;
    float starting_pheromone_lvl;
    float additional_pheromones;
public:
    ant_colony_algorithm(float eps = 0.5f, unsigned int target_population = 10, unsigned int population_size = 50, float evapotarion_degree = 0.3f,
                         float starting_pheromone_lvl = 1.0f, float additional_pheromones = 5.0f): eps(eps), target_population(target_population),
                         population_size(population_size), evaporation_degree(evaporation_degree), starting_pheromone_lvl(starting_pheromone_lvl),
                         additional_pheromones(additional_pheromones) {}
    void set_eps(float new_eps)
    {
        eps = new_eps;
    }
    void set_target_population(unsigned int new_target_population)
    {
        target_population = new_target_population;
    }
    void set_population_size(unsigned int new_population_size)
    {
        population_size = new_population_size;
    }
    void set_evaporation_degree(float new_evaporation_degree)
    {
        evaporation_degree = new_evaporation_degree;
    }
    void set_starting_pheromone_lvl(float new_starting_pheromone_lvl)
    {
        starting_pheromone_lvl = new_starting_pheromone_lvl;
    }
    void set_additional_pheromones(float new_additional_pheromones)
    {
        additional_pheromones = new_additional_pheromones;
    }

    std::vector<unsigned int> operator()(const std::vector<task>& tasks)
    {
        std::map<std::vector<unsigned int>, float> paths;
        
    }
};