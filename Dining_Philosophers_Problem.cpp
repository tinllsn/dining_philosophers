#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <vector>

const int numPhilosophers = 5;


std::mutex forks[numPhilosophers];
std::condition_variable cv[numPhilosophers];


void pickUpForks(int philosopher, std::mutex& leftFork, std::mutex& rightFork) {
    std::unique_lock<std::mutex> leftLock(leftFork);
    std::unique_lock<std::mutex> rightLock(rightFork);


    cv[philosopher].wait(leftLock, [] { return true; });
    cv[philosopher].wait(rightLock, [] { return true; });
}


void putDownForks(int philosopher, std::mutex& leftFork, std::mutex& rightFork) {
    cv[philosopher].notify_one(); // Notify waiting philosophers
    cv[philosopher].notify_one(); // Notify waiting philosophers
}

// Philosopher thread function
void philosopher(int id) {
    int leftFork = id;
    int rightFork = (id + 1) % numPhilosophers;

    while (true) {
        // Thinking
        std::cout << "Philosopher " << id << " is thinking.\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(1000)); // Simulate thinking

        // Pick up forks
        pickUpForks(id, forks[leftFork], forks[rightFork]);
        std::cout << "Philosopher " << id << " is eating.\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(1000)); // Simulate eating

        // Put down forks
        putDownForks(id, forks[leftFork], forks[rightFork]);
    }
}

int main() {
    std::vector<std::thread> philosophers;

    // Create philosopher threads
    for (int i = 0; i < numPhilosophers; ++i) {
        philosophers.emplace_back(philosopher, i);
    }

    // Join threads
    for (auto& t : philosophers) {
        t.join();
    }

    return 0;
}
