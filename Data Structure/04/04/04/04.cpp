//#include <iostream>
//#include <list>
//#include <queue>
//#include <vector>
//#include <map>
//
//using namespace std;
//
//// 图的邻接表表示
//class Graph {
//    int V; // 顶点数
//    map<char, list<char>> adj; // 邻接表
//
//public:
//    Graph(int V) {
//        this->V = V;
//    }
//
//    // 添加边
//    void addEdge(char v, char w) {
//        adj[v].push_back(w);
//        adj[w].push_back(v); // 因为是无向图
//    }
//
//    // 打印邻接表
//    void printAdjList() {
//        for (auto const& pair : adj) {
//            char v = pair.first;
//            cout << v;
//            for (auto x : adj[v])
//                cout << " -> " << x;
//            cout << endl;
//        }
//    }
//
//    // 深度优先搜索
//    void DFSUtil(char v, map<char, bool>& visited, vector<char>& result) {
//        visited[v] = true;
//        result.push_back(v);
//
//        for (auto i = adj[v].begin(); i != adj[v].end(); ++i)
//            if (!visited[*i])
//                DFSUtil(*i, visited, result);
//    }
//
//    vector<char> DFS(char v) {
//        vector<char> result;
//        map<char, bool> visited;
//        for (auto const& pair : adj)
//            visited[pair.first] = false;
//
//        DFSUtil(v, visited, result);
//        return result;
//    }
//
//    // 广度优先搜索
//    vector<char> BFS(char s) {
//        vector<char> result;
//        map<char, bool> visited;
//        for (auto const& pair : adj)
//            visited[pair.first] = false;
//
//        queue<char> queue;
//        visited[s] = true;
//        queue.push(s);
//
//        while (!queue.empty()) {
//            s = queue.front();
//            result.push_back(s);
//            queue.pop();
//
//            for (auto i = adj[s].begin(); i != adj[s].end(); ++i) {
//                if (!visited[*i]) {
//                    visited[*i] = true;
//                    queue.push(*i);
//                }
//            }
//        }
//        return result;
//    }
//};
//
//int main() {
//    Graph g(9);
//    g.addEdge('A', 'B');
//    g.addEdge('A', 'D');
//    g.addEdge('A', 'E');
//    g.addEdge('A', 'F');
//    g.addEdge('B', 'C');
//    g.addEdge('C', 'D');
//    g.addEdge('E', 'D');
//    g.addEdge('F', 'G');
//    g.addEdge('G', 'H');
//    g.addEdge('G', 'I');
//    g.addEdge('H', 'I');
//
//    cout << "Graph Adjacency List:" << endl;
//    g.printAdjList();
//    cout << endl;
//
//    char startVertex = 'A';
//
//    cout << "Depth First Traversal (starting from vertex " << startVertex << "): ";
//    vector<char> dfsResult = g.DFS(startVertex);
//    for (char v : dfsResult)
//        cout << v << " ";
//    cout << endl;
//
//    cout << "Breadth First Traversal (starting from vertex " << startVertex << "): ";
//    vector<char> bfsResult = g.BFS(startVertex);
//    for (char v : bfsResult)
//        cout << v << " ";
//    cout << endl;
//
//    return 0;
//


#include <iostream>
#include <list>
#include <queue>
#include <vector>
#include <map>

using namespace std;

class Graph {
    int V;
    map<char, list<char>> adj;

public:
    Graph(int V) {
        this->V = V;
    }

    void addEdge(char v, char w) {
        adj[v].push_back(w);
        adj[w].push_back(v);
    }

    void printAdjList() {
        cout << "Adjacency List:" << endl;
        for (auto const& pair : adj) {
            char v = pair.first;
            cout << v << " -> ";
            for (auto x : adj[v])
                cout << x << " ";
            cout << endl;
        }
    }

    void DFSUtil(char v, map<char, bool>& visited, vector<char>& result) {
        visited[v] = true;
        result.push_back(v);

        for (auto i = adj[v].begin(); i != adj[v].end(); ++i)
            if (!visited[*i])
                DFSUtil(*i, visited, result);
    }

    vector<char> DFS(char v) {
        vector<char> result;
        map<char, bool> visited;
        for (auto const& pair : adj)
            visited[pair.first] = false;

        DFSUtil(v, visited, result);
        return result;
    }

    vector<char> BFS(char s) {
        vector<char> result;
        map<char, bool> visited;
        for (auto const& pair : adj)
            visited[pair.first] = false;

        queue<char> queue;
        visited[s] = true;
        queue.push(s);

        while (!queue.empty()) {
            s = queue.front();
            result.push_back(s);
            queue.pop();

            for (auto i = adj[s].begin(); i != adj[s].end(); ++i) {
                if (!visited[*i]) {
                    visited[*i] = true;
                    queue.push(*i);
                }
            }
        }
        return result;
    }
};

int main() {
    Graph g(9);
    g.addEdge('A', 'B');
    g.addEdge('A', 'D');
    g.addEdge('A', 'E');
    g.addEdge('A', 'F');
    g.addEdge('B', 'C');
    g.addEdge('C', 'D');
    g.addEdge('E', 'D');
    g.addEdge('F', 'G');
    g.addEdge('G', 'H');
    g.addEdge('G', 'I');
    g.addEdge('H', 'I');

    g.printAdjList();

    char startVertex = 'A';

    cout << "\nDepth First Traversal (starting from vertex " << startVertex << "): ";
    vector<char> dfsResult = g.DFS(startVertex);
    for (char v : dfsResult)
        cout << v << " ";

    cout << "\nBreadth First Traversal (starting from vertex " << startVertex << "): ";
    vector<char> bfsResult = g.BFS(startVertex);
    for (char v : bfsResult)
        cout << v << " ";

    cout << endl;

    return 0;
}
