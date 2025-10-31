int main() {
    Automaton afd;

    afd.add_state("q0");
    afd.add_state("q1");
    afd.add_state("q5");
    afd.add_state("q6");
    afd.add_state("q2");
    afd.add_state("q3");

    afd.add_alphabet_symbol("0");
    afd.add_alphabet_symbol("1");

    afd.set_stater_state("q0");
    afd.add_final_state("q0");

    afd.add_transition("q0", "0", {"q1"});
    afd.add_transition("q0", "1", {"q2"});
    afd.add_transition("q1", "0", {"q0"});
    afd.add_transition("q1", "1", {"q3"});
    afd.add_transition("q2", "0", {"q3"});
    afd.add_transition("q2", "1", {"q0"});
    afd.add_transition("q3", "0", {"q2"});
    afd.add_transition("q3", "1", {"q1"});

    vector<string> palavras = {"", "0", "1", "00", "11", "010", "101", "0011", "1100"};

    cout << "Teste antes da minimizacao:" << endl;
    for (const auto& w : palavras) {
        cout << w << " -> " << (afd.verify_word(w) ? "ACEITA" : "REJEITA") << endl;
    }

    Automaton min = afd.minimize();

    cout << "\nTeste depois da minimizacao:" << endl;
    for (const auto& w : palavras) {
        cout << w << " -> " << (min.verify_word(w) ? "ACEITA" : "REJEITA") << endl;
    }

    cout << "\nEstados originais: ";
    for (auto& s : afd.states) cout << s << " ";
    cout << endl;

    cout << "Estados minimizados: ";
    for (auto& s : min.states) cout << s << " ";
    cout << endl;

    cout << "Estado inicial: " << min.starterState << endl;

    cout << "Estados finais: ";
    for (auto& f : min.finalStates) cout << f << " ";
    cout << endl;

    return 0;
}