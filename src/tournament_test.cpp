#include "genetic_algorithm.hpp"
#include "core.hpp"
#include <algorithm>
#include <numeric>

std::ifstream fin;
std::ofstream fout;
int main(int argc, char** argv)
{
    std::string data_path = "/home/denis/work/Graduation-thesis/datasets/40_tasks.txt";
    std::string output_path = "/home/denis/work/Graduation-thesis/datasets/40_tasks_res.txt";
    std::string res_check_path = "/home/denis/work/Graduation-thesis/datasets/40_tasks_check.txt";
    unsigned int data_size = 40;
    if(argc > 1)
    {
    data_size = std::atoi(argv[1]);
    data_path = argv[2];
    output_path = argv[3];
    res_check_path = argv[4];
    }
    fin.open(data_path);
    fout.open(output_path);

    auto decider=[](unsigned int value_to_compare, unsigned int value_to_compare_with){
        return value_to_compare<value_to_compare_with;
    };

    std::vector<unsigned int> positions{data_size/4, data_size/2 + data_size/4};


    std::vector<reproductive_strategy<unsigned int>*> reproductive_strategies;
    reproductive_strategies.push_back(new positive_assotiative_reproductive_sterategy<unsigned int>());
    // reproductive_strategies.push_back(new negative_assotiative_reproductive_sterategy<unsigned int>());
    // reproductive_strategies.push_back(new roulette_reproductive_strategy<unsigned int>());

    std::vector<std::vector<std::vector<unsigned int>>> tasks_data = load_tasks(fin, data_size);

    std::vector<std::vector<std::shared_ptr<individual<unsigned int>>>> medians;
    std::vector<task> tasks;

    for(unsigned int i = 0; i < tasks_data.size(); i++)
    {
        medians.push_back(std::vector<std::shared_ptr<individual<unsigned int>>>());
        std::cout << "Progressing task set " << i <<"...\n";
        for(unsigned int j = 0; j < reproductive_strategies.size(); j++)
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
                                   >evo(new classic_crossover<unsigned int, decltype(calc)>(),
                                        new saltation_mut<unsigned int>(),//new point_ordered_mut<unsigned int>(),
                                        reproductive_strategies[j],
                                        new beta_tournament<unsigned int,decltype(decider)>(5),
                                        new min_adaptation_cond<unsigned int>(data_size)
                                       );
            auto pool=generate_random_population(data_size,data_size,calc);
            std::shared_ptr<individual<unsigned int>> res = evo(pool, calc, decider, 30);

            medians[i].push_back(res);

            tasks.clear();
        }
        std::cout << "Completed...\n";
    }

    fin.close();
    std::cout << "Opening check file...\n";
    fin.open(res_check_path);
    std::cout << "Opened...\n";
    std::vector<std::vector<float>> variance(reproductive_strategies.size(),std::vector<float>());
    unsigned int check;
    for(unsigned int i = 0; i < medians.size(); ++i)
    {
        fin >> check;
        for(unsigned int j = 0; j < reproductive_strategies.size(); ++j)
        {
            if(check == 0)
            {
                variance[j].push_back((float)(medians[i][j]->adapt()));
            }else
            {
                variance[j].push_back(((float)(medians[i][j]->adapt()) - (float)(check))/(float)(check));
            }
            fout << (float)check << "," << (float)(medians[i][j]->adapt()) << "," << variance[j].back() <<";\n";
            // fout << " [";
            // for(auto x : *(medians[i][j]))
            // {
            //     fout << x << ", ";
            // }
            // fout << "]\n";
        }
    }

    for(unsigned int i = 0; i < reproductive_strategies.size(); ++i)
    {
        std::sort(variance[i].begin(),variance[i].end());
        variance[i].erase(variance[i].begin(),variance[i].begin()+variance[i].size()/10);
        variance[i].erase(variance[i].end()-variance[i].size()/10, variance[i].end());
        float var = std::accumulate(variance[i].begin(), variance[i].end(), 0.0f);
        std::cout << var/(float)variance[i].size()<< " ";
    }
    std::cout << std::endl;
}