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

    vector<int> get_path(int v1,int v2)
    {
        vector<int> path;
        path.push_back(v1);
        vector<int> calc = calc_path(v1,v2);
        path.insert(path.end(),calc.begin(),calc.end());
        path.push_back(v2);
        return path;
    }

};




int main()
{
    int no_of_vertex;
    int no_of_Edges;
    cin>>no_of_vertex>>no_of_Edges;
    map<int,vector<pair<int,int>>> graph;

    for(int i=0; i<no_of_Edges; i++)
    {
        int a,b,w;
        cin>>a>>b>>w;
        graph[a].push_back({b,w});
        graph[b].push_back({a,w});
    }

    Shortest_path_finder g1(graph);
    vector_printer(g1.get_path(22,15));
    return 0;
}
