# Finite State Automata and Translators

## Program Input
The program will prompt the user to enter:

### 1) Number of states
### 2) Whether thee program is a translator (1 = yes, 0 = no)
### 3) Definition of each state:
- Whether it is a final state  
- Number of transitions  
- For each transition:    
  - Transition symbol    
  - Next State    
  - (if a translator) Transition output   

### 4) Input string to evaluate   

## Code Structure  
- `setupFSA()`: Initializes states and transitions  
- `checkString()`: Checks if the string is accepted and generates output (if applicable)  
- `main()`: Manages input, execution, and output  


## Graph Generation Commands
To generate a graph representation
  `dot -Tpng grafo.dot -o grafo.png`    GENERATES A PNG  
  `dot -Tpdf grafo.dot -o grafo.pdf`    GENERATES A PDF  

  - FOR PNG FILES  
    `xdg-open grafo.png`  # Linux  
    `open grafo.png`      # macOS  

  - FOR PDF FILES  
    `xdg-open grafo.pdf`  # Linux  
    `open grafo.pdf `     # macOS  


  - POWERSHELL # Windows  
    `Start-Process grafo.png`  
    `Start-Process grafo.pdf`  

  - CMD # Windows  
    `start grafo.png`  
    `start grafo.pdf`  