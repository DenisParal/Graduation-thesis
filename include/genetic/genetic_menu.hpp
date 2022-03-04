#pragma once
#include "genetic_algorithm.hpp"
#include <string>

template<typename Adaptation_func, typename Comparator>
class menu{
public:
menu(const std::vector<forming_algorithm<Adaptation_func>*>& form_str, const std::vector<selection_strategy<int,Comparator>*>& sel_str, const std::vector<mutation<int>*>& mut_str, 
const std::vector<reproductive_strategy<int>*>& rep_str, const std::vector<crossover<int,Adaptation_func>*>& cros_str,
const std::vector<end_condition<int>*>& end_cond, const Adaptation_func& adapt_func, const Comparator& compare_func, std::size_t genotype_size): forming_strategies(form_str),selection_strategies(sel_str),mutation_strategies(mut_str),reproductive_strategies(rep_str),
crossover_strategies(cros_str), end_conditions(end_cond), adapt_func(adapt_func), compare_func(compare_func), genotype_size(genotype_size){}

void start_menu(){
    int decision=-1;
    std::cout <<"\tMENU\n";
    std::cout <<"1)Configure\n2)Set data\n3)Start algorithm\n4)Generate start population\n5)Exit\n";
    while(decision<1 || decision>5){
        std::cout <<"\nDecision: ";
        std::cin >>decision;
    }
    switch(decision){
        case 1: configure();
        case 2: set_data();
        case 3: start();
        case 4: generate_start_population();
        case 5: exit(1);
    }
}
private:
void set_data(){
    int decision=-1;
    int value=-1;
    std::cout <<"\tDATA\n";
    std::cout <<"1)Population size\n2)Mutation chance\n3)End condition data\n4)Exit\n";
    while(decision<1 || decision>4){
        std::cout <<"\nDecision: ";
        std::cin >>decision;
    }
    if(decision!=4){
        while(value<0){
            std::cout <<"\nValue: ";
            std::cin >>value;
        }
    }
    if(decision==1){
        population_size=value;
    }else if(decision==2){
        mutation_chance=value;
    }else if(decision==3){
        if(end_cond==nullptr){
            std::cout <<"Choose end condition!\n";
            set_data();
        }
        end_cond->set_data(value);
    }else if(decision==4){
        start_menu();
    }
    set_data();
}

void generate_start_population(){
    if(forming_str==nullptr){
        std::cout <<"Choose forming strategy!\n";
        start_menu();
    }
    pool=(*forming_str)(genotype_size,population_size,adapt_func);
    for(std::size_t i=0;i<pool.size();i++){
            std::cout <<i+1<<") ";
            print_key(*(pool[i]));
            std::cout <<" ("<<pool[i]->adapt()<<")\n";
    }
    start_menu();
}

void start(){
    bool permission=true;
    if(forming_str==nullptr){
        std::cout <<"Choose forming algorithm!\n";
        permission=false;
    }
    if(selection_str==nullptr){
        std::cout <<"Choose selection strategy!\n";
        permission=false;
    }
    if(mutation_str==nullptr){
        std::cout <<"Choose mutation algorithm!\n";
        permission=false;
    }
    if(reproductive_str==nullptr){
        std::cout <<"Choose reproduction strategy!\n";
        permission=false;
    }
    if(crossover_str==nullptr){
        std::cout <<"Choose crossover algorithm!\n";
        permission=false;
    }
    if(end_cond==nullptr){
        std::cout <<"Choose ending condition!\n";
        permission=false;
    }
    if(pool.size()==0){
        std::cout <<"Generate start population!\n";
        permission=false;
    }
    if(!permission){
    configure();
    }
    auto algorithm=algorithm_configuration<crossover<int,Adaptation_func>,mutation<int>,reproductive_strategy<int>,selection_strategy<int,Comparator>,end_condition<int>>(
        crossover_str,mutation_str,reproductive_str,selection_str,end_cond
    );
    algorithm(pool,adapt_func,compare_func,mutation_chance);
    (*end_cond).reset();
    start_menu();
}

void configure(){
    int decision=-1;
    std::cout <<"\tConfiguration\n";
    std::cout <<"1)Start population forming method\n2)Selection strategy\n3)Mutation algorithm\n4)Reproduction strategy\n5)Crossover algorithm\n6)End condition\n7)Menu\n";
    while(decision<1 || decision>7){
        std::cout <<"\nDecision: ";
        std::cin >>decision;
    }
    if(decision==1){
        forming_str=set_option(forming_strategies_names,forming_strategies);
    }else if(decision==2){
        selection_str=set_option(selection_strategies_names,selection_strategies);
    }else if(decision==3){
        mutation_str=set_option(mutation_strategies_names,mutation_strategies);
    }else if(decision==4){
        reproductive_str=set_option(reproductive_strategies_names,reproductive_strategies);
    }else if(decision==5){
        crossover_str=set_option(crossover_strategies_names,crossover_strategies);
    }else if(decision==6){
        end_cond=set_option(end_conditions_names,end_conditions);
    }else if(decision==7){
        start_menu();
    }
    configure();
}

template<typename Option>
Option* set_option(const std::vector<std::string>& option_names, std::vector<Option*> option_list){
    int decision=-1;
    for(int i=0;i<option_names.size();i++){
        std::cout <<i+1<<") "<<option_names[i]<<"\n";
    }
    while(decision<1 || decision>option_names.size()){
        std::cout <<"\nDecision: ";
        std::cin >>decision;
    }
    return option_list[decision-1];
}

std::vector<std::string> forming_strategies_names{"Random","Greedy"};
std::vector<std::string> selection_strategies_names{"Proportial","Beta tournament"};
std::vector<std::string> mutation_strategies_names{"Point ordered", "Saltation"};
std::vector<std::string> reproductive_strategies_names{"Positive assotiative", "Negative assotiative", "Inbreeding"};
std::vector<std::string> crossover_strategies_names{"Double point","Classic"};
std::vector<std::string> end_conditions_names{"Max generation","Average adaptation","Max adaptation"};

std::vector<forming_algorithm<Adaptation_func>*> forming_strategies;
std::vector<selection_strategy<int,Comparator>*> selection_strategies;
std::vector<mutation<int>*> mutation_strategies;
std::vector<reproductive_strategy<int>*> reproductive_strategies;
std::vector<crossover<int,Adaptation_func>*> crossover_strategies;
std::vector<end_condition<int>*> end_conditions;

forming_algorithm<Adaptation_func>* forming_str=nullptr;
selection_strategy<int,Comparator>* selection_str=nullptr;
mutation<int>* mutation_str=nullptr;
reproductive_strategy<int>* reproductive_str=nullptr;
crossover<int,Adaptation_func>* crossover_str=nullptr;
end_condition<int>* end_cond=nullptr;

std::size_t genotype_size;
std::size_t mutation_chance;
const Adaptation_func& adapt_func;
const Comparator& compare_func;
std::size_t population_size=0;

std::vector<std::shared_ptr<individual<int>>> pool;
};
