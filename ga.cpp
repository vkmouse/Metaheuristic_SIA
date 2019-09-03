#include "__header__.h"
ga::ga(int num_cms, double Pc, double Pm)
{
    this->num_cms = num_cms;
    this->Pc = Pc;
    this->Pm = Pm;
}
void ga::run()
{
    init();
    while (now_evaluation_time < max_evaluation_time)
    {
        selection();
        crossover();
        mutation();
        fitness();
    }
}
void ga::init()
{
    cms_sol.resize(num_cms);
    cms_objectvalue.resize(num_cms);
    for (int i = 0; i < num_cms; i++)
    {
        cms_sol[i] = create_sol();
        cms_objectvalue[i] = evaluation(cms_sol[i]);
        determination(cms_sol[i], cms_objectvalue[i]);
    }
}
void ga::crossover()
{
    for (int i = 0; i < cms_sol.size() / 2; i++)
    {
        double f = static_cast<double>(rand()) / RAND_MAX;
        if (f < Pc)
        {
            int random_cx = random(0, num_K_resource - 1);
            crossover_2d(cms_sol[i * 2], cms_sol[i * 2 + 1]);
        }
    }
}
void ga::mutation()
{
    for (int i = 0; i < num_cms; i++)
    {
        double f = static_cast<double>(rand()) / RAND_MAX;
        if (f < Pm)
            cms_sol[i] = transition(cms_sol[i]);
    }
}
void ga::fitness()
{
    for (int i = 0; i < num_cms; i++)
    {
        cms_objectvalue[i] = evaluation(cms_sol[i]);
        determination(cms_sol[i], cms_objectvalue[i]);
    }
}
void ga::selection()
{
    v3d<int> copy_cms_sol = cms_sol;
    double total = 0;
    v1d<double> RouletteWheel;

    for (int i = 0; i < num_cms; i++)
        total += 1 / cms_objectvalue[i];

    RouletteWheel.push_back((1.0 / cms_objectvalue[0]) / total);
    for (int i = 1; i < cms_objectvalue.size(); i++)
        RouletteWheel.push_back((1.0 / cms_objectvalue[i]) / total + RouletteWheel.back());

    for (int i = 0; i < cms_sol.size(); i++)
    {
        double f = static_cast<double>(rand()) / RAND_MAX;
        if (f < RouletteWheel[0])
            cms_sol[i] = copy_cms_sol[0];
        else if (f == 1)
            cms_sol[i] = copy_cms_sol.back();
        else
        {
            for (int j = 1; j < RouletteWheel.size(); j++)
                if (f >= RouletteWheel[j - 1] && f < RouletteWheel[j])
                {
                    cms_sol[i] = copy_cms_sol[j];
                    break;
                }
        }
    }
    cms_sol[0] = best_sol;
    cms_objectvalue[0] = best_objectvalue;
}