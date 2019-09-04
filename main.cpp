#include "__header__.h"

int main(int argc, char **argv)
{
    string algorithm = argv[1]; // algorithm
    search_algorithm *search_alg;

    if (algorithm == "hc")
        search_alg = new hc();
    else if (algorithm == "sa")
        search_alg = new sa(
            atoi(argv[6]), // neighbor
            atof(argv[7]), // max_temperature
            atof(argv[8]), // min_temperature
            atof(argv[9])  // decrease_rate
        );
    else if (algorithm == "ga")
        search_alg = new ga(
            atoi(argv[6]), // num_cms
            atof(argv[7]), // Pc
            atof(argv[8])  // Pm
        );
    else if (algorithm == "se")
        search_alg = new se(
            atoi(argv[6]), // num_searcher
            atoi(argv[7]), // num_region
            atoi(argv[8]), // num_sample
            atoi(argv[9])  // num_player
        );
    else if (algorithm == "RIA")
        search_alg = new RIA();
    else if (algorithm == "ACA")
        search_alg = new ACA();
    else if (algorithm == "FCFS")
        search_alg = new FCFS();

    search_alg->init(
        argv[2],        // dataset_path
        atoi(argv[3]),  // max_evaluation_time
        atoi(argv[4]),  // per_evaluation_time
        atoi(argv[5])); // num_run

    search_alg->run_all();
}
