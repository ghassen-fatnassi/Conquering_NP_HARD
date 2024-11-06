#include <bits/stdc++.h>
using namespace std;

#define ll long long
#define FASTIO  \
    cin.tie(0); \
    ios_base::sync_with_stdio(false);
#define TC while (t--)
const int MOD = 1e9 + 7;

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

pair<vector<int>, double> ACO(int n, vector<Point> &cities, const vector<vector<double>> &weights)
{
    const double EVAPORATION_RATE = 0.5;
    const double ALPHA = 1.0;
    const double BETA = 2.0;
    const int NUM_ANTS = 10;
    const int NUM_ITERATIONS = 100;

    vector<vector<double>> pheromone(n, vector<double>(n, 1.0));
    vector<int> path;
    double total_cost = numeric_limits<double>::max();

    for (int iteration = 0; iteration < NUM_ITERATIONS; iteration++)
    {
        vector<vector<int>> ant_paths(NUM_ANTS);
        vector<double> ant_costs(NUM_ANTS, 0.0);

        for (int ant = 0; ant < NUM_ANTS; ant++)
        {
            vector<bool> visited(n, false);
            int current = rand() % n;
            ant_paths[ant].push_back(current);
            visited[current] = true;

            for (int i = 1; i < n; i++)
            {
                double total_pheromone = 0.0;
                for (int j = 0; j < n; j++)
                {
                    if (!visited[j])
                    {
                        total_pheromone += pow(pheromone[current][j], ALPHA) * pow(1.0 / weights[current][j], BETA);
                    }
                }

                double random_value = static_cast<double>(rand()) / RAND_MAX;
                double cumulative = 0.0;
                for (int j = 0; j < n; j++)
                {
                    if (!visited[j])
                    {
                        cumulative += pow(pheromone[current][j], ALPHA) * pow(1.0 / weights[current][j], BETA) / total_pheromone;
                        if (cumulative >= random_value)
                        {
                            current = j;
                            ant_paths[ant].push_back(current);
                            visited[current] = true;
                            break;
                        }
                    }
                }
            }

            for (int i = 0; i < (int)ant_paths[ant].size() - 1; i++)
            {
                ant_costs[ant] += travelCost(cities[ant_paths[ant][i]], cities[ant_paths[ant][i + 1]], weights[ant_paths[ant][i]][ant_paths[ant][i + 1]]);
            }

            if (ant_costs[ant] < total_cost)
            {
                path = ant_paths[ant];
                total_cost = ant_costs[ant];
            }
        }

        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < n; j++)
            {
                pheromone[i][j] *= (1.0 - EVAPORATION_RATE);
            }
        }

        for (int ant = 0; ant < NUM_ANTS; ant++)
        {
            for (int i = 0; i < (int)ant_paths[ant].size() - 1; i++)
            {
                pheromone[ant_paths[ant][i]][ant_paths[ant][i + 1]] += 1.0 / ant_costs[ant];
            }
        }
    }

    return {path, total_cost};
}

vector<Point> generateRandomInput(int n)
{
    vector<Point> cities(n);
    random_device rd;
    mt19937 gen(rd());

    double left = -10, right = 10;
    uniform_real_distribution<double> dis(left, right);
    for (int i = 0; i < n; i++)
    {
        cities[i].x = dis(gen);
        cities[i].y = dis(gen);
    }

    return cities;
}

vector<vector<double>> generateRandomWeights(int n)
{
    vector<vector<double>> weights(n, vector<double>(n, 0));
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<double> dis(0.0, 10.0);

    for (int i = 0; i < n; i++)
    {
        for (int j = i + 1; j < n; j++)
        {
            if (i != j)
            {
                double weight = dis(gen);
                weights[i][j] = weight;
                weights[j][i] = weight;
            }
        }
    }
    return weights;
}

void testACO(int num_tests)
{
    for (int i = 0; i < num_tests; i++)
    {
        int n = rand() % 10 + 3;
        vector<Point> cities = generateRandomInput(n);
        vector<vector<double>> weights = generateRandomWeights(n);

        auto result = ACO(n, cities, weights);

        cout << "Test " << (i + 1) << ": \n";
        cout << "Number of cities: " << n << "\n";
        cout << "Path: ";
        for (int idx : result.first)
        {
            cout << idx << " ";
        }
        cout << "\nTotal cost: " << result.second << "\n\n";
    }
}

int main()
{
    FASTIO
    int num_tests = 5;
    testACO(num_tests);
    return 0;
}