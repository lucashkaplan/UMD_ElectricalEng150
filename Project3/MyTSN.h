
#ifndef MyTSN
#define MyTSN

#define NEW(x) (x*)malloc(sizeof(x))

typedef struct user
{ int uid;			// a 6-digit user ID
  char password[17];		// password up to 16 characters
  int number;			// number of friends in MyTSN database
  struct user *next;		// next user in the linked list of all users
  struct user *friend;		// pointer to the next friend in the list of 
							// friends for one user 
} USER;

typedef struct
{ USER *head;			// first user in the list of all users
  int number;			// number of users in the list
} ROOT;				// This is the root of the linked list of 
					// all the users in the MyTSN database


typedef struct node
{ int uid;
  struct node *next;		// next node in a path
} NODE;				// this is the node in a path

typedef struct path
{ NODE *head;
  NODE *tail;
  int length;
  struct path *next;
} PATH;			// this is a shortest path from NODE head to
				// NODE tail. There are length+1 NODES and 
				// length number of edges in the path, 
				// including both head and tail

typedef struct 
{ PATH *head;
  int number;
} PATH_ROOT;	// a root to all the paths that will be asked
				// to find in the MyTSN database
    

USER *make_user(int);		// allocate memory for a new user with UID
							// and a random password

ROOT *make_root();		// root for the entire database

void add_user (int, ROOT * r);	// add a new user with the given UID to the
								// the database that pointer r points to
								// assumptions 1): r exits; 2) UID does not exit in the database

USER *copy_user (USER *);	// make a copy of a user 

void add_friendship(int id1, int id2, ROOT * r);
				// add a pair of users (id1, id2) as friends
				// in the database that pointer r points to
				// assumptions: 1) r is not NULL; 2) users with id1 and id2
				//      exist in the database.
				// id2 will be added as a friend of id1 and vice versa

NODE *make_node(int);		// make a new node in a path with a given uid

PATH *make_path(NODE *, NODE *);	
				// make a path between two nodes/users, and 
				// set the two users as head and tail

PATH_ROOT *keep_paths();	// make a root to all the paths that might be
							// queried during the execution 


void assignPassword(ROOT *r);	// assign a random password to each user in r

void writeOutput(FILE *, ROOT *r, PATH_ROOT *p);
				// produce the output file. 
				// r points to the user database
				// p points to all the shortest paths that 
				// have been queried during the execution

USER *search_user(ROOT *r, int uid);
				// search user with uid in the database r
				// return a pointer to this user if found
				// return NULL if such uid does not exist
				// this make the function reusable


void print_Friends(USER *u);	// print out all the friends of user u
	// using the following format
	// User <UID> has no friends.
	// User <UID>'s friends: <UID1>, <UID2>, ... <UIDk>.


void add_a_friend(ROOT *r, int, int);
				// add two users as friends to each other
				// both users are already in the database
				// if they are already friends, print out
				// Users <UID1> and <UID2> are already friends.


int remove_friendship(ROOT *r, int, int);
	// remove a friendship from the database
	// assumption: two uids are both valid and exist in the database 
	// Users <UID1> and <UID2> are not friends.  		return 0
	// Users <UID1> and <UID2> are no longer friends.	return 1		 

void remove_user(ROOT *r, int);
	// remove the user with the given uid
	// assumption: uid is valid and exits in the database
	// User <UID> has been removed from MyTSN.



PATH *find_path(ROOT *r, int id1, int id2, PATH_ROOT *p);
				// find a shortest path between two users
				// a pointer to the path is returned
				// the path is added to the list pointed to by p
				// assumption: id1 and id2 are valid and exist in the database

#endif
