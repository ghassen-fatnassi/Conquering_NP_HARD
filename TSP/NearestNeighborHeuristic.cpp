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

int closest_unvisited(const vector<Point> &cities, const vector<bool> &visited, int point_index, const vector<vector<double>> &weights)
{
    int minor = point_index;
    double minor_cost = INT_MAX;
    for (int i = 0; i < cities.size(); i++)
    {
        if (i != point_index && !visited[i])
        {
            double curr_cost = travelCost(cities[i], cities[point_index], weights[point_index][i]);
            if (curr_cost < minor_cost)
            {
                minor = i;
                minor_cost = curr_cost;
            }
        }
    }
    return minor;
}

pair<vector<int>, double> NNH(int n, vector<Point> &cities, const vector<vector<double>> &weights)
{
    vector<bool> visited(n, false);
    vector<int> path;
    double total_cost = 0;

    int curr = 0;
    path.push_back(curr);
    visited[curr] = true;

    for (int i = 0; i < n - 1; i++)
    {
        int curr_closest = closest_unvisited(cities, visited, curr, weights);
        visited[curr_closest] = true;
        path.push_back(curr_closest);
        total_cost += travelCost(cities[curr], cities[curr_closest], weights[curr][curr_closest]);
        curr = curr_closest;
    }
    path.push_back(0);
    total_cost += travelCost(cities[path[n - 1]], cities[path[0]], weights[path[n - 1]][path[0]]);

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
                weights[j][i] = weight; // we are just ensuring the additional cost of going from B to A is the same as going from A to B.
            }
        }
    }
    return weights;
}

void testNNH(int num_tests)
{
    for (int i = 0; i < num_tests; i++)
    {
        int n = rand() % 10 + 3;
        vector<Point> cities = generateRandomInput(n);
        vector<vector<double>> weights = generateRandomWeights(n);

        auto result = NNH(n, cities, weights);

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
    testNNH(num_tests);
    return 0;
}
