#include<bits/stdc++.h>
using namespace std;


struct hashFunction //hash function for unordered map
{
    size_t operator()(const vector<int>
                      &myVector) const
    {
        std::hash<int> hasher;
        size_t answer = 0;

        for (int i : myVector)
        {
            answer ^= hasher(i) + 0x9e3779b9 +
                      (answer << 6) + (answer >> 2);
        }
        return answer;
    }
};

void vector_printer( vector<int> v)
{
    cout<<"\n------------------------\n";
    for(int i=0; i<v.size(); i++)
        cout<<v[i]<<" ";
    cout<<"\n------------------------\n";
}
int main()
{
    unordered_set<vector<int>,hashFunction> z;
    vector<int> t1 = {1,2,3,4,5};
    vector<int> t2 = {3,2,3,4,5};
    z.insert(t1);
    z.insert(t2);
    for(auto i:z)
        vector_printer(i);

    return 0;
}
