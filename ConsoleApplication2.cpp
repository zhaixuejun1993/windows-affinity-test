// ConsoleApplication2.cpp : This file contains the 'main' function. Program execution begins and ends there.
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

   // std::cout << "Multiplication of given two matrices is:\n";

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


void proc_1(void)
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

    unsigned long cores[] = { cpuSets[1].CpuSet.Id};
    SetThreadSelectedCpuSets(GetCurrentThread(), cores, 1);

    while (1) {
        i++;
        if (i == 100) {
            i = 1;
            j++;
        }
        if (j == 100000)
            break;
    }
}

void proc(void)
{
    // using namespace std::chrono_literals;
    // std::this_thread::sleep_for(5s);
    std::cout << "cur thread id: " << GetCurrentThreadId() << std::endl;

    int mat1[R1][C1] = { { 1, 1 },
                     { 2, 2 } };

    int mat2[R2][C2] = { { 1, 1 },
                         { 2, 2 } };

    auto time_s = std::chrono::high_resolution_clock::now();
    while (1) {
        int rand_ms = rand() % 3 + 1;
        std::this_thread::sleep_for(std::chrono::milliseconds(rand_ms));

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
// Todo
// 1) long time run (> 2mins) 3 threads & 3 core (3/5/7) -> done
// 2) random time sleep -> done
// 3) more multi operations & more commands 
// 4) big/small core
// 5) windows 11
// 6) group property try

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

    
    std::vector<std::thread> threads;

    threads.emplace_back(proc);
    SetThreadAffinityMask(threads.back().native_handle(), DWORD_PTR(1) << 3);

    threads.emplace_back(proc);
    SetThreadAffinityMask(threads.back().native_handle(), DWORD_PTR(1) << 5);

    threads.emplace_back(proc);
    SetThreadAffinityMask(threads.back().native_handle(), DWORD_PTR(1) << 7);

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
