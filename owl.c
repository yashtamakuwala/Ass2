#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

bool differByOne(char *word1, char *word2);
bool is1LetterChange(char *word1, char *word2);
bool is1LetterAdded(char *word1, char *word2, int length1, int length2);

int main() {
    char s1[100], s2[100];

    scanf("%s %s", s1, s2);

    printf("\n%s and %s differ by 1? %d", s1, s2, differByOne(s1, s2));
    printf("\n");

    return EXIT_SUCCESS;
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