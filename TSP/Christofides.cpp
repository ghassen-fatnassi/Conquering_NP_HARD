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

pair<vector<int>, double> Christofides(int n, vector<Point> &cities, const vector<vector<double>> &weights)
{
    vector<int> path;
    double total_cost = 0;

    return {path, total_cost};
}

vector<Point> generateRandomInput(int n)
{
    vector<Point> cities;
    random_device rd;
    mt19937 gen(rd());
    set<pair<double, double>> f;
    double left = -1000, right = 1000;
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

void testChristofides(int num_tests)
{
    for (int i = 0; i < num_tests; i++)
    {
        int n = rand() % 10 + 3;
        vector<Point> cities = generateRandomInput(n);
        vector<vector<double>> weights = generateRandomWeights(n);

        auto result = Christofides(n, cities, weights);

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
    int num_tests = 10;
    testChristofides(num_tests);
    return 0;
}
