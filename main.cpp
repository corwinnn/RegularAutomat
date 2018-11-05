#include <iostream>
#include <vector>
#include <stack>
#include <map>
#include <cstdio>

using std::vector;
using std::string;

const int MAX_NEW_EDGES_BY_SYMBOL = 5;

class RegularAutomaton {

public:

    explicit RegularAutomaton(const string& regular) {
        m_graph.resize(regular.size() * MAX_NEW_EDGES_BY_SYMBOL);
        for (char c : regular) {
            addSymbol(c);
        }
    }

    void showEdges() const {
        for (int i = 0; i <= m_vertexNumber; i++) {
            for (auto j : m_graph[i]) {
                std::cout << i << " " << j.first << " " << j.second << std::endl;
            }
        }
    }


private:

    vector<std::map<int, char>> m_graph;
    int m_vertexNumber = 0;
    std::stack<int>  m_beginVertexOfExpression;
    std::stack<int>  m_endVertexOfExpression;

    int newVertex() {
        return ++m_vertexNumber;
    }

    void addSymbol(char newSymbol) {
        switch(newSymbol) {
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
                m_graph[firstExpression.second][newVertexForEnd] = '1';
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
                m_beginVertexOfExpression.push(newVertexForBegin);
                m_endVertexOfExpression.push(newVertexForEnd);
                break;
            }
            
            default: {
                int newVertexForBegin = newVertex();
                int newVertexForEnd = newVertex();
                m_graph[newVertexForBegin][newVertexForEnd] = newSymbol;
                m_beginVertexOfExpression.push(newVertexForBegin);
                m_endVertexOfExpression.push(newVertexForEnd);

            }
        }
    }

};

int main() {
    std::freopen("in.txt", "r", stdin);
    std::string s;
    std::cin >> s;
    RegularAutomaton ra = RegularAutomaton(s);
    ra.showEdges();
    return 0;
}