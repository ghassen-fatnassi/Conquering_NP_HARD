#include <bits/stdc++.h>
using namespace std;

#define ll long long
#define FASTIO cin.tie(0); ios_base::sync_with_stdio(false);
#define TC while (t--)
const int MOD = 1e9 + 7;

struct Point {
    double x, y;
};

void make_set(int v, vector<int> &parent, vector<int> &rank) {
    parent[v] = v;
    rank[v] = 0;
}

int find_set(int v, vector<int> &parent) {
    if (v == parent[v])
        return v;
    return parent[v] = find_set(parent[v], parent);
}

void union_sets(int a, int b, vector<int> &parent, vector<int> &rank) {
    a = find_set(a, parent);
    b = find_set(b, parent);
    if (a != b) {
        if (rank[b] > rank[a]) {
            swap(a, b);
        }
        parent[b] = a;
        if (rank[a] == rank[b]) {
            rank[a]++;
        }
    }
}

double distance(const Point &a, const Point &b) {
    return sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2));
}

double travelCost(const Point &a, const Point &b, double weight) {
    double dist = distance(a, b);
    return dist + weight;
}

vector<pair<int, int>> getMST(int n, const vector<Point> &cities, const vector<vector<double>> &weights, vector<int> links_per_index) {
    vector<pair<int, int>> mst;
    vector<int> parent(n), rank(n);
    set<tuple<double, int, int>> edges;

    for (int i = 0; i < n; i++) {
        make_set(i, parent, rank);
    }

    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            edges.insert({travelCost(cities[i], cities[j], weights[i][j]), i, j});
        }
    }

    for (auto &[w, u, v] : edges) {
        if (find_set(u, parent) != find_set(v, parent)) {
            mst.push_back({u, v});
            union_sets(u, v, parent, rank);
            links_per_index[u]++;
            links_per_index[v]++;
        }
    }
    return mst;
}

void getDivergent(vector<int> &divergents, vector<int> &links_per_index) {
    for (int i = 0; i < links_per_index.size(); i++) {
        if (links_per_index[i] % 2 != 0) {
            divergents.push_back(i);
        }
    }
}

vector<pair<int, int>> matcher(const vector<int> &divergents, vector<Point> &cities, const vector<vector<double>> &weights) {
    vector<pair<int, int>> matching;
    vector<int> matched(divergents.size());

    for (int i = 0; i < divergents.size(); i++) {
        double min_cost = MAXFLOAT;
        int min_index = -1;

        if (matched[i]) {
            continue;
        }
        for (int j = i + 1; j < divergents.size(); j++) {
            double curr_cost = travelCost(cities[i], cities[j], weights[i][j]);

            if (!matched[j] && curr_cost < min_cost) {
                min_cost = curr_cost;
                min_index = j;
            }
        }
        if (min_index != -1) {
            matching.push_back({divergents[i], divergents[min_index]});
            matched[min_index] = 1;
            matched[i] = 1;
        }
    }
    return matching;
}

void EulerianCircuit(int n, vector<pair<int, int>> &multiGraph, const vector<pair<int, int>> &mst, const vector<pair<int, int>> &matching) {
    for (int i = 0; i < mst.size(); i++) {
        multiGraph.push_back(mst[i]);
    }
    for (int i = 0; i < matching.size(); i++) {
        multiGraph.push_back(matching[i]);
    }
}

void dfs(vector<vector<int>> &Graph, vector<int> &path, vector<int> &vis, int curr) {
    path.push_back(curr);
    vis[curr] = 1;

    for (auto elm : Graph[curr]) {
        if (!vis[elm]) {
            dfs(Graph, path, vis, elm);
        }
    }
}

double calculateTourCost(const vector<int>& tour, const vector<Point>& cities, const vector<vector<double>>& weights) {
    double cost = 0;
    for (int i = 0; i < tour.size() - 1; i++) {
        cost += travelCost(cities[tour[i]], cities[tour[i + 1]], weights[tour[i]][tour[i + 1]]);
    }
    return cost;
}

vector<int> twoOptSwap(const vector<int>& tour, int i, int j) {
    vector<int> newTour = tour;
    reverse(newTour.begin() + i, newTour.begin() + j + 1);
    return newTour;
}

