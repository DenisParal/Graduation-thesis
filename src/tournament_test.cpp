#include "genetic_algorithm.hpp"
#include "core.hpp"
#include <algorithm>
#include <numeric>

std::ifstream fin;
std::ofstream fout;
int main(int argc, char** argv)
{
    std::string data_path = argv[1];
    std::string output_path = argv[2];
    fin.open(data_path);
    fout.open(output_path);
    unsigned int data_size = std::atoi(argv[3]);

    auto decider=[](unsigned int value_to_compare, unsigned int value_to_compare_with){
        return value_to_compare<value_to_compare_with;
    };

    std::vector<unsigned int> positions{data_size/4, data_size/2 + data_size/4};


    std::vector<reproductive_strategy<unsigned int>*> reproductive_strategies;
    reproductive_strategies.push_back(new positive_assotiative_reproductive_sterategy<unsigned int>());
    reproductive_strategies.push_back(new negative_assotiative_reproductive_sterategy<unsigned int>());
    reproductive_strategies.push_back(new ordered_inbreeding_reproductive_strategy<unsigned int>(8));

    std::vector<std::vector<std::vector<unsigned int>>> tasks_data = load_tasks(fin, data_size);

    std::vector<std::vector<std::shared_ptr<individual<unsigned int>>>> medians;
    std::vector<task> tasks;

    for(unsigned int i = 0; i < tasks_data.size(); i++)
    {
        medians.push_back(std::vector<std::shared_ptr<individual<unsigned int>>>());
        for(unsigned int j = 0; j < 2; j++)
        {
            for(std::size_t k = 0; k < data_size; ++k)
            {
                tasks.push_back(task(tasks_data[i][0][k], tasks_data[i][1][k], tasks_data[i][2][k]));
            }

            penalty_calculator calc{tasks};

            algorithm_configuration<crossover<unsigned int, decltype(calc)>,
                                    mutation<unsigned int>,
                                    reproductive_strategy<unsigned int>,
                                    selection_strategy<unsigned int,decltype(decider)>,
                                    end_condition<unsigned int>
                                   >evo(new npoint_ordered_crossover<unsigned int, decltype(calc)>(positions),
                                        new saltation_mut<unsigned int>(),//new point_ordered_mut<unsigned int>(),
                                        reproductive_strategies[j],
                                        new beta_tournament<unsigned int,decltype(decider)>(5),
                                        new min_adaptation_cond<unsigned int>(50)
                                       );
            auto pool=generate_random_population(data_size,50,calc);
            std::shared_ptr<individual<unsigned int>> res = evo(pool, calc, decider, 30);

            medians[i].push_back(res);

            tasks.clear();
        }
    }

    fin.close();
    fin.open("/home/dparanic/Study/Graduation-thesis/datasets/40_tasks_check.txt");
    std::vector<std::vector<float>> variance(2,std::vector<float>());
    unsigned int check;
    for(unsigned int i = 0; i < medians.size(); ++i)
    {
        fin >> check;
        for(unsigned int j = 0; j < 2; ++j)
        {
            if(check == 0)
            {
                variance[j].push_back((float)(medians[i][j]->adapt()));
            }else
            {
                variance[j].push_back((float)(medians[i][j]->adapt())/(float)check);
            }
            fout << (float)check << " " << (float)(medians[i][j]->adapt()) << " " << variance[j].back() << " [";
            for(auto x : *(medians[i][j]))
            {
                fout << x << ", ";
            }
            fout << "]\n";
        }
    }
    // float var1 = std::reduce(variance[0].begin(), variance[0].end(), 0.0f);
    // float var2 = std::reduce(variance[1].begin(), variance[1].end(), 0.0f);

    // std::cout << var1/(float)variance[0].size()<< " " << var2/(float)variance[0].size() << "\n";
}