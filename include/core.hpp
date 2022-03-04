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
        completion_times[i]=start_times[i] + tasks[i].task_time;
    }
    return completion_times;
}

std::vector<unsigned int> get_start_times(const std::vector<task>& tasks, const std::vector<unsigned int> order)
{
    std::vector<unsigned int> start_times(order.size());
    start_times[order[0]] = 0;
    for(unsigned int i = 1; i < order.size(); ++i)
    {
        start_times[order[i]] = start_times[i-1] + tasks[order[i-1]].task_time;
    }
    return start_times;
}

unsigned int get_penalty(const std::vector<task>& tasks, const std::vector<unsigned int> completion_times)
{
    unsigned int penalty = 0;
    for(unsigned int i = 1; i < completion_times.size(); ++i)
    {
        penalty = completion_times[i] > tasks[i].direct_time ? penalty + (completion_times[i] - tasks[i].direct_time) * tasks[i].penalty : penalty;
    }
    return penalty;
}
