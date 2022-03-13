#include "genetic_algorithm.hpp"
#include "core.hpp"

std::ifstream fin;
int main()
{
    fin.open("/home/dparanic/Study/Graduation-thesis/datasets/40_tasks.txt");
    unsigned int data_size = 40;



    std::vector<std::vector<std::vector<unsigned int>>> tasks_data = load_tasks(fin, data_size);
    std::vector<task> tasks;
    unsigned int task_set_index = 5;

    for(std::size_t k = 0; k < data_size; ++k)
    {
        tasks.push_back(task(tasks_data[task_set_index][0][k], tasks_data[task_set_index][1][k], tasks_data[task_set_index][2][k]));
    }

    std::vector<unsigned int> positions{data_size/4, data_size/2 + data_size/4};

    auto decider=[](unsigned int value_to_compare, unsigned int value_to_compare_with){
        return value_to_compare<value_to_compare_with;
    };

    penalty_calculator calc{tasks};

    algorithm_configuration<crossover<unsigned int, decltype(calc)>,
                                    mutation<unsigned int>,
                                    reproductive_strategy<unsigned int>,
                                    selection_strategy<unsigned int,decltype(decider)>,
                                    end_condition<unsigned int>
                                   >evo(new npoint_ordered_crossover<unsigned int, decltype(calc)>(2),
                                        new saltation_mut<unsigned int>(),//new point_ordered_mut<unsigned int>(),
                                        new positive_assotiative_reproductive_sterategy<unsigned int>(),
                                        new beta_tournament<unsigned int,decltype(decider)>(5),
                                        new min_adaptation_cond<unsigned int>(50)
                                       );
    auto pool=generate_random_population(data_size,50,calc);
    std::shared_ptr<individual<unsigned int>> res = evo(pool, calc, decider, 30);

    std::cout << res->adapt() << " [";
    for(auto& x : *res)
    {
        std::cout << x << ", ";
    }
    std::cout << "]\n";
}