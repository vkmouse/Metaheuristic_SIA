#include <iostream>
#include <vector>

using std::cout;
using std::endl;
using std::vector;

template <typename T>
using v1d = vector<T>;
template <typename T>
using v2d = vector<vector<T>>;

int random(int min, int max);
double random(double min, double max);

int main(int argc, char **argv)
{
    if (argc < 8)
        exit(0);
    int num_I_interface = atoi(argv[1]);
    int num_J_service = atoi(argv[2]);
    int num_K_resource = atoi(argv[3]);
    int min_resource_each = atoi(argv[4]);
    int max_resource_each = atoi(argv[5]);
    int min_resource_cost = atoi(argv[6]);
    int max_resource_cost = atoi(argv[7]);

    v2d<int> I_interface_ik(num_I_interface, v1d<int>(num_K_resource, -1));
    v2d<int> J_service_jk(num_J_service, v1d<int>(num_K_resource, -1));
    v2d<int> c_ik(num_I_interface, v1d<int>(num_K_resource, -1));

    // interface
    for (int i = 0; i < num_I_interface; i++)
        for (int k = 0; k < num_K_resource; k++)
            I_interface_ik[i][k] = random(min_resource_each, max_resource_each);

    // count total resource
    v1d<int> total_resource;
    for (int k = 0; k < num_K_resource; k++)
    {
        int tmp = 0;
        for (int i = 0; i < num_I_interface; i++)
            tmp += I_interface_ik[i][k];
        total_resource.push_back(tmp);
    }

    // service
    for (int j = 0; j < num_J_service; j++)
        for (int k = 0; k < num_K_resource; k++)
        {
            J_service_jk[j][k] = random(0, total_resource[k] / (num_J_service - j));
            total_resource[k] -= J_service_jk[j][k];
        }

    // cost
    for (int i = 0; i < num_I_interface; i++)
        for (int k = 0; k < num_K_resource; k++)
            c_ik[i][k] = random(min_resource_cost, max_resource_cost);

    // output
    cout << "num_K_resource = " << num_K_resource << endl;
    cout << "num_I_interface = " << num_I_interface << endl;
    cout << "num_J_service = " << num_J_service << endl;

    for (int i = 0; i < num_I_interface; i++)
    {
        for (int k = 0; k < num_K_resource; k++)
            cout << I_interface_ik[i][k] << "\t";
        cout << endl;
    }

    for (int j = 0; j < num_J_service; j++)
    {
        for (int k = 0; k < num_K_resource; k++)
            cout << J_service_jk[j][k] << "\t";
        cout << endl;
    }

    for (int i = 0; i < num_I_interface; i++)
    {
        for (int k = 0; k < num_K_resource; k++)
            cout << c_ik[i][k] << "\t";
        cout << endl;
    }

    return 0;
}
int random(int min, int max)
{
    double f = static_cast<double>(rand()) / RAND_MAX;
    while ((int)f == 1)
        f = static_cast<double>(rand()) / RAND_MAX;
    f = f * (max - min + 1) + min;
    return f;
}
double random(double min, double max)
{
    double f = static_cast<double>(rand()) / RAND_MAX;
    f = f * (max - min) + min;
    return f;
}
