#include<bits/stdc++.h>
using namespace std;

template<typename Z>
void Vec_2d_printer(vector<vector<Z>> x)
{
    cout<<"\n--------------------\n";
    for(auto i:x)
    {
        for(auto j:i)
            cout<<j<<" ";
        cout<<"\n";
    }
    cout<<"\n--------------------\n";
}

class Brute_all_paths   //cost[][] , scr , delivery_locations[] , no_of_drivers , delivery_deadline
{
public :
    vector<vector<int>> path;
    int penalty=100;
    int src=1;
    vector<int> delivery_deadline;
    vector<vector<int>> floyd_Warshall_cost;
    struct path_desc
    {
        int cost=0;
        int time_taken=0;
        int distance_travelled=0;
    };

    path_desc calc_path_desc(int src)
    {
        int tot_dist=0;
        int tot_time=INT_MIN;
        int total_delays=0;
        int cost=0;
        for(int i=0; i<path.size(); i++)
        {
            int this_time=0;
            int this_dist=0;
            for(int j=0; j<path[i].size(); j++)
            {
                this_dist += (j == 0) ? floyd_Warshall_cost[src][path[i][j]] : floyd_Warshall_cost[path[i][j-1]][path[i][j]];
                this_time += (j == 0) ? floyd_Warshall_cost[src][path[i][j]] : floyd_Warshall_cost[path[i][j-1]][path[i][j]];
                if(this_dist > delivery_deadline[path[i][j]]) //delay hogaya
                    total_delays++;
            }

            tot_time = max(tot_time,this_time);
            tot_dist += this_dist;
        }
        cost = tot_dist + total_delays*penalty;
        path_desc p;
        p = {cost,tot_time,tot_dist};
        return p;
    }

    void cal_all_paths(vector<int> &delivery_locations,int index,int no_of_drivers)
    {
        if(index == delivery_locations.size())
        {
            Vec_2d_printer(path);
            cout<<"path despription - \n";
            path_desc p = calc_path_desc(src);
            cout<<" { "<<p.cost<<" , "<<p.distance_travelled<<" , "<<p.time_taken<<" } \n";
            return;
        }
        else
        {
            for(int i=0; i<no_of_drivers; i++) //row
            {
                path[i].push_back(delivery_locations[index]);
                cal_all_paths(delivery_locations,index+1,no_of_drivers);
                path[i].pop_back();
            }
        }
    }

    Brute_all_paths(vector<vector<int>> &floyd_Warshall_costs,int src,vector<int> &delivery_locations,int no_of_drivers,vector<int> &delivery_deadlines)
    {
        path.resize(no_of_drivers,vector<int>());
        floyd_Warshall_cost = floyd_Warshall_costs;
        delivery_deadline = delivery_deadlines;
        sort(delivery_locations.begin(),delivery_locations.end());
        do
        {
        cal_all_paths(delivery_locations,0,no_of_drivers);
        }
        while(next_permutation(delivery_locations.begin(),delivery_locations.end()));
    }
};

int main()
{
    //GO_FAST
    //freopen("input.txt", "r", stdin);
    //freopen("myout.txt", "w", stdout);
    int no_of_drivers,no_of_delivery_locations;
    cin>>no_of_drivers>>no_of_delivery_locations;
    vector<int> delivery_locations(no_of_delivery_locations);

    for(int i=0; i<no_of_delivery_locations; i++)
        cin>>delivery_locations[i];

    return 0;
}
