/**
 * @file owl.c
 * @author Yash Tamakuwala(z5248584)
 * @brief Client side that uses Graph and Quack interfaces to read input, create graph and find longest ladder length and path
 * @version 0.1
 * @date 2019-07-20
 * 
 * @copyright Copyright (c) 2019
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "Graph.h"
#include "Quack.h"

#define VISITED 1
#define MAXWORDS 1000
#define MAXWORDLENGTH 21
#define MAXPATHS 100

bool differByOne(char *word1, char *word2);
void dfs(Graph g, Vertex rootv, int numV, char words[MAXWORDS][MAXWORDLENGTH]);
void dfsR(Graph g, Vertex v, int numV, int *counter, Quack *allQuacks,
 int *maxLength, int *currLength, Quack tempQuack);

void copyQuackAtIndex(Quack srcQuack, int index, Quack *allQuacks);
void popLastElement(Quack srcQuack);
void createDuplicateQuackTillNum(Quack *allQuacks, int ithQuack, int num);
void resetArray(int *v, int numv);
void setVisitedArray(Quack srcQuack, int *visited);



int main() {
    char words[MAXWORDS][MAXWORDLENGTH];   //max number of words - 1000, max length of word - 20 characters
    int wordCount = 0;
    char word[MAXWORDLENGTH], prevWord[MAXWORDLENGTH] = "";

    //Reads input and rejects duplicate words
    while ((scanf("%s", word)) == 1) {
        if (strcmp(word, prevWord) != 0) {
            strcpy(words[wordCount], word);
            strcpy(prevWord, word);
            wordCount++;
        }
    }

    //Make dictionary if there are words
    if (wordCount > 0) {
        printf("Dictionary");
        for (int i = 0; i < wordCount; i++) {
            printf("\n%d: %s", i, words[i]);
        }
        
        Graph g = newGraph(wordCount);

        for (int i = 0; i < wordCount - 1; i++) {
            for (int j = i + 1; j < wordCount; j++) {
                if (differByOne(words[i], words[j]) == true) {
                    insertEdge(newEdge(i, j), g);
                }
            }
        }

        printf("\nOrdered Word Ladder Graph\n");
        showGraph(g);
        dfs(g, 0, wordCount, words);
        freeGraph(g);
    }
    return EXIT_SUCCESS;
}

/**
 * @brief function that sets an array to UNVISITED
 * 
 * @param v 
 * @param numv 
 */
void resetArray(int *v, int numv) {
    for (int i = 0; i < numv; i++) {
        v[i] = UNVISITED;
    }
}

/**
 * @brief wrapper function for recursive-DFS, gets longest ladder length and prints those paths
 * 
 * @param g 
 * @param rootv 
 * @param numV 
 * @param words 
 */
void dfs(Graph g, Vertex rootv, int numV, char words[MAXWORDS][MAXWORDLENGTH]) {

    int visited[numV + 1];
    resetArray(visited, numV);

    int maxLength = 0, currLength = 0;
    int counter = 0;

    Vertex startv = rootv;             
    Quack allQuacks[MAXWORDS];
    Quack tempQuack = createQuack();

    char *word = "";

    for (int i = 0; i < MAXWORDS; i++) {
        allQuacks[i] = createQuack();
    }
    
    while (startv <= numV) {         

        //A longer path from a certain root vertex won't exist if it is present in the list of visited 
        // nodes of previous longest path         
        if (visited[startv] == UNVISITED) {
            
            dfsR(g, startv, numV, &counter, allQuacks, &maxLength, &currLength, tempQuack);
            setVisitedArray(allQuacks[0], visited);
            makeEmptyQuack(tempQuack);
            currLength = 0;
        }
        startv++;
    }

    printf("Longest ladder length: %d", maxLength);
    printf("\nLongest ladders:");

    for (int i = 1; i <= counter && i < MAXPATHS; i ++){    //Maximum there will be MAXPATHS to print
        
        Quack tempQuack = allQuacks[i];
        int j = 0;

        //Pop one index from the quack at a time and print the correspoding word from the dictionary
        while (!isEmptyQuack(tempQuack)) {
            word = words[pop(tempQuack)];
            if (strlen(word) > 0) {
                if (j == 0 ) {
                    printf("\n%2d: ", i);
                    printf("%s", word);
                } else {
                    printf(" -> %s", word);
                }
                j++;
            }
        }
    }
    printf("\n");

    for (int i = 0; i < MAXWORDS; i++) {
        destroyQuack(allQuacks[i]);
    }
    destroyQuack(tempQuack);
    return;
}

