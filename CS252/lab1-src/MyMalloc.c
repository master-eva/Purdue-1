//
// CS252: MyMalloc Project
//
// The current implementation gets memory from the OS
// every time memory is requested and never frees memory.
//
// You will implement the allocator as indicated in the handout.
// 
// Also you will need to add the necessary locking mechanisms to
// support multi-threaded programs.
//

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/mman.h>
#include <pthread.h>
#include "MyMalloc.h"

static pthread_mutex_t mutex;

const int ArenaSize = 2097152;
const int NumberOfFreeLists = 1;

// Header of an object. Used both when the object is allocated and freed
struct ObjectHeader {
    size_t _objectSize;         // Real size of the object.
    int _allocated;             // 1 = yes, 0 = no 2 = sentinel
    struct ObjectHeader * _next;       // Points to the next object in the freelist (if free).
    struct ObjectHeader * _prev;       // Points to the previous object.
};

struct ObjectFooter {
    size_t _objectSize;
    int _allocated;
};

  //STATE of the allocator

  // Size of the heap
  static size_t _heapSize;

  // initial memory pool
  static void * _memStart;

  // number of chunks request from OS
  static int _numChunks;

  // True if heap has been initialized
  static int _initialized;

  // Verbose mode
  static int _verbose;

  // # malloc calls
  static int _mallocCalls;

  // # free calls
  static int _freeCalls;

  // # realloc calls
  static int _reallocCalls;
  
  // # realloc calls
  static int _callocCalls;

  // Free list is a sentinel
  static struct ObjectHeader _freeListSentinel; // Sentinel is used to simplify list operations
  static struct ObjectHeader *_freeList;


  //FUNCTIONS

  //Initializes the heap
  void initialize();

  // Allocates an object 
  void * allocateObject( size_t size );

  // Frees an object
  void freeObject( void * ptr );

  // Returns the size of an object
  size_t objectSize( void * ptr );

  // At exit handler
  void atExitHandler();

  //Prints the heap size and other information about the allocator
  void print();
  void print_list();

  // Gets memory from the OS
  void * getMemoryFromOS( size_t size );

  void increaseMallocCalls() { _mallocCalls++; }

  void increaseReallocCalls() { _reallocCalls++; }

  void increaseCallocCalls() { _callocCalls++; }

  void increaseFreeCalls() { _freeCalls++; }

extern void
atExitHandlerInC()
{
  atExitHandler();
}

void initialize()
{
  // Environment var VERBOSE prints stats at end and turns on debugging
  // Default is on
  _verbose = 1;
  const char * envverbose = getenv( "MALLOCVERBOSE" );
  if ( envverbose && !strcmp( envverbose, "NO") ) {
    _verbose = 0;
  }

  pthread_mutex_init(&mutex, NULL);
  void * _mem = getMemoryFromOS( ArenaSize + (2*sizeof(struct ObjectHeader)) + (2*sizeof(struct ObjectFooter)) );

  // In verbose mode register also printing statistics at exit
  atexit( atExitHandlerInC );

  //establish fence posts
  struct ObjectFooter * fencepost1 = (struct ObjectFooter *)_mem;
  fencepost1->_allocated = 1;
  fencepost1->_objectSize = 123456789;
  char * temp = 
      (char *)_mem + (2*sizeof(struct ObjectFooter)) + sizeof(struct ObjectHeader) + ArenaSize;
  struct ObjectHeader * fencepost2 = (struct ObjectHeader *)temp;
  fencepost2->_allocated = 1;
  fencepost2->_objectSize = 123456789;
  fencepost2->_next = NULL;
  fencepost2->_prev = NULL;

  //initialize the list to point to the _mem
  temp = (char *) _mem + sizeof(struct ObjectFooter);
  struct ObjectHeader * currentHeader = (struct ObjectHeader *) temp;
  temp = (char *)_mem + sizeof(struct ObjectFooter) + sizeof(struct ObjectHeader) + ArenaSize;
  struct ObjectFooter * currentFooter = (struct ObjectFooter *) temp;
  _freeList = &_freeListSentinel;
  currentHeader->_objectSize = ArenaSize + sizeof(struct ObjectHeader) + sizeof(struct ObjectFooter); //2MB
  currentHeader->_allocated = 0;
  currentHeader->_next = _freeList;
  currentHeader->_prev = _freeList;
  currentFooter->_allocated = 0;
  currentFooter->_objectSize = currentHeader->_objectSize;
  _freeList->_prev = currentHeader;
  _freeList->_next = currentHeader; 
  _freeList->_allocated = 2; // sentinel. no coalescing.
  _freeList->_objectSize = 0;
  _memStart = (char*) currentHeader;
}

