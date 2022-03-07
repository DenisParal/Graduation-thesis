#pragma once
#include <vector>

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
