#include <windows.h>
#include<bits/stdc++.h>
int __i__,__j__;
#define line_printer(l) cout<<"\n"; for(__i__=0;__i__<l;__i__++){cout<<"-";}cout<<endl;
using namespace std;
using namespace std::chrono;

template<typename Z>
void Vec_2d_printer(vector<vector<Z>> x)
{
    line_printer(20);
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
    line_printer(20);
}

void disp_graph(map<int,vector<pair<int,int>>> graph)
{
    line_printer(20);
    for(auto i:graph)
    {
        cout<<i.first<<" -> ";
        for(auto j:i.second)
            cout<<"{ "<<j.first<<" , "<<j.second<<" } ";
        cout<<"\n";
    }
    line_printer(20);
}


void vector_printer( vector<int> v)
{
    line_printer(20);
    for(int i=0; i<v.size(); i++)
        cout<<v[i]<<" ";
    line_printer(20);
}


class Shortest_path_finder
{
public :
    vector<vector<int>> all_pair_shortest_distance; //this 2d array stores floyd warshall mini distance btw two vertices
    vector<vector<int>> all_pair_shortest_distance_intermediate_vertex; //stores the final intermediate vertices btw every pair

    Shortest_path_finder()
    {
        //empty Constructor for object overloading
    }

    Shortest_path_finder(map<int,vector<pair<int,int>>> &graph) //constructor that takes adjacency list of graph to perform Floyd Warshall
    {
        //disp_graph(graph);
        all_pair_shortest_distance = adj_list_to_matrix(graph);
        //Vec_2d_printer(all_pair_shortest_distance);
        Floyd_Warshall(graph);
        //Vec_2d_printer(all_pair_shortest_distance);
        //Vec_2d_printer(all_pair_shortest_distance_intermediate_vertex);
    }

    vector<vector<int>> adj_list_to_matrix(map<int,vector<pair<int,int>>> &graph) //this function just convertes adj_list to adj matrix to apply floyd warshall
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

    void Floyd_Warshall(map<int,vector<pair<int,int>>> &graph) //this function applies floyd warshall on the converted matrix
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


    vector<int> calc_path(int v1,int v2) //returns the shorted path between two vertices as a vector
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

    vector<vector<int>> get_traversal_path(vector<vector<int>> &traversal_order,int src) //this function takes the cluster and converts in to a path in that order using floyd warshall
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

    vector<vector<int>> mini_time_traversal; //this 2d vector stores the cluster obtained to get mini time
    vector<vector<int>> mini_distance_traversal; //this 2d vector stores the cluster obtained to get mini distance
    vector<vector<int>> mini_cost_traversal; //this 2d vector stores the cluster obtained to get mini cost
    set<pair<set<vector<int>>,vector<int>>> Global_Memoize; //this set of pair of (vector,vector) is used to memoize the brute to obtain all paths

    int no_of_Drivers; // stores no of drivers
    int mini_cost=INT_MAX; // stores minimum cost
    int mini_time=INT_MAX; //stores minimum time
    int mini_distance=INT_MAX; //stores minimum distance
    vector<vector<int>> path; //stores the currently obtained path in brute
    int penalty=100; //stores the currently set penalty
    vector<int> delivery_deadline; //stores the delivery deadlines as a hash  (index-> vertex no and value is the deadline for that index )
    vector<vector<int>> floyd_Warshall_cost; //2d vector obtained from shortest path finder class



    struct path_desc //structure that contains the description of the currently bruted path
    {
        int cost=0;
        int time_taken=0;
        int distance_travelled=0;
    };

    void Print_cluster(vector<vector<int>> &cluster) //just a cluster printer function
    {
        for(auto i:cluster)
        {
            cout<<"[ ";
            for(auto j:i)
                cout<<j<<" ";
            cout<<"]\n";
        }
    }

    path_desc calc_path_desc(int src) //calculates mini distance , time and cost of the currently bruted path
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