void * allocateObject( size_t size )
{
  //Make sure that allocator is initialized
  if ( !_initialized ) {
    _initialized = 1;
    initialize();
  }

  // Add the ObjectHeader/Footer to the size and round the total size up to a multiple of
  // 8 bytes for alignment.
  size_t roundedSize = (size + sizeof(struct ObjectHeader) + sizeof(struct ObjectFooter) + 7) & ~7;

  // Naively get memory from the OS every time
  // void * _mem = getMemoryFromOS( roundedSize );
  int allocated = 0;

  // go through the free list to find a space allocate the object
  void * result;
  struct ObjectHeader * currentHeader = _freeList->_next;
  while (currentHeader != _freeList && allocated == 0) {
    if (currentHeader->_objectSize < roundedSize) {
      //remove the block
      currentHeader->_next->_prev = currentHeader->_prev;
      currentHeader->_prev->_next = currentHeader->_next;
    } else if (currentHeader->_objectSize <= roundedSize + 8 + sizeof(struct ObjectHeader) + sizeof(struct ObjectFooter)) {
      allocated = 1;
      struct ObjectFooter * currentFooter =  (struct ObjectFooter *) ((void *) currentHeader + currentHeader->_objectSize - (sizeof(struct ObjectFooter)));
      // set block to allocated
      currentHeader->_allocated = 1;
      currentFooter->_allocated = 1;
      // remove from free list
      currentHeader->_prev->_next = currentHeader->_next;
      currentHeader->_next->_prev = currentHeader->_prev;
      result = ((void *) currentHeader + (sizeof(struct ObjectHeader)));
    } else {
      allocated = 1;
      // calculate footers and headers
      struct ObjectFooter * currentFooter = (struct ObjectFooter *) ((void *) currentHeader + currentHeader->_objectSize - (sizeof(struct ObjectFooter)));
      struct ObjectFooter * newFooter = (struct ObjectFooter *) ((void *) currentHeader + roundedSize - (sizeof(struct ObjectFooter)));
      struct ObjectHeader * newHeader = (struct ObjectHeader *) ((void *) newFooter + (sizeof(struct ObjectFooter)));
      // calculate parameter of free block
      newHeader->_allocated = 0;
      newHeader->_objectSize = currentHeader->_objectSize - roundedSize;
      currentFooter->_allocated = 0;
      currentFooter->_objectSize = newHeader->_objectSize;
      // assign parameters to the used block
      currentHeader->_allocated = 1;
      newFooter->_allocated = 1;
      currentHeader->_objectSize = roundedSize;
      newFooter->_objectSize = roundedSize;
      // replace the orignal block with the new free block
      newHeader->_prev = currentHeader->_prev;
      newHeader->_next = currentHeader->_next;
      newHeader->_prev->_next = newHeader;
      newHeader->_next->_prev = newHeader;
      result = ((void *) currentHeader + (sizeof(struct ObjectHeader)));
    }
    currentHeader = currentHeader->_next;
  }

  // if didn't find a space for th object
  if (allocated == 0) {
    void * _mem = getMemoryFromOS( ArenaSize + 2 * (sizeof(struct ObjectHeader)) + 2 * (sizeof(struct ObjectFooter)));

    // set dummy footer and header
    struct ObjectFooter * fencepost1 = (struct ObjectFooter *)_mem;
    fencepost1->_allocated = 1;
    fencepost1->_objectSize = 123456789;
    struct ObjectHeader * fencepost2 = (struct ObjectHeader *) ((void *)_mem + (sizeof(struct ObjectHeader)) + 2 * (sizeof(struct ObjectFooter)) + ArenaSize);
    fencepost2->_allocated = 1;
    fencepost2->_objectSize = 123456789;
    fencepost2->_next = NULL;
    fencepost2->_prev = NULL;

    struct ObjectHeader * currentHeader = (struct ObjectHeader *) ((void *) _mem + (sizeof(struct ObjectFooter)));
    struct ObjectFooter * currentFooter = (struct ObjectFooter *) ((void *) currentHeader + roundedSize - (sizeof(struct ObjectFooter)));
    struct ObjectHeader * newHeader = (struct ObjectHeader *) ((void *) currentFooter + (sizeof(struct ObjectFooter)));
    struct ObjectFooter * newFooter = (struct ObjectFooter *) ((void *) currentHeader + ArenaSize - (sizeof(struct ObjectFooter)));
    
    // assign free block
    newHeader->_allocated = 0;
    newHeader->_objectSize = ArenaSize + (sizeof(struct ObjectHeader)) + (sizeof(struct ObjectFooter)) - roundedSize;
    newFooter->_allocated = 0;
    newFooter->_objectSize = newHeader->_objectSize;
    //insert it into free list
    struct ObjectHeader * ptr = _freeList->_next; 
    while (ptr != _freeList) {
      if (newHeader < ptr) break;
      ptr = ptr->_next;
    }

    if (ptr == _freeList) {
      //insert at last
      newHeader->_prev = _freeList->_prev;
      newHeader->_next = _freeList;
      _freeList->_prev->_next = newHeader;
      _freeList->_prev = newHeader;
    } else {
      //insert before ptr
      newHeader->_prev = ptr->_prev;
      newHeader->_next = ptr;
      ptr->_prev->_next = newHeader;
      ptr->_prev = newHeader;
    }
    // assign object
    currentHeader->_allocated = 1;
    currentFooter->_allocated = 1;
    currentHeader->_objectSize = roundedSize;
    currentFooter->_objectSize = roundedSize;

    result = ((void *) currentHeader + (sizeof(struct ObjectHeader)));
  }

  //o->_objectSize = roundedSize;
  pthread_mutex_unlock(&mutex);

  // Return a pointer to usable memory
  return result;
}

