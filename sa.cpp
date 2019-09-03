#include "__header__.h"
sa::sa(int num_neighbor, double max_temperature, double min_temperature, double decrease_rate)
{
    this->num_neighbor = num_neighbor;
    this->max_temperature = max_temperature;
    this->min_temperature = min_temperature;
    this->decrease_rate = decrease_rate;
}
void sa::run()
{
    init();
    while (now_evaluation_time < max_evaluation_time)
    {
        create_neighbor();
        determination(now_sol, now_objectvalue);
        double r = 0.0 + rand() / static_cast<double>(RAND_MAX);
        double p = exp(double(best_objectvalue - now_objectvalue) / now_temperature);
        if (r >= p)
        {
            now_sol = best_sol;
            now_objectvalue = best_objectvalue;
        }
        if (now_temperature > min_temperature)
            now_temperature *= decrease_rate;
    }
}
void sa::init()
{
    now_sol = create_sol();
    now_objectvalue = evaluation(now_sol);
    determination(now_sol, now_objectvalue);
}
void sa::create_neighbor()
{
    v2d<int> best_sol, tmp_sol;
    double best_objectvalue, tmp_objectvalue;

    best_sol = transition(now_sol);
    best_objectvalue = evaluation(best_sol);

    for (int i = 0; i < num_neighbor - 1; i++)
    {
        tmp_sol = transition(now_sol);
        tmp_objectvalue = evaluation(tmp_sol);

        if (best_objectvalue > tmp_objectvalue)
        {
            best_sol = tmp_sol;
            best_objectvalue = tmp_objectvalue;
        }
    }

    now_sol = best_sol;
    now_objectvalue = best_objectvalue;
}