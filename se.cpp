#include "__header__.h"
se::se(int num_searcher, int num_region, int num_sample, int num_player)
{
    this->num_searcher = num_searcher;
    this->num_region = num_region;
    this->num_sample = num_sample;
    this->num_player = num_player;
}
void se::run()
{
    init();                 // 1. initialization
    resource_arrangement(); // 2. resource arrangement
    while (now_evaluation_time < max_evaluation_time)
    {
        vision_search();
        marketing_survey();
    }
}
// 1. initialization
void se::init()
{
    // set aside arrays for searchers, samples, and sampleV (the
    // crossover results of searchers and samples)
    searcher_sol.assign(num_searcher, v2d<int>(num_K_resource, v1d<int>(num_J_service, -1)));
    sample_sol.assign(num_region, v3d<int>(num_sample, v2d<int>(num_K_resource, v1d<int>(num_J_service, -1))));
    sample_sol_best.assign(num_region, v2d<int>(v2d<int>(num_K_resource, v1d<int>(num_J_service, -1))));
    sampleV_sol.assign(num_searcher, v4d<int>(num_region, v3d<int>(num_sample * 2, v2d<int>(num_K_resource, v1d<int>(num_J_service, -1)))));

    searcher_sol_fitness.assign(num_searcher, INT_MAX);
    sample_sol_fitness.assign(num_region, v1d<double>(num_sample, INT_MAX));
    sample_sol_best_fitness.assign(num_region, INT_MAX);
    sampleV_sol_fitness.assign(num_searcher, v2d<double>(num_region, v1d<double>(num_sample * 2, INT_MAX)));

    for (int i = 0; i < num_searcher; i++)
    {
        searcher_sol[i] = create_sol();
        searcher_sol_fitness[i] = evaluation(searcher_sol[i]);
    }
}
// 2. resource arrangement
void se::resource_arrangement()
{
    // 2.1. initialize searchers and regions
    // 2.1.1 assign searcher to its region and their investment

    // 2.1.2 initialize the sample solutions
    for (int i = 0; i < num_region; i++)
        for (int j = 0; j < num_sample; j++)
        {
            sample_sol[i][j] = create_sol();
            sample_sol_fitness[i][j] = evaluation(sample_sol[i][j]);
        }

    // 2.2. initialize the investment and set how long regions have not been searched
    region_it.assign(num_region, 0.0);
    region_nit.assign(num_region, 1.0);
    for (int i = 0; i < num_searcher; i++)
    {
        int idx = i % num_region;
        region_it[idx]++;
        region_nit[idx] = 1.0;
    }

    // 2.3. initialize the expected values (ev)
    expected_value.assign(num_searcher, v1d<double>(num_region, 0.0));
    T_j.assign(num_region, 0.0);
    V_ij.assign(num_searcher, v1d<double>(num_region, 0.0));
    M_j.assign(num_region, 0.0);
}
void se::vision_search()
{
    // 3.1 construct V (searcher X sample)
    transit();
    // 3.2 compute the expected value of all regions of searchers
    compute_expected_value();
    // 3.3 select region to which a searcher will be assigned
    vision_selection(num_player);
}
void se::transit()
{
    // crossover
    for (int i = 0; i < num_searcher; i++)
        for (int j = 0; j < num_region; j++)
            for (int k = 0; k < num_sample; k++)
            {
                int m = k * 2;
                sampleV_sol[i][j][m] = searcher_sol[i];
                sampleV_sol[i][j][m + 1] = sample_sol[j][k];
                crossover_2d(sampleV_sol[i][j][m], sampleV_sol[i][j][m + 1]);
            }
    // transition and match region
    for (int i = 0; i < num_searcher; i++)
        for (int j = 0; j < num_region; j++)
            for (int k = 0; k < 2 * num_sample; k++)
            {
                sampleV_sol[i][j][k] = transition(sampleV_sol[i][j][k]);
                while (sampleV_sol[i][j][k][0][0] % num_region != j)
                    sampleV_sol[i][j][k] = transition(sampleV_sol[i][j][k], 0, 0);
            }
}
void se::compute_expected_value()
{
    // fitness value of sampleV, update sampleV to sample, V_ij
    for (int i = 0; i < num_searcher; i++)
        for (int j = 0; j < num_region; j++)
        {
            V_ij[i][j] = 0.0;
            for (int k = 0; k < num_sample; k++)
            {
                int m = k * 2;

                // fitness value of sampleV
                sampleV_sol_fitness[i][j][m] = evaluation(sampleV_sol[i][j][m]);
                sampleV_sol_fitness[i][j][m + 1] = evaluation(sampleV_sol[i][j][m + 1]);

                // update sampleV to sample
                if (sampleV_sol_fitness[i][j][m] < sample_sol_fitness[j][k])
                {
                    sample_sol[j][k] = sampleV_sol[i][j][m];
                    sample_sol_fitness[j][k] = sampleV_sol_fitness[i][j][m];
                }
                if (sampleV_sol_fitness[i][j][m + 1] < sample_sol_fitness[j][k])
                {
                    sample_sol[j][k] = sampleV_sol[i][j][m + 1];
                    sample_sol_fitness[j][k] = sampleV_sol_fitness[i][j][m + 1];
                }

                // V_ij
                V_ij[i][j] += (1.0 / sampleV_sol_fitness[i][j][m]) + (1.0 / sampleV_sol_fitness[i][j][m + 1]);
            }
            V_ij[i][j] /= 2 * num_sample;
        }

    // update sample best fitness, compute all sample fitness
    double all_sample_fitness = 0.0;
    for (int j = 0; j < num_region; j++)
    {
        double rbj = sample_sol_best_fitness[j];
        int index = -1;

        for (int k = 0; k < num_sample; k++)
        {
            all_sample_fitness += 1.0 / sample_sol_fitness[j][k];
            // update fbj
            if (sample_sol_fitness[j][k] < rbj)
            {
                index = k;
                rbj = sample_sol_fitness[j][k];
            }
        }

        if (index >= 0)
        {
            sample_sol_best_fitness[j] = rbj;
            sample_sol_best[j] = sample_sol[j][index];
        }
    }
    T_j.assign(num_region, 0.0);
    V_ij.assign(num_searcher, v1d<double>(num_region, 0.0));
    M_j.assign(num_region, 0.0);
    // M_j
    for (int j = 0; j < num_region; j++)
        M_j[j] = (1.0 / sample_sol_best_fitness[j]) / all_sample_fitness; //tsp取倒數

    // T_j
    for (int j = 0; j < num_region; j++)
        T_j[j] = region_it[j] / region_nit[j];

    // compute the expected_value
    for (int i = 0; i < num_searcher; i++)
        for (int j = 0; j < num_region; j++)
            expected_value[i][j] = T_j[j] * V_ij[i][j] * M_j[j];
}
void se::vision_selection(int player)
{
    for (int i = 0; i < num_region; i++)
        region_nit[i]++;

    for (int i = 0; i < num_searcher; i++)
    {
        // find the idx of the best vij
        int play0_idx = rand() % num_region;
        double play0_ev = expected_value[i][play0_idx];

        for (int j = 0; j < num_player - 1; j++)
        {
            int play1_idx = rand() % num_region;
            if (expected_value[i][play1_idx] > play0_ev)
            {
                play0_idx = play1_idx;
                play0_ev = expected_value[i][play0_idx];
            }
        }

        // update searcher_sol
        if (sample_sol_best_fitness[play0_idx] > searcher_sol_fitness[i])
        {
            searcher_sol[i] = sample_sol_best[play0_idx];
            searcher_sol_fitness[i] = sample_sol_best_fitness[play0_idx];
        }

        // update region_it[i] and region_nit[i];
        region_it[play0_idx]++;
        region_nit[play0_idx] = 1;
    }
}
// 4. marketing survey
void se::marketing_survey()
{
    for (int j = 0; j < num_region; j++)
    {
        // 4.1 update region_it
        if (region_nit[j] > 1)
            region_it[j] = 1.0;
        // 4.2 update the best solution
        determination(sample_sol_best[j], sample_sol_best_fitness[j]);
    }
}