pair<vector<int>, double> simulatedAnnealing(
    const vector<int>& initialTour,
    const vector<Point>& cities,
    const vector<vector<double>>& weights,
    double initialTemp = 1000.0,
    double coolingRate = 0.995,
    int maxIterations = 10000,
    double minTemp = 0.01
) {
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<double> random(0.0, 1.0);
    
    vector<int> currentTour = initialTour;
    vector<int> bestTour = currentTour;
    double currentCost = calculateTourCost(currentTour, cities, weights);
    double bestCost = currentCost;
    double temperature = initialTemp;
    
    int n = cities.size();
    uniform_int_distribution<> segmentDist(1, n-2);
    
    while (temperature > minTemp && maxIterations-- > 0) {
        int i = segmentDist(gen);
        int j = segmentDist(gen);
        if (i > j) swap(i, j);
        
        vector<int> newTour = twoOptSwap(currentTour, i, j);
        double newCost = calculateTourCost(newTour, cities, weights);
        
        double deltaCost = newCost - currentCost;
        
        if (deltaCost < 0 || random(gen) < exp(-deltaCost / temperature)) {
            currentTour = newTour;
            currentCost = newCost;
            
            if (currentCost < bestCost) {
                bestTour = currentTour;
                bestCost = currentCost;
            }
        }
        
        temperature *= coolingRate;
    }
    
    return {bestTour, bestCost};
}

pair<vector<int>, double> Christofides(int n, vector<Point> &cities, const vector<vector<double>> &weights) {
    vector<int> path;
    vector<int> links_per_index(n);
    double total_cost = 0;

    vector<pair<int, int>> mst = getMST(n, cities, weights, links_per_index);
    vector<int> divergents;
    getDivergent(divergents, links_per_index);
    vector<pair<int, int>> matching = matcher(divergents, cities, weights);
    vector<pair<int, int>> multiGraph;
    vector<vector<int>> Graph(n);
    EulerianCircuit(n, multiGraph, mst, matching);
    for (auto elm : multiGraph) {
        Graph[elm.first].push_back(elm.second);
        Graph[elm.second].push_back(elm.first);
    }
    vector<int> vis(n);
    int curr = 0;
    dfs(Graph, path, vis, curr);
    path.push_back(0);
    for (int i = 0; i < n; i++) {
        total_cost += travelCost(cities[path[i]], cities[path[i + 1]], weights[path[i]][path[i + 1]]);
    }
    return {path, total_cost};
}

pair<vector<int>, double> ChristofidesWithAnnealing(int n, vector<Point>& cities, const vector<vector<double>>& weights) {
    auto [initialPath, initialCost] = Christofides(n, cities, weights);
    return simulatedAnnealing(initialPath, cities, weights);
}

vector<Point> generateRandomInput(int n) {
    vector<Point> cities;
    random_device rd;
    mt19937 gen(rd());
    set<pair<double, double>> f;
    double left = -100, right = 100;
    uniform_real_distribution<double> dis(left, right);
    while (f.size() < n) {
        double x = dis(gen);
        double y = dis(gen);
        f.insert({x, y});
    }
    for (auto &el : f) {
        Point p;
        p.x = el.first;
        p.y = el.second;
        cities.push_back(p);
    }
    return cities;
}

vector<vector<double>> generateRandomWeights(int n) {
    vector<vector<double>> weights(n, vector<double>(n, 0));
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<double> dis(0, 100.0);

    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            if (i != j) {
                double weight = dis(gen);
                weights[i][j] = weight;
                weights[j][i] = weight;
            }
        }
    }
    return weights;
}

void testChristofidesWithAnnealing(int num_tests) {
    for (int i = 0; i < num_tests; i++) {
        int n = rand() % 10 + 3;
        vector<Point> cities = generateRandomInput(n);
        vector<vector<double>> weights = generateRandomWeights(n);

        auto initial_result = Christofides(n, cities, weights);
        auto improved_result = ChristofidesWithAnnealing(n, cities, weights);

        cout << "Test " << (i + 1) << ": \n";
        cout << "Number of cities: " << n << "\n";
        cout << "Initial Christofides path: ";
        for (int idx : initial_result.first) {
            cout << idx << " ";
        }
        cout << "\nInitial cost: " << initial_result.second;
        
        cout << "\nImproved path after annealing: ";
        for (int idx : improved_result.first) {
            cout << idx << " ";
        }
        cout << "\nImproved cost: " << improved_result.second;
        cout << "\nImprovement: " << 
            ((initial_result.second - improved_result.second) / initial_result.second * 100) 
            << "%\n\n";
    }
}

int main() {
    FASTIO
    int num_tests = 10;
    testChristofidesWithAnnealing(num_tests);
    return 0;
}