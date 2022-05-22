#include<iostream>
#include<cstring>
#include<vector>
#include<set>
#include<map>
#include<algorithm>
#include<cmath>
#include<climits>
#define lli long long int
#define GO_FAST ios_base::sync_with_stdio(0);ios::sync_with_stdio(0);cin.tie(0);cout.tie(0);
// basic debugging macros
lli __i__,__j__;
lli inf = 9e18;
#define line_printer(l) cout<<"\n"; for(__i__=0;__i__<l;__i__++){cout<<"-";}cout<<endl;
#define array_printer(a,l) cout<<#a<<": ";for(__i__=0;__i__<l;__i__++){cout<<a[__i__]<<" ";}cout<<"\n";
#define array_2d_printer(a,r,c) cout<<"\n"<<#a<<":\n";for(__i__=0;__i__<r;__i__++){for(__j__=0;__j__<c;__j__++){cout<<a[__i__][__j__]<<" ";}cout<<"\n";}
using namespace std;



int main()
{
    //GO_FAST
    //freopen("input.txt", "r", stdin);
    //freopen("myout.txt", "w", stdout);
    vector<int> t1;
    vector<int> t2;
    t1.push_back(4);
    t1.push_back(45);
    t1.push_back(0);
    t1.push_back(9);
    t1.push_back(3);
    t1.push_back(7);
    t1.push_back(6);
    t2.push_back(1);
    t2.push_back(22);
    t2.push_back(16);
    set<vector<int>> a;
    a.insert(t1);
    a.insert(t2);
    for (auto i:a){
        for (auto j:i){
            cout<<j<<" ";
        }
        cout<<endl;
    }
    return 0;
}
