#include <bits/stdc++.h>
using namespace std;

#define ll long long
#define FASTIO  \
    cin.tie(0); \
    ios_base::sync_with_stdio(false);
#define TC while (t--)
const int MOD = 1e9 + 7;

// I structured this file based on my understanding of GeneticAlgorithm , hopefully it works when i'm finished coding it ;
// the process goes like this :
/*  1) intialization of the population , in our case a population is a set of possible paths , each path is an individual
    2) scoring and seleciton :
       as in natural selection , the environment forces the wekest individuals to die and strongest to continue :
        -environment : fitnessScore
        -dying and surviving : keeping paths in our set of paths or excluding them , we will use a selection process for this , it should be randomized and also favor the elite of the population ,in shrot it should be a fair tradeoff between exploration and exploitation
        **selection process** =>tournament selection(selecting the best individuals in a subset will keep the best in worst genes so we good :D) +elitism(to not lose good path candidates) +random immigration(introducing new elements to the population will lead to better exploration)
        => in short : -tournament selection => makes our decisions flexible without opting for bad decisions hoping randomness will fix it :D
                      -elitism => exploitation ++
                      -random immigrants => exploration ++
    3) crossover : this is where the magic happens ,

*/

struct Point
{
    double x, y;
};

double distance(const Point &a, const Point &b)
{
    return sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2));
}

double travelCost(const Point &a, const Point &b, double weight)
{
    double dist = distance(a, b);
    return dist + weight;
}

pair<vector<int>, double> Evolve(int n, vector<Point> &cities, const vector<vector<double>> &weights)
{
    vector<int> path;
    double total_cost = 0;

    return {path, total_cost};
}

vector<double> getFitnessDist(vector<Point> &cities, vector<vector<int>> paths, vector<vector<int>> &weights)
{
    vector<double> dist;
    vector<double> reverse_dist;
    double maxx = -MAXFLOAT;
    double sum = 0;
    for (int i = 0; i < paths.size(); i++)
    {
        double curr = pathCost(cities, paths[i], weights);
        if (maxx < curr)
        {
            maxx = curr;
        }
        reverse_dist.push_back(curr);
    }
    for (int i = 0; i < paths.size(); i++)
    {
        dist.push_back(maxx - reverse_dist[i]);
        sum += dist[i];
    }
    vector<double> prob_dist;
    for (int i = 0; i < paths.size(); i++)
    {
        prob_dist.push_back(dist[i] / sum);
    }
    return prob_dist;
}

double pathCost(vector<Point> &cities, vector<int> path, vector<vector<int>> &weights)
{
    double cost = 0;
    for (int i = 0; i < cities.size(); i++)
    {
        cost += travelCost(cities[path[i]], cities[path[i + 1]], weights[path[i]][path[i + 1]]);
    }
    return cost;
}

vector<vector<int>> starterPack(int n, int pop_size)
{
    vector<int> path;
    set<vector<int>> paths;
    for (int i = 0; i < n; i++)
    {
        path.push_back(i);
    }
    random_device rd;
    mt19937 rng(rd()); // rd is the seed and mt*** is the seeded generator , it's like plugging rd into  a device , rd control the randomness coming out of the device
    for (int i = 0; i < pop_size; i++)
    {
        shuffle(path.begin(), path.end(), rng);
        paths.insert(path);
    }
    vector<vector<int>> final_paths;
    for (auto elm : paths)
    {
        final_paths.push_back(elm);
    }
    return final_paths;
}

vector<Point> generateRandomInput(int n)
{
    vector<Point> cities;
    random_device rd;
    mt19937 gen(rd());
    set<pair<double, double>> f;
    double left = -100, right = 100;
    uniform_real_distribution<double> dis(left, right);
    while (f.size() < n)
    {
        double x = dis(gen);
        double y = dis(gen);
        f.insert({x, y});
    }
    for (auto &el : f)
    {
        Point p;
        p.x = el.first;
        p.y = el.second;
        cities.push_back(p);
    }

    return cities;
}

vector<vector<double>> generateRandomWeights(int n)
{
    vector<vector<double>> weights(n, vector<double>(n, 0));
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<double> dis(0, 100.0);

    for (int i = 0; i < n; i++)
    {
        for (int j = i + 1; j < n; j++)
        {
            if (i != j)
            {
                double weight = dis(gen);
                weights[i][j] = weight;
                weights[j][i] = weight; // we are just ensuring the additional cost of going from B to A is the same as going from A to B.
            }
        }
    }
    return weights;
}

void testEvolve(int num_tests)
{
    for (int i = 0; i < num_tests; i++)
    {
        int n = rand() % 10 + 3;
        vector<Point> cities = generateRandomInput(n);
        vector<vector<double>> weights = generateRandomWeights(n);

        auto result = Evolve(n, cities, weights);

        cout << "Test " << (i + 1) << ": \n";
        cout << "Number of cities: " << n << "\n";
        cout << "Path: ";
        for (int idx : result.first)
        {
            cout << idx << " ";
        }
        cout << "\nTotal cost: " << result.second << "\n\n";
    }
};

int main()
{
    FASTIO
    int num_tests = 10;
    testEvolve(num_tests);
    return 0;
}
