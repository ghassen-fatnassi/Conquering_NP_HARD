#include <iostream>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <algorithm>

using namespace std;

const double ALPHA = 1.0;
const double BETA = 5.0;
const double EVAPORATION = 0.5;
const double Q = 100.0;
const int NUM_ANTS = 50;
const int MAX_ITERATIONS = 1000;

struct Ant
{
    vector<int> tour;
    vector<bool> visited;
    double tourLength;

    Ant(int numCities) : tour(numCities), visited(numCities, false), tourLength(0) {}
};

double calculateTourLength(const vector<int> &tour, const vector<vector<int>> &distMatrix)
{
    double totalDistance = 0;
    for (size_t i = 0; i < tour.size() - 1; ++i)
    {
        totalDistance += distMatrix[tour[i]][tour[i + 1]];
    }
    totalDistance += distMatrix[tour.back()][tour[0]];
    return totalDistance;
}

pair<vector<int>, double> antColonyOptimization(const vector<vector<int>> &distanceMatrix)
{
    int numCities = distanceMatrix.size();
    vector<vector<double>> pheromones(numCities, vector<double>(numCities, 1.0));

    vector<int> bestTour;
    double bestTourLength = numeric_limits<double>::infinity();

    srand(time(0));

    for (int iter = 0; iter < MAX_ITERATIONS; ++iter)
    {
        vector<Ant> ants(NUM_ANTS, Ant(numCities));

        for (Ant &ant : ants)
        {
            int startCity = rand() % numCities;
            ant.tour[0] = startCity;
            ant.visited[startCity] = true;

            for (int i = 1; i < numCities; ++i)
            {
                int currentCity = ant.tour[i - 1];
                vector<double> probabilities(numCities, 0.0);
                double totalProbability = 0.0;

                for (int nextCity = 0; nextCity < numCities; ++nextCity)
                {
                    if (!ant.visited[nextCity])
                    {
                        double pheromoneLevel = pow(pheromones[currentCity][nextCity], ALPHA);
                        double distanceFactor = pow(1.0 / distanceMatrix[currentCity][nextCity], BETA);
                        probabilities[nextCity] = pheromoneLevel * distanceFactor;
                        totalProbability += probabilities[nextCity];
                    }
                }

                double randomChoice = (double)rand() / RAND_MAX * totalProbability;
                double cumulativeProbability = 0.0;
                int selectedCity = -1;

                for (int nextCity = 0; nextCity < numCities; ++nextCity)
                {
                    if (!ant.visited[nextCity])
                    {
                        cumulativeProbability += probabilities[nextCity];
                        if (cumulativeProbability >= randomChoice)
                        {
                            selectedCity = nextCity;
                            break;
                        }
                    }
                }

                ant.tour[i] = selectedCity;
                ant.visited[selectedCity] = true;
            }

            ant.tourLength = calculateTourLength(ant.tour, distanceMatrix);

            if (ant.tourLength < bestTourLength)
            {
                bestTourLength = ant.tourLength;
                bestTour = ant.tour;
            }
        }

        for (int i = 0; i < numCities; ++i)
        {
            for (int j = 0; j < numCities; ++j)
            {
                pheromones[i][j] *= (1 - EVAPORATION);
            }
        }

        for (const Ant &ant : ants)
        {
            for (size_t i = 0; i < ant.tour.size() - 1; ++i)
            {
                int from = ant.tour[i];
                int to = ant.tour[i + 1];
                pheromones[from][to] += Q / ant.tourLength;
                pheromones[to][from] += Q / ant.tourLength;
            }
            int from = ant.tour.back();
            int to = ant.tour[0];
            pheromones[from][to] += Q / ant.tourLength;
            pheromones[to][from] += Q / ant.tourLength;
        }
    }

    return {bestTour, bestTourLength};
}

int main()
{
    vector<vector<int>> distanceMatrix = {
        {0, 10, 15, 20},
        {10, 0, 35, 25},
        {15, 35, 0, 30},
        {20, 25, 30, 0}};

    auto [bestTour, bestTourLength] = antColonyOptimization(distanceMatrix);

    for (int city : bestTour)
        cout << city << " ";
    cout << "\nMinimum distance: " << bestTourLength << endl;

    return 0;
}
