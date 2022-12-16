// ConsoleApplication3.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>
#include <thread>
#include <chrono>

#include <windows.h>

#define R1 2 // number of rows in Matrix-1
#define C1 2 // number of columns in Matrix-1
#define R2 2 // number of rows in Matrix-2
#define C2 2 // number of columns in Matrix-2

void mulMat(int mat1[][C1], int mat2[][C2])
{
    int rslt[R1][C2];

    //std::cout << "Multiplication of given two matrices is:\n";

    for (int i = 0; i < R1; i++) {
        for (int j = 0; j < C2; j++) {
            rslt[i][j] = 0;

            for (int k = 0; k < R2; k++) {
                rslt[i][j] += mat1[i][k] * mat2[k][j];
            }

            //std::cout << rslt[i][j] << "\t";
        }

        //std::cout << std::endl;
    }
}


void proc_1(int core_index)
{
    // using namespace std::chrono_literals;
    // std::this_thread::sleep_for(5s);
    double i = 1;
    double j = 1;
    std::cout << "cur thread id: " << GetCurrentThreadId() << std::endl;

    unsigned long size;
    HANDLE curProc = GetCurrentProcess();
    GetSystemCpuSetInformation(nullptr, 0, &size, curProc, 0);
    std::unique_ptr<uint8_t[]> buffer(new uint8_t[size]);
    PSYSTEM_CPU_SET_INFORMATION cpuSets = reinterpret_cast<PSYSTEM_CPU_SET_INFORMATION>(buffer.get());
    GetSystemCpuSetInformation(cpuSets, size, &size, curProc, 0);
    unsigned long cores[] = { cpuSets[core_index].CpuSet.Id, cpuSets[4].CpuSet.Id, cpuSets[2].CpuSet.Id, cpuSets[5].CpuSet.Id };

    SetThreadSelectedCpuSets(GetCurrentThread(), cores, 4);

    int mat1[R1][C1] = { { 1, 1 },
                      { 2, 2 } };

    int mat2[R2][C2] = { { 1, 1 },
                         { 2, 2 } };

    auto time_s = std::chrono::high_resolution_clock::now();
    while (1) {
       // int rand_ms = rand() % 3 + 1;
       // std::this_thread::sleep_for(std::chrono::milliseconds(rand_ms));

        double i = 1;
        while (i < 1000) {
            mulMat(mat1, mat2);
            i++;
        }

        auto time_e = std::chrono::high_resolution_clock::now();
        if (std::chrono::duration_cast<std::chrono::minutes>(time_e - time_s) > std::chrono::minutes(2))
            break;
    }
}


int main()
{   /*
    int mat1[R1][C1] = { { 1, 1 },
                         { 2, 2 } };

    int mat2[R2][C2] = { { 1, 1 },
                         { 2, 2 } };

    if (C1 != R2) {
        std::cout << "The number of columns in Matrix-1  must "
            "be equal to the number of rows in "
            "Matrix-2"
            << std::endl;
        std::cout << "Please update MACROs according to your "
            "array dimension in #define section"
            << std::endl;

        exit(EXIT_FAILURE);
    }

    // Function call
    mulMat(mat1, mat2);

    return 0;
    */

    /*
    ULONG bufferSize;
    // Get the Current Process Handle.
    HANDLE curProc = GetCurrentProcess();

    // Get total number (size) of elements in the data structure.
    GetSystemCpuSetInformation(nullptr, 0, &bufferSize, curProc, 0);

    // Allocate data structures based on size returned from first call.
    auto buffer = std::make_unique<uint8_t[]>(bufferSize);
    // Get all of the CPUSet elements
    if (!GetSystemCpuSetInformation(reinterpret_cast<PSYSTEM_CPU_SET_INFORMATION>(buffer.get()), bufferSize, &bufferSize, curProc, 0))
    {
        return false;
    }

    uint8_t* cpuSetPtr = buffer.get();

    for (ULONG cpuSetSize = 0; cpuSetSize < bufferSize; )
    {
        auto nextCPUSet = reinterpret_cast<PSYSTEM_CPU_SET_INFORMATION>(cpuSetPtr);

        cpuSetPtr += nextCPUSet->Size;
        cpuSetSize += nextCPUSet->Size;
    }
    */
    std::vector<std::thread> threads;

    threads.emplace_back(proc_1, 3);

    threads.emplace_back(proc_1, 3);

    //threads.emplace_back(proc_1, 7);

    for (auto& t : threads)
        t.join();

}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
