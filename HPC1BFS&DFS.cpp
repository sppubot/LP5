#include <iostream>
#include <omp.h>
#include <stack>
#include <queue>
#include <chrono>
#include <vector>
using namespace std;
using namespace std::chrono;

void DFS(vector<vector<int>> &adj_matrix, int n, int start)
{
    vector<bool> visited(n, false);
    stack<int> s;
    visited[start] = true;
    s.push(start);

    while (!s.empty())
    {
        int node = s.top();
        s.pop();
        cout << node << " ";

        for (int i = 0; i < n; ++i)
        {
            if (adj_matrix[node][i] == 1 && !visited[i])
            {
                s.push(i);
                visited[i] = true;
            }
        }
    }
}

void BFS(vector<vector<int>> &adj_matrix, int n, int start)
{
    vector<bool> visited(n, false);
    queue<int> q;
    visited[start] = true;
    q.push(start);
    while (!q.empty())
    {
        int node = q.front();
        q.pop();
        cout << node << " ";
        for (int i = 0; i < n; ++i)
        {
            if (adj_matrix[node][i] == 1 && !visited[i])
            {
                q.push(i);
                visited[i] = true;
            }
        }
    }
}

// Parallel BFS
void parallel_BFS(const vector<vector<int>> &adj_matrix, int n, int start)
{
    vector<bool> visited(n, false);
    queue<int> q;
    visited[start] = true;
    q.push(start);

#pragma omp parallel // creates team of threads and all threads execute the code
    {
#pragma omp single // only single thread will execute
        {
            while (!q.empty())
            {
                int node;
#pragma omp critical
                {
                    if (!q.empty())
                    {
                        node = q.front();
                        q.pop();
                    }
                    else
                    {
                        node = -1;
                    }
                }

                if (node != -1)
                {
                    cout << node << " ";

#pragma omp parallel for
                    for (int i = 0; i < n; ++i)
                    {
                        if (adj_matrix[node][i] == 1 && !visited[i])
                        {
#pragma omp critical
                            {
                                if (!visited[i])
                                {
                                    visited[i] = true;
                                    q.push(i);
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}
// void par_dfs(vector<vector<int>> &adj, int n, int start)
// {
//     stack<int> s;
//     vector<bool> visited(n, false);
//     visited[start] = true;
//     s.push(start);
//     #pragma omp parallel
//     {
//         #pragma omp single
//         {
//             while ((!s.empty()))
//             {
//                 int node;
//                 #pragma omp critical
//                 {
//                     if (!s.empty())
//                     {
//                         node = s.top();
//                         s.pop();
//                     }
//                     else
//                     {
//                         node = -1;
//                     }
//                 }
//                 if(node!=-1)
//                 {
//                 #pragma omp parallel for
//                 for(int i=0;i<n;++i){
//                     if(adj[node][i]==1 && !visited[i]){
//                         #pragma omp critical
//                         {
//                             s.push(i);
//                             visited[i]=true;
//                         }
//                     }
//                 }
//                  }
//             }
//         }
//     }
// }
// Parallel DFS
void parallel_DFS(const vector<vector<int>> &adj_matrix, int n, int start)
{
    vector<bool> visited(n, false);
    stack<int> s;
    visited[start] = true;
    s.push(start);

    #pragma omp parallel
    {
        #pragma omp single
        {
            while (!s.empty())
            {
                int node;
                #pragma omp critical
                {
                    if (!s.empty())
                    {
                        node = s.top();
                        s.pop();
                    }
                    else
                    {
                        node = -1;
                    }
                }

                if (node != -1)
                {
                    cout << node << " ";

                    #pragma omp parallel for
                    for (int i = 0; i < n; ++i)
                    {
                        if (adj_matrix[node][i] == 1 && !visited[i])
                        {
                            #pragma omp critical
                            {
                                if (!visited[i])
                                {
                                    visited[i] = true;
                                    s.push(i);
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

void add(vector<vector<int>> &adj_matrix, int u, int v)
{
    adj_matrix[u][v] = 1;
    adj_matrix[v][u] = 1;
}

int main()
{
    int n;

    cout << "Enter number of nodes: ";
    cin >> n;
    vector<vector<int>> adj_matrix(n, vector<int>(n, 0)); //(n,vector<int>(n,0)); important

    // Randomly generating the graph
    for (int i = 0; i < n; ++i)
    {
        int u = rand() % n;
        int v = rand() % n;
        add(adj_matrix, u, v);
    }

    int start = adj_matrix[0][0]; // Just a starting point for demonstration

    // Sequential BFS
    cout << "\nSequential BFS: \n";
    // auto start_bfs = high_resolution_clock::now();
    double start_bfs=omp_get_wtime();
    BFS(adj_matrix, n, start);
    // auto end_bfs = high_resolution_clock::now();
    double end_bfs=omp_get_wtime();
    // double seq_bfs = duration<double, milli>(end_bfs - start_bfs).count();
    double seq_bfs=end_bfs-start_bfs;
    cout << "\nTime Taken: " << seq_bfs << " ms\n\n";

    // Sequential DFS
    cout << "\nSequential DFS: \n";
    // auto dfs_start = high_resolution_clock::now();
    double dfs_start=omp_get_wtime();
    DFS(adj_matrix, n, start);
    // auto dfs_end = high_resolution_clock::now();
    double dfs_end=omp_get_wtime();
    // double seq_dfs = duration<double, milli>(dfs_end - dfs_start).count();
    double seq_dfs=dfs_end-dfs_start;
    cout << "\nTime Taken: " << seq_dfs << " ms\n\n";

    // Parallel BFS
    cout << "\nParallel BFS: \n";
    // auto start_parallel_bfs = high_resolution_clock::now();
    double start_parallel_bfs=omp_get_wtime();
    parallel_BFS(adj_matrix, n, start);
    // auto end_parallel_bfs = high_resolution_clock::now();
    double end_parallel_bfs=omp_get_wtime();
    // double par_bfs = duration<double, milli>(end_parallel_bfs - start_parallel_bfs).count();
    double par_bfs=end_parallel_bfs-start_parallel_bfs;
    cout << "\nTime Taken: " << par_bfs << " ms";
    cout << "\nSpeedup Factor: " << seq_bfs / par_bfs << "\n\n";

    // Parallel DFS
    cout << "\nParallel DFS: \n";
    // auto start_parallel_dfs = high_resolution_clock::now();
    double start_parallel_dfs=omp_get_wtime();
    parallel_DFS(adj_matrix, n, start);
    // auto end_parallel_dfs = high_resolution_clock::now();
    double end_parallel_dfs=omp_get_wtime();
    // double par_dfs = duration<double, milli>(end_parallel_dfs - start_parallel_dfs).count();
    double par_dfs=end_parallel_dfs-start_parallel_dfs;
    cout << "\nTime Taken: " << par_dfs << " ms";
    cout << "\nSpeedup Factor: " << seq_dfs / par_dfs << "\n\n";

    return 0;
}