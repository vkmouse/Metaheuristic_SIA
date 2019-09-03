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

    // method
    virtual void run() = 0;
    void run_all();
    void init(string dataset_path, int max_evaulation_time, int per_evaulation_time, int num_run);
    void read_dataset();
    int random(int min, int max);
    v2d<int> create_sol();
    v2d<int> transition(v2d<int> sol);
    int evaluation(v2d<int> sol);
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
#endif