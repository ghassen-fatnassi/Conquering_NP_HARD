#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>

using namespace std;

int calculateDistance(const vector<int>& path, const vector<vector<int>>& distMatrix) {
    int distance = 0;
    for (size_t i = 0; i < path.size() - 1; ++i) {
        distance += distMatrix[path[i]][path[i + 1]];
    }
    distance += distMatrix[path.back()][path[0]]; // Return to start
    return distance;
}

pair<vector<int>, int> tsp(const vector<vector<int>>& distMatrix) {
    int n = distMatrix.size();
    vector<int> cities(n);
    for (int i = 0; i < n; ++i) cities[i] = i;

    int minDist = INT_MAX;
    vector<int> bestPath;

    do {
        int currentDist = calculateDistance(cities, distMatrix);
        if (currentDist < minDist) {
            minDist = currentDist;
            bestPath = cities;
        }
    } while (next_permutation(cities.begin() + 1, cities.end())); // Fix start city

    return {bestPath, minDist};
}

int main() {
    vector<vector<int>> distanceMatrix = {
        {0, 10, 15, 20},
        {10, 0, 35, 25},
        {15, 35, 0, 30},
        {20, 25, 30, 0}
    };

    auto [bestPath, minDistance] = tsp(distanceMatrix);

    cout << "Best route: ";
    for (int city : bestPath) {
        cout << city << " ";
    }
    cout << "\nMinimum distance: " << minDistance << endl;

    return 0;
}
