class State:
    name = ""
    number = 0

class Character:
    name = ""
    number = 0

class NFA:
    states = []
    alphabet = []
    move_n = []
    initialState = State()
    finalStates = []

def getStateByName(name, states):
    s = State()
    for tmp in states:
        if(tmp.name == name):
            s = tmp
    return s

def getCharacterByName(name, characters):
    c = Character()
    for tmp in characters:
        if(tmp.name == name):
            c = tmp
    return c

def stringOf(c):
    return c

def printNFA(N):
    log.write(("N = (S, A, move_n, s0, F)\n")
    log.write(("S = ")
    for state in N.states:
        log.write((s.name + " ")
    log.write("\n A = ")
    for c in N.alphabet:
        log.write(c.name + " ")
    log.write("\n move_n = \n"):
    for i in range(len(N.states)):
        log.write(N.states[i].name + ": ")
        for j in range(len(N.alphabet)):
            for s in N.move_n[i][j]:
                log.write(s.name + " ")
            log.write("  ")
        log.write("\n")
    log.write("$0 = " + N.initialState.name)
    log.write("\n F = ")
    for s in N.finalStates:
        log.write(s.name + " ")
    log.write("\n")

def E_closure(s, pila, N, alreadyOn):
    pila.append(s)
    alreadyOn[s.number] = True

    for u in N.move_n[s.number][E.number]:
        if(!alreadyOn[u.number]):
            E_closure(u, pila, N, alreadyOn)

def closure(c, input, output, N, alreadyOn):
    for stateInput in input:
        for u in N.move_n[stateInput.number][c.number]:
            if(!alreadyOn(u.number)):
                log.write("move_n [ " + u.name + "] [" + c.name + "]\n")
                output.append(u)
                alreadyOn[u.number] = True

def simulation(s, N):
    i = 0
    alreadyOn = [False]*len(N.states)
    s.append("$")
    states = []
    tmp = []

    E_closure(N.states[0], states, N, alreadyOn)
    alreadyOn = [False]*len(N.states)

    log.write("\n\n SIMULATION \n\n")
    log.write("First closure : ")
    for stateLog in states:
        log.write(stateLog.name + " ")
    symbol = s[i]
    i++

    while (symbol != "$"):
        log.write("\n\n Checking symbol " + symbol + "\n")
        closure(getCharacterByName(symbol, N.alphabet), states, tmp, N, alreadyOn)
        alreadyOn = [False]*len(N.states)    

        log.write("\n Move: ")
        for stateLog in tmp:
            log.write(statelog.name + " ")
        states = []
        for stateOfT in tmp:
            E_closure(stateOfT, states, N, alreadyOn)
        alreadyOn = [False]*len(N.states)    

        log.write("E_closure: ")
        for stateLog in states:
            log.write(stateLog.name(" "))

        tmp = []
        symbol = s[i]
        i++    
    belongsTo = False

    for stateOfT in states:
        for finalState in N.finalStates:
            if(finalstate.name == stateOfT.name):
                belongsTo = True

    return belongsTo

log = open("log.txt", "w")


def main():
    N = NFA()
    tmp = 0

if __name__ == '__main__':
    main()