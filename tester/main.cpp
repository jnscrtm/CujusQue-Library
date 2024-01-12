#include <array>
#include <algorithm>
#include <iostream>
#include <thread>
#include <chrono>
#include <iomanip>
#include <numeric>

#include "CQue.hpp"

using namespace CQue;

void generate_report(const std::string& test_title, int NTry, int NRepPerTry, std::vector<double>& intervals)
{
    std::cout << std::string(80, '=') << '\n';
    std::string msg = "TEST: " + test_title;
    std::cout << msg << '\n' << std::string(80, '-') << '\n';

    double total_duration = std::accumulate(intervals.begin(), intervals.end(), 0.0);
    std::sort(intervals.begin(), intervals.end());

    std::cout << "Try count               : " << NTry << '\n';
    std::cout << "Repetitions per try     : " << NRepPerTry << '\n';
    std::cout << "Shortest execution time : " << intervals[0] << "ms\n";
    std::cout << "Longest execution time  : " << intervals[NTry-1] << "ms\n";
    std::cout << "Average execution time  : " << total_duration/NTry << "ms\n";
    std::cout << "Sum of execution times  : " << total_duration << "ms\n";
    std::cout << std::string(80, '-') << '\n';
}

template <int M, int N>
void benchmark_what(void(*pFunc)(), std::vector<double>& intervals_out)
{
    intervals_out.clear();
    intervals_out.reserve(M);

    decltype(std::chrono::high_resolution_clock::now()) start, end;

    for(int A = 0; A < M; A++)
    {
        start = std::chrono::high_resolution_clock::now();

        for(int i = 0; i < N; i++)
            pFunc();

        end = std::chrono::high_resolution_clock::now();
        intervals_out.push_back((double)std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() / 1000.0);
    }
}

int Wrap(int what)
{
	return what;
}

std::string sample1 = "";
std::string sample2 = "";

int _Arr[] = { std::rand(), 172623069, 200471142, 219891604, 240513271, 368837645, 384485106, 483998907, 514254658, 530507369, 565705035, 583685066, 699079864, 711265725, 733005251, 740277151, 756510150, 775786071, 796407002, 871944705, 892250822, 938751969, 945570648, 959681844, 981571299, 95150165, 138435788, 220221302, 243214833, 252094439, 291570764, 295755047};

void list_test1()
{
	CQue::List<int> lst;

	for (std::size_t i = 0; i < 32; i++)
		lst.Add(_Arr[i]);

	for (int i = 0; i < 64; i++)
		lst.Add(i);

	for (std::size_t i = 0; i < 128; i++)
		lst.Add(std::rand());
}

void list_test2()
{
	CQue::List<int> lst;

	lst.AddRange(IterWrapper<int>(_Arr, &_Arr[32]));
	lst.AddRange(IterWrapper<int>(_Arr, &_Arr[32]));
	lst.AddRange(IterWrapper<int>(_Arr, &_Arr[32]));
	lst.AddRange(IterWrapper<int>(_Arr, &_Arr[32]));
	lst.AddRange(IterWrapper<int>(_Arr, &_Arr[32]));
	lst.AddRange(IterWrapper<int>(_Arr, &_Arr[32]));
	lst.AddRange(IterWrapper<int>(_Arr, &_Arr[32]));
	lst.AddRange(IterWrapper<int>(_Arr, &_Arr[32]));

	return;
}

void list_test3()
{
	CQue::List<int> lst;

	for (std::size_t i = 0; i < 32; i++)
		lst.Insert(0, _Arr[i]);

	for (int i = 0; i < 64; i++)
		lst.Insert(0, i);

	for (std::size_t i = 0; i < 128; i++)
		lst.Insert(0, std::rand());
}

void list_test4()
{
	CQue::List<int> lst;

	lst.InsertRange(0, IterWrapper<int>(_Arr, &_Arr[32]));
	lst.InsertRange(0, IterWrapper<int>(_Arr, &_Arr[32]));
	lst.InsertRange(0, IterWrapper<int>(_Arr, &_Arr[32]));
	lst.InsertRange(0, IterWrapper<int>(_Arr, &_Arr[32]));
	lst.InsertRange(0, IterWrapper<int>(_Arr, &_Arr[32]));
	lst.InsertRange(0, IterWrapper<int>(_Arr, &_Arr[32]));
	lst.InsertRange(0, IterWrapper<int>(_Arr, &_Arr[32]));
	lst.InsertRange(0, IterWrapper<int>(_Arr, &_Arr[32]));

	return;
}

void vector_test1()
{
	std::vector<int> lst;

	for (std::size_t i = 0; i < 32; i++)
		lst.push_back(_Arr[i]);

	for (int i = 0; i < 64; i++)
		lst.push_back(i);

	for (std::size_t i = 0; i < 128; i++)
		lst.push_back(std::rand());
}

void vector_test2()
{
	std::vector<int> lst;

	lst.insert(lst.end(), _Arr, &_Arr[32]);
	lst.insert(lst.end(), _Arr, &_Arr[32]);
	lst.insert(lst.end(), _Arr, &_Arr[32]);
	lst.insert(lst.end(), _Arr, &_Arr[32]);
	lst.insert(lst.end(), _Arr, &_Arr[32]);
	lst.insert(lst.end(), _Arr, &_Arr[32]);
	lst.insert(lst.end(), _Arr, &_Arr[32]);
	lst.insert(lst.end(), _Arr, &_Arr[32]);
}

