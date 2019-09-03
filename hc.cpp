#include "__header__.h"
void hc::run()
{
    init();
    while (now_evaluation_time < max_evaluation_time)
    {
        now_sol = transition(best_sol);
        now_objectvalue = evaluation(now_sol);
        determination(now_sol, now_objectvalue);
    }
}
void hc::init()
{
    now_sol = create_sol();
    now_objectvalue = evaluation(now_sol);
    determination(now_sol, now_objectvalue);
}