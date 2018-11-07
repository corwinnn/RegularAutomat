#include "Automaton.h"

void testEq(int myAns, int ans) {
    if (myAns == ans) {
        std::cout << "OK";
    }
    else {
        std::cout << myAns << " ";
        std::cout << "FAILED";
    }
    std::cout << std::endl;
}

void test() {
    string s = "acb..bab.c.*.ab.ba.+.+*a.";
    RegularAutomaton a(s);
    testEq(a.bfs('b', 3), 7);
    testEq(a.bfs('b', 4), 7);
    testEq(a.bfs('b', 5), 10);
    testEq(a.bfs('b', 6), 10);
    testEq(a.bfs('b', 7), 13);
    testEq(a.bfs('c', 5), 16);
    testEq(a.bfs('a', 5), 13);
    s = "ab+c.aba.*.bac.+.+*";
    a = RegularAutomaton(s);
    testEq(a.bfs('b', 3), 6);
    testEq(a.bfs('b', 4), 8);
    testEq(a.bfs('b', 5), 10);
    testEq(a.bfs('b', 6), 12);
    testEq(a.bfs('b', 7), 14);
    testEq(a.bfs('c', 5), 10);
    testEq(a.bfs('a', 5), 8);


}

int main() {
    // test();
    std::string s;
    while (std::cin >> s) {
        char c;
        int k;
        std::cin >> c >> k;
        RegularAutomaton ra = RegularAutomaton(s);
        std::cout << ra.bfs(c, k) << std::endl;
    }
    return 0;
}