    void cal_all_paths(vector<int> &delivery_locations,int index,int no_of_drivers,int src) //recursive function that brutes all possible paths
    {
        if(index == delivery_locations.size()) //base condition for recursion
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
            set <vector<int>> memoize_set;
            for(int i=0; i < no_of_drivers; i++)
                memoize_set.insert(path[i]);

            vector<int> temp;

            for(int i=index; i<delivery_locations.size(); i++)
                temp.push_back(delivery_locations[i]);

            pair<set<vector<int>>,vector<int>> memoize_pair = {memoize_set,temp};

            if( Global_Memoize.count(memoize_pair) != 0 ) //memoization
                return;


            for(int i=0; i < no_of_drivers; i++) //row
            {
                path[i].push_back(delivery_locations[index]);
                cal_all_paths(delivery_locations,index+1,no_of_drivers,src);
                path[i].pop_back();
            }

            Global_Memoize.insert(memoize_pair);
        }
    }

    void print_path_traversed_by_drivers(vector<vector<int>> &traversal_order,int src,Shortest_path_finder &g1) //takes the cluster and prints the path followed by each driver
    {
        vector<vector<int>> temp = g1.get_traversal_path(traversal_order,src);
        for(int i=0; i<no_of_Drivers; i++)
        {
            cout<<"\n path Traversed by Driver "<<i+1<<" -> ";
            for(auto j:temp[i])
                cout<<j<<" ";
            cout<<"\n";
        }
        cout<<"\n";
    }

    void display(Shortest_path_finder &g1,int src) //display details of the best possible path
    {
        line_printer(20);
        cout<<"\nMinimum time possible - "<<mini_time<<"\n";
        cout<<"Mini time Cluster - \n";
        Print_cluster(mini_time_traversal);
        cout<<"Mini time path traversed - \n";
        print_path_traversed_by_drivers(mini_time_traversal,src,g1);
        line_printer(20);
        cout<<"\nMinimum distance possible - "<<mini_distance<<"\n";
        cout<<"Mini Distance Cluster - \n";
        Print_cluster(mini_distance_traversal);
        cout<<"Mini distance traversal - \n";
        print_path_traversed_by_drivers(mini_distance_traversal,src,g1);
        line_printer(20);
        cout<<"Minimum cost possible - "<<mini_cost<<"\n";
        cout<<"Mini Cost Cluster - \n";
        Print_cluster(mini_cost_traversal);
        cout<<"\nMini Cost traversal - \n";
        print_path_traversed_by_drivers(mini_cost_traversal,src,g1);
        line_printer(20);

    }

    Brute_all_paths(vector<vector<int>> &floyd_Warshall_costs,int src,vector<int> &delivery_locations,int no_of_drivers,vector<int> &delivery_deadlines) //constructor that calls all permutations of delivery location to be bruted
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
    Shortest_path_finder g1; // to store a copy of already computed object of Shortest_path_finder class
    int src; //stores the source vertex
    vector<int> delivery_locations; //delivery locations
    int no_of_drivers; //stores no of drivers
    vector<int> delivery_deadlines; //stores the delivery deadlines as a hash  (index-> vertex no and value is the deadline for that index )
    vector<int> cluster_initializers; //stores the initial cluster initilizers
    vector<vector<int>> clusters; //stores the final cluster obtained through greedy
    vector<int> rem_delivery_locations; //store the delivery locations remaining after removing the initial cluster initialisers
    int penalty = 100; //stores the penalty for delay

    struct path_desc //structure that contains the description of the current path
    {
        int cost=0;
        int time_taken=0;
        int distance_travelled=0;
    };

    void calc_path_desc() //calculates mini distance , time and cost
    {
        int tot_dist=0;
        int tot_time=INT_MIN;
        int total_delays=0;
        int cost=0;
        for(int i=0; i<clusters.size(); i++)
        {
            int this_time=0;
            int this_dist=0;
            for(int j=0; j<clusters[i].size(); j++)
            {
                this_dist += (j == 0) ? g1.all_pair_shortest_distance[src][clusters[i][j]] : g1.all_pair_shortest_distance[clusters[i][j-1]][clusters[i][j]];
                this_time += (j == 0) ? g1.all_pair_shortest_distance[src][clusters[i][j]] : g1.all_pair_shortest_distance[clusters[i][j-1]][clusters[i][j]];
                if(this_dist > delivery_deadlines[clusters[i][j]]) //delay hogaya
                    total_delays++;
            }
            tot_time = max(tot_time,this_time);
            tot_dist += this_dist;
        }
        cost = tot_dist + total_delays*penalty;
        path_desc p;
        p = {cost,tot_time,tot_dist};
        cout<<"\n time = "<<p.time_taken<<" distance = "<<p.distance_travelled<<" cost = "<<p.cost<<"\n";
    }

    Greedy_Cluster(Shortest_path_finder &g,int source,vector<int> &Delivery_locations,int No_of_drivers,vector<int> &Delivery_deadlines) //constructor that initializes all the variables
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
        //cout<<"farthest points = "<<a<<" "<<b<<" \n";
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
            pair<long double,int> temp_dist; //distance,vertex
            pair<long double,int> max_dist = {-1e5,-1e5}; //distance,vertex

            for(auto i:delivery_locations)
            {
                if(count(cluster_initializers.begin(),cluster_initializers.end(),i) == 0) // this means that this delivery vertex is currently not a cluster vertex
                {
                    long double dist=0;
                    for(auto j:cluster_initializers)
                        dist += powl((g1.all_pair_shortest_distance[i][j]),0.5);
                    temp_dist = {dist,i};
                    max_dist = max(max_dist,temp_dist);
                }
            }
            //cout<<"max vertex = "<<max_dist.second<<"\n";
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
            for(int i=0; i<clusters.size(); i++)
            {
                int dist = g1.all_pair_shortest_distance[(clusters[i][0])][rem_delivery_locations[0]]; //distance btw ith cluster initilizer and this delivery location
                mini = min(mini, {dist,i});
            }
            //cout<<"best cluster for "<<rem_delivery_locations[0]<<" is cluster "<<clusters[mini.second][0]<<"\n";
            clusters[mini.second].push_back(rem_delivery_locations[0]);
            rem_delivery_locations.erase(rem_delivery_locations.begin());
        }
    }

    void display_clusters() //just a cluster printer function
    {
        cout<<"Clusters - \n";
        for(auto i:clusters)
        {
            cout<<"[ ";
            for(auto j:i)
                cout<<j<<" ";
            cout<<" ] \n";
        }
    }


    void job_scheduling_on_clusters() //sorts the clusters on the basis of their deadline
    {
        vector<vector<int>> modified_cluster;
        for(auto i:clusters)
        {
            vector<pair<int,int>> this_cluster; //deadline , delivery_vertex
            vector<int> temp;
            for(auto j:i)
                this_cluster.push_back({delivery_deadlines[j],j});
            sort(this_cluster.begin(),this_cluster.end());
            for(auto j:this_cluster)
                temp.push_back(j.second);
            modified_cluster.push_back(temp);
        }
        clusters = modified_cluster;
    }

    void Display_cluster_path_traversed() //prints the path from the cluster
    {
        vector<vector<int>> path_traversed = g1.get_traversal_path(clusters,src);
        for(int i=0; i<no_of_drivers; i++)
        {
            cout<<"\n path Traversed by Driver "<<i+1<<" -> ";
            for(auto j:path_traversed[i])
                cout<<j<<" ";
            cout<<"\n";
        }
    }
};