/**
 * @brief Recursive DFS that maintains max path Length and current path length 
 * finds longest paths and puts it into allQuacks array
 * 
 * @param g 
 * @param v 
 * @param numV 
 * @param counter 
 * @param allQuacks 
 * @param maxLength 
 * @param currLength 
 * @param tempQuack 
 */
void dfsR(Graph g, Vertex v, int numV, int *counter, Quack *allQuacks,
 int *maxLength, int *currLength, Quack tempQuack) {
    qush(v, tempQuack);
    *currLength = *currLength + 1;
    
    //If true then it means we have found a new maximum length path
    if (*currLength > *maxLength) {
        *counter = 1;
        *maxLength = *currLength;
        copyQuackAtIndex(tempQuack, *counter, allQuacks);
        //if true then means we have found 1 more path of same max length and store the path
    } else if (*currLength == *maxLength) {
        *counter = *counter + 1;
        copyQuackAtIndex(tempQuack, *counter, allQuacks);
    } 

    //Start looking from the successor only
    for (Vertex w = v + 1; w < numV; w++) {
        // Go further only if its a successor
        if (isEdge(newEdge(v,w), g)) {
            dfsR(g, w, numV, counter, allQuacks, maxLength, currLength, tempQuack);
            popLastElement(tempQuack);
            *currLength = *currLength - 1;        
        }  
    }
    return;
}

/**
 * @brief function that pops the last element from the quack. 
 * this function will remove '2' from the Quack - [0 -> 1 -> 2]
 * 
 * @param srcQuack 
 */
void popLastElement(Quack srcQuack) {
    Quack auxQuack = createQuack();
    int a ;
    while (!isEmptyQuack(srcQuack)) {
        a = pop(srcQuack);
        push(a, auxQuack);
    }

    pop(auxQuack);
    while (!isEmptyQuack(auxQuack)) {
        a = pop(auxQuack);
        push(a, srcQuack);
    }
    // printf("\nAfter popping ");
    // showQuack(srcQuack);
    destroyQuack(auxQuack);
}

/**
 * @brief function to copy a quack into a particular index of array of quacks
 * 
 * @param srcQuack 
 * @param index 
 * @param allQuacks 
 */
void copyQuackAtIndex(Quack srcQuack, int index, Quack *allQuacks) {
    makeEmptyQuack(allQuacks[index]);
    Quack auxQuack = createQuack();
    int a;
    while (!isEmptyQuack(srcQuack)) {
        a = pop(srcQuack);
        qush(a, allQuacks[index]);
        push(a, auxQuack);
    }

    while (!isEmptyQuack(auxQuack)) {
        a = pop(auxQuack);
        push(a, srcQuack);
    }

    destroyQuack(auxQuack);
}

/**
 * @brief Checks if the two words have an edit distance of '1'
 * 
 * @param word1 
 * @param word2 
 * @return true 
 * @return false 
 */
bool differByOne(char *word1, char *word2) {

    int length1 = strlen(word1);
    int length2 = strlen(word2);
    int changes = 0, i = 0, j = 0;

    while (i < length1 && j < length2) { 
        // If current characters don't match 
        if (word1[i] != word2[j]) { 
            if (changes == 1) //When 2nd change found
                return false; 

            //Move forward for the longer word
            if (length1 > length2) {
                i++; 
            } else if (length1 < length2) {
                j++; 
            } else { //Iflengths of both strings is same 
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

    //When one word is a perfect substring of larger word
    if (j < length2) {
        changes += length2 - j;
    } else {
        changes += length1 - i;
    }
  
    return changes == 1;
}

/**
 * @brief Set the Visited Array object to VISITED based on the elements of Quack
 * 
 * @param srcQuack 
 * @param visited 
 */
void setVisitedArray(Quack srcQuack, int *visited) {
    Quack auxQuack = createQuack();  
    int a;
    while (!isEmptyQuack(srcQuack)) {
        a = pop(srcQuack);
        visited[a] = VISITED;
        push(a, auxQuack);
    }

    while (!isEmptyQuack(auxQuack)) {
        a = pop(auxQuack);
        push(a, srcQuack);
    }

    destroyQuack(auxQuack);
}