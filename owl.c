#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "Graph.h"

bool differByOne(char *word1, char *word2);
bool is1LetterChange(char *word1, char *word2);
bool is1LetterAdded(char *word1, char *word2, int length1, int length2);
Graph constructGraph(Graph g, int wordCount, char **words);

int main() {
    // char s1[100], s2[100];
    char words[1000][20];   //max number of words - 1000, max length of word - 20 characters
    // scanf("%s %s", s1, s2);

    // printf("\n%s and %s differ by 1? %d", s1, s2, differByOne(s1, s2));
    // printf("\n");

    int i = 1, wordCount = 0;
    char word[20];
    wordCount = 0;
    while((scanf("%s", word)) == 1) {
        // printf("\n%s", word);
        strcpy(words[wordCount], word);
        printf("\nwords[i] :%s", words[wordCount]);
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
    
    return EXIT_SUCCESS;
}

Graph constructGraph(Graph g, int wordCount, char **words) {
    return g;
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

bool is1LetterAdded(char *word1, char *word2, int length1, int length2){
    char *largerWord, *smallerWord;
    int changes = 0;

    if (length1 > length2) {
        strcpy(largerWord, word1);
        strcpy(smallerWord, word2);
    } else {
        strcpy(smallerWord, word1);
        strcpy(largerWord, word2);
    }

    int i = 0, j = 0;
    
    while (i < length1 && j < length2) {
        if (word1[i] != word2[j]) {
            if (length1 > length2) {
                i++;
            } else {
                j++;
            }
            changes += 1;
        } else {
            i++;
            j++;
        }
    }
    
    return true;
}

bool is1LetterChange(char *word1, char *word2){

    //Same word is incorrect
    if (strcmp(word1, word2) == 0) {
        return false;
    }

    bool isChange = false;
    
    for (int i = 0; word1[i] != '\0'; i++) {
        if (word1[i] != word2[i]){
            if (isChange == true) { //if another change detected then return false
                return false;
            } else {
                isChange = true;
            }
        }    
    }

    return isChange;
}