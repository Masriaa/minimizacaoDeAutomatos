#ifndef AUTOMATON_H
#define AUTOMATON_H

#include <iostream>
#include <string>
#include <set>
#include <map>
#include <vector>
#include <tuple>
#include <queue>

using namespace std;

class Automaton {
public:
    set<string> alphabet;
    set<string> states;
    map<pair<string, string>, set<string>> transitions;
    set<string> finalStates;
    string starterState;
    
    Automaton();
    Automaton(const set<string>& Q, const set<string>& Sigma, const string& q0, const set<string>& F);
    Automaton(const set<string>& Q, const set<string>& Sigma, const map<pair<string, string>, set<string>>& delta, const string& q0, const set<string>& F);

    void add_transition(const string& fromState, const string& symbol, const set<string>& toStates);
    void add_alphabet_symbol(const string& symbol);
    void add_state(const string& state);
    void add_final_state(const string& finalState);
    void set_stater_state(const string& state);

    set<string> closure(const set<string>& initialStates) const;
    set<string> epsilon_closure(const set<string>& initialStates) const;
    bool verify_word(const string& word);
    string automaton_type();

    Automaton afn_epsilon_to_afn();
    Automaton afn_to_afd();
    Automaton minimize();
    Automaton minimize_hopcrof_moore_algorithm();
};

#endif
