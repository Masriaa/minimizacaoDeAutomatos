#include "automaton.h" 
#include <stdexcept>   
#include <algorithm>   


Automaton::Automaton(){
  const string& q0 = "q0";
  
  states.insert(q0);
  starterState = q0;
}

Automaton::Automaton(
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

Automaton::Automaton(
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

void Automaton::add_transition(const string& fromState, const string& symbol, const set<string>& toStates) {
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

void Automaton::add_alphabet_symbol(const string& symbol){
 	if(symbol.empty()){
 		throw runtime_error("empty word can't become a alphabet symbol");
 	}
 	if(alphabet.find(symbol) != alphabet.end()){
 		return ;
 	}
 	alphabet.insert(symbol);
}

void Automaton::add_state(const string& state){
 	if(state.empty()){
 		throw runtime_error("States aren't enable to become unnamed states");
 	}
 	if (states.find(state) != states.end()){
 		return ;
 	}
 	
 	
 	states.insert(state);
}

void Automaton::add_final_state(const string& finalState){
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

void Automaton::set_stater_state(const string& state){
 	if(states.find(state) == states.end()){
 		throw runtime_error("The stater state: '" + state + "' isn't in states set.");
 	}
 	if( starterState == state){
 		return ;
 	}
 	
 	starterState = state;
}

set<string> Automaton::closure(const set<string>& initialStates) const {
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

set<string> Automaton::epsilon_closure(const set<string>& initialStates) const {
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

bool Automaton::verify_word(const string& word){
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

string Automaton::automaton_type(){
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



Automaton Automaton::afn_epsilon_to_afn(){
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

Automaton Automaton::afn_to_afd(){
 	set<string> Q;
 	const set<string>& Sigma = alphabet;
 	map<pair<string, string>, set<string>> delta;
 	
 	string q0_afd_name = starterState; 
 	
 	set<string> F;

 	queue<set<string>> states_to_process;
 	set<string> q0_subset = {starterState};

 	states_to_process.push(q0_subset);

 	Q.insert(q0_afd_name); 

 	if(finalStates.count(starterState)){
 		F.insert(q0_afd_name);
 	}

 	while(!states_to_process.empty()){
 		const set<string> currentSet = states_to_process.front(); 
 		states_to_process.pop();
 		
 		string stateName = "";
 		
 		bool first = true; 
 		for(const string& currentState : currentSet){
 			if (!first) {
 				stateName += "_";
 			}
 			stateName += currentState;
 			first = false;
 		}

 		Q.insert(stateName);
 		
 		for (const string& symbol : alphabet) {
 			set<string> nextStates;
 			
 			for(const string& state : currentSet){
 				pair<string, string> key = {state, symbol};
 				const auto& it = transitions.find(key);
 				
 				if(it != transitions.end()){
 					nextStates.insert(it->second.begin(), it->second.end());
 				}
 			}

 			if (!nextStates.empty()) {
 				string newState_name = "";
 				for(const string& state : nextStates){
 					newState_name += state;
 				}
 				
 				pair<string, string> key = {stateName, symbol};
 				delta[key] = {newState_name}; 
 					
 				if (Q.find(newState_name) == Q.end()) {
 					Q.insert(newState_name);
 					states_to_process.push(nextStates); 
 					
 					for(const string& state : nextStates){
 						if (finalStates.count(state)) {
 							F.insert(newState_name);
 							break;
 						}
 					}
 				}
 			}
 		}
 	}

 	Automaton automaton(Q, Sigma, delta, q0_afd_name, F);
 	return automaton;
}

Automaton Automaton::minimize(){
 	if(automaton_type() == "afn" || automaton_type() == "afn_epsilon"){
 		throw runtime_error("The automaton type is wrong, isen't a AFD");
 	}

 	set<string> reachableStates = closure({starterState});
 	map<pair<string, string>, bool> keys;

 	for(const string& stateOne : reachableStates){
 		for(const string& stateTwo : reachableStates) {
 			if(stateOne < stateTwo){
 				pair<string, string> key = {stateOne, stateTwo};
 				
 				bool one_is_final = finalStates.count(stateOne) > 0;
 				bool two_is_final = finalStates.count(stateTwo) > 0;

 				if(one_is_final != two_is_final){
 					keys[key] = true;
 				}else{
 					keys[key] = false;
 				}
 			}
 		}
 	}

 	bool change;

 	do {
 		change = false;

 		for(auto& key : keys){
 			const string& p = key.first.first;
 			const string& q = key.first.second;

 			if(key.second){
 				continue; 
 			} 

 			for(const string& symbol : alphabet){
 				pair<string,string> pKey = {p, symbol};
 				pair<string,string> qKey = {q, symbol};

 				auto itP = transitions.find(pKey);
 				auto itQ = transitions.find(qKey);

 				if(itP == transitions.end() || itQ == transitions.end()){
 					continue;
 				} 

 				const set<string>& pNextStates = itP->second;
 				const set<string>& qNextStates = itQ->second;

 				bool shouldMark = false;

 				for(const string& pNext : pNextStates){
 					for(const string& qNext : qNextStates){
 						if(pNext == qNext) continue;

 						pair<string,string> next = (pNext < qNext) ? make_pair(pNext, qNext) : make_pair(qNext, pNext);

 						if(keys[next]){ 
 							shouldMark = true;
 							break;
 						}
 					}
 					if(shouldMark) break;
 				}

 				if(shouldMark){
 					keys[key.first] = true;
 					change = true;
 					break; 
 				}
 			}
 		}

 	} while(change);

 	map<string, string> charges;

 	for(const string& state : reachableStates){
 		charges[state] = state;
 	}

 	for(const auto& key : keys){
 		if(!key.second){ 
 			const string& firstState = key.first.first;
 			const string& secondState = key.first.second;

 			string minor = min(charges[firstState], charges[secondState]);
 			charges[firstState] = minor;
 			charges[secondState] = minor;
 		}
 	}

 	set<string> Q;
 	const set<string>& Sigma = alphabet;
 	map<pair<string, string>, set<string>> delta;
 	const string& q0 = charges[starterState];
 	set<string> F;


 	for(const auto& charge : charges){
 		Q.insert(charge.second);   
 	}

 	for(const auto& transition : transitions){
 		const string& oldState = transition.first.first;
 		const string& symbol = transition.first.second;
 		
 		for(const string& state : transition.second){
 			const string& newFromState = charges[oldState];
 			const string& newToState = charges[state];

 			delta[{newFromState, symbol}] = {newToState};
 		}
 	}

 	for(const string& state : finalStates){
 		F.insert(charges[state]);
 	}

 	Automaton automaton(Q, Sigma, delta, q0, F);
 	return automaton;
}

Automaton Automaton::minimize_hopcrof_moore_algorithm() {
 	set<string> reachableStates = closure({starterState}); 
 	
 	set<set<string>> currentPartition;
 	set<set<string>> nextPartition;

 	set<string> acceptableStates;
 	set<string> unacceptableStates;
 	
 	for (const string& state : reachableStates) {
 		if (finalStates.count(state)) {
 			acceptableStates.insert(state);
 		} else {
 			unacceptableStates.insert(state);
 		}
 	}

 	if (!acceptableStates.empty()){
 		currentPartition.insert(acceptableStates);
 	} 
 	if (!unacceptableStates.empty()){
 		currentPartition.insert(unacceptableStates);
 	} 
 	
 	bool changed = true;
 	while (changed) {
 		changed = false;
 		nextPartition.clear();

 		for (const set<string>& currentBlock : currentPartition) {
 			
 			map<vector<string>, set<string>> subBlockMap;

 			for (const string& state : currentBlock) {
 				
 				vector<string> transitionPattern;
 				for (const string& symbol : alphabet) {
 					
 					set<string> destinationSet = transitions.count({state, symbol}) ? transitions.at({state, symbol}) : set<string>{};
 					string destinationState = destinationSet.empty() ? "TRASH" : *(destinationSet.begin());
 					
 					if (destinationState != "TRASH" && !reachableStates.count(destinationState)) {
 						destinationState = "TRASH";
 					}
 					
 					string destinationBlockRep = "TRASH"; 
 					if (destinationState != "TRASH") {
 						for (const set<string>& block : currentPartition) {
 							if (block.count(destinationState)) {
 								destinationBlockRep = *(block.begin()); 
 								break;
 							}
 						}
 					}
 					transitionPattern.push_back(destinationBlockRep);
 				}

 				subBlockMap[transitionPattern].insert(state);
 			}

 			for (const auto& pair : subBlockMap) {
 				nextPartition.insert(pair.second);
 				if (pair.second.size() < currentBlock.size()) {
 					changed = true;
 				}
 			}
 		}

 		currentPartition = nextPartition;
 	} 

 	set<string> Q;
 	string q0;
 	set<string> F;
 	map<pair<string, string>, set<string>> delta;

 	map<string, string> stateToNewName; 
 	int blockCounter = 0;

 	for (const set<string>& block : currentPartition) {
 		string stateName = "qMin" + to_string(blockCounter++);
 		string representative = *(block.begin());
 		
 		Q.insert(stateName);
 		
 		for (const string& state : block) {
 			stateToNewName[state] = stateName;
 		}

 		if (block.count(starterState)) {
 			q0 = stateName;
 		}
 		
 		if (finalStates.count(representative)) {
 			F.insert(stateName);
 		}
 	}

 	for (const set<string>& block : currentPartition) {
 		string currentState = stateToNewName[*(block.begin())];
 		string representative = *(block.begin());

 		for (const string& symbol : alphabet) {
 			set<string> destSet = transitions.count({representative, symbol}) ? transitions.at({representative, symbol}) : set<string>{};
 			string destOrig = destSet.empty() ? "" : *(destSet.begin());

 			if (stateToNewName.count(destOrig)) {
 				string destState = stateToNewName[destOrig];
 				delta[{currentState, symbol}] = {destState};
 			}
 		}
 	}

 	Automaton automaton(Q, alphabet, delta, q0, F);

 	return automaton;
}