void freeObject( void * ptr )
{
  struct ObjectHeader * currentHeader = (struct ObjectHeader *) (void *) (ptr - (sizeof(struct ObjectHeader)));
  struct ObjectFooter * currentFooter = (struct ObjectFooter *) ((void *) currentHeader + currentHeader->_objectSize - (sizeof(struct ObjectFooter)));
  struct ObjectFooter * prevFooter = (struct ObjectFooter *) ((void *) currentHeader -  (sizeof(struct ObjectFooter)));
  struct ObjectHeader * nextHeader = (struct ObjectHeader *) ((void *) currentFooter +  (sizeof(struct ObjectFooter)));
  currentHeader->_allocated = 0;
  currentFooter->_allocated = 0;

  if (prevFooter->_allocated != 0 && nextHeader->_allocated != 0) {
    //insert this one to freelist
    struct ObjectHeader * ptr = _freeList->_next; 
    while (ptr != _freeList) {
      if (currentHeader < ptr) break;
      ptr = ptr->_next;
    }
    //insert before ptr
    currentHeader->_prev = ptr->_prev;
    currentHeader->_next = ptr;
    ptr->_prev->_next = currentHeader;
    ptr->_prev = currentHeader;
  } else if (prevFooter->_allocated == 0 && nextHeader->_allocated != 0) {
    //extend prev object to a larger size
    struct ObjectHeader * prevHeader = (struct ObjectHeader *) ((void *) currentHeader - prevFooter->_objectSize);
    prevHeader->_objectSize = prevHeader->_objectSize + currentHeader->_objectSize;
    currentFooter->_objectSize = prevHeader->_objectSize;
  } else if (prevFooter->_allocated != 0 && nextHeader->_allocated == 0) {
    struct ObjectFooter * nextFooter = (struct ObjectFooter *) ((void *) nextHeader + nextHeader->_objectSize - (sizeof(struct ObjectFooter)));
    currentHeader->_objectSize = currentHeader->_objectSize + nextHeader->_objectSize;
    nextFooter->_objectSize = currentHeader->_objectSize;
    // change the free list
    currentHeader->_next = nextHeader->_next;
    currentHeader->_prev = nextHeader->_prev;
    currentHeader->_next->_prev = currentHeader;
    currentHeader->_prev->_next = currentHeader;
  } else if (prevFooter->_allocated == 0 && nextHeader->_allocated == 0) {
    struct ObjectHeader * prevHeader = (struct ObjectHeader *) ((void *) currentHeader - prevFooter->_objectSize);
    struct ObjectFooter * nextFooter = (struct ObjectFooter *) ((void *) nextHeader + nextHeader->_objectSize - (sizeof(struct ObjectFooter)));
    prevHeader->_objectSize = prevHeader->_objectSize + currentHeader->_objectSize + nextHeader->_objectSize;
    nextFooter->_objectSize = prevHeader->_objectSize;
    // modify free list, delete the second free block
    nextHeader->_next->_prev = nextHeader->_prev;
    nextHeader->_prev->_next = nextHeader->_next;
  }

  pthread_mutex_unlock(&mutex);

  return;
}

