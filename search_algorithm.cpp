#include "__header__.h"
#include <time.h>
#include <sstream>

void search_algorithm::run_all()
{
    srand(time(0));
    for (int now_run = 0; now_run < num_run; now_run++)
    {
        now_evaluation_time = 0;
        best_objectvalue = INT_MAX;
        run();
    }
}
void search_algorithm::init(string dataset_path, int max_evaluation_time, int per_evaluation_time, int num_run)
{
    this->dataset_path = dataset_path;
    this->max_evaluation_time = max_evaluation_time;
    this->per_evaluation_time = per_evaluation_time;
    this->num_run = num_run;
    read_dataset();
}
void search_algorithm::read_dataset()
{
    std::ifstream file;
    file.open(dataset_path.c_str());
    string line;
    std::stringstream ss;

    for (int i = 0; i < 3; i++)
    {
        getline(file, line);
        string str1, str2;
        ss.clear();
        ss << line;
        ss >> str1;
        ss >> str2;
        ss >> str2;
        if (str1 == "num_K_resource")
            num_K_resource = atoi(str2.c_str());
        else if (str1 == "num_I_interface")
            num_I_interface = atoi(str2.c_str());
        else if (str1 == "num_J_service")
            num_J_service = atoi(str2.c_str());
    }
    I_interface_ik.assign(num_I_interface, v1d<int>(num_K_resource, -1));
    J_service_jk.assign(num_J_service, v1d<int>(num_K_resource, -1));
    c_ik.assign(num_I_interface, v1d<int>(num_K_resource, -1));

    for (int i = 0; i < num_I_interface; i++)
    {
        getline(file, line);
        ss.clear();
        ss << line;
        for (int k = 0; k < num_K_resource; k++)
            ss >> I_interface_ik[i][k];
    }

    for (int j = 0; j < num_J_service; j++)
    {
        getline(file, line);
        ss.clear();
        ss << line;
        for (int k = 0; k < num_K_resource; k++)
            ss >> J_service_jk[j][k];
    }

    for (int i = 0; i < num_I_interface; i++)
    {
        getline(file, line);
        ss.clear();
        ss << line;
        for (int k = 0; k < num_K_resource; k++)
            ss >> c_ik[i][k];
    }
}
int search_algorithm::random(int min, int max)
{
    double f = static_cast<double>(rand()) / RAND_MAX;
    while ((int)f == 1)
        f = static_cast<double>(rand()) / RAND_MAX;
    f = f * (max - min + 1) + min;
    return f;
}
v2d<int> search_algorithm::create_sol()
{
    v2d<int> sol(num_K_resource, v1d<int>(num_J_service, -1));
    for (int k = 0; k < num_K_resource; k++)
    {
        v1d<int> i1dtmp1;
        for (int j = 0; j < num_J_service; j++)
            i1dtmp1.push_back(j);
        for (int j = 0; j < num_J_service; j++)
        {
            int rand_num = random(0, i1dtmp1.size() - 1);
            sol[k][j] = i1dtmp1[rand_num];
            i1dtmp1.erase(i1dtmp1.begin() + rand_num);
        }
    }
    return sol;
}
v2d<int> search_algorithm::transition(v2d<int> sol)
{
    int rand_K_resource = rand() % num_K_resource;
    int tmp, rand_J_serviece1, rand_J_serviece2;

    rand_J_serviece1 = rand() % num_J_service;
    do
    {
        rand_J_serviece2 = rand() % num_J_service;
    } while (rand_J_serviece1 == rand_J_serviece2);

    tmp = sol[rand_K_resource][rand_J_serviece1];
    sol[rand_K_resource][rand_J_serviece1] = sol[rand_K_resource][rand_J_serviece2];
    sol[rand_K_resource][rand_J_serviece2] = tmp;

    return sol;
}
int search_algorithm::evaluation(v2d<int> sol)
{

    v2d<int> b_ik = I_interface_ik; // interfaces capacities b
    v2d<int> A_ij(num_I_interface, v1d<int>(num_J_service, 0));
    int total_cost = 0;

    for (int k = 0; k < num_K_resource; k++)
        for (int j = 0; j < num_J_service; j++)
        {
            int d_jk = sol[k][j];                       // services number
            int service_demand = J_service_jk[d_jk][k]; // the resource which service demand
            int non_split_cost = -1;
            int split_cost = 0;

            v1d<int> split_index;
            int non_split_index = -1;
            int num_connot_bunden = 0;
            for (int i = 0; i < num_I_interface; i++)
                // if number of bunden > service need
                if (b_ik[i][k] >= service_demand)
                {
                    non_split_index = i;
                    non_split_cost = c_ik[i][k] * service_demand;

                    total_cost += non_split_cost;
                    A_ij[i][j] = 1;
                    b_ik[i][k] -= service_demand;

                    break;
                }
            if (non_split_index == -1)
            {
                for (int i = 0; i < num_I_interface; i++)
                {
                    if (service_demand >= b_ik[i][k])
                    {
                        split_cost += c_ik[i][k] * b_ik[i][k];
                        service_demand -= b_ik[i][k];
                        A_ij[i][j] = 1;
                        b_ik[i][k] = 0;
                    }
                    else
                    {
                        split_cost += c_ik[i][k] * service_demand;
                        service_demand = 0;
                        A_ij[i][j] = 1;
                        b_ik[i][k] -= service_demand;
                    }
                    if (service_demand == 0)
                        break;
                }
                total_cost += split_cost;
            }
        }
    for (int i = 0; i < num_I_interface; i++)
    {
        int A_i = 0;
        for (int j = 0; j < num_J_service; j++)
            if (A_ij[i][j] == 1)
            {
                A_i = 1;
                break;
            }
        total_cost += 100 * A_i; // set each element in F (Fi) is 100.
    }

    now_evaluation_time++;
    if (now_evaluation_time % per_evaluation_time == 0 && now_evaluation_time != 0)
        cout << now_evaluation_time << "," << best_objectvalue << endl;
    if (now_evaluation_time == 1)
        cout << now_evaluation_time << "," << total_cost << endl;

    return total_cost;
}
void search_algorithm::determination(v2d<int> now_sol, int now_objectvalue)
{
    if (now_objectvalue < best_objectvalue)
    {
        best_sol = now_sol;
        best_objectvalue = now_objectvalue;
    }
}
void search_algorithm::crossover_2d(v2d<int> &sol1, v2d<int> &sol2)
{
    v2d<int> mask(sol1.size(), v1d<int>(num_J_service));
    v1d<int> gene1, gene2;
    v1d<int> tmp_gene1, tmp_gene2;
    v1d<int> count(sol1.size(), 0);

    int x1, y1, x2, y2;

    x1 = rand() % sol1.size();
    y1 = rand() % num_J_service;
    x2 = rand() % sol1.size();
    y2 = rand() % num_J_service;

    while (x1 == x2 && y1 == y2)
    {
        x2 = rand() % sol1.size();
        y2 = rand() % num_J_service;
    }

    if (x1 * num_J_service + y1 > x2 * num_J_service + y2)
    {
        int tmp = x1;
        x1 = x2;
        x2 = tmp;
        tmp = y1;
        y1 = y2;
        y2 = tmp;
    }

    int mask01 = random(0, 1);
    int point1 = x1 * num_J_service + y1;
    int point2 = x2 * num_J_service + y2;
    for (int i = 0; i < mask.size(); i++)
        for (int j = 0; j < num_J_service; j++)
        {
            int point = i * num_J_service + j;
            if (point > point1 && point < point2)
                mask[i][j] = mask01;
            else
                mask[i][j] = !mask01;
        }

    for (int i = 0; i < mask.size(); i++)
        for (int j = 0; j < num_J_service; j++)
            if (mask[i][j] == 1)
            {
                gene1.push_back(sol1[i][j]);
                gene2.push_back(sol2[i][j]);
                count[i]++;
            }

    for (int i = 0; i < count.size(); i++)
    {
        if (count[i] == num_J_service)
            for (int j = 0; j < num_J_service; j++)
                tmp_gene1.push_back(sol2[i][j]);
        else
            for (int j = 0; j < num_J_service; j++)
                for (int k = 0; k < count[i]; k++)
                    if (gene1[k] == sol2[i][j])
                    {
                        tmp_gene1.push_back(sol2[i][j]);
                        break;
                    }
        gene1.erase(gene1.begin(), gene1.begin() + count[i]);
    }

    for (int i = 0; i < count.size(); i++)
    {
        if (count[i] == num_J_service)
            for (int j = 0; j < num_J_service; j++)
                tmp_gene2.push_back(sol1[i][j]);
        else
            for (int j = 0; j < num_J_service; j++)
                for (int k = 0; k < count[i]; k++)
                    if (gene2[k] == sol1[i][j])
                    {
                        tmp_gene2.push_back(sol1[i][j]);
                        break;
                    }
        gene2.erase(gene2.begin(), gene2.begin() + count[i]);
    }

    for (int i = 0; i < mask.size(); i++)
        for (int j = 0; j < num_J_service; j++)
            if (mask[i][j] == 1)
            {
                sol1[i][j] = takeout(tmp_gene1, 0);
                sol2[i][j] = takeout(tmp_gene2, 0);
            }
}
template <class T>
T search_algorithm::takeout(v1d<T> &vec, int num)
{
    T t = vec[num];
    vec.erase(vec.begin() + num);
    return t;
}
