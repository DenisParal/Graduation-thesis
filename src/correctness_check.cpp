#include "genetic_algorithm.hpp"
#include "core.hpp"

std::ifstream fin;
int main()
{
    fin.open("/home/dparanic/Study/Graduation-thesis/datasets/40_tasks.txt");
    unsigned int data_size = 6;

    std::vector<std::vector<std::vector<unsigned int>>> tasks_data = load_tasks(fin, data_size);
    std::vector<task> tasks;
    unsigned int task_set_index = 5;

    for(std::size_t k = 0; k < data_size; ++k)
    {
        tasks.push_back(task(tasks_data[task_set_index][0][k], tasks_data[task_set_index][1][k], tasks_data[task_set_index][2][k]));
    }

    // std::vector<unsigned int> order{1, 37, 3, 35, 36, 5, 14, 19, 16, 29, 9, 26, 33, 25, 28, 22, 24, 39, 18, 11, 2, 10, 6, 0, 21, 38, 4, 30, 31, 34, 32, 27, 13, 20, 8, 23, 17, 15, 7, 12};
    std::vector<unsigned int> order{16, 18, 29, 9, 3, 2, 31, 19, 28, 36, 38, 37, 24, 7, 17, 5, 39, 11, 22, 35, 34, 10, 4, 1, 21, 32, 25, 30, 15, 33, 20, 27, 12, 13, 8, 23, 6, 0, 26, 14};
    auto p1 = penalty_calculator{tasks}(order);
    auto p2 = calculate_penalty(tasks, order);
    std::cout << p1 << " " << p2 << "\n";
}