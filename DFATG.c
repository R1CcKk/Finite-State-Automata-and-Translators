#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// struct that defines the transition
typedef struct
{
    char symbol; // symbol being considered
    int NextState;
    char *StateOutput;
    int StateLenOutput;
} transition_t;

// struct that defines the state
typedef struct
{
    int id;      // state name
    int isFinal; /*1 if true, 0 if false*/
    int numTransitions;
    transition_t *transitions;
} state_t;

// function that initializes the states by reading the number of transitions for each

void setupFSA(state_t *fsa, int numStates, char *filename)
{
    int translator;

    FILE *file = fopen(filename, "w");
    if (file == NULL)
    {
        perror("Error opening the file");
        exit(EXIT_FAILURE);
    }

    printf("Is it a translator? (1 for yes 0 for no)\n");
    if (scanf(" %d", &translator) != 1 || (translator != 0 && translator != 1))
    {
        printf("Input error!\n");
        exit(EXIT_FAILURE);
    }

    fprintf(file, "digraph G {\n");

    for (int i = 0; i < numStates; i++)
    {
        printf("Definition of state %d\n", i);
        fsa[i].id = i;

        printf("Is the state final? (1 for yes 0 for no)\n");
        if (scanf(" %d", &fsa[i].isFinal) != 1 || (fsa[i].isFinal != 0 && fsa[i].isFinal != 1))
        {
            printf("Input error!\n");
            exit(EXIT_FAILURE);
        }
        if (i == 0)
        {
            if (fsa[i].isFinal)
            {
                fprintf(file, "    start [shape=point, style = invis];\n"); // Invisible node
                fprintf(file, " %d [shape=doublecircle];\n", fsa[i].id);    // Starting node
                fprintf(file, "start -> %d;\n", fsa[i].id);
            }
            else
            {
                fprintf(file, "    start [shape=point, style = invis];\n"); // Invisible node
                fprintf(file, " %d [shape=circle];\n", fsa[i].id);          // Starting node
                fprintf(file, "start -> %d;\n", fsa[i].id);
            }
        }
        else
        {
            if (fsa[i].isFinal)
            {
                fprintf(file, " %d [shape=doublecircle];\n", fsa[i].id);
            }
            else
            {
                fprintf(file, " %d [shape=circle];\n", fsa[i].id);
            }
        }

        printf("Number of transitions for state %d: \n", i);
        if (scanf(" %d", &fsa[i].numTransitions) != 1 || fsa[i].numTransitions < 0)
        {
            printf("Input error!\n");
            fclose(file);
            exit(EXIT_FAILURE);
        }

        // dynamic allocation for transitions
        fsa[i].transitions = malloc(sizeof(transition_t) * fsa[i].numTransitions);
        if (fsa[i].transitions == NULL)
        {
            perror("Memory allocation error!\n");
            fclose(file);
            exit(EXIT_FAILURE);
        }

        for (int j = 0; j < fsa[i].numTransitions; j++)
        {
            if (translator)
            {
                printf("Enter output size %d (int): \n", j + 1);
                if (scanf(" %d", &fsa[i].transitions[j].StateLenOutput) != 1 || fsa[i].transitions[j].StateLenOutput < 0)
                {
                    printf("Input error!\n");
                    fclose(file);
                    exit(EXIT_FAILURE);
                }

                if (fsa[i].transitions[j].StateLenOutput > 0)
                {
                    fsa[i].transitions[j].StateOutput = malloc(sizeof(char) * (fsa[i].transitions[j].StateLenOutput + 1));
                    if (fsa[i].transitions[j].StateOutput == NULL)
                    {
                        printf("Memory allocation error!\n");
                        exit(EXIT_FAILURE);
                    }

                    printf("Enter output for transition %d (string): \n", j + 1);
                    scanf(" %s", fsa[i].transitions[j].StateOutput);
                }
            }
            printf("Transition %d - enter input (char) and id (int) of the destination state: \n", j + 1);
            if (scanf(" %c %d", &fsa[i].transitions[j].symbol, &fsa[i].transitions[j].NextState) != 2 || fsa[i].transitions[j].NextState < 0)
            {
                printf("Input error!\n");
                fclose(file);
                exit(EXIT_FAILURE);
            }
            if (fsa[i].transitions[j].StateLenOutput == 0)
            {
                fprintf(file, " %d -> %d [label = \" %c/_\"];\n", fsa[i].id, fsa[i].transitions[j].NextState, fsa[i].transitions[j].symbol);
            }
            else
            {
                fprintf(file, " %d -> %d [label = \" %c/%s\"];\n", fsa[i].id, fsa[i].transitions[j].NextState, fsa[i].transitions[j].symbol, fsa[i].transitions[j].StateOutput);
            }
        }
    }
    fprintf(file, "}\n");
}

