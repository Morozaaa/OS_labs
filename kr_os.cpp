#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <chrono>
#include <random>

class BankAccount {
private:
    double balance;
    std::mutex mtx;
    std::condition_variable cvar;

public:
    BankAccount(double _balance) : balance(_balance) {}

    void changeBalance(double amount) {
        std::unique_lock<std::mutex> lock(mtx);
        cvar.wait(lock, [this, amount]() { return balance + amount >= 0; });

        std::this_thread::sleep_for(std::chrono::milliseconds(50 + rand() % 51));

        balance += amount;
        std::cout << "Previous=" << balance - amount << ", operation " << (amount > 0 ? "+" : "-") << std::abs(amount) << ", current=" << balance << std::endl;
        lock.unlock();
        cvar.notify_all();
    }

    double getBalance() const {
        return balance;
    }
};

void performOperations(BankAccount& account, const std::vector<double>& operations) {
    for (double amount : operations) {
        bool success = false;
        int tryCount = 0;
        while (!success && tryCount < 3) {
            try {
                account.changeBalance(amount);
                success = true;
            }
            catch (const std::exception& e) {
                std::cerr << "Error: " << e.what() << std::endl;
                ++tryCount;
            }
        }
        if (success) {
            std::cerr << "Opreation failed" << std::endl;
        }
    }
}

int main() {
    srand(time(nullptr));

    BankAccount account(100.0);

    std::vector<double> v1 = { (double)(rand()%120), (double)((-1)*rand() % 120),(double)(rand() % 120) };
    std::vector<double> v2 = { (double)(rand() % 120),(double)((-1)*rand() % 120), (double)(rand() % 120) };

    std::thread thread1(performOperations, std::ref(account), v1);
    std::thread thread2(performOperations, std::ref(account), v2);

    thread1.join();
    thread2.join();

    std::cout << "Final balance " << account.getBalance() << std::endl;

    return 0;
}