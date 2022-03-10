#include "genetic_algorithm.hpp"
#include "core.hpp"
#include <fstream>

std::ifstream fin;
std::ofstream fout;



int main(int argc, char** argv)
{
    std::string data_path = argv[1];
    std::string output_path = argv[2];
    fout.open(output_path);
    unsigned int data_size = std::atoi(argv[3]);

    unsigned int task_time;
    unsigned int penalty;
    unsigned int directive;

    auto decider=[](unsigned int value_to_compare, unsigned int value_to_compare_with){
        return value_to_compare<value_to_compare_with;
    };

    std::vector<unsigned int> positions{data_size/4, data_size/2 + data_size/4};
    
    std::vector<selection_strategy<unsigned int,decltype(decider)>*> selection_strategies;
    std::vector<mutation<unsigned int>*> mutation_strategies;
    std::vector<reproductive_strategy<unsigned int>*> reproductive_strategies;
    std::vector<end_condition<unsigned int>*> end_conditions;

    selection_strategies.push_back(new stable_select_strategy<unsigned int,decltype(decider)>(50));
    selection_strategies.push_back(new beta_tournament<unsigned int,decltype(decider)>(5));

    mutation_strategies.push_back(new point_ordered_mut<unsigned int>());
    mutation_strategies.push_back(new saltation_mut<unsigned int>());

    reproductive_strategies.push_back(new positive_assotiative_reproductive_sterategy<unsigned int>());
    reproductive_strategies.push_back(new negative_assotiative_reproductive_sterategy<unsigned int>());
    reproductive_strategies.push_back(new ordered_inbreeding_reproductive_strategy<unsigned int>(8));

    end_conditions.push_back(new max_generation_cond<unsigned int>(50));
    end_conditions.push_back(new average_adaptation_cond_reversed<unsigned int>(100));
    end_conditions.push_back(new min_adaptation_cond<unsigned int>(50));


    std::vector<task> tasks;

    //Load tasksets
    std::vector<std::vector<std::vector<unsigned int>>> tasks_data = load_tasks(fin, )
    
    for(unsigned int j = 0; j < tasks_data.size(); j++)
    {
        for(std::size_t i = 0; i < data_size; ++i)
        {
            tasks.push_back(task(tasks_data[j][0][i], tasks_data[j][1][i], tasks_data[j][2][i]));
        }

        penalty_calculator calc{tasks};

        std::vector<crossover<unsigned int, decltype(calc)>*> crossover_strategies;
        crossover_strategies.push_back(new npoint_ordered_crossover<unsigned int, decltype(calc)>(positions));
        crossover_strategies.push_back(new classic_crossover<unsigned int, decltype(calc)>());

        algorithm_configuration<crossover<unsigned int, decltype(calc)>,
                                mutation<unsigned int>,
                                reproductive_strategy<unsigned int>,
                                selection_strategy<unsigned int,decltype(decider)>,
                                end_condition<unsigned int>
                                > evo(crossover_strategies[0], mutation_strategies[0], reproductive_strategies[0],
                                      selection_strategies[1], end_conditions[2]);
        auto pool=generate_random_population(data_size,50,calc);
        std::shared_ptr<individual<unsigned int>> res = evo(pool, calc, decider, 60);
        fout << res->adapt() << std::endl;
        tasks.clear();
        // std::cout << "|||||||||||||||||||||||||||||||||||||||||||||||||||||||\n";
        // for(std::size_t i = 0; i < data_size; ++i)
        // {
        //     std::cout << tasks[i].task_time << " " << tasks[i].penalty << " " << tasks[i].direct_time << "\n";
        // }
        // std::vector<unsigned int> order1{4, 24, 32, 5, 8, 37, 35, 30, 34, 3, 26, 29, 23, 38, 22, 14, 10, 28, 21, 0, 31, 17, 25, 20, 12, 6, 15, 11, 2, 36, 16, 18, 7, 13, 33, 9, 19, 39, 1, 27};
        // std::cout << penalty_calculator(tasks).operator()(order1);
    }
    

}