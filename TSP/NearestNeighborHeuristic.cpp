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

int closest_unvisited(const vector<Point> &cities, const vector<bool> &visited, int point_index)
{
    int minor = point_index; // it's the closest to itself anyway
    double minor_dist = INT_MAX;
    for (int i = 0; i < cities.size(); i++)
    {
        if (i != point_index)
        {
            int curr_dist = distance(cities[i], cities[point_index]);
            if (curr_dist < minor_dist && !visited[i])
            {
                minor = i;
                minor_dist = curr_dist;
            };
        }
    }
    return minor;
}

pair<vector<int>, double> NNH(int n, vector<Point> &cities)
{
    vector<bool> visited(n, false);
    vector<int> path;
    double total_dist = 0;

    int curr = 0;
    path.push_back(curr);
    visited[curr] = 1;

    for (auto elm : cities)
    {
        int curr_closest = closest_unvisited(cities, visited, curr);
        visited[curr_closest] = 1;
        path.push_back(curr_closest);
        curr = curr_closest;
    }
    for (int i = 0; i < n - 1; i++)
    {
        total_dist += distance(cities[path[i]], cities[path[i + 1]]);
    }
    return {path, total_dist};
}

vector<Point> generateRandomInput(int n)
{
    // used chatgpt to do this function :D
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

int main()
{
    FASTIO
    int n = 10;
    vector<Point> space = generateRandomInput(n);
    auto tour = NNH(n, space);
    auto path = tour.first;
    double cost = tour.second;

    return 0;
}
