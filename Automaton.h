    #include <iostream>
    #include <vector>
    #include <stack>
    #include <map>
    #include <cstdio>
    #include <queue>
    #include <cassert>

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
                number(n) {}
    };


    struct Expression {
        int begin;
        int end;

        Expression(int begin, int end) :
                begin(begin),
                end(end) {}
    };

    class RegularAutomaton {

    public:

        explicit RegularAutomaton(const string &regular) {
            m_graph.resize(regular.size() * MAX_NEW_VERTICES_BY_SYMBOL);
            m_TGraph.resize(regular.size() * MAX_NEW_VERTICES_BY_SYMBOL);
            for (char c : regular) {
                addSymbol(c);
            }
            assert(m_expresions.size() == 1);
            m_terminals[m_expresions.top().end] = 1;
            getRidOfEpsilon();
            // showTerminals();
            // showEdges();
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
            return m_expresions.top().begin;
        }

        void makeWords(int v, string s, int len = 5) {
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
            int s = m_expresions.top().begin;
            std::queue<AutomatonVertex> q;
            q.push(AutomatonVertex(s, 0, 0));
            while (!q.empty()) {
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
        std::stack<Expression> m_expresions;
        std::map<int, int> m_terminals;

        int newVertex() {
            return ++m_vertexNumber;
        }

        void drawEdge(int vertexFrom, int vertexTo, char symbol) {
            m_graph[vertexFrom][vertexTo] = symbol;
            m_TGraph[vertexTo][vertexFrom] = symbol;
        }

        void addSymbol(char newSymbol) {
            switch (newSymbol) {
                case '.' : {
                    assert(m_expresions.size() >= 2);
                    Expression secondExpression = m_expresions.top();
                    m_expresions.pop();
                    Expression firstExpression = m_expresions.top();
                    m_expresions.pop();
                    drawEdge(firstExpression.end, secondExpression.begin, '1');
                    m_expresions.emplace(firstExpression.begin, secondExpression.end);
                    break;
                }


                case '+' : {
                    assert(m_expresions.size() >= 2);
                    Expression secondExpression = m_expresions.top();
                    m_expresions.pop();
                    Expression firstExpression = m_expresions.top();
                    m_expresions.pop();
                    int newVertexForBegin = newVertex();
                    int newVertexForEnd = newVertex();
                    drawEdge(newVertexForBegin, firstExpression.begin, '1');
                    drawEdge(newVertexForBegin, secondExpression.begin, '1');
                    drawEdge(firstExpression.end, newVertexForEnd, '1');
                    drawEdge(secondExpression.end, newVertexForEnd, '1');
                    m_expresions.emplace(newVertexForBegin, newVertexForEnd);
                    break;
                }

                case '*' : {
                    assert(m_expresions.size() >= 1);
                    Expression firstExpression = m_expresions.top();
                    m_expresions.pop();
                    int newVertexForBegin = newVertex();
                    int newVertexForEnd = newVertex();
                    drawEdge(firstExpression.end, firstExpression.begin, '1');
                    drawEdge(firstExpression.end, newVertexForEnd, '1');
                    drawEdge(newVertexForBegin, firstExpression.begin, '1');
                    drawEdge(newVertexForBegin, newVertexForEnd, '1');
                    m_expresions.emplace(newVertexForBegin, newVertexForEnd);
                    break;
                }

                default: {
                    int newVertexForBegin = newVertex();
                    int newVertexForEnd = newVertex();
                    drawEdge(newVertexForBegin, newVertexForEnd, newSymbol);
                    m_expresions.emplace(newVertexForBegin, newVertexForEnd);

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
