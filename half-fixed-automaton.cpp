#include <iostream>
#include <string>
#include <set>
#include <map>
#include <vector>
#include <tuple>

using namespace std;

struct Automaton {
    set<string> alphabet;
    set<string> states;
    map<pair<string, string>, set<string>> transitions;
    set<string> finalStates;
    string starterState;


    Automaton(){
        const string& q0 = "q0";

        states.insert(q0);
        starterState = q0;
    }

    Automaton(
        const set<string>& Q,
        const set<string>& Sigma,
        const string& q0,
        const set<string>& F
    ) : states(Q),
        alphabet(Sigma),
        starterState(q0),
        finalStates(F)
    {
        if (Sigma.empty()) {
            throw runtime_error("The alphabet couldn't be empty");
        }
        if (Q.empty()) {
            throw runtime_error("The states set couldn't be empty");
        }
        if (Q.find(q0) == Q.end()) {
            throw runtime_error("The starter state '" + q0 + "' isn't in states set");
        }

        for (const string& state : Q) {
            if(state.empty()){
                throw runtime_error("States aren't enable to become unnamed states");
            }
        }

        for (const string& state : F) {
            if (Q.find(state) == Q.end()) {
                throw runtime_error("The final state '" + state + "' isn't in states set");
            }
        }
    }

    Automaton(
        const set<string>& Q,
        const set<string>& Sigma,
        const map<pair<string, string>, set<string>>& delta,
        const string& q0,
        const set<string>& F
    ) : states(Q),
        alphabet(Sigma),
        transitions(delta),
        starterState(q0),
        finalStates(F)
    {
        if (Sigma.empty()) {
            throw runtime_error("The alphabet couldn't be empty");
        }
        if (Q.empty()) {
            throw runtime_error("The states set couldn't be empty");
        }
        if (delta.empty()) {
            throw runtime_error("The transitions set couldn't be empty");
        }
        if (Q.find(q0) == Q.end()) {
            throw runtime_error("The starter state '" + q0 + "' isn't in states set");
        }

        for (const string& state : Q) {
            if(state.empty()){
                throw runtime_error("States aren't enable to become unnamed states");
            }
        }

        for (const string& state : F) {
            if (Q.find(state) == Q.end()) {
                throw runtime_error("The final state '" + state + "' isn't in states set");
            }
        }
    }

    void add_transition(const string& fromState, const string& symbol, const set<string>& toStates) {
        if (states.find(fromState) == states.end()) {
            throw runtime_error("The origin state: '" + fromState + "' isn't in states set.");
        }
        if (alphabet.find(symbol) == alphabet.end() && symbol != "") {
            throw runtime_error("The symbol '" + symbol + "' isn't in alphabet.");
        }

        for (const string& state : toStates) {
            if (states.find(state) == states.end()) {
                throw runtime_error("The state '" + state + "' isn't in states set");
            }
            if(state.empty()){
                throw runtime_error("States aren't enable to become unnamed states");
            }
        }

        pair<string, string> origin = {fromState, symbol};

        auto it = transitions.find(origin);
        if (it != transitions.end()) {
            it->second.insert(toStates.begin(), toStates.end());
        } else {
            transitions[origin] = toStates;
        }
    }

    void add_alphabet_symbol(const string& symbol){
        if(symbol.empty()){
            throw runtime_error("empty word can't become a alphabet symbol");
        }
        if(alphabet.find(symbol) != alphabet.end()){
            return ;
        }
        alphabet.insert(symbol);
    }

    void add_state(const string& state){
        if(state.empty()){
            throw runtime_error("States aren't enable to become unnamed states");
        }
        if (states.find(state) != states.end()){
            return ;
        }


        states.insert(state);
    }

    void add_final_state(const string& finalState){
        if(finalState.empty()){
            throw runtime_error("States aren't enable to become unnamed states");
        }
        if(states.find(finalState) == states.end()){
            throw runtime_error("The final state: '" + finalState + "' isn't in states set.");
        }
        if(finalStates.find(finalState) != finalStates.end()){
            return ;
        }

        finalStates.insert(finalState);
    }

    void set_stater_state(const string& state){
        if(states.find(state) == states.end()){
            throw runtime_error("The stater state: '" + state + "' isn't in states set.");
        }
        if( starterState == state){
            return ;
        }

        starterState = state;
    }

    set<string> closure(const set<string>& initialStates) const {
        set<string> closure = initialStates;
        vector<string> stack(initialStates.begin(), initialStates.end());

        while (!stack.empty()) {
            string currentState = stack.back();
            stack.pop_back();

            for(const string& symbol : alphabet){
                pair<string, string> key = {currentState, symbol};

                auto it = transitions.find(key);
                if (it != transitions.end()) {
                    for (const string& nextState : it->second) {
                        if (closure.find(nextState) == closure.end()) {
                            closure.insert(nextState);
                            stack.push_back(nextState);
                        }
                    }
                }
            }
        }

        return closure;
    }
    
