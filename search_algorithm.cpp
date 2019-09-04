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
v2d<int> search_algorithm::transition(v2d<int> sol, int x, int y)
{

    int rand_K_resource = (x != -1) ? x : rand() % num_K_resource;
    int tmp, rand_J_serviece1, rand_J_serviece2;
    rand_J_serviece1 = (y != -1) ? y : rand() % num_J_service;
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
v2d<int> search_algorithm::create_sol_FCFS()
{
    v2d<int> sol(num_K_resource, v1d<int>(num_J_service, -1));
    for (int k = 0; k < num_K_resource; k++)
    {
        for (int j = 0; j < num_J_service; j++)
        {
            sol[k][j] = j;
        }
    }
    return sol;
}
v2d<int> search_algorithm::create_sol_RANDOM_INIT_EQUAL_SHARES()
{
    v2d<int> sol;
    for (int k = 0; k < num_K_resource; k++)
    {
        v1d<int> tmp_sol;
        tmp_sol.push_back(0);
        for (int j = 1; j < num_J_service; j++)
        {
            // index1 => the last bigger than itself
            // index2 => the first smaller than itself
            int index1 = -1, index2 = 0;
            for (int i = 0; i < tmp_sol.size(); i++)
            {
                // the order is from big to small
                if (J_service_jk[j][k] > J_service_jk[tmp_sol[i]][k])
                    index1 = i;
                else if (J_service_jk[j][k] < J_service_jk[tmp_sol[i]][k])
                {
                    index2 = i;
                    tmp_sol.insert(tmp_sol.begin() + random(index1 + 1, index2), j);
                    break;
                }
            }
            // if the above is not execute, either it is bigger than all or all of them are the same
            if (j == tmp_sol.size())
                if (J_service_jk[j][k] >= J_service_jk[tmp_sol.back()][k])
                    tmp_sol.push_back(j);
                else
                    tmp_sol.insert(tmp_sol.begin() + random(0, tmp_sol.size() - 1), j);
        }
        sol.push_back(tmp_sol);
    }
    return sol;
}
v2d<int> search_algorithm::create_sol_AVERAGE_COST_ALLOCATION()
{
    v2d<int> sol;
    v1d<int> max_resource(num_K_resource, -1);
    v2d<double> d_jk(num_J_service, v1d<double>(num_K_resource, -1.0));
    v1d<double> demand_resource_j(num_J_service, 0.0);
    v1d<int> C_k(num_K_resource, 0);

    for (int k = 0; k < num_K_resource; k++)
        for (int j = 0; j < num_J_service; j++)
            if (J_service_jk[j][k] > max_resource[k])
                max_resource[k] = J_service_jk[j][k];

    for (int k = 0; k < num_K_resource; k++)
        for (int i = 0; i < num_I_interface; i++)
            C_k[k] += c_ik[i][k] * I_interface_ik[i][k];

    for (int j = 0; j < num_J_service; j++)
        for (int k = 0; k < num_K_resource; k++)
        {
            d_jk[j][k] = ((double)J_service_jk[j][k] / max_resource[k]);
            demand_resource_j[j] += d_jk[j][k] * C_k[k];
        }

    v1d<int> tmp_sol;
    tmp_sol.push_back(0);
    for (int j = 1; j < num_J_service; j++)
    {
        // index1 => the last bigger than itself
        // index2 => the first smaller than itself
        int index1 = -1, index2 = 0;
        for (int i = 0; i < tmp_sol.size(); i++)
        {
            // the order is from big to small
            if (demand_resource_j[j] > demand_resource_j[tmp_sol[i]])
                index1 = i;
            else if (demand_resource_j[j] < demand_resource_j[tmp_sol[i]])
            {
                index2 = i;
                tmp_sol.insert(tmp_sol.begin() + random(index1 + 1, index2), j);
                break;
            }
        }
        // if the above is not execute, either it is bigger than all or all of them are the same
        if (j == tmp_sol.size())
            if (demand_resource_j[j] >= demand_resource_j[tmp_sol.back()])
                tmp_sol.push_back(j);
            else
                tmp_sol.insert(tmp_sol.begin() + random(0, tmp_sol.size() - 1), j);
    }
    sol.assign(num_K_resource, tmp_sol);

    return sol;
}
int search_algorithm::evaluation_rule_based(v2d<int> sol)
{
    v3d<int> x_ijk(num_I_interface, v2d<int>(num_J_service, v1d<int>(num_K_resource, 0)));
    v2d<int> b_ik = I_interface_ik; // interfaces capacities b
    v2d<int> A_ij(num_I_interface, v1d<int>(num_J_service, 0));
    int total_cost = 0;

    for (int k = 0; k < num_K_resource; k++)
        for (int j = 0; j < num_J_service; j++)
        {
            int d_jk = sol[k][j];                       // services number
            int service_demand = J_service_jk[d_jk][k]; // the resource which service demand
            int non_split_cost = -1;
            int split_cost = -1;

            v1d<int> split_index;
            int non_split_index = -1;
            int min_c_k = INT_MAX;
            int num_connot_bunden = 0;
            for (int i = 0; i < num_I_interface; i++)
            {
                // if number of bunden > service need
                if (b_ik[i][k] >= service_demand)
                {
                    // if is smaller than the min
                    if (min_c_k >= c_ik[i][k])
                    {
                        min_c_k = c_ik[i][k];
                        non_split_index = i;
                    }
                }
                else if (b_ik[i][k] != 0)
                    num_connot_bunden++;
            }
            if (non_split_index != -1) // can bunden without split
                non_split_cost = min_c_k * service_demand;

            if (non_split_index == -1 || num_connot_bunden > 2)
            {
                v1d<int> tmp_c_k; // resource k cost tmp
                for (int i = 0; i < num_I_interface; i++)
                    tmp_c_k.push_back(c_ik[i][k]);

                while (service_demand > 0)
                {
                    split_index.push_back(-1);
                    int min_c_k = tmp_c_k[0];

                    for (int i = 0; i < num_I_interface; i++)
                    {
                        // if is smaller than the min
                        if (min_c_k >= tmp_c_k[i])
                        {
                            min_c_k = tmp_c_k[i];
                            split_index.back() = i;
                        }
                    }
                    if (service_demand > b_ik[split_index.back()][k])
                    {
                        if (split_cost == -1)
                            split_cost = c_ik[split_index.back()][k] * b_ik[split_index.back()][k];
                        else
                            split_cost += c_ik[split_index.back()][k] * b_ik[split_index.back()][k];
                        service_demand -= b_ik[split_index.back()][k];
                        tmp_c_k[split_index.back()] = INT_MAX;
                    }
                    else
                    {
                        split_cost += c_ik[split_index.back()][k] * service_demand;
                        service_demand = 0;
                    }
                }
            }

            if (non_split_cost != -1 && split_cost != -1)
                if (non_split_cost < split_cost)
                {
                    total_cost += non_split_cost;
                    A_ij[non_split_index][j] = 1;
                    b_ik[non_split_index][k] -= service_demand;
                }
                else
                {
                    total_cost += split_cost;

                    service_demand = J_service_jk[d_jk][k];
                    for (int i = 0; i < split_index.size() - 1; i++)
                    {
                        service_demand -= b_ik[split_index[i]][k];
                        A_ij[split_index[i]][j] = 1;
                        b_ik[split_index[i]][k] = 0;
                    }
                    A_ij[split_index.back()][j] = 1;
                    b_ik[split_index.back()][k] -= service_demand;
                }
            else if (non_split_cost == -1)
            {
                total_cost += split_cost;

                service_demand = J_service_jk[d_jk][k];
                for (int i = 0; i < split_index.size() - 1; i++)
                {
                    service_demand -= b_ik[split_index[i]][k];
                    A_ij[split_index[i]][j] = 1;
                    b_ik[split_index[i]][k] = 0;
                }
                A_ij[split_index.back()][j] = 1;
                b_ik[split_index.back()][k] -= service_demand;
            }
            else if (split_cost == -1)
            {
                total_cost += non_split_cost;
                A_ij[non_split_index][j] = 1;
                b_ik[non_split_index][k] -= service_demand;
            }
            else
                cout << "all is -1..." << endl;
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
        total_cost += 100 * A_i;
    }

    now_evaluation_time++;
    if (now_evaluation_time % per_evaluation_time == 0 && now_evaluation_time != 0)
        cout << now_evaluation_time << "," << best_objectvalue << endl;
    if (now_evaluation_time == 1)
        cout << now_evaluation_time << "," << total_cost << endl;

    return total_cost;
}