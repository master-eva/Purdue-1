
typedef struct SLListEntry {
    char * name;
    char * value;
    struct SLListEntry * next;
} SLListEntry;

typedef struct SLList {
   SLListEntry * head;
} SLList;