int checkString(state_t *fsa, int numStates, const char *input, char **output, int *LenOutput) // function that checks the string
{
    int currentState = 0;
    int flag = 0, stop = 0; // control variables for loops
    int k = 0;              // count variable for output
    int tempLen = 0;        // count variable for output length

    *LenOutput = 0; // initialize output length to 0

    if (input[0] == '\0')
    { // if the first state is accepting and the string is empty, accept immediately
        if (fsa[currentState].isFinal == 1)
        {
            return fsa[currentState].isFinal;
        }
        else
        {
            return 0;
        }
    }
    else
    {
        for (int i = 0; input[i] != '\0'; i++) // loop to iterate through the string
        {
            char symbol = input[i];
            flag = 0;
            stop = 0;

            for (int j = 0; j < fsa[currentState].numTransitions && !stop; j++) // loop to iterate through transitions
            {
                if (fsa[currentState].transitions[j].symbol == symbol) // check if symbols match
                {
                    if (fsa[currentState].transitions[j].StateLenOutput > 0)
                    {                                                               // if there's an output to produce (length at least 1)
                        tempLen += fsa[currentState].transitions[j].StateLenOutput; // increment output length
                        // printf("TEMP LENGTH CYCLE %d %d\n", tempLen, j);
                        char *temp = realloc(*output, sizeof(char) * tempLen); // reallocate memory for output
                        if (temp == NULL)                                      // check if allocation was successful
                        {
                            printf("Memory allocation error!\n");
                            exit(EXIT_FAILURE);
                        }
                        *output = temp;                                                           // assign the reallocated memory address
                        for (int l = 0; l < fsa[currentState].transitions[j].StateLenOutput; l++) // loop to insert output
                        {
                            (*output)[k] = fsa[currentState].transitions[j].StateOutput[l]; // insert output
                            k++;                                                            // increment counter
                        }
                    }
                    currentState = fsa[currentState].transitions[j].NextState; // change state to the next one

                    flag = 1;
                    stop = 1; // being deterministic, once I find a valid transition, exit the loop
                    // printf("CYCLE %d\n", i);
                }
            }
            if (!flag)
                return 0; // reject the string when no transition is possible
        }
    }
    *LenOutput = tempLen; // assign the output length to be returned to the caller
    return fsa[currentState].isFinal;
}

int main()
{
    int numStates; // number of required states
    int LenInput;
    char *input;
    char *output = NULL;
    int LenOutput = 0;
    system("figlet Automaton FSA");

    // request the number of required states
    printf("Enter the number of states: \n");
    scanf("%d", &numStates);

    // dynamically allocate memory for states
    state_t *fsa = malloc(sizeof(state_t) * numStates);

    // set up the states
    setupFSA(fsa, numStates, "graph.dot");

    // insert the input string
    printf("Enter string length: \n");
    scanf(" %d", &LenInput);

    input = malloc(sizeof(char) * (LenInput + 1));
    if (LenInput == 0)
    {
        input[0] = '\0';
    }
    else
    {
        printf("Enter the string: \n");
        scanf(" %s", input);
    }

    if (checkString(fsa, numStates, input, &output, &LenOutput))
    {
        printf("String ACCEPTED\n");
        if (output != NULL)
        {
            printf("Output: ");
            // printf("LENGTH LENOUTPUT %d\n", LenOutput);
            for (int i = 0; i < LenOutput; i++)
            {
                printf("%c", output[i]);
            }
        }
    }
    else
    {
        printf("String REJECTED");
    }

    // free previously allocated memory
    for (int i = 0; i < numStates; i++)
    {
        for (int j = 0; j < fsa[i].numTransitions; j++)
        {
            if (fsa[i].transitions[j].StateLenOutput > 0)
            {
                free(fsa[i].transitions[j].StateOutput);
            }
        }
        free(fsa[i].transitions);
    }
    free(fsa);
    free(input);
    free(output);

    return 0;
}
