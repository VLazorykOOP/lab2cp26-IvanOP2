#include <iostream>
#include <thread>
#include <chrono>
#include <mutex>
#include <random>

using namespace std;


const int WIDTH = 50;
const int HEIGHT = 20;

const int V = 1;          
const int DELAY_MS = 500;    
const int N_SECONDS = 3;      

mutex printMutex;


void normalRabbit() {
    int x = 10, y = 10;
    int dx = 1, dy = 1;

    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> dir(-1, 1);

    auto nextChange = chrono::steady_clock::now() + chrono::seconds(N_SECONDS);

    while (true) {
        if (chrono::steady_clock::now() >= nextChange) {
            do {
                dx = dir(gen);
                dy = dir(gen);
            } while (dx == 0 && dy == 0);

            nextChange = chrono::steady_clock::now() + chrono::seconds(N_SECONDS);

            lock_guard<mutex> lock(printMutex);
            cout << "[Normal rabbit] changed direction\n";
        }

        x += dx * V;
        y += dy * V;

        if (x <= 0 || x >= WIDTH) dx *= -1;
        if (y <= 0 || y >= HEIGHT) dy *= -1;

        {
            lock_guard<mutex> lock(printMutex);
            cout << "[Normal rabbit] X=" << x << " Y=" << y << endl;
        }

        this_thread::sleep_for(chrono::milliseconds(DELAY_MS));
    }
}

void albinoRabbit() {
    int x = 0;
    int y = 5;
    int dx = 1;

    while (true) {
        x += dx * V;

        if (x <= 0 || x >= WIDTH)
            dx *= -1;

        {
            lock_guard<mutex> lock(printMutex);
            cout << "[Albino rabbit] X=" << x << " Y=" << y << endl;
        }

        this_thread::sleep_for(chrono::milliseconds(DELAY_MS));
    }
}

int main() {
    thread t1(normalRabbit);
    thread t2(albinoRabbit);

    t1.join();
    t2.join();

    return 0;
}
