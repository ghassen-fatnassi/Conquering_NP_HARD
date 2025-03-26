#include <bits/stdc++.h>
#include <omp.h>

using namespace std;

#define ll long long
#define FASTIO  \
    cin.tie(0); \
    ios_base::sync_with_stdio(false);
#define TC while (t--)
const int MOD = 1e9 + 7;
//hello o1

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

set<int> getEliteIndexes(vector<double> fitnessDist, int num_elites)
{
    set<pair<double, int>> ord;
    for (int i = 0; i < fitnessDist.size(); i++)
    {
        ord.insert({fitnessDist[i], i});
    }
    set<int> elite;
    for (auto elm : ord)
    {
        elite.insert(elm.second);
        num_elites--;
        if (!num_elites)
        {
            break;
        }
    }
    return elite;
}

vector<vector<int>> survivalToTheShortest(int tournament_size, int num_elites, vector<Point> &cities, vector<vector<int>> &paths, vector<vector<int>> &weights) // a selection process will be used here(tournament + elitism+rndom immigrant) to simulate natural selction
{
    int pop_size = paths.size();
    set<vector<int>> survivors;
    vector<double> fitnessDist = getFitnessDist(cities, paths, weights);
    // elite first:
    for (int i : getEliteIndexes(fitnessDist, num_elites))
    {
        survivors.insert(paths[i]);
    }
    // now the tournament:
    for (int i = 0; i < paths.size(); i += tournament_size)
    {
        int max_j;
        double max_fit = MAXFLOAT;
        for (int j = 0; i < tournament_size; j++)
        {
            double fit_j = fitnessDist[i + j];
            if (fit_j > max_fit)
            {
                max_fit = fit_j;
                max_j = j;
            }
        }
        survivors.insert(paths[i + max_j]);
    }
    // finally the random immigrants:
    /*insert some random paths to spice things up
    (just like NPC's in the game , except we have hope in these to become main charachters)*/
    random_device rd;
    mt19937 rng(rd() + 1);
    vector<int> random_path = paths[15];
    while (survivors.size() < pop_size)
    {
        shuffle(random_path.begin(), random_path.end(), rng);
        survivors.insert(random_path);
    }
    vector<vector<int>> vector_survivors;
    for (auto elm : survivors)
    {
        vector_survivors.push_back(elm);
    }
    return vector_survivors;
}

vector<double> getFitnessDist(vector<Point> &cities, vector<vector<int>> &paths, vector<vector<int>> &weights)
{
    vector<double> reverse_dist(paths.size());
    double maxx = -MAXFLOAT;

#pragma omp parallel for
    for (int i = 0; i < paths.size(); i++)
    {
        reverse_dist[i] = pathCost(cities, paths[i], weights);
    }

#pragma omp parallel for reduction(max : maxx)
    for (int i = 0; i < reverse_dist.size(); i++)
    {
        if (reverse_dist[i] > maxx)
        {
            maxx = reverse_dist[i];
        }
    }

    vector<double> dist(paths.size());
    double sum = 0;

#pragma omp parallel for reduction(+ : sum)
    for (int i = 0; i < paths.size(); i++)
    {
        dist[i] = maxx - reverse_dist[i];
        sum += dist[i];
    }

    vector<double> prob_dist(paths.size());
#pragma omp parallel for
    for (int i = 0; i < paths.size(); i++)
    {
        prob_dist[i] = dist[i] / sum;
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

vector<vector<int>> starterPack(int n, int pop_size) // this is the initial population (Adam & Eve)
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
