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

pair<vector<int>, double> greedyHeuristic(int n, vector<Point> &cities, const vector<vector<double>> &weights)
{
    // To be implemented with a greedy approach based on the weighted cost
    vector<bool> visited(n, false);
    vector<int> path;
    double total_cost = 0;

    return {path, total_cost};
}

vector<Point> generateRandomInput(int n)
{
    vector<Point> cities(n);
    random_device rd;
    mt19937 gen(rd());

    double mean = 0, std_dev = 10;
    normal_distribution<double> dis(mean, std_dev);
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
        for (int j = 0; j < n; j++)
        {
            if (i != j)
            {
                weights[i][j] = dis(gen);
            }
        }
    }
    return weights;
}

void testGreedyHeuristic(int num_tests)
{
    for (int i = 0; i < num_tests; i++)
    {
        int n = rand() % 10 + 3;
        vector<Point> cities = generateRandomInput(n);
        vector<vector<double>> weights = generateRandomWeights(n);

        auto result = greedyHeuristic(n, cities, weights);

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
    testGreedyHeuristic(num_tests);
    return 0;
}
