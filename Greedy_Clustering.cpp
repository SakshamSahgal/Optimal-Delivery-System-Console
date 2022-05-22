#include<bits/stdc++.h>
using namespace std;

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

void disp_graph(map<int,vector<pair<int,int>>> graph)
{
    cout<<"\n----------------------------------\n";
    for(auto i:graph)
    {
        cout<<i.first<<" -> ";
        for(auto j:i.second)
            cout<<"{ "<<j.first<<" , "<<j.second<<" } ";
        cout<<"\n";
    }
    cout<<"\n----------------------------------\n";
}


void vector_printer( vector<int> v)
{
    cout<<"\n------------------------\n";
    for(int i=0; i<v.size(); i++)
        cout<<v[i]<<" ";
    cout<<"\n------------------------\n";
}



class Shortest_path_finder
{
public :
    vector<vector<int>> all_pair_shortest_distance;
    vector<vector<int>> all_pair_shortest_distance_intermediate_vertex;

    Shortest_path_finder()
    {
        //empty Constructor for overloading
    }

    Shortest_path_finder(map<int,vector<pair<int,int>>> graph)
    {
        disp_graph(graph);
        all_pair_shortest_distance = adj_list_to_matrix(graph);
        Vec_2d_printer(all_pair_shortest_distance);
        Floyd_Warshall(graph);
        Vec_2d_printer(all_pair_shortest_distance);
        Vec_2d_printer(all_pair_shortest_distance_intermediate_vertex);
    }

    vector<vector<int>> adj_list_to_matrix(map<int,vector<pair<int,int>>> graph)
    {
        int n = graph.size();
        int inf = INT_MAX;
        vector<vector<int>> adj_matrix(n+1,vector<int>(n+1));

        for(int i=0; i<=n; i++)
        {
            for(int j=0; j<=n; j++)
                (i == j) ? adj_matrix[i][j] = 0 : adj_matrix[i][j] = inf;
        }

        for(auto i:graph)
        {
            for(auto j:i.second)
            {
                adj_matrix[i.first][j.first] = j.second;
                adj_matrix[j.first][i.first] = j.second;
            }

        }
        return adj_matrix;
    }

    void Floyd_Warshall(map<int,vector<pair<int,int>>> graph)
    {
        int n = graph.size();
        int inf = INT_MAX;

        vector<int> temp(n+1,-1);
        all_pair_shortest_distance_intermediate_vertex.resize(n+1,temp);

        for(int k=1; k<=n; k++)
        {
            for(int i=1; i<=n; i++)
            {
                for(int j=1; j<=n; j++)
                {
                    if(all_pair_shortest_distance[i][k] != inf && all_pair_shortest_distance[k][j] != inf)
                    {
                        if(all_pair_shortest_distance[i][j] > all_pair_shortest_distance[i][k] + all_pair_shortest_distance[k][j])
                        {
                            all_pair_shortest_distance[i][j] = all_pair_shortest_distance[i][k] + all_pair_shortest_distance[k][j];
                            all_pair_shortest_distance[j][i] = all_pair_shortest_distance[i][k] + all_pair_shortest_distance[k][j];
                            all_pair_shortest_distance_intermediate_vertex[i][j] = k;
                            all_pair_shortest_distance_intermediate_vertex[j][i] = k;
                        }
                    }
                }
            }
        }

    }


    vector<int> calc_path(int v1,int v2)
    {
        if( all_pair_shortest_distance_intermediate_vertex[v1][v2] == -1)
        {
            vector<int> ep;
            return ep;
        }
        else
        {
            int mid = all_pair_shortest_distance_intermediate_vertex[v1][v2];
            vector<int> a = calc_path(v1,mid);
            vector<int> b = calc_path(mid,v2);
            a.push_back(mid);
            a.insert(a.end(),b.begin(),b.end());
            return a;
        }
    }

    vector<int> get_path(int v1,int v2) //this function returns optimal path between two vertices
    {
        vector<int> path;
        path.push_back(v1);
        vector<int> calc = calc_path(v1,v2);
        path.insert(path.end(),calc.begin(),calc.end());
        path.push_back(v2);
        return path;
    }

    vector<vector<int>> get_traversal_path(vector<vector<int>> &traversal_order,int src)
    {
        vector<vector<int>> path_traversed;
        for(int i=0; i<traversal_order.size(); i++)
        {
            vector<int> this_guy_path;
            for(int j=-1; j<(int)traversal_order[i].size()-1; j++)
            {
                vector<int> this_path;
                if(j == -1)
                {
                    vector<int> temp = get_path(src,traversal_order[i][j+1]);
                    this_path.insert(this_path.end(),temp.begin(),temp.end());
                }
                else
                {
                    vector<int> temp = get_path(traversal_order[i][j],traversal_order[i][j+1]);
                    this_path.insert(this_path.end(),temp.begin(),temp.end());
                }

                if(j != traversal_order[i].size()-2)
                    this_path.pop_back();
                this_guy_path.insert(this_guy_path.end(),this_path.begin(),this_path.end());
            }
            path_traversed.push_back(this_guy_path);
        }
        return path_traversed;
    }

};



