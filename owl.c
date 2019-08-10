#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "Graph.h"
#include "Quack.h"

bool differByOne(char *word1, char *word2);
void dfs(Graph g, Vertex rootv, int numV, char words[1000][20]);
void dfsR(Graph g, Vertex v, int numV, int *order, int *visited, int *counter, Quack *allQuacks, int *maxLength, int *currLength, int *lastConnected, Quack tempQuack);
void printArray(char *word, int *v, int numV);
int* mallocArray(int n);
void resetArray(int *v, int numv);
void copyQuackAtIndex(Quack srcQuack, int index, Quack *allQuacks);
void popLastElement(Quack srcQuack);
void createDuplicateQuackTillNum(Quack *allQuacks, int ithQuack, int num) ;
// #define UNVISITED 0

int main() {
    char words[1000][20];   //max number of words - 1000, max length of word - 20 characters
    int i = 1, wordCount = 0;
    char word[20];
    wordCount = 0;

    while((scanf("%s", word)) == 1) {
        if (strcmp(word, words[wordCount]) == 0) {
            continue;
        } else {
            strcpy(words[wordCount], word);
            wordCount++;
        }
    }

    printf("Dictionary");
    for (i = 0; i < wordCount; i++) {
        printf("\n%d: %s", i, words[i]);
    }
    
    Graph g = newGraph(wordCount);

    for(i = 0; i < wordCount; i++) {
        for (int j = 0; j < wordCount; j++) {
            if(differByOne(words[i], words[j]) == true) {
                insertEdge(newEdge(i, j), g);
            }
        }
    }

    printf("\nOrdered Word Ladder Graph\n");
    showGraph(g);
    dfs(g, 0, wordCount, words);
    return EXIT_SUCCESS;
}

void dfs(Graph g, Vertex rootv, int numV, char words[1000][20]) {

    int *visited = mallocArray(numV);  
    resetArray(visited, numV);

    int order = 0;
    Vertex startv = rootv;             
    int allVis = 0;
    Quack allQuacks[1000];

    for (int i = 0; i < 1000; i++) {
        allQuacks[i] = createQuack();
    }

    int maxLength = 0, currLength = 0;
    int counter = 0;
    while (startv <= numV) {                  
        
        int lastConnected = startv;
        Quack tempQuack = createQuack();
        dfsR(g, startv, numV, &order, visited, &counter, allQuacks, &maxLength, &currLength, &lastConnected, tempQuack);
        currLength = 0;
        startv++;
    }

    printf("Longest ladder length: %d", maxLength);
    printf("\nLongest ladders:");
    // printf("\nCounter: %d", counter);
    // if (counter == 0 || counter == 1) {
    //     counter = 0;
    // }
    
    for (int i = 0; i <= counter && i < 100; i ++){    //Maximum there will be 100 paths
        
        Quack tempQuack = allQuacks[i];
        int j = 0;
        while (!isEmptyQuack(tempQuack)) {
            if (j == 0) {
                printf("\n%2d: ", i + 1);
                printf("%s", words[pop(tempQuack)]);
            } else {
                printf(" -> %s", words[pop(tempQuack)]);
            }
            j++;
        }
    }
    printf("\n");
    free(visited);
    return;
}

void dfsR(Graph g, Vertex v, int numV, int *order, int *visited, int *counter,
 Quack *allQuacks, int *maxLength, int *currLength, int *lastConnected, Quack tempQuack) {
    // printf("\nInside dfsR");
    qush(v, tempQuack);
    *currLength = *currLength + 1;
    
    if (*currLength > *maxLength && *currLength != 1) {
        // printf("\ncurrLength > maxLength");
        // showQuack(allQuacks[0]);
        *counter = 0;
        *maxLength = *currLength;
        copyQuackAtIndex(tempQuack, *counter, allQuacks);
    } else if (*currLength == *maxLength) {
        *counter = *counter + 1;
        copyQuackAtIndex(tempQuack, *counter, allQuacks);
    } 

    visited[v] = *order;
    // printf("\nVertex v:%d, numv : %d, order: %d Visited[v]: %d", v, numV, *order, visited[v]);
    *order = *order + 1;

    //Start looking from the successor only
    for (Vertex w = v + 1; w < numV; w++) {
        
        //TODO: change unvisited to last child of node, last child is when no more children>currNode
        if (isEdge(newEdge(v,w), g)) {
            // printf("\nlastConnected : %d less than vertex w: %d", *lastConnected, w);
            // printf("\nValid edge v: %d w: %d\n", v, w);
            // *lastConnected = w;
            dfsR(g, w, numV, order, visited, counter, allQuacks, 
                maxLength, currLength, lastConnected, tempQuack);
            popLastElement(tempQuack);
            *currLength = *currLength - 1;
                
            // else {
            //     int i = 0;
            //     printf("\nlastConnected : %d not less than vertex w: %d", *lastConnected, w);
            //     *counter = *counter + 1;
            //     push(v, allQuacks[*counter]);
            //     // dfsR(g, w, numV, order, visited, counter, allQuacks, maxLength, currLength, lastConnected);
            // } 
        }
        
    }
    return;
}

