#include <iostream>
#include <vector>
#include <stack>
#include <map>
#include <cstdio>
#include <queue>

using std::vector;
using std::string;

const int MAX_NEW_VERTICES_BY_SYMBOL = 5;

struct AutomatonVertex {
    int vertex;
    int length;
    int number;
    AutomatonVertex(int v, int l, int n) :
            vertex(v),
            length(l),
            number(n)
    {}
};

class RegularAutomaton {

public:

    explicit RegularAutomaton(const string &regular) {
        m_graph.resize(regular.size() * MAX_NEW_VERTICES_BY_SYMBOL);
        m_TGraph.resize(regular.size() * MAX_NEW_VERTICES_BY_SYMBOL);
        for (char c : regular) {
            addSymbol(c);
        }
        m_terminals[m_endVertexOfExpression.top()] = 1;
        getRidOfEpsilon();
        showTerminals();
        showEdges();
    }

    void showEdges() const {
        for (int i = 0; i <= m_vertexNumber; i++) {
            for (auto j : m_graph[i]) {
                std::cout << i << " " << j.first << " " << j.second << std::endl;
            }
        }
        std::cout << std::endl;
    }

    void showTerminals() {
        for (auto i : m_terminals) {
            std::cout << i.first << " ";
        }
        std::cout << std::endl;
    }

    int getStartVertice() {
        return m_beginVertexOfExpression.top();
    }

    void makeWords(int v, string s ,int len=5) {
        if (m_terminals.find(v) != m_terminals.end()) {
            std::cout << s << std::endl;
        }
        if (len == 0) return;
        for (auto i : m_graph[v]) {
            makeWords(i.first, s + i.second, len - 1);
        }
    }

    int bfs(char c, int number) const {
        vector<std::map<int, int>> lengthByNumber(m_vertexNumber + 1);
        int s  = m_beginVertexOfExpression.top();
        std::queue<AutomatonVertex> q; //vertex length number
        q.push(AutomatonVertex(s, 0, 0));
        while(!q.empty()) {
            AutomatonVertex v = q.front();
            q.pop();
            if (v.number == number && m_terminals.find(v.vertex) != m_terminals.end()) {
                return v.length;
            }
            lengthByNumber[v.vertex][v.number] = v.length;
            for (auto i : m_graph[v.vertex]) {
                int delta = (i.second == c) ? 1 : 0;
                if (lengthByNumber[i.first].find(v.number + delta) == lengthByNumber[i.first].end() &&
                    v.number + delta <= number) {
                    q.push(AutomatonVertex(i.first, v.length + 1, v.number + delta));
                }
            }
        }
        return -1;
    }

private:

    vector<std::map<int, char>> m_graph;
    vector<std::map<int, char>> m_TGraph;
    int m_vertexNumber = 0;
    std::stack<int> m_beginVertexOfExpression;
    std::stack<int> m_endVertexOfExpression;
    std::map<int, int> m_terminals;

    int newVertex() {
        return ++m_vertexNumber;
    }

