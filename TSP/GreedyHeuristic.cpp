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

void fill_edges(set<pair<double, pair<int, int>>> &edges, vector<Point> &cities, const vector<vector<double>> &weights)
{
    for (int i = 0; i < cities.size(); i++)
    {
        for (int j = i + 1; j < cities.size(); j++)
        {
            edges.insert({travelCost(cities[i], cities[j], weights[i][j]), {i, j}});
        }
    }
}

// ok so we use DSU here to optimize cycle detection where checking if the newly chosen edge creates a cycle (i'll put links about DSU in the ReadMe of the repo)
void make_set(int v, vector<int> &parent, vector<int> &rank)
{
    parent[v] = v;
    rank[v] = 0;
}

int find_set(int v, vector<int> &parent)
{
    if (v == parent[v])
        return v;
    return parent[v] = find_set(parent[v], parent);
}

void union_sets(int a, int b, vector<int> &parent, vector<int> &rank)
{
    a = find_set(a, parent);
    b = find_set(b, parent);
    if (a != b)
    {
        if (rank[b] > rank[a])
        {
            swap(a, b);
        }
        parent[b] = a;
        if (rank[a] == rank[b])
        {
            rank[a]++;
        }
    }
}
void dfs(vector<vector<int>> &connections, vector<bool> &vis, int cur, vector<int> &path)
{
    path.push_back(cur);
    vis[cur] = 1;
    for (int c : connections[cur])
    {
        if (!vis[c])
        {
            dfs(connections, vis, c, path);
        }
    }
}
pair<vector<int>, double> greedyHeuristic(int n, vector<Point> &cities, const vector<vector<double>> &weights)
{
    vector<int> path;
    vector<vector<int>> connections(n);
    vector<int> links_per_index(n);
    set<pair<double, pair<int, int>>> all_edges;
    vector<int> parent(n); // will use it in DSU optimization
    vector<int> rank(n);   // will use it in DSU optimization
    vector<bool> vis(n);
    double total_cost = 0;

    fill_edges(all_edges, cities, weights); // now we have sorted edges so we can get them in order and put them in the graph
    for (int i = 0; i < n; i++)
    {
        make_set(i, parent, rank);
    }

    for (auto elm : all_edges)
    {
        int a, b;
        a = elm.second.first;
        b = elm.second.second;
        double curr_cost = elm.first;
        if (find_set(a, parent) != find_set(b, parent) && links_per_index[a] < 2 && links_per_index[b] < 2)
        // if the 2 vertices are already connected  || or one of the nodes will form a 3rd degree connection
        {
            links_per_index[a]++;
            links_per_index[b]++;
            union_sets(a, b, parent, rank);
            total_cost += curr_cost;
            connections[a].push_back(b);
            connections[b].push_back(a);
        }
    }
    dfs(connections, vis, 0, path);
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
    int num_tests = 10;
    testGreedyHeuristic(num_tests);
    return 0;
}
