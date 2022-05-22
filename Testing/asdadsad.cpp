#include<bits/stdc++.h>
using namespace std;

long long int sum_btw(int l,int r,vector<int> &ps)
{
    if(l == 0)
    return ps[r];
    else
    return ps[r] - ps[l-1];
}

long long int fun(long long int id,vector<vector<long long int>> &v,long long int m,long long int prev )
{

     if(id > v.size())
       return 0;

    if(id == v.size())
     return 1;



    if( id >= v.size() && m == 0)
        return 1;

    if(m <= 0 )
     return 0;

    if( *max_element(v[id].begin(), v[id].end()) < prev )
        return 0;

    for(long long int i=0;i<v[id].size();i++)
    {
        if(v[id][i] > prev)
        {
                long long int z =  fun(id+(i*2)+1,v,m-i,v[id][i]);
                if(z == 1)
                  return 1;
        }

    }
    return 0;
}

void vector_printer( vector<int> v)
{
    cout<<"\n------------------------\n";
    for(int i=0; i<v.size(); i++)
        cout<<v[i]<<" ";
    cout<<"\n------------------------\n";
}

int Possibility (int N, int M, vector<long long> A) {
   // Write your code here

   vector<int> prefix(N);
   long long sum=0;
   for(int i=0;i<N;i++)
   {
       sum += a[i];
   }

    vector_printer(prefix);
   vector<vector<long long int>> v(N);

   for(int i=0;i<N-2;i++)
   {
       for(int j=i;j<N;j+=2)
       {
           cout<<"i = "<<i<<" j = "<<j<<" \n";
           v[i].push_back(sum_btw(i,j,prefix));
       }

   }

   for(int i=0;i<N;i++)
   {
       for(int j=0;j<v[i].size();j++)
       {
           cout<<v[i][j]<<" ";
       }
       cout<<"\n";
   }

    for(int i=0;i<v[0].size();i++)
    {
        long long int z =  fun(i*2+1,v,M-i,v[0][i]);
        if(z == 1)
            return 1;
    }
   return 0;
}

int main() {

    ios::sync_with_stdio(0);
    cin.tie(0);
    int T;
    cin >> T;
    for(int t_i = 0; t_i < T; t_i++)
    {
        int N;
        cin >> N;
        int M;
        cin >> M;
        vector<long long> A(N);
        for(int i_A = 0; i_A < N - 2; i_A++)
        {
        	cin >> A[i_A];
        }

        int out_;
        out_ = Possibility(N, M, A);


        cout << out_;
        cout << "\n";
    }
}