    void addSymbol(char newSymbol) {
        switch (newSymbol) {
            case '.' : {
                std::pair<int, int> secondExpression = std::make_pair(m_beginVertexOfExpression.top(),
                                                                      m_endVertexOfExpression.top());
                m_beginVertexOfExpression.pop();
                m_endVertexOfExpression.pop();
                std::pair<int, int> firstExpression = std::make_pair(m_beginVertexOfExpression.top(),
                                                                     m_endVertexOfExpression.top());
                m_beginVertexOfExpression.pop();
                m_endVertexOfExpression.pop();
                m_graph[firstExpression.second][secondExpression.first] = '1';
                m_TGraph[secondExpression.first][firstExpression.second] = '1';
                m_beginVertexOfExpression.push(firstExpression.first);
                m_endVertexOfExpression.push(secondExpression.second);
                break;
            }

            case '+' : {
                std::pair<int, int> secondExpression = std::make_pair(m_beginVertexOfExpression.top(),
                                                                      m_endVertexOfExpression.top());
                m_beginVertexOfExpression.pop();
                m_endVertexOfExpression.pop();
                std::pair<int, int> firstExpression = std::make_pair(m_beginVertexOfExpression.top(),
                                                                     m_endVertexOfExpression.top());
                m_beginVertexOfExpression.pop();
                m_endVertexOfExpression.pop();
                int newVertexForBegin = newVertex();
                int newVertexForEnd = newVertex();
                m_graph[newVertexForBegin][firstExpression.first] = '1';
                m_graph[newVertexForBegin][secondExpression.first] = '1';
                m_graph[firstExpression.second][newVertexForEnd] = '1';
                m_graph[secondExpression.second][newVertexForEnd] = '1';
                m_TGraph[firstExpression.first][newVertexForBegin] = '1';
                m_TGraph[secondExpression.first][newVertexForBegin] = '1';
                m_TGraph[newVertexForEnd][firstExpression.second] = '1';
                m_TGraph[newVertexForEnd][secondExpression.second] = '1';
                m_beginVertexOfExpression.push(newVertexForBegin);
                m_endVertexOfExpression.push(newVertexForEnd);
                break;
            }

            case '*' : {
                std::pair<int, int> firstExpression = std::make_pair(m_beginVertexOfExpression.top(),
                                                                     m_endVertexOfExpression.top());
                m_beginVertexOfExpression.pop();
                m_endVertexOfExpression.pop();
                int newVertexForBegin = newVertex();
                int newVertexForEnd = newVertex();
                m_graph[firstExpression.second][firstExpression.first] = '1';
                m_graph[firstExpression.second][newVertexForEnd] = '1';
                m_graph[newVertexForBegin][firstExpression.first] = '1';
                m_graph[newVertexForBegin][newVertexForEnd] = '1';
                m_TGraph[newVertexForEnd][newVertexForBegin] = '1';
                m_TGraph[firstExpression.first][firstExpression.second] = '1';
                m_TGraph[newVertexForEnd][firstExpression.second] = '1';
                m_TGraph[firstExpression.first][newVertexForBegin] = '1';
                m_beginVertexOfExpression.push(newVertexForBegin);
                m_endVertexOfExpression.push(newVertexForEnd);
                break;
            }

            default: {
                int newVertexForBegin = newVertex();
                int newVertexForEnd = newVertex();
                m_graph[newVertexForBegin][newVertexForEnd] = newSymbol;
                m_TGraph[newVertexForEnd][newVertexForBegin] = newSymbol;
                m_beginVertexOfExpression.push(newVertexForBegin);
                m_endVertexOfExpression.push(newVertexForEnd);

            }
        }
    }

    bool allEpsilonIn(int vertex) const {
        bool ans = false;
        for (auto i : m_TGraph[vertex]) {
            ans = true;
            if (i.second != '1')
                return false;
        }
        return ans;
    }

    bool anyEpsilonIn(int vertex) const {
        for (auto i : m_TGraph[vertex]) {
            if (i.second == '1')
                return true;
        }
        return false;
    }

    void getRidOfEpsilon() {
        for (int i = 0; i <= m_vertexNumber; i++) {
            if (allEpsilonIn(i)) {
                bool isLastTerminal = (m_terminals.find(i) != m_terminals.end());
                if (isLastTerminal) {
                    m_terminals.clear();
                }
                for (auto vertexFrom : m_TGraph[i]) {
                    if (isLastTerminal) {
                        m_terminals[vertexFrom.first] = 1;
                    }
                    for (auto vertexTo : m_graph[i]) {
                        m_graph[vertexFrom.first][vertexTo.first] = vertexTo.second;
                        m_TGraph[vertexTo.first][vertexFrom.first] = vertexTo.second;
                    }
                }
                for (auto vertexTo : m_graph[i]) {
                    m_TGraph[vertexTo.first].erase(i);
                }
                m_graph[i].clear();
                for (auto vertexFrom : m_TGraph[i]) {
                    m_graph[vertexFrom.first].erase(i);
                }
                m_TGraph[i].clear();
            }
        }

        for (int i = 0; i <= m_vertexNumber; i++) {
            if (anyEpsilonIn(i)) {
                vector<int> toErase;
                for (auto vertexFrom : m_TGraph[i]) {
                    if (vertexFrom.second == '1') {
                        toErase.push_back(vertexFrom.first);
                        for (auto vertexTo : m_graph[i]) {
                            m_graph[vertexFrom.first][vertexTo.first] = vertexTo.second;
                            m_TGraph[vertexTo.first][vertexFrom.first] = vertexTo.second;
                            if (m_terminals.find(i) != m_terminals.end()) {
                                m_terminals[vertexFrom.first] = 1;
                            }
                        }
                    }
                }
                for (int erased : toErase) {
                    m_TGraph[i].erase(erased);
                    m_graph[erased].erase(i);
                }

            }
        }
    }

};

int main() {
    std::freopen("in.txt", "r", stdin);
    std::string s;
    std::cin >> s;
    char c;
    int k;
    // std::cin >> c >> k;
    RegularAutomaton ra = RegularAutomaton(s);
    for (int i = 0; i <= 10; i++) {
        std::cout << 'a' << " " << i << " " << ra.bfs('a', i) << std::endl;
        std::cout << 'b' << " " << i << " " << ra.bfs('b', i) << std::endl;
        std::cout << 'c' << " " << i << " " << ra.bfs('c', i) << std::endl;
    }
    // std::cout << ra.bfs(c, k);
    return 0;
}