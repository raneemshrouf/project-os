#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;

struct Process {
    int id;
    int arrivalTime;
    int cpuBurst;
    int originalBurst;
    int remainingBurst;
    int turnaroundTime;
    int waitingTime;
    int finishTime;
};


struct CompareBurst {
    bool operator()(Process const& p1, Process const& p2) {
        if (p1.remainingBurst == p2.remainingBurst) {
            return p1.arrivalTime > p2.arrivalTime;
        }
        return p1.remainingBurst > p2.remainingBurst;
    }
};

void calculateWaitingTime(vector<Process>& processes) {
    for (auto& p : processes) {
        p.waitingTime = p.finishTime - p.arrivalTime - p.originalBurst;
    }
}

void printProcesses(const vector<Process>& processes) {
    cout << "ID\tArrival\tBurst\tWaiting\tTurnaround\tFinish\n";
    for (const auto& process : processes) {
        cout << process.id << "\t" << process.arrivalTime << "\t"
            << process.originalBurst << "\t" << process.waitingTime << "\t"
            << process.turnaroundTime << "\t" << process.finishTime << endl;
    }
}

void printGanttChart(const vector<Process>& processes) {
    cout << "Gantt Chart:\n";
    for (const auto& process : processes) {
        cout << "[P" << process.id << "] ";
        for (int i = 0; i < process.originalBurst; ++i) {
            cout << "-";
        }
        cout << " ";
    }
    cout << "\n";
}

// Function to simulate FCFS scheduling
void FCFS(vector<Process>& processes) {
    sort(processes.begin(), processes.end(), [](const Process& a, const Process& b) {
        return a.arrivalTime < b.arrivalTime;
        });
    int currentTime = 0;
    vector<Process> completedProcesses;
    for (auto& p : processes) {
        if (currentTime < p.arrivalTime) {
            currentTime = p.arrivalTime;
        }
        p.finishTime = currentTime + p.cpuBurst;
        p.turnaroundTime = p.finishTime - p.arrivalTime;
        currentTime = p.finishTime;
        p.remainingBurst = 0;  // ááÍÝÇÙ Úáì ÊäÇÓÞ ÇáÍÞæá
        completedProcesses.push_back(p);
    }
    calculateWaitingTime(completedProcesses);
    cout << "FCFS Scheduling:\n";
    printProcesses(completedProcesses);
    printGanttChart(completedProcesses);
}

// Function to simulate SRT scheduling
void SRTF(vector<Process>& processes) {
    priority_queue<Process, vector<Process>, CompareBurst> pq;
    vector<Process> completedProcesses;
    int currentTime = 0, i = 0;

    while (i < processes.size() || !pq.empty()) {
        while (i < processes.size() && processes[i].arrivalTime <= currentTime) {
            pq.push(processes[i]);
            i++;
        }

        if (!pq.empty()) {
            Process current = pq.top();
            pq.pop();
            int nextTime = (i < processes.size()) ? processes[i].arrivalTime : currentTime + current.remainingBurst;
            int duration = min(current.remainingBurst, nextTime - currentTime);
            current.remainingBurst -= duration;
            currentTime += duration;

            if (current.remainingBurst == 0) {
                current.finishTime = currentTime;
                current.turnaroundTime = currentTime - current.arrivalTime;
                completedProcesses.push_back(current);
            }
            else {
                pq.push(current);
            }
        }
        else {
            currentTime = (i < processes.size()) ? processes[i].arrivalTime : currentTime;
        }
    }
    calculateWaitingTime(completedProcesses);
    cout << "SRTF Scheduling:\n";
    printProcesses(completedProcesses);
    printGanttChart(completedProcesses);
}

// Function to simulate RR scheduling
void RR(vector<Process>& processes, int quantum) {
    queue<Process> q;
    vector<Process> completedProcesses;
    int currentTime = 0;

    for (const auto& p : processes) {
        q.push(p);
    }

    while (!q.empty()) {
        Process current = q.front();
        q.pop();
        int duration = min(current.remainingBurst, quantum);
        current.remainingBurst -=

            duration;
        currentTime += duration;

        if (current.remainingBurst == 0) {
            current.finishTime = currentTime;
            current.turnaroundTime = currentTime - current.arrivalTime;
            completedProcesses.push_back(current);
        }
        else {
            q.push(current);
        }
    }
    calculateWaitingTime(completedProcesses);
    cout << "Round Robin Scheduling:\n";
    printProcesses(completedProcesses);
    printGanttChart(completedProcesses);
}

int main() {
    vector<Process> processes = {
        {1, 0, 8, 8, 8},
        {2, 1, 4, 4, 4},
        {3, 2, 9, 9, 9},
        {4, 3, 5, 5, 5}
    };

    FCFS(processes);
    SRTF(processes);
    RR(processes, 4);

    return 0;
}