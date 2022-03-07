#pragma once
#include "includes.h"


template<typename T>
class end_condition{
public:
    virtual bool operator()(const std::vector<std::shared_ptr<individual<T>>>&, const std::shared_ptr<individual<T>>&)=0;
    virtual void reset()=0;
    virtual void set_data(int data)=0;
};

template<typename T>
class max_generation_cond : public end_condition<T>{
public:
max_generation_cond(int max_generation_number):max_generation_number(max_generation_number){}
bool operator()(const std::vector<std::shared_ptr<individual<T>>>&, const std::shared_ptr<individual<T>>&){
    ++generation_number;
    if(generation_number<=max_generation_number){
        return true;
    }
    return false;
}
void reset(){
    generation_number=1;
}
void set_data(int data){
    max_generation_number=data;
}
int max_generation_number;
int generation_number=1;
};

template<typename T>
class max_adaptation_cond : public end_condition<T>{
public:
max_adaptation_cond(int max_stagnation_duration):max_stagnation_duration(max_stagnation_duration){}
bool operator()(const std::vector<std::shared_ptr<individual<T>>>& population, const std::shared_ptr<individual<T>>& best_individ){
    if(last_adapt_maximum==-1){
        last_adapt_maximum=best_individ->adapt();
        return true;
    }else
    if(best_individ->adapt()>last_adapt_maximum){
       last_adapt_maximum=best_individ->adapt();
       stagnation_duration=0;
       return true;
    }else{
        ++stagnation_duration;
        last_adapt_maximum=best_individ->adapt();
        if(stagnation_duration>max_stagnation_duration){
            return false;
        }
    }
    return true;
}
void reset(){
    stagnation_duration=0;
    last_adapt_maximum=-1;
}
void set_data(int data){
    max_stagnation_duration=data;
}
long last_adapt_maximum;
int max_stagnation_duration;
int stagnation_duration=0;
};

template<typename T>
class min_adaptation_cond : public end_condition<T>{
public:
min_adaptation_cond(int max_stagnation_duration):max_stagnation_duration(max_stagnation_duration){}
bool operator()(const std::vector<std::shared_ptr<individual<T>>>& population, const std::shared_ptr<individual<T>>& best_individ){
    if(last_adapt_minimum==-1){
        last_adapt_minimum=best_individ->adapt();
        return true;
    }else
    if(best_individ->adapt()<last_adapt_minimum){
       last_adapt_minimum=best_individ->adapt();
       stagnation_duration=0;
       return true;
    }else{
        ++stagnation_duration;
        last_adapt_minimum=best_individ->adapt();
        if(stagnation_duration>max_stagnation_duration){
            return false;
        }
    }
    return true;
}
void reset(){
    stagnation_duration=0;
    last_adapt_minimum=-1;
}
void set_data(int data){
    max_stagnation_duration=data;
}
long last_adapt_minimum=-1;
int max_stagnation_duration;
int stagnation_duration=0;
};

template<typename T>
class average_adaptation_cond : public end_condition<T>{
public:
average_adaptation_cond(int max_stagnation_duration):max_stagnation_duration(max_stagnation_duration){}
bool operator()(const std::vector<std::shared_ptr<individual<T>>>& population, const std::shared_ptr<individual<T>>& best_individ){
    long average_adapt=0;
    for(auto& individ: population){
        average_adapt+= individ->adapt();
    }
    // std::cout <<"\nCOND_LOG: current average: "<<average_adapt<<" last average: "<<last_average_adapt<<"\n\n";
    if(last_average_adapt==-1){
        last_average_adapt=average_adapt;
        return true;
    }else
    if(average_adapt>last_average_adapt){
       last_average_adapt=average_adapt;
       stagnation_duration=0;
       return true;
    }else{
        ++stagnation_duration;
        last_average_adapt=average_adapt;
        if(stagnation_duration>max_stagnation_duration){
            return false;
        }
    }
    return true;
}
void reset(){
    stagnation_duration=0;
    last_average_adapt=-1;
}
void set_data(int data){
    max_stagnation_duration=data;
}
long last_average_adapt;
int max_stagnation_duration;
int stagnation_duration=0;
};

template<typename T>
class average_adaptation_cond_reversed : public end_condition<T>{
public:
average_adaptation_cond_reversed(int max_stagnation_duration):max_stagnation_duration(max_stagnation_duration){}
bool operator()(const std::vector<std::shared_ptr<individual<T>>>& population, const std::shared_ptr<individual<T>>& best_individ){
    long average_adapt=0;
    for(auto& individ: population){
        average_adapt+= individ->adapt();
    }
     if(last_average_adapt==-1){
        last_average_adapt=average_adapt;
        return true;
    }else
    if(average_adapt<last_average_adapt){
       last_average_adapt=average_adapt;
       stagnation_duration=0;
       return true;
    }else{
        ++stagnation_duration;
        if(stagnation_duration>max_stagnation_duration){
            return false;
        }
    }
    return true;
}
void reset(){
    stagnation_duration=0;
    last_average_adapt=-1;
}
void set_data(int data){
    max_stagnation_duration=data;
}
long last_average_adapt;
int max_stagnation_duration;
int stagnation_duration=0;
};