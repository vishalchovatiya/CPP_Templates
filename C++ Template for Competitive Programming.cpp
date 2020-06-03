#include <iostream>
#include <sstream>
#include <cstdio>
#include <cmath>
#include <cstring>
#include <cctype>
#include <string>
#include <chrono>
#include <iomanip>
#include <vector>
#include <list>
#include <set>
#include <unordered_set>
#include <map>
#include <unordered_map>
#include <queue>
#include <stack>
#include <algorithm>
#include <functional>
using namespace std;

fstream infile {"db.txt"};
for (string line; getline(infile, line);) {
	cout<<line<<endl;
}

for (string s; getline(cin >> ws, s, ',');) {
	if (s.empty()) { 
		break; 
	}
	cout << "name: \"" << s << "\"\n";
}


/* ------------------------- HELPERS DEFINE -------------------------- */
template <typename T>
ostream &operator<<(ostream &o, pair<const T, T> &pair) // To print map in PRINT
{
    o << pair.first << " " << pair.second << endl;
    return o;
}

template <typename... T>
void read(T &... args)
{
    ((cin >> args), ...);
}

template <typename... T>
void write(T... args)
{
    ((cout << args), ...);
}

#define DEBUG(X) cout << #X << " = " << X << endl;
#define PRINT(C)                           \
    do                                     \
    {                                      \
        cout << setw(10) << #C << " : \n"; \
        for (auto &&i : C)                 \
        {                                  \
            cout << i << " ";              \
        }                                  \
        cout << endl;                      \
    } while (0);
#define SIZE_OF(ARR) (sizeof(ARR) / sizeof(ARR[0]))
#define ALL(C) (C).begin(), (C).end()
#define PRESENT(C, X) ((C).find() != (C).end())
#define FOR(i, begin, end) for (__typeof(end) i = (begin) - ((begin) > (end)); i != (end) - ((begin) > (end)); i += 1 - 2 * ((begin) > (end)))
#define BENCHMARK(f)         \
    do                       \
    {                        \
        cout << #f << " = "; \
        benchmark(f);        \
    } while (0);

const auto benchmark = [](function<void(void)> f) {
    chrono::steady_clock::time_point start = chrono::steady_clock::now();
    f();
    cout << chrono::duration_cast<chrono::microseconds>(chrono::steady_clock::now() - start).count() << " microseconds\n";
};

using ll = long long;
ll mod = 1e9 + 7;
ll get_no_of_query();

template <typename T>
vector<T> get_array(T Q);

/* ----------------------------- MAIN CODE ----------------------------- */

int main()
{
    auto Q = get_no_of_query();
    auto arr = get_array(Q);

    return 0;
}

/* -------------------- HELPERS IMPLEMENTATION ------------------------ */
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
