#ifndef _HEADER_H_
#define _HEADER_H_

#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>

using std::cout;
using std::endl;
using std::string;

template <typename T>
using v1d = std::vector<T>;
template <typename T>
using v2d = std::vector<std::vector<T>>;
template <typename T>
using v3d = std::vector<std::vector<std::vector<T>>>;
template <typename T>
using v4d = std::vector<std::vector<std::vector<std::vector<T>>>>;
template <typename T>
using v5d = std::vector<std::vector<std::vector<std::vector<std::vector<T>>>>>;

class search_algorithm
{
public:
    // Parameter for every algorithm
    string dataset_path;
    int now_evaluation_time = 0;
    int max_evaluation_time = INT_MAX;
    int per_evaluation_time = 1000;
    int num_run = 1;

    v2d<int> I_interface_ik;
    v2d<int> J_service_jk;
    v2d<int> c_ik;
    int num_K_resource;
    int num_I_interface;
    int num_J_service;
    v2d<int> best_sol;
    int best_objectvalue = INT_MAX;

    v2d<int> create_sol_FCFS();
    v2d<int> create_sol_RANDOM_INIT_EQUAL_SHARES();
    v2d<int> create_sol_AVERAGE_COST_ALLOCATION();
    int evaluation(v2d<int> sol);
    int evaluation_rule_based(v2d<int> sol);

    // method
    virtual void run() = 0;
    void run_all();
    void init(string dataset_path, int max_evaulation_time, int per_evaulation_time, int num_run);
    void read_dataset();
    int random(int min, int max);
    v2d<int> create_sol();
    v2d<int> transition(v2d<int> sol, int x = -1, int y = -1);
    void determination(v2d<int> now_sol, int now_objectvalue);
    void crossover_2d(v2d<int> &sol1, v2d<int> &sol2);
    template <class T>
    T takeout(v1d<T> &vec, int num);
};
class hc : public search_algorithm
{
public:
    virtual void run();

private:
    v2d<int> now_sol;
    int now_objectvalue;
    void init();
};
class sa : public search_algorithm
{
public:
    sa(int num_neighbor, double max_temperature, double min_temperature, double decrease_rate);
    virtual void run();

private:
    int num_neighbor;
    double max_temperature;
    double min_temperature;
    double decrease_rate;

    v2d<int> now_sol;
    int now_objectvalue;
    double now_temperature;
    void init();
    void create_neighbor();
};
class ga : public search_algorithm
{
public:
    ga(int num_cms, double Pc, double Pm);
    virtual void run();

private:
    int num_cms;
    double Pc;
    double Pm;
    v3d<int> cms_sol;
    v1d<int> cms_objectvalue;
    void init();
    void crossover();
    void mutation();
    void fitness();
    void selection();
};
class se : public search_algorithm
{
public:
    se(int num_searcher, int num_region, int num_sample, int num_player);
    virtual void run();

private:
    int num_searcher;
    int num_region;
    int num_sample;
    int num_player;

    // 0.3 search algorithm
    v3d<int> searcher_sol;    // [searcher, num_bit_sol]
    v4d<int> sample_sol;      // [region, sample, num_bit_sol]
    v3d<int> sample_sol_best; // [region, num_bit_sol]
    v5d<int> sampleV_sol;     // [searcher, region, sample, num_bit_sol]

    v1d<double> searcher_sol_fitness;
    v2d<double> sample_sol_fitness;
    v1d<double> sample_sol_best_fitness;
    v3d<double> sampleV_sol_fitness;

    v1d<double> region_it;
    v1d<double> region_nit;

    v2d<double> expected_value;
    v1d<double> T_j;
    v2d<double> V_ij;
    v1d<double> M_j;

    void init();
    void resource_arrangement();
    void vision_search();
    void transit();
    void compute_expected_value();
    void vision_selection(int player);
    void marketing_survey();
};
class FCFS : public search_algorithm
{
    virtual void run()
    {
        cout << evaluation(create_sol_FCFS()) << endl;
    }
};
class RIA : public search_algorithm
{
    virtual void run()
    {
        cout << evaluation_rule_based(create_sol_RANDOM_INIT_EQUAL_SHARES()) << endl;
    }
};
class ACA : public search_algorithm
{
    virtual void run()
    {
        cout << evaluation_rule_based(create_sol_AVERAGE_COST_ALLOCATION()) << endl;
    }
};
#endif