void popLastElement(Quack srcQuack) {
    // printf("\n ***********popping last element");
    // showQuack(srcQuack);
    Quack tempQuack = createQuack();
    int a ;
    while(!isEmptyQuack(srcQuack)) {
        a = pop(srcQuack);
        push(a, tempQuack);
    }

    pop(tempQuack);
    while (!isEmptyQuack(tempQuack)) {
        a = pop(tempQuack);
        push(a, srcQuack);
    }
    // printf("\nAfter popping ");
    // showQuack(srcQuack);
}

void copyQuackAtIndex(Quack srcQuack, int index, Quack *allQuacks) {
    // printf("\ncopy quack");
    // printf("\n srcQuack");
    // showQuack(srcQuack);

    
    Quack tempQuack = createQuack();
    allQuacks[index] = createQuack();   //TODO: use another way to null/destroy????
    int a;
    while(!isEmptyQuack(srcQuack)) {
        a = pop(srcQuack);
        qush(a, allQuacks[index]);
        push(a, tempQuack);
    }

    while (!isEmptyQuack(tempQuack)) {
        a = pop(tempQuack);
        push(a, srcQuack);
    }

    // printf("\n Quack index: %d ", index);
    // showQuack(allQuacks[index]);
}

// create 
void createDuplicateQuackTillNum(Quack *allQuacks, int ithQuack, int num) {
    Quack tempQuack = createQuack();
    int nextQuack = ithQuack + 1;
    int a;
    while(a != num) {
        int a = pop(allQuacks[ithQuack]);
        qush(a, allQuacks[nextQuack]);
        push(a, tempQuack);
    }

    while (!isEmptyQuack(tempQuack)) {
        a = pop(tempQuack);
        push(a, allQuacks[ithQuack]);
    }
}

void printArray(char *word, int *v, int numV) {
    printf("\n%s: ",word);

    // printf("%d", *v);
    if (numV > 0) {
        printf("{ ");
        for (int i = 0; i < numV; i++) {
            printf("%d ",v[i]);
        }
        printf("}\n");
    }
}

int* mallocArray(int n) {
    return malloc(n * sizeof(int));
}

void resetArray(int *v, int numv) {
    for(int i = 0; i < numv; i++) {
        v[i] = UNVISITED;
    }
}

bool differByOne(char *word1, char *word2) {

    int length1 = strlen(word1);
    int length2 = strlen(word2);
    int changes = 0, i = 0, j = 0;

    //Same length then maybe 1 letter change
    // if (length1 == length2) {
    //     return is1LetterChange(word1, word2);

    // //Length differs by 1 then maybe letter added
    // } else if (abs(length1 - length2) == 1) {
        
    // } else {

    //     return false;
    // }

    while (i < length1 && j < length2) { 
        // If current characters don't match 
        if (word1[i] != word2[j]) 
        { 
            if (changes == 1) //When 2nd change found
                return false; 

            //Move forward for the longer word
            if (length1 > length2) 
                i++; 
            else if (length1 < length2) 
                j++; 
            else { //Iflengths of both strings is same 
                i++; 
                j++; 
            } 
            changes++; 
        } 
  
        else { // If current characters match
            i++; 
            j++; 
        } 
    } 
  
    // If any word has last character as extra
    // if (i < length1 || j < length2) 
    //     changes++; 

    if (j < length2) {
        changes += length2 - j;
    } else {
        changes+= length1 - i;
    }
  
    return changes == 1;
}