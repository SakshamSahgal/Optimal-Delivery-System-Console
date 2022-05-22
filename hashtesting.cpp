#include <iostream>
#include <unordered_set>
#include <vector>
#include <utility>

using namespace std;

template<typename T>
size_t CombineHash(size_t cur, const T& obj) {
    std::hash<T> hasher;
    return cur ^ (hasher(obj) + 0x9e3779b9 + (cur << 6) + (cur >> 2));
}

template<>
template<typename T>
struct std::hash<vector<T>> {
    size_t operator()(const vector<T>& v) const {
        size_t answer = 0;
        for (const auto& item : v) {
            answer = CombineHash(answer, item);
        }
        return answer;
    }
};

template<>
template<typename T>
struct std::hash<unordered_set<T>> {
    size_t operator()(const unordered_set<T>& v) const {
        size_t answer = 0;
        for (const auto& item : v) {
            answer = CombineHash(answer, item);
        }
        return answer;
    }
};

template<>
template<typename First, typename Second>
struct std::hash<pair<First, Second>> {
    size_t operator()(const pair<First, Second>& p) const {
        size_t answer = 0;
        answer = CombineHash(answer, p.first);
        answer = CombineHash(answer, p.second);
        return answer;
    }
};


int main() {
    unordered_set<pair<unordered_set<vector<int>>, vector<int>>> s;
    unordered_set<vector<int>> v;
}
