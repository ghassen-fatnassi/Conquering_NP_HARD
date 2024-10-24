#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>

struct Job {
    int id;
    int processingTime;
    
    Job(int id, int processingTime) : id(id), processingTime(processingTime) {}
};

void printSchedule(const std::vector<Job>& jobs, const std::string& strategy) {
    std::cout << strategy << " Schedule:\n";
    int currentTime = 0;
    for (const auto& job : jobs) {
        currentTime += job.processingTime;
        std::cout << "Job " << job.id << " (Processing Time: " << job.processingTime << ") -> Completed at: " << currentTime << "\n";
    }
    std::cout << "\n";
}

std::vector<Job> greedySchedule(std::vector<Job> jobs) {
    std::sort(jobs.begin(), jobs.end(), [](const Job& a, const Job& b) {
        return a.processingTime < b.processingTime;
    });
    return jobs;
}

std::vector<Job> fifoSchedule(const std::vector<Job>& jobs) {
    return jobs;
}

int main() {
    std::vector<Job> jobs = {
        Job(1, 10),
        Job(2, 5),
        Job(3, 8),
        Job(4, 2),
        Job(5, 7)
    };

    std::vector<Job> fifoJobs = fifoSchedule(jobs);
    printSchedule(fifoJobs, "FIFO");

    std::vector<Job> greedyJobs = greedySchedule(jobs);
    printSchedule(greedyJobs, "Greedy");

    return 0;
}