size_t objectSize( void * ptr )
{
  // Return the size of the object pointed by ptr. We assume that ptr is a valid obejct.
  struct ObjectHeader * o =
    (struct ObjectHeader *) ( (char *) ptr - sizeof(struct ObjectHeader) );

  // Substract the size of the header
  return o->_objectSize;
}

void print()
{
  printf("\n-------------------\n");

  printf("HeapSize:\t%zd bytes\n", _heapSize );
  printf("# mallocs:\t%d\n", _mallocCalls );
  printf("# reallocs:\t%d\n", _reallocCalls );
  printf("# callocs:\t%d\n", _callocCalls );
  printf("# frees:\t%d\n", _freeCalls );

  printf("\n-------------------\n");
}

void print_list()
{
  printf("FreeList: ");
  if ( !_initialized ) {
    _initialized = 1;
    initialize();
  }
  struct ObjectHeader * ptr = _freeList->_next;
  while(ptr != _freeList){
      long offset = (long)ptr - (long)_memStart;
      printf("[offset:%ld,size:%zd]",offset,ptr->_objectSize);
      ptr = ptr->_next;
      if(ptr != NULL){
          printf("->");
      }
  }
  printf("\n");
}

void * getMemoryFromOS( size_t size )
{
  // Use sbrk() to get memory from OS
  _heapSize += size;
 
  void * _mem = sbrk( size );

  if(!_initialized){
      _memStart = _mem;
  }

  _numChunks++;

  return _mem;
}

void atExitHandler()
{
  // Print statistics when exit
  if ( _verbose ) {
    print();
  }
}

//
// C interface
//

extern void *
malloc(size_t size)
{
  pthread_mutex_lock(&mutex);
  increaseMallocCalls();
  
  return allocateObject( size );
}

extern void
free(void *ptr)
{
  pthread_mutex_lock(&mutex);
  increaseFreeCalls();
  
  if ( ptr == 0 ) {
    // No object to free
    pthread_mutex_unlock(&mutex);
    return;
  }
  
  freeObject( ptr );
}

extern void *
realloc(void *ptr, size_t size)
{
  pthread_mutex_lock(&mutex);
  increaseReallocCalls();
    
  // Allocate new object
  void * newptr = allocateObject( size );

  // Copy old object only if ptr != 0
  if ( ptr != 0 ) {
    
    // copy only the minimum number of bytes
    size_t sizeToCopy =  objectSize( ptr );
    if ( sizeToCopy > size ) {
      sizeToCopy = size;
    }
    
    memcpy( newptr, ptr, sizeToCopy );

    //Free old object
    freeObject( ptr );
  }

  return newptr;
}

extern void *
calloc(size_t nelem, size_t elsize)
{
  pthread_mutex_lock(&mutex);
  increaseCallocCalls();
    
  // calloc allocates and initializes
  size_t size = nelem * elsize;

  void * ptr = allocateObject( size );

  if ( ptr ) {
    // No error
    // Initialize chunk with 0s
    memset( ptr, 0, size );
  }

  return ptr;
}

