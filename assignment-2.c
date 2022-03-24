/**
 * Assignment #2
 * This program computes simple DNA matching between 2 sequences.
 **/

#include <stdio.h>
#include <stdbool.h>

#define BASE_SEQ_LEN 20
#define TARGET_SEQ_LEN 5
#define NUM_BASES 4
#define THRESHOLD 3

/**********************************************************************/

/* function prototypes */
/*  See function definitions below for documentation */
_Bool read_sequence(char[], int);
_Bool match(const char[], const char[], int, int, int);
void print_sequence_part(const char[], int, int);
void print_sequence(const char[], int);
_Bool is_valid_base(char);

/* external variables */
const char bases[] = {'A', 'T', 'C', 'G'};

/**
 * main: This function needs to read and store a sequence of length
 *       BASE_SEQ_LEN. Then it needs to read and store a sequence of 
 *       TARGET_SEQ_LEN. Finally it needs to call match() with both sequences.
**/
int main() {
    char s1[BASE_SEQ_LEN], s2[TARGET_SEQ_LEN];

    // 1: Read base input sequence into s1 array
    if (read_sequence(s1, BASE_SEQ_LEN) == 0) {
      // if read_sequence returned false then there was an error
      printf("ERROR: sequence 1 is bad.  Exiting\n");
      return -1;
    }
    // FIXME: You need to finish the main function 
    // FIXME: 2: Read target input sequence into s2 array

    // FIXME: 3: Call match function to look for match and print merged
    // sequence if a match is found




    return 0;
}

/****************************************************************************
 * Prints a sequence part indicated by the start and end (excluded) indices.*
 ****************************************************************************/
void print_sequence_part(const char s[], int start, int end) {
    for (int i=start; i<end; i++) {
        printf("%c", s[i]);
    }
}

/****************************************************************************
 * Prints a sequence of bases.                                              *
 ****************************************************************************/
void print_sequence(const char s[], int len) {
    print_sequence_part(s, 0, len);
    printf("\n");
}

/****************************************************************************
 * Prompts the user to input a seq_len sequence and press Enter/Return      *
 * reads a sequence of bases (A, T, C, G) and stores it in s.               *
 * Invalid bases (values that not one of the four valid bases) are ignored. *
 * If the  sequence inputed is too short then return false                  *
 *                                                                          *
 * A newline '\n' is generated when the user presses the Enter or Return    *
 * key. In such cases scanf will read the special ascii value '\n' into the *
 * variable.                                                                *  
 *                                                                          *
 * In our case we should continue to read values until we encounter a       *
 * newline.  Prior to reading a newline each valid base should be stored    *
 * consecutively into the s array until seq_len valid values have been      *
 * read.  Any addtioinal values should be ignored.                          *
 * eg.                                                                      *
 *   assuming seq_len=3                                                     *
 *    INPUT: "AGG\n"->  s[0]='A' s[1]='G' s[2]='G' return true              *
 *    INPUT: "aAgGqGv\n"->  s[0]='A' s[1]='G' s[2]='G' return true          *
 *    INPUT: "A\n"-> s[0]='A' return false                                  *
 *    INPUT: "aaaaaaaaaaaaaa' -> return false                               *
 *    INPUT: "AGGTAGGT" -> s[0]='A' s[1]='G' s[2]='G' return true           *
 ****************************************************************************/
_Bool read_sequence(char s[], int seq_len) {
    char b;
    int i = 0;

    printf("Enter a sequence of length %d: ", seq_len);
    // Read first character in to get us started
    scanf("%c", &b);

    // Loop until new line 
    // FIXME: ADD your loop here
    //i think this is only going to work with the firsdt character of the input, have to index thru entire thing
    for (j = 0; j < seq_len; j++) {
      if (is_valid_base(b)) {
        s[i] = b;
        i++;
      }
      if (b != '\n') {
        break;
      }
    }

    // When we are done looping i should hold the length of valid bases read
    if (i != seq_len) {
      printf("Invalid: input sequence too short\n");
       return 0; // sequence too short
    }
    printf("Valid input sequence: ");
    print_sequence(s, seq_len);
    return 1;
}

/****************************************************************************
 * Checks whether the input character represents a valid base.              *
 * Returns false if b is not in the bases array which is preloaded with     *
 *  'A', 'C', 'G', 'T'.                                                     *
 ****************************************************************************/
_Bool is_valid_base(char b) {
  //index through the 4 values in the bases array
  for (i = 0; i < 3; i++) {
    //compare each character in the array to b, if there is a match return true
    if (b == bases[i]) {
      return 1;
    }
  }

  // if we got here then we must not have matched any of the bases elements
  return 0;
}

/****************************************************************************
 *  There are two cases for matching that the function checks for.          *
 *  First is if the target sequence appears inside the base sequence        *
 *  itself.                                                                 *
 *                                                                          *
 *  Example:                                                                *
 *  --------                                                                *
 *  Base: AAACTGGGT             =>  A match was found.                      *
 *  Target: ACTGG                   AAACTGGGT                               *
 *                                                                          *
 *  This would be a match because the target string ACTGG appears fully     *
 *  within the base sequence.                                               *
 *                                                                          *
 *  The other case is if the base sequence is a proper prefix to the target *
 *  string (the last bases of base sequence are the same as the first bases *
 *  of the target sequence) AND the length of the overlap is equal to or    *
 *  greater than the threshold.                                             *
 *                                                                          *
 *  Example, threshold: 3:                                                  *
 *  --------                                                                *
 *  Base: AAACTGGG              =>  A match was found.                      *
 *  Target:    GGGTC                AAACTGGGTC                              *
 *                                                                          *
 *  Base: AAACTGGG              =>  No match found.                         *
 *  Target:     GGACT                                                       *
 *                                                                          *
 *  The first example is a match because GGG is the last 3 bases of the     *
 *  base and the first 3 bases of the target. The second example is NOT a   *
 *  match because only 2 bases overlap.                                     *
 *                                                                          *
 *  Lastly, if the target can both fully appear in the base and be base's   *
 *  suffix, the program should prefer the latter case.                      *
 *                                                                          *
 *  Example:                                                                *
 *  --------                                                                *
 *  s1 = AAAAAAAA, s2 = AAAAA, threshold = 3                                *
 *                                                                          *
 *   =>  A match was found.                                                 *
 *       AAAAAAAAAA                                                         *
 *                                                                          *
 *  Example:                                                                * 
 *  --------                                                                * 
 *  s1 = CCGTTACAGG, s2 = TACAG, threshold = 3                              * 
 *                                                                          * 
 *  CCGTTACAGG                                                              * 
 *         TACAG                                                            * 
 *                                                                          * 
 *  CCGTTACAGG                                                              * 
 *        TACAG                                                             * 
 *                                                                          * 
 *  CCGTTACAGG                                                              * 
 *       TACAG                                                              * 
 *                                                                          * 
 *  CCGTTACAGG                  =>  A match was found.                      *
 *      TACAG                       CCGTTACAGG                              *
 ****************************************************************************/

_Bool match(const char s1[], const char s2[],
     int len1, int len2, int threshold) {
   // FIXME:  This is where the real work has to happen 
   //         implement this function using print_sequence_part as needed

    printf("No match found.\n");
    return 0;
}
