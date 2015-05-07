
typedef struct DLListEntry {
  public: 
    char * name;
    char * value;
    DLListEntry * next;
    DLListEntry * previous;
};

class DLList {
  DLListEntry * head;  // This node delimits the list. It does not stores any data. 
public:
  // Constructor
  DLList();

 // Adds new name, value pair at the end of the list. If name already exists substitute value in that entry and return 1. Otherwise return 0
  int add(char *  name, char * value); 

  // Removes node from the list with that name. Return 0 if nde does not exist.
  int remove(char * name);

  // Concatenates an existing list to this list.
  void concatenate(DLList * list);

  // Destructor
  ~DLList();
};





