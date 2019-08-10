#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

bool differByOne(char *word1, char *word2);

// #define UNVISITED 0

int main() {
    
    char *s1 = "lent";
    char *s2 = "lentor";
    printf("%d", differByOne(s1, s2));
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
    // if (i < length1 || j < length2) 
    //     changes++;
    if (j < length2) {
        changes += length2 - j;
    } else {
        changes+= length1 - i;
    }
  
    return changes == 1;
}