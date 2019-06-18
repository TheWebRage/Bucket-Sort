#include <iostream>
#include <random>
#include <chrono>
#include <thread>

using std::cout;
using std::cin;
using std::endl;
using std::thread;

unsigned long* arr{ nullptr };
const unsigned int ARR_SIZE = 100000000;
const unsigned int NUM_THREADS = 4;

void loadRandomValues(int threadID) {
	std::random_device rd;  //Will be used to obtain a seed for the random number engine
	std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
	std::uniform_int_distribution<> dis(1, 100);
	//std::uniform_int_distribution<> dis(1, UINT32_MAX);
	//cout << "I am thread " << threadID << " ready to generate random numbers" << endl;
	int startIndex = ARR_SIZE / NUM_THREADS * threadID;
	int endIndex = ARR_SIZE / NUM_THREADS * (threadID + 1);

	for (int i = startIndex; i < endIndex; i++) {
		arr[i] = dis(gen);
	}
}

void countSixes() {
	int count{ 0 };
	for (int i = 0; i < ARR_SIZE; i++) {
		if (arr[i] == 6) {
			count++;
		}
	}
	cout << "There were " << count << " sixes" << endl;
}


int main() {
	arr = new unsigned long[ARR_SIZE];
	thread* myThreads = new thread[NUM_THREADS]; // myThreads simply tracks threads, they are NOT threads themselves

	auto t1 = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < NUM_THREADS; i++) {
		myThreads[i] = thread(loadRandomValues, i); // The ith thread runs its own version of loadRandomValues()
	}
	for (int i = 0; i < NUM_THREADS; i++) {
		myThreads[i].join();
	}
	//loadRandomValues();
	auto t2 = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double, std::milli> fp_ms = t2 - t1;
	cout << "Random number generation took " << fp_ms.count() << " milliseconds " << endl;
	
	t1 = std::chrono::high_resolution_clock::now();
	countSixes();
	t2 = std::chrono::high_resolution_clock::now();
	fp_ms = t2 - t1;
	cout << "Counting sixes took " << fp_ms.count() << " milliseconds " << endl;

	return 0;
	cin.get();
}