void vector_test3()
{
	std::vector<int> lst;

	for (std::size_t i = 0; i < 32; i++)
		lst.insert(lst.begin(), _Arr[i]);

	for (int i = 0; i < 64; i++)
		lst.insert(lst.begin(), i);

	for (std::size_t i = 0; i < 128; i++)
		lst.insert(lst.begin(), std::rand());
}

void vector_test4()
{
	std::vector<int> lst;

	lst.insert(lst.begin(), _Arr, &_Arr[32]);
	lst.insert(lst.begin(), _Arr, &_Arr[32]);
	lst.insert(lst.begin(), _Arr, &_Arr[32]);
	lst.insert(lst.begin(), _Arr, &_Arr[32]);
	lst.insert(lst.begin(), _Arr, &_Arr[32]);
	lst.insert(lst.begin(), _Arr, &_Arr[32]);
	lst.insert(lst.begin(), _Arr, &_Arr[32]);
	lst.insert(lst.begin(), _Arr, &_Arr[32]);
}

void FInsTest()
{
	CQue::List<int> lst;

	lst.InsertRange(0, IterWrapper<int>(_Arr, &_Arr[32]));
	lst.InsertRange(0, IterWrapper<int>(_Arr, &_Arr[32]));
	lst.InsertRange(0, IterWrapper<int>(_Arr, &_Arr[32]));
	lst.InsertRange(0, IterWrapper<int>(_Arr, &_Arr[32]));
	lst.InsertRange(0, IterWrapper<int>(_Arr, &_Arr[32]));
	lst.InsertRange(0, IterWrapper<int>(_Arr, &_Arr[32]));
	lst.InsertRange(0, IterWrapper<int>(_Arr, &_Arr[32]));
	lst.InsertRange(0, IterWrapper<int>(_Arr, &_Arr[32]));
}

void BInsTest()
{
	CQue::List<int> lst;

	lst.AddRange(IterWrapper<int>(_Arr, &_Arr[32]));
	lst.AddRange(IterWrapper<int>(_Arr, &_Arr[32]));
	lst.AddRange(IterWrapper<int>(_Arr, &_Arr[32]));
	lst.AddRange(IterWrapper<int>(_Arr, &_Arr[32]));
	lst.AddRange(IterWrapper<int>(_Arr, &_Arr[32]));
	lst.AddRange(IterWrapper<int>(_Arr, &_Arr[32]));
	lst.AddRange(IterWrapper<int>(_Arr, &_Arr[32]));
	lst.AddRange(IterWrapper<int>(_Arr, &_Arr[32]));
}

int main()
{
#ifdef _MSC_VER
	std::cout << "(Windows 11, MSVC x64, /O2 Optimized)\n\n";
#elif defined(__GNUC__)
	#if defined(__clang__)
		#if defined(_WIN32)
	std::cout << "(Windows 11, LLVM CLang, -O3 Optimized)\n\n";
		#else
	std::cout << "(WSL Linux, LLVM CLang, -O3 Optimized)\n\n";
		#endif
	#else
		#if defined(_WIN32)
	std::cout << "(Windows 11, GNU GCC, -O3 Optimized)\n\n";
		#else
	std::cout << "(WSL Linux, GNU GCC, -O3 Optimized)\n\n";
		#endif
	#endif
#endif
	constexpr int M = 10;
	constexpr int N = 10000;

	std::vector<double> l, v;

	std::thread th1(benchmark_what<M, N>, list_test1, std::ref(l));
	std::thread th2(benchmark_what<M, N>, vector_test1, std::ref(v));
	
    th1.join();
    th2.join();

	generate_report("CQue::List<int> - Endurance Test: Individual Back-Insertion", M, N, l);
	generate_report("std::vector<int> - Endurance Test: Individual Back-Insertion", M, N, v);

	std::cout << '\n';

	th1 = std::thread(benchmark_what<M, N>, list_test2, std::ref(l));
	th2 = std::thread(benchmark_what<M, N>, vector_test2, std::ref(v));

	th1.join();
	th2.join();

	generate_report("CQue::List<int> - Endurance Test: Ranged Back-Insertion", M, N, l);
	generate_report("std::vector<int> - Endurance Test: Ranged Back-Insertion", M, N, v);

	std::cout << '\n';

	th1 = std::thread(benchmark_what<M, N>, list_test3, std::ref(l));
	th2 = std::thread(benchmark_what<M, N>, vector_test3, std::ref(v));

	th1.join();
	th2.join();

	generate_report("CQue::List<int> - Endurance Test: Individual Front-Insertion", M, N, l);
	generate_report("std::vector<int> - Endurance Test: Individual Front-Insertion", M, N, v);

	std::cout << '\n';

	th1 = std::thread(benchmark_what<M, N>, list_test4, std::ref(l));
	th2 = std::thread(benchmark_what<M, N>, vector_test4, std::ref(v));

	th1.join();
	th2.join();

	generate_report("CQue::List<int> - Endurance Test: Ranged Front-Insertion", M, N, l);
	generate_report("std::vector<int> - Endurance Test: Ranged Front-Insertion", M, N, v);

	std::cout << '\n';

	return 0;
}