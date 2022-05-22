#include<bits/stdc++.h>
using namespace std;
int no_of_drivers;
int rec_calls=0;
int prevcost=INT_MAX;
vector<vector<vector<int>>> all_paths;
vector<vector<int>> bestPathsAfterCost;
vector<vector<int>> paths;
vector<vector<int>> permutations;
vector<pair<vector<int>,vector<int>>> comb_rem; // combination and remaining delivery

void vector_printer( vector<int> v)
{
    cout<<"\n------------------------\n";
    for(int i=0; i<v.size(); i++)
        cout<<v[i]<<" ";
    cout<<"\n------------------------\n";
}
template<typename Z>
void Vec_2d_printer(vector<vector<Z>> x)
{
    cout<<"\n--------------------\n";
    for(auto i:x)
    {
        for(auto j:i)
        {
            if(j == INT_MAX)
                cout<<"I ";
            else
                cout<<j<<" ";
        }

        cout<<"\n";
    }
    cout<<"\n--------------------\n";
}

void printer() //this function prints the combination and remaining array pair
{
    for(auto i:comb_rem)
    {
        cout<<" { ";
        for(auto j:i.first)
            cout<<j<<" ";
        cout<<" , ";
        for(auto j:i.second)
            cout<<j<<" ";
        cout<<" }\n";
    }
}


vector<int> calc_rem_locations(vector<int> &done_locations,vector<int> &delivery_locations) //this function calculates the remaining delivery locations
{
    vector<int> rem_delivery_locations;
    for(int i=0; i<delivery_locations.size(); i++)
    {
        auto is_present = find(done_locations.begin(),done_locations.end(),delivery_locations[i]);
        if(is_present == done_locations.end())
            rem_delivery_locations.push_back(delivery_locations[i]);
    }

    if( done_locations.size() > rem_delivery_locations.size() ) //if remaining locations are less than no of drivers then adding -1
    {
        int sz =  done_locations.size() - rem_delivery_locations.size();
        for(int i=0; i<sz; i++)
            rem_delivery_locations.push_back(-1);
    }
    return rem_delivery_locations;
}


void calc_comb(vector<int> &delivery_locations,int id,int rem,vector<int> comb) //rem-> rem no of drivers , comb till now
{
    rec_calls++;
    if(rem == 0)
    {
        //vector_printer(comb);
        vector<int> rem_delivery_locations = calc_rem_locations(comb,delivery_locations);
        comb_rem.push_back({comb,rem_delivery_locations});
        return;
    }

    for(int i=id; i<delivery_locations.size(); i++)
    {
        comb.push_back(delivery_locations[i]);
        calc_comb(delivery_locations,i+1,rem-1,comb);
        comb.pop_back();
    }
}

void calc_all_permutations(vector<int> v)
{
    sort(v.begin(),v.end());
    do
    {
        permutations.push_back(v);
    }
    while(next_permutation(v.begin(),v.end()));
}

bool is_end(vector<int> &cur_locations) //tells whether the recursive calls end or not
{
    for(int i=0; i<cur_locations.size(); i++)
    {
        if(cur_locations[i] != -1)
            return 0;
    }
    return 1;
}
int costfunc(vector<vector<int>> tempPaths)
{
    return 1;
}
void func_path(vector<int> cur_locations,vector<int> done_locations,vector<int> rem_locations)
{
    //Vec_2d_printer(paths);
    //vector_printer(cur_locations);
    //calculating the number of available drivers.
    int no_of_ava_drivers = 0;
    for(auto i:cur_locations)
    {
        if(i >= 0)
            no_of_ava_drivers++;
    }

    rec_calls++;
    if(is_end(cur_locations))
    {
        if (costfunc(paths)<prevcost){
            Vec_2d_printer(paths);
            bestPathsAfterCost=paths;
            cout<<"Ended"<<endl;
        }
        return;
    }
    else
    {

        int j=0;
        for (auto &&i:paths){
            if (i[i.size()-1]>=0)
                i.push_back(cur_locations[j++]);
            else
                i.push_back(-1);
        }
        vector<int> local_comb;
        comb_rem.clear();
        calc_comb(rem_locations,0,no_of_ava_drivers,local_comb);
        vector<pair<vector<int>,vector<int>>> local_comb_rem = comb_rem;
        for(auto i:local_comb_rem)
        {
            permutations.clear();
            calc_all_permutations(i.first);
            vector<int> done = done_locations;

            for(auto k:i.first)
                done.push_back(k);

            vector<vector<int>> temp = permutations;
            for(auto j:temp)
                func_path(j,done,i.second);
        }
        for (auto &&i:paths){
            i.pop_back();
        }

    }
}

void all_paths_printer()
{
    for(auto i:all_paths)
    {
        cout<<"\n--------------------------\n";
        for(auto j:i)
        {
            for(auto k:j)
                cout<<k<<" ";
            cout<<"\n";
        }
        cout<<"\n--------------------------\n";
    }
}

int main()
{
    //GO_FAST
    //freopen("input.txt", "r", stdin);
    //freopen("myout.txt", "w", stdout);

    cin>>no_of_drivers;

    vector<int> delivery_locations;
    int src = 1;
    delivery_locations.push_back(3);
    delivery_locations.push_back(4);
    delivery_locations.push_back(5);
    delivery_locations.push_back(6);
    delivery_locations.push_back(7);
    delivery_locations.push_back(8);
    for (int i=0;i<no_of_drivers-1;i++)
        delivery_locations.push_back(-i-1);
    vector<int> temp_comb; // empty temp vector
    calc_comb(delivery_locations,0,no_of_drivers,temp_comb);
    //printer();

    vector<pair<vector<int>,vector<int>>> local_comb_rem = comb_rem;

    for(int j=0;j<local_comb_rem[0].first.size();j++)
    {
        vector<int> temp;
        temp.push_back(src);
        paths.push_back(temp);
    }

    for(auto i:local_comb_rem)
    {
        func_path(i.first,i.first,i.second);
    }
    all_paths_printer();

    cout<<all_paths.size()<<"\n";
    return 0;
}
