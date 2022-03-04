#include "genetic_algorithm.hpp"
#include "core.hpp"
#include <fstream>

std::ifstream fin;

int main(int argc, char** argv)
{
    std::vector<unsigned int> task_times;
    std::vector<unsigned int> penalties;
    std::vector<unsigned int> directives;
    fin.open(argv[1]);
    std::size_t data_size = std::atoi(argv[2]);

    unsigned int task_time;
    unsigned int penalty;
    unsigned int directive;

    // while(!fin.eof())
    {
        std::vector<task> tasks;
        for(std::size_t i = 0; i < data_size; ++i)
        {
            fin >> task_time;
            task_times.push_back(task_time);
        }
        for(std::size_t i = 0; i < data_size; ++i)
        {
            fin >> penalty;
            penalties.push_back(penalty);
        }
        for(std::size_t i = 0; i < data_size; ++i)
        {
            fin >> directive;
            directives.push_back(directive);
        }
        for(std::size_t i = 0; i < data_size; ++i)
        {
            tasks.push_back(task(task_times[i], penalties[i], directives[i]));
        }
        for(std::size_t i = 0; i < data_size; ++i)
        {
            std::cout << tasks[i].task_time << " " << tasks[i].penalty << " " << tasks[i].direct_time << "\n";
        }
    }
}