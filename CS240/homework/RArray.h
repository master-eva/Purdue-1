
typedef struct RArrayEntry {
  char * value;
} RArrayEntry;

typedef struct RArray {
  int nElements;  // Number of elements in the array that are in use
  int maxElements; // Maximum number of elements in the array
  RArrayEntry * array;
} RArray;

