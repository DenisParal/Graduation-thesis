#pragma once
#include <vector>
#include "utils.h"

struct task
{
    unsigned int task_time;
    unsigned int penalty;
    unsigned int direct_time;
    task(unsigned int task_time, unsigned int penalty, unsigned int direct_time):task_time(task_time), penalty(penalty), direct_time(direct_time){}
};

std::vector<unsigned int> get_completion_times(const std::vector<unsigned int>& start_times, const std::vector<task>& tasks)
{
    std::vector<unsigned int> completion_times(start_times.size());
    for(unsigned int i = 0; i < start_times.size(); ++i)
    {
        completion_times[i] = start_times[i] + tasks[i].task_time;
    }
    return completion_times;
}

std::vector<unsigned int> get_start_times(const std::vector<task>& tasks, const std::vector<unsigned int> order)
{
    std::vector<unsigned int> start_times(order.size());
    unsigned int current_time = 0;
    for(unsigned int i = 0; i < order.size(); ++i)
    {
        start_times[order[i]] = current_time;
        current_time += tasks[order[i]].task_time; //Should I add 1 to current time to start next job?
    }
    return start_times;
}

unsigned int get_penalty(const std::vector<task>& tasks, const std::vector<unsigned int> completion_times)
{
    unsigned int penalty = 0;
    for(unsigned int i = 0; i < completion_times.size(); ++i)
    {
        penalty = completion_times[i] > tasks[i].direct_time ? penalty + (completion_times[i] - tasks[i].direct_time) * tasks[i].penalty : penalty;
    }
    return penalty;
}

unsigned int calculate_penalty(const std::vector<task>& tasks, const std::vector<unsigned int> order)
{
    unsigned int penalty = 0;
    unsigned int current_time = 0;
    for(unsigned int i = 0; i < order.size(); ++i)
    {
        current_time += tasks[order[i]].task_time;
        if(current_time > tasks[order[i]].direct_time)
        {
            penalty += (current_time - tasks[order[i]].direct_time) * tasks[order[i]].penalty;
        }
    }
    return penalty;
}

class penalty_calculator
{
private:
    std::vector<task> tasks;
public:
    penalty_calculator(const std::vector<task> tasks) : tasks(tasks) {}
    template<typename T>
    unsigned int operator()(const std::vector<T>& order) const
    {
        auto start_times = get_start_times(tasks, order);
        auto completion_times = get_completion_times(start_times, tasks);
        return get_penalty(tasks, completion_times);
    }
};

template<typename Adapt_func>
std::vector<std::shared_ptr<individual<unsigned int>>> generate_random_population(std::size_t individ_size, std::size_t pop_size,const Adapt_func& func){
    std::vector<std::shared_ptr<individual<unsigned int>>> result;
    std::vector<unsigned int> base;
    std::vector<unsigned int> order(individ_size);
    int pos;
    for(std::size_t i=0;i<pop_size;i++){
        for(int j=0;j<individ_size;j++){
            base.push_back(j);
            order[j]=rand()%individ_size;
        }
        for(int j=0;j<individ_size;j++){
            pos=rand()%base.size();
            order[j]=base[pos];
            base.erase(base.begin()+pos);
        }
        result.push_back(std::make_shared<individual<unsigned int>>(order,func));
    }
    return result;
}
