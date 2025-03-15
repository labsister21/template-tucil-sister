#include <iostream>
#include <iterator>
#include <string>
#include <vector>
#include <set>
#include <limits>
#include <numeric>
#include <cmath>
#include <algorithm>

using namespace std;

const double _INFINITY = numeric_limits<double>::infinity();

const int _MAX_DISTANCE = 5;

void update_row(vector<vector<double>> &D, const int i, const int n, const int k, const double r)
{
    vector<double> &row = D[i];
    for (int j = 0; j < n; j++)
    {
        if (i == j)
            continue;

        double a = D[i][k];
        double b = D[k][j];
        double t = pow((pow(a, r) + pow(b, r)), (1 / r));

        if (t < row[j])
        {
            row[j] = t;
        }
    }

    return;
}

void floyd_warshall(vector<vector<double>> &D, int q, int r)
{
    int n = D.size();
    for (int k = 0; k < min(q + 1, n); k++)
    {
        for (int i = 0; i < n; i++)
        {
            update_row(D, i, n, k, r);
        }
    }

    return;
}

vector<vector<double>> pathfinder_network(const vector<vector<double>> &graph, int q, int r)
{
    int n = graph.size();

    // Create a deep copy of the original graph
    vector<vector<double>> D(n);
    for (int i = 0; i < n; i++)
    {
        D[i] = vector<double>(graph[i]);
    }
    // Find shortest path between nodes
    floyd_warshall(D, q, r);

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            if (graph[i][j] < D[i][j])
            {
                D[i][j] = graph[i][j];
            }
        }
    }

    return D;
}

double cosine_similarity(const vector<double> &a, const vector<double> &b)
{
    double dot = inner_product(a.begin(), a.end(), b.begin(), 0.0);
    double n1 = sqrt(inner_product(a.begin(), a.end(), a.begin(), 0.0));
    double n2 = sqrt(inner_product(b.begin(), b.end(), b.begin(), 0.0));
    if (n1 == 0 || n2 == 0)
        return 0;
    return dot / (n1 * n2);
}

int main()
{
    // Parse string from input
    istream_iterator<string> it(cin), end;
    vector<string> text(it, end);

    // TIMING STARTS HERE

    // Transform into set to eliminate duplicate and sort the content
    set<string> wordSet(text.begin(), text.end());

    // Initialize empty graph
    int n = wordSet.size();
    vector<vector<double>> graph(n);
    for (auto it = graph.begin(); it != graph.end(); ++it)
    {
        *it = vector<double>(n);
    }

    // Create graph
    for (int i = 0; i < text.size(); i++)
    {
        // find the index of the word in the ordered set
        int token_i = distance(wordSet.begin(), wordSet.find(text[i]));

        // find the maximum neighboring word before end of text
        int max_neighbor = min((int)text.size(), (i + 1 + _MAX_DISTANCE));
        for (int j = i + 1; j < max_neighbor; j++)
        {
            int token_j = distance(wordSet.begin(), wordSet.find(text[j]));
            if (token_i != token_j)
            {
                graph[token_i][token_j]++;
                graph[token_j][token_i]++;
            }
        }
    }

    vector<vector<double>> D(n);
    for (auto it = D.begin(); it != D.end(); ++it)
    {
        *it = vector<double>(n);
    }
    for (int i = 0; i < n; i++)
    {
        D[i][i] = 0;
        for (int j = i + 1; j < n; j++)
        {
            double similarity = cosine_similarity(graph[i], graph[j]);
            double inverse_similarity;
            if (similarity == 0)
            {
                inverse_similarity = _INFINITY;
            }
            else
            {
                inverse_similarity = 1 - similarity;
            }
            D[i][j] = inverse_similarity;
            D[j][i] = inverse_similarity;
        }
    }

    int q = n - 1; // number of iteration
    double r = 1;  // Minowski distance power

    vector<vector<double>> pf_net = pathfinder_network(D, q, r);

    for (auto i : pf_net)
    {
        for (auto j : i)
        {
            cout << j << ' ';
        }
        cout << '\n';
    }
}
