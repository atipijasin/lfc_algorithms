#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

struct state {
    string name;
    int number;   
}; 

struct character {
    string name;
    int number;   
}; 

struct NFA {
    vector <state> states;
    vector <character> alphabet;
    vector <vector <vector <state>>> move_n;
    state initialState;
    vector <state> finalStates;
};

typedef vector <state> stack;

state getStateByName(string, const vector <state> &); 
character getCharacterByName(string, const vector <character> &); 
string stringOf(char);
void arrayToFalse(bool[]);
void printNFA(NFA);
void E_closure(state, stack &, const NFA &, bool[]);
void closure(character, const stack &, stack &, const NFA &, bool[]);
bool simulation(string, const NFA &);

character E;
ifstream in ("input.txt");
ofstream log ("log.txt");

int main() {

    NFA N;
    int tmp;

    //states
    in >> tmp;

    for(int i = 0; i < tmp; i++) {
        state s;
        in >> s.name;
        s.number = i;
        N.states.push_back(s);
    }

    //alphabet
    in >> tmp;

    for(int i = 0; i < tmp; i++) {
        character c;
        in >> c.name;
        c.number = i;
        N.alphabet.push_back(c);
    }
    E.name = "Epsilon";
    E.number = N.alphabet.size();

    //move_n
    N.move_n.resize(N.states.size());
    string stringTmp;

    for(int i = 0; i < N.states.size(); i++) {
        N.move_n[i].resize(N.alphabet.size() + 1);
        for(int j = 0; j < N.alphabet.size() + 1; j++) {
            in >> stringTmp;
            while(stringTmp != "-") {
                state s = getStateByName(stringTmp, N.states);
                N.move_n[i][j].push_back(s);
                in >> stringTmp;
            }
        }
    }

    //initialState
    in >> stringTmp;
    state initialState = getStateByName(stringTmp, N.states);
    N.initialState = initialState;

    //finalStates
    in >> tmp;

    for(int i = 0; i < tmp; i++) {
        in >> stringTmp;
        state s = getStateByName(stringTmp, N.states);
        N.finalStates.push_back(s);
    }

    printNFA(N);

    string s;
    cout << "Inserisci stringa: ";
    cin >> s;

    bool belongsTo = simulation(s, N);

    cout << (belongsTo ? "YES" : "NO") << endl;

}

state getStateByName(string name, const vector <state> &states) {
    state s;
    for(state tmp : states) {
        if(tmp.name == name) {
            s = tmp;
        }
    }
    return s;
}

character getCharacterByName(string name, const vector <character> &characters) {
    character c;
    for(character tmp : characters) {
        if(tmp.name == name) {
            c = tmp;
        }
    }
    return c;
}

string stringOf(char c) {
    string s;
    s.push_back(c);
    return s;
}

void arrayToFalse(bool b[], int size) {
    for(int i = 0; i < size; i++) {
        b[i] = false;
    }
}

void printNFA(NFA N) {
    log << "N = (S, A, move_n, s0, F)";
    log << endl << "S = ";
    for(state s : N.states) {
        log << s.name << " ";
    }
    log << endl << "A = ";
    for(character c : N.alphabet) {
        log << c.name << " ";
    }
    log << endl << "move_n = " << endl;
    for(int i = 0; i < N.states.size(); i++) {
        log << N.states[i].name << ": ";
        for(int j = 0; j < N.alphabet.size() + 1; j++) {
            for(state s : N.move_n[i][j]) {
                log << s.name << " ";
            }
            log << "  ";
        }
        log << endl;
    }
    log << "S0 = " << N.initialState.name;
    log << endl << "F = ";
    for(state s : N.finalStates) {
        log << s.name << " ";
    }
    log << endl << endl;
}

void E_closure(state s, stack &pila, const NFA &N, bool alreadyOn[]) {
    pila.push_back(s);
    alreadyOn[s.number] = true;
    
    for(state u : N.move_n[s.number][E.number]) {
        if(!alreadyOn[u.number]) {
            E_closure(u, pila, N, alreadyOn);
        }
    }
}

void closure(character c, const stack &input, stack &output, const NFA &N, bool alreadyOn[]) {
    for(state stateInput : input) {
        for(state u : N.move_n[stateInput.number][c.number]) {
            if(!alreadyOn[u.number]) {
                log << "move_n [" << u.name << "] [" << c.name << "]" << endl;
                output.push_back(u);
                alreadyOn[u.number] = true;
            }
        }
    }
}

bool simulation(string s, const NFA &N) {
    int i = 0;
    bool alreadyOn[N.states.size()] = {false};
    s.push_back('$');
    stack states, tmp;

    E_closure(N.states[0], states, N, alreadyOn);
    arrayToFalse(alreadyOn, N.states.size());
    
    log << endl << endl << "SIMULATION" << endl << endl;
    log << "First closure : ";
    for(state stateLog : states) {
        log << stateLog.name << " ";
    }
    string symbol = stringOf(s[i++]);

    while(symbol != "$") {
        log << endl << endl << "Checking symbol " << symbol << endl;
        closure(getCharacterByName(symbol, N.alphabet), states, tmp, N, alreadyOn);
        arrayToFalse(alreadyOn, N.states.size());   

        log << endl << "Move: ";
        for(state stateLog : tmp) {
            log << stateLog.name << " ";
        }
        states.clear();
        for(state stateOfT : tmp) {
            E_closure(stateOfT, states, N, alreadyOn);
        }
        arrayToFalse(alreadyOn, N.states.size());

        log << endl << "E-closure: ";
        for(state stateLog : states) {
            log << stateLog.name << " ";
        }
        tmp.clear();
        symbol = stringOf(s[i++]);
    }

    bool belongsTo = false;

    for(state stateOfT : states) {
        for(state finalState : N.finalStates) {
            if(finalState.name == stateOfT.name) {
                belongsTo = true;
            }
        }
    }
    return belongsTo;
}
