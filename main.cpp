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
    // else if (algorithm == "cro")
    //     search_alg = new cro(
    //         atoi(argv[8]),  // n1
    //         atoi(argv[9]),  // n2
    //         atof(argv[10]), // Pi
    //         atof(argv[11]), // Fb
    //         atoi(argv[12]), // K
    //         atof(argv[13]), // Fa
    //         atof(argv[14]), // Fd
    //         atof(argv[15])  // Pd
    //     );
    // else if (algorithm == "se")
    //     search_alg = new se(
    //         atoi(argv[8]),  // num_searcher
    //         atoi(argv[9]),  // num_region
    //         atoi(argv[10]), // num_sample
    //         atoi(argv[11])  // num_player
    //     );
    // else if (algorithm == "sasp")
    //     search_alg = new sasp(
    //         atoi(argv[8]),  // numPartition
    //         atoi(argv[9]),  // neighbor
    //         atof(argv[10]), // max_temperature
    //         atof(argv[11])  // min_temperature
    //     );

    search_alg->init(
        argv[2],        // dataset_path
        atoi(argv[3]),  // max_evaluation_time
        atoi(argv[4]),  // per_evaluation_time
        atoi(argv[5])); // num_run

    search_alg->run_all();
}