void SetColor(int ForgC) //function that sets console color
{
    WORD wColor;
    //This handle is needed to get the current background attribute

    HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    //csbi is used for wAttributes word

    if(GetConsoleScreenBufferInfo(hStdOut, &csbi))
    {
        //To mask out all but the background attribute, and to add the color
        wColor = (csbi.wAttributes & 0xF0) + (ForgC & 0x0F);
        SetConsoleTextAttribute(hStdOut, wColor);
    }
    return;
}

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
    //disp_graph(graph);
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
    SetColor(34);
    //line_printer(20);
    cout<<"---------------------OUTPUT------------------------\n";
    Shortest_path_finder g1(graph);
    //=============Inputs, Creation of Graph, Floyd Warshall application done
    auto start = high_resolution_clock::now();
    cout<<"Through Brute -> \n";
    Brute_all_paths brute_paths(g1.all_pair_shortest_distance,src,delivery_locations,no_of_drivers,delivery_deadlines);
    brute_paths.display(g1,src);
    brute_paths.Global_Memoize.clear();
    cout<<"Done\n";
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(stop - start);
    cout<<endl << "Time taken by brute function : "<<duration.count() << " ms ->  "<<duration.count()/60000.0<<"min\n\n";
    //=============Brute all possible path to find path for minimum time , distance and penalty application done

    auto start2 = high_resolution_clock::now();
    SetColor(39);
    line_printer(50);
    if(no_of_drivers != 1)
    {
        Greedy_Cluster greedy(g1,src,delivery_locations,no_of_drivers,delivery_deadlines);
        greedy.calc_cluster_initilizers();
        greedy.populate_clusters();
        cout<<" Clusters generated by Greedy - \n";
        //greedy.display_clusters();
        greedy.job_scheduling_on_clusters();
        greedy.display_clusters();
        greedy.Display_cluster_path_traversed();
        auto stop2 = high_resolution_clock::now();
        auto duration2 = duration_cast<milliseconds>(stop2 - start2);
        greedy.calc_path_desc();
        cout<<endl << "Time taken by Greedy Clustering function : "<<duration2.count() << "ms\n";
    }
    int done;
    cin>>done;
    system("color 0A");
    return 0;
}

