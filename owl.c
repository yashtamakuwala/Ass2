#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "Graph.h"

bool differByOne(char *word1, char *word2);
void dfs(Graph g, Vertex rootv, int numV);
void dfsR(Graph g, Vertex v, int numV, int *order, int *visited);
void printArray(char *word, int *v, int numV);
int* mallocArray(int n);
void resetArray(int *v, int numv);
// #define UNVISITED 0

int main() {
    char words[1000][20];   //max number of words - 1000, max length of word - 20 characters
    int i = 1, wordCount = 0;
    char word[20];
    wordCount = 0;

    while((scanf("%s", word)) == 1) {
        strcpy(words[wordCount], word);
        wordCount++;
    }

    printf("\nDictionary:");
    for (i = 0; i < wordCount; i++) {
        printf("\n%d: %s", i, words[i]);
    }
    printf("\n");

    Graph g = newGraph(wordCount);

    for(i = 0; i < wordCount; i++) {
        for (int j = 0; j < wordCount; j++) {
            if(differByOne(words[i], words[j]) == true) {
                insertEdge(newEdge(i, j), g);
            }
        }
    }

    showGraph(g);
    dfs(g, 0, wordCount);
    return EXIT_SUCCESS;
}

void dfs(Graph g, Vertex rootv, int numV) {//'wrapper' for recursive dfs
   int *visited = mallocArray(numV);  // ... handles disconnected graphs
   resetArray(visited, numV);
   printArray("Visited: ", visited, numV);
   int order = 0;
   Vertex startv = rootv;             // this is the starting vertex
   int allVis = 0;                    // assume not all visited
   while (!allVis) {                  // as long as there are vertices
      dfsR(g, startv, numV, &order, visited);
      allVis = 1;                     // are all visited now?
      for (Vertex w = 0; w < numV && allVis; w++) { // look for more
         if (visited[w] == -1) { 
            printf("Graph is disconnected\n"); // debug
            allVis = 0;               // found an unvisited vertex
            startv = w;              // next loop dfsR this vertex
         }
      }
   }
   printArray("Visited: ", visited, numV);
   free(visited);
   return;
}

void dfsR(Graph g, Vertex v, int numV, int *order, int *visited) {
    visited[v] = *order;
    printf("\nVertex v:%d, numv : %d, order: %d Visited[v]: %d",v,numV,*order, visited[v]);
    *order = *order+1;
    for (Vertex w = v + 1; w < numV; w++) {
        // printf("\nv: %d w: %d", v,w);
        if (isEdge(newEdge(v,w), g) && visited[w] == -1) {
            printf("\nValid edge v: %d w: %d", v, w);
            dfsR(g, w, numV, order, visited);
        }
    }
    return;
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
    if (i < length1 || j < length2) 
        changes++; 
  
    return changes == 1;
}