class Brute_all_paths   //cost[][] , scr , delivery_locations[] , no_of_drivers , delivery_deadline
{
public :
    vector<vector<int>> mini_time_traversal;
    vector<vector<int>> mini_distance_traversal;
    vector<vector<int>> mini_cost_traversal;
    int no_of_Drivers;
    int mini_cost=INT_MAX;
    int mini_time=INT_MAX;
    int mini_distance=INT_MAX;
    vector<vector<int>> path;
    int penalty=100;
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

    void cal_all_paths(vector<int> &delivery_locations,int index,int no_of_drivers,int src)
    {
        if(index == delivery_locations.size())
        {
//            Vec_2d_printer(path);
//            cout<<"path despription - \n";
            path_desc p = calc_path_desc(src);
//            cout<<" { cost = "<<p.cost<<" , distance travelled = "<<p.distance_travelled<<" , over all time taken = "<<p.time_taken<<" } \n";
            if(mini_cost > p.cost )
            {
                mini_cost = p.cost;
                mini_cost_traversal = path;
            }

            if(mini_time > p.time_taken )
            {
                mini_time = p.time_taken;
                mini_time_traversal = path;
            }

            if( mini_distance > p.distance_travelled )
            {
                mini_distance = p.distance_travelled;
                mini_distance_traversal = path;
            }
            return;
        }
        else
        {
            for(int i=0; i < no_of_drivers; i++) //row
            {
                path[i].push_back(delivery_locations[index]);
                cal_all_paths(delivery_locations,index+1,no_of_drivers,src);
                path[i].pop_back();
            }
        }
    }


    void display(Shortest_path_finder &g1,int src)
    {
        cout<<"Minimum time possible - "<<mini_time<<"\n";
        //cout<<"Mini time traversal order - ";
        //Vec_2d_printer(mini_time_traversal);
        cout<<"Mini time path traversed - ";
        vector<vector<int>> temp = g1.get_traversal_path(mini_time_traversal,src);
        for(int i=0; i<no_of_Drivers; i++)
        {
            cout<<"\ndriver "<<i+1<<" -> ";
            vector_printer(temp[i]);
        }
        cout<<"\n";
        cout<<"Minimum distance possible - "<<mini_distance<<"\n";
        //cout<<"Mini distance traversal order - ";
        //Vec_2d_printer(mini_distance_traversal);
        cout<<"Mini distance traversal - ";
        temp = g1.get_traversal_path(mini_distance_traversal,src);
        for(int i=0; i<no_of_Drivers; i++)
        {
            cout<<"\ndriver "<<i+1<<" -> ";
            vector_printer(temp[i]);
        }
        cout<<"\n";
        cout<<"Minimum cost possible - "<<mini_cost<<"\n";
        //cout<<"Mini cost traversal order - ";
        //Vec_2d_printer(mini_cost_traversal);
        cout<<"Mini Cost traversal - ";
        temp = g1.get_traversal_path(mini_cost_traversal,src);
        for(int i=0; i<no_of_Drivers; i++)
        {
            cout<<"\ndriver "<<i+1<<" -> ";
            vector_printer(temp[i]);
        }
        cout<<"\n";
    }

    vector<vector<int>> get_min_time_traversal()
    {
        return mini_time_traversal;
    }

    vector<vector<int>> get_min_distance_traversal()
    {
        return mini_distance_traversal;
    }

    vector<vector<int>> get_min_cost_traversal()
    {
        return mini_cost_traversal;
    }

    Brute_all_paths(vector<vector<int>> &floyd_Warshall_costs,int src,vector<int> &delivery_locations,int no_of_drivers,vector<int> &delivery_deadlines)
    {
        no_of_Drivers = no_of_drivers;
        path.resize(no_of_drivers,vector<int>());
        floyd_Warshall_cost = floyd_Warshall_costs;
        delivery_deadline = delivery_deadlines;
        sort(delivery_locations.begin(),delivery_locations.end());
        do
        {
            cal_all_paths(delivery_locations,0,no_of_drivers,src);
        }
        while(next_permutation(delivery_locations.begin(),delivery_locations.end()));
    }
};



class Greedy_Cluster
{
public:
    Shortest_path_finder g1;
    int src;
    vector<int> delivery_locations;
    int no_of_drivers;
    vector<int> delivery_deadlines;
    vector<int> cluster_initializers;
    vector<vector<int>> clusters;
    vector<int> rem_delivery_locations;

    Greedy_Cluster(Shortest_path_finder &g,int source,vector<int> &Delivery_locations,int No_of_drivers,vector<int> &Delivery_deadlines)
    {
        g1 = g;
        src = source;
        delivery_locations = Delivery_locations;
        no_of_drivers = No_of_drivers;
        delivery_deadlines = Delivery_deadlines;
    }