    set<string> epsilon_closure(const set<string>& initialStates) const {
        set<string> closure = initialStates;
        vector<string> stack(initialStates.begin(), initialStates.end());

        while (!stack.empty()) {
            string currentState = stack.back();
            stack.pop_back();

            pair<string, string> epsilon_key = {currentState, ""};

            auto it = transitions.find(epsilon_key);
            if (it != transitions.end()) {
                for (const string& nextState : it->second) {
                    if (closure.find(nextState) == closure.end()) {
                        closure.insert(nextState);
                        stack.push_back(nextState);
                    }
                }
            }
        }

        return closure;
    }

    bool verify_word(const string& word){
        set<string> currentStates;
        currentStates.insert(starterState);
        currentStates = epsilon_closure(currentStates);

        for(const char& w : word){
            string wSymbol(1, w);

            if(alphabet.find(wSymbol) == alphabet.end()){
                return false;
            }else{
                set<string> nextStates;

                for(const string& currentState : currentStates){
                    pair<string, string> key = {currentState, wSymbol};

                    auto it = transitions.find(key);

                    if(it != transitions.end()){
                        nextStates.insert(it->second.begin(), it->second.end());
                    }
                }

                currentStates = epsilon_closure(nextStates);

                if (currentStates.empty()) {
                    return false;
                }
            }
        }

        for(const string& state : currentStates){
            if(finalStates.count(state)){
                return true;
            }
        }

        return false;
    }

    string automaton_type(){
        for(const auto& transition : transitions){
            const auto& transition_iniState = transition.first;
            const auto& iniState_symbol = transition_iniState.second;
            const auto& transition_finalStates = transition.second;

            if(iniState_symbol == ""){
                return "afn_epsilon";
            }else if (transition_finalStates.size() > 1){
                return "afn";
            }
        }

        return "afd";

    }



    Automaton afn_epsilon_to_afn(){
        if(automaton_type() != "afn_epsilon"){
            return ;
        }

        const set<string>& Q = states;
        const set<string>& Sigma = alphabet;
        map<pair<string, string>, set<string>> delta;
        const string& q0 = starterState;
        set<string> F;

        for(const string& state : states){
            const set<string>& key_closure = {state};
            const auto& closure = epsilon_closure(key_closure);

            for(const string& symbol : alphabet){
                set<string> union_of_destinations;

                for(const auto& item : closure){

                    pair<string, string> key = {item, symbol};
                    const auto& it = transitions.find(key);

                    if(it != transitions.end()){
                        union_of_destinations.insert(it->second.begin(), it->second.end());
                    }
                }

                if (!union_of_destinations.empty()) {
                    const auto& destination = epsilon_closure(union_of_destinations);

                    pair<string, string> key = {state, symbol};
                    delta[key] = destination;
                }
            }
        }

        for(const string& state : states){
            const set<string>& key_closure = {{state}};
            const auto& closure = epsilon_closure(key_closure);

            for(const auto& item : closure){
                if(finalStates.find(item) != finalStates.end()){
                    F.insert(state);
                    break;
                }
            }
        }

        Automaton automaton(Q, Sigma, delta, q0, F);
        return automaton;
    }

    Automaton afn_to_afd(){
        if(automaton_type() != "afn"){
            return ;
        }

        set<string> Q;
        const set<string>& Sigma = alphabet;
        map<pair<string, string>, set<string>> delta;
        const string& q0 = starterState;
        set<string> F;

        set<string> currentState = {q0};

        for(const string& symbol : alphabet){
            pair<string, string> key = (currentState, symbol);
            const auto& it = transitions.find(key);
        }

        Automaton automaton(Q, Sigma, delta, q0, F);
        return automaton;
    }

    Automaton minimize(Automaton automaton){
        return automaton;
    }
};

int main()
{
    cout << "Simulação de Autômato Finito Não-Determinístico (AFN)\n\n";

    set<string> Q = {"q0", "q1", "q2", "q3", "q4"};
    set<string> Sigma = {"a", "b"};
    string q0 = "q0";
    set<string> F = {"q0"};

    Automaton afn(Q, Sigma, q0, F);

    afn.add_transition("q0", "a", {"q1"});
    afn.add_transition("q0", "b", {"q3"});
    afn.add_transition("q1", "a", {"q2"});
    afn.add_transition("q3", "b", {"q4"});
    afn.add_transition("q1", "b", {"q0"});
    afn.add_transition("q2", "b", {"q1"});
    afn.add_transition("q3", "a", {"q0"});
    afn.add_transition("q4", "a", {"q3"});

    vector<string> testWords = {"baba", "a", "b", "aba", "bab", "abb", "ab", "", "babababababababbaaba"};

    cout << "Testando palavras:\n";
    for (const string& word : testWords) {
        string status = afn.verify_word(word) ? "ACEITA" : "REJEITADA";
        cout << "Palavra: \"" << word << "\" -> " << status << "\n";
    }

    cout <<" Esse automato é: " << afn.automaton_type() << "\n";

    return 0;
}
