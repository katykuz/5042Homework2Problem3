#include <iostream>
#include <sstream>
#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>

std::mutex m;
std::condition_variable cV;

void threadMethod1(int &ID, int &count1, int &mID);

int main() {

   //initialize the main ID
    int ID = 1;

    //initialize method id for each thread
    int m1ID = 1;
    int m2ID = 2;
    int m3ID = 3;

    //initialize counts for each thread
    int count1 = 0;
    int count2 = 0;
    int count3 = 0;

    //start each thread
    std::thread th1(threadMethod1, std::ref(ID), std::ref(count1), std::ref(m1ID));

    std::thread th2(threadMethod1, std::ref(ID), std::ref(count2), std::ref
    (m2ID));

    std::thread th3(threadMethod1, std::ref(ID), std::ref(count3), std::ref
    (m3ID));

    //join threads
    th1.join();
    th2.join();
    th3.join();

    return 0;
}

void threadMethod1(int &ID, int &count, int &mID)
{
    //declare a unique lock variable with mutex
    std::unique_lock<std::mutex> lk(m);

    while (count < 2) {
        if (mID == ID) {

            std::printf("Thead ");
            std::printf("%d", mID);
            std::printf("'s turn!\n");

            //increment ID and count
            ID++;
            count++;

            //reset the ID if it gets to 4
            if (ID == 4) {

                ID = 1;
            }

            //if count is 2 then terminate the thread
            if (count == 2) {

                std::printf("Thead ");
                std::printf("%d", mID);
                std::printf(" completed.\n");

            }

            //notify other threads to unlock
            cV.notify_all();
        }

        else {

            std::printf("Not thead ");
            std::printf("%d", mID);
            std::printf("'s turn!\n");

            //lock thread to wait until notified
            cV.wait(lk);
        }
    }
}