    void calc_cluster_initilizers() //this function finds the initial two cluster initilizers
    {
        int maxx_distance = INT_MIN;
        int a=-1;
        int b=-1;
        rem_delivery_locations = delivery_locations;

        for(auto i:delivery_locations)
        {
            for(auto j:delivery_locations)
            {
                if(i != j)
                {
                    if( g1.all_pair_shortest_distance[i][j] > maxx_distance )
                    {
                        maxx_distance = g1.all_pair_shortest_distance[i][j];
                        a = i;
                        b = j;
                    }
                }
            }
        }
        cout<<"farthest points = "<<a<<" "<<b<<" \n";
        cluster_initializers.push_back(a);
        cluster_initializers.push_back(b);
        rem_delivery_locations.erase(find(rem_delivery_locations.begin(),rem_delivery_locations.end(),a));
        rem_delivery_locations.erase(find(rem_delivery_locations.begin(),rem_delivery_locations.end(),b));
        find_rem_cluster_sources();
    }

    void find_rem_cluster_sources() //this function finds remaining cluster initilizers
    {
        while(cluster_initializers.size() < no_of_drivers)
        {
            pair<int,int> temp_dist; //distance,vertex
            pair<int,int> max_dist = {-1e5,-1e5}; //distance,vertex

            for(auto i:delivery_locations)
            {
                if(count(cluster_initializers.begin(),cluster_initializers.end(),i) == 0) // this means that this delivery vertex is currently not a cluster vertex
                {
                    int dist=0;
                    for(auto j:cluster_initializers)
                        dist += powl((g1.all_pair_shortest_distance[i][j]),2);
                    temp_dist = {dist,i};
                    max_dist = max(max_dist,temp_dist);
                }
            }
            cout<<"max vertex = "<<max_dist.second<<"\n";
            cluster_initializers.push_back(max_dist.second);
            rem_delivery_locations.erase(find(rem_delivery_locations.begin(),rem_delivery_locations.end(),max_dist.second));
        }
    }



    void populate_clusters() //this function assigns a cluster to all remaining delivery locations
    {
        for(auto i:cluster_initializers)
        {
            vector<int> temp;
            temp.push_back(i);
            clusters.push_back(temp);
        }

        while(rem_delivery_locations.size())
        {
            pair<int,int> mini = {1e5,1e5}; //distance , vertex
            for(int i=0;i<clusters.size();i++)
            {
                int dist = g1.all_pair_shortest_distance[(clusters[i][0])][rem_delivery_locations[0]]; //distance btw ith cluster initilizer and this delivery location
                mini = min(mini,{dist,i});
            }
            cout<<"best cluster for "<<rem_delivery_locations[0]<<" is cluster "<<clusters[mini.second][0]<<"\n";
            clusters[mini.second].push_back(rem_delivery_locations[0]);
            rem_delivery_locations.erase(rem_delivery_locations.begin());

        }
    }


    void display_clusters()
    {
        cout<<"Clusters - \n";
        for(auto i:clusters)
        {
            cout<<"\n-----------------------------\n";
            for(auto j:i)
                cout<<j<<" ";
            cout<<"\n-----------------------------\n";
        }
    }
};

int main()
{
    int no_of_vertex;
    int no_of_Edges;
    int src;
    int no_of_drivers;
    cin>>no_of_vertex>>no_of_Edges>>src>>no_of_drivers;
    map<int,vector<pair<int,int>>> graph;

    for(int i=0; i<no_of_Edges; i++)
    {
        int a,b,w;
        cin>>a>>b>>w;
        graph[a].push_back({b,w});
        graph[b].push_back({a,w});
    }

    disp_graph(graph);
    int no_of_delivery_locations;
    cin>>no_of_delivery_locations;
    vector<int> delivery_locations(no_of_delivery_locations);
    vector<int> delivery_deadlines(no_of_vertex+1);

    for(int i=0; i<no_of_delivery_locations; i++)
    {
        int v,t;
        cin>>v>>t;
        delivery_locations[i] = v;
        delivery_deadlines[v] = t; //hashed delivery deadline
    }

    Shortest_path_finder g1(graph);
    //vector_printer(g1.get_path(12,14));
    //=============Inputs, Creation of Graph, Floyd Warshall application done


    //Brute_all_paths brute_paths(g1.all_pair_shortest_distance,src,delivery_locations,no_of_drivers,delivery_deadlines);
    //brute_paths.display(g1,src);
    //cout<<"Done\n";

    //=============Brute all possible path to find path for minimum time , distance and penalty application done

    Greedy_Cluster greedy(g1,src,delivery_locations,no_of_drivers,delivery_deadlines);
    greedy.calc_cluster_initilizers();
    greedy.populate_clusters();
    greedy.display_clusters();
    return 0;
}
