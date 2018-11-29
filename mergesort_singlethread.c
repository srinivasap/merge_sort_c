#include <stdio.h> 
#include <stdlib.h> 
#include <time.h> 
#include <ctype.h> 
#include <string.h>

  // default file to be used as input if no program args passed
  #define DEFAULT_FILE "random_numbers_10.in"
//input array
int *ia;

/* 
 * Data structure to maintain the array boundary information.
 */
typedef struct Arr {
  int low;
  int high;
}
ArrayBoundsInfo;

// function declaration area
void printArray(int startIndex, int endIndex);
void merge(int low, int high);
void * merge_sort(void * a);
int init_array_from_file(int argc, char * argv[]);

/**
 * Compare left and right sorted subarrays and merge them into one sorted subarray
 * with the help of auxiliary array.
 * 
 * @param low start of the left subarray
 * @param high end of right subarray
 */
void merge(int low, int high) {
  printf("\nmerge(%d, %d) unsorted: ", low, high);
  printArray(low, high + 1);

  int mid = (low + high) / 2;
  int left = low;
  int right = mid + 1;
  // aux array
  int b[high - low + 1];
  int i, cur = 0;

  // compare left and right while incrementing reference correspondig to smallest
  // until either hits mid or high boundary limits
  while (left <= mid && right <= high) {
    if (ia[left] > ia[right])
      b[cur++] = ia[right++];
    else
      b[cur++] = ia[left++];
  }
  // if right hit bounday, move all remaining left to aux
  while (left <= mid) {
    b[cur++] = ia[left++];
  }
  // if left hit bounday, move all remaining right to aux
  while (right <= high) {
    b[cur++] = ia[right++];
  }
  // copy back from aux array to original array
  for (i = 0; i < (high - low + 1); i++) {
    ia[low + i] = b[i];
  }
  printf("\nmerge(%d, %d) sorted: ", low, high);
  printArray(low, high + 1);
}

/**
 * Recursively divide array into subarrays and merge them into sorted subarrays
 * @param a pointer reference to ArrayBoundsInfo
 */
void * merge_sort(void * a) {
  ArrayBoundsInfo * pa = (ArrayBoundsInfo * ) a;
  if (pa-> low >= pa -> high)
    return 0;
  
  int low = pa-> low, high = pa -> high, mid = (low + high) / 2;
  printf("\nmerge_sort (%d, %d) : ", low, high);
  printArray(low, high + 1);

  ArrayBoundsInfo aIndex[2];
  aIndex[0].low = low;
  aIndex[0].high = mid;
  merge_sort( & aIndex[0]);

  aIndex[1].low = mid + 1;
  aIndex[1].high = high;
  merge_sort( & aIndex[1]);

  merge(low, high);

  return 0;
}

/**
 * Reads the input file containing randomly ordered numbers and 
 * initializes ia in the same order for sorting.
 * 
 * @param argc count of program args
 * @param argv the args as array 
 */
int init_array_from_file(int argc, char * argv[]) {
  // use default file if input arg is not passed
  char * input_file_name = DEFAULT_FILE;
  // if input arg passed, used it as input file
  if (argc > 1) {
    input_file_name = argv[1];
  }

  // check if input file is readable, if not exit
  FILE * fptr;
  fptr = fopen(input_file_name, "r");
  if (!fptr) {
    printf("\nCan't open input file %s, exiting...\n", input_file_name);
    exit(0);
  }

  // read content if input file and store into string
  int fsize;
  int icount = 0;
  char ch;
  fseek(fptr, 0L, SEEK_END);
  fsize = ftell(fptr);
  fseek(fptr, 0L, SEEK_SET);
  char string[fsize];
  int i = 0;
  while ((ch = getc(fptr)) != EOF) {
    //printf("\nread char %c", ch);
    string[i] = ch;
    i++;
  }

  // count snumber of intergers in input and
  // format input to replace newline with tab
  for (i = 0; string[i] != '\0'; i++) {
    if (isdigit(string[i])) {
      while (string[i] != '\t' && string[i] != '\n') {
        i++;
      }
      // if found new line replace with tab
      if (string[i] == '\n') {
        //printf("\nReplacing new line with tab...");
        string[i] = '\t';
      }
      icount++;
    }
  }

  // allocate memory to input array (ia)
  ia = malloc(sizeof(int) * icount);
  i = 0;

  char * snumber;
  snumber = strtok(string, "\t");
  while (snumber != NULL) {
    ia[i] = atoi(snumber);
    //printf("\nFound snumber %s %d", snumber, ia[i]);
    snumber = strtok(NULL, "\t");
    i++;
  }

  fclose(fptr);
  return icount;
}

/**
 *  Print the array ai
 *  @param arrlen the length of the array
 */
void printArray(int startIndex, int endIndex) {
  for (int i = startIndex; i < endIndex; i++)
    printf("%d ", ia[i]);
}

/**
 * Entry point of the program
 * argv[1] : is the treated as input file containing numbers to sort
 *           by default DEFAULT_FILE "random_snumbers_10.in" will be input if no arg is passed
 */
int main(int argc, char * argv[]) {
  int arrlen = init_array_from_file(argc, argv);
  printf("Unsorted input array of length %d : ", arrlen);
  printArray(0, arrlen);
  printf("\n");

  // set array metadata for merge sort
  ArrayBoundsInfo ai;
  ai.low = 0;
  ai.high = arrlen - 1;

  // measure time taken by merge sort
  clock_t t;
  t = clock();
  merge_sort( & ai);
  t = clock() - t;
  double time_taken = ((double) t) / CLOCKS_PER_SEC;
  printf("\n\nsingle-thread mergesort took %f milliseconds to sort array of length %d: ", time_taken * 1000, arrlen);
  printArray(0, arrlen);
  printf("\n");

  // test start
  unsigned long time_spent = 18446744073709551615;
  printf("\n\nvalue of time_spent %lu", time_spent);
  printf("\nvalue of time_spent >> 32 %lu", time_spent >> 32);
  printf("\nvalue of time_spent << 32 %lu", time_spent << 32);
  // test end

  return 0;
}