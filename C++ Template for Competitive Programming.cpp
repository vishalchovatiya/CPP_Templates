#include <iostream>
#include <sstream>
#include <cstdio>
#include <cmath>
#include <cstring>
#include <cctype>
#include <string>
#include <vector>
#include <list>
#include <set>
#include <map>
#include <queue>
#include <stack>
#include <algorithm>
#include <functional>
using namespace std;

/* ------------------------- HELPERS DEFINE -------------------------- */
#define DEBUG(X) std::cout << #X << " = " << X << std::endl;
#define PRINT(C, WAY)                      \
    do                                     \
    {                                      \
        cout << setw(10) << #C << " : \n"; \
        for (auto &&i : C)                 \
        {                                  \
            WAY;                           \
        }                                  \
        cout << endl;                      \
    } while (0);
#define ALL(C) (C).begin(), (C).end()
#define PRESENT(C, X) ((C).find() != (C).end())

#define BENCHMARK(f)         \
    do                       \
    {                        \
        cout << #f << " = "; \
        benchmark(f);        \
    } while (0);

const auto benchmark = [](function<void(void)> f) {
    std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
    f();
    std::cout << std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - start).count() << " microseconds\n";
};

ll get_no_of_query();

template <typename T>
vector<T> get_array(T Q);

/* ----------------------------- MAIN CODE ----------------------------- */

int main()
{
    // auto Q = get_no_of_query();
    // auto arr = get_array(Q);

    return 0;
}

/* --------------------------- HELPERS IMPLE -------------------------- */
ll get_no_of_query()
{
    ll Q;
    cin >> Q;
    return Q;
}

template <typename T>
vector<T> get_array(T Q)
{
    vector<T> res(Q);

    for (T i = 0; i < Q; i++)
    {
        T temp;
        cin >> temp;
        res[i] = temp;
    }

    return res;
}
