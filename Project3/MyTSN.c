/*
Lucas Kaplan
ENEE150
0101
4/29/20

This program implements the functions called in p3.c so that the main function can run
correctly.
*/


#include <stdio.h>
#include <stdlib.h>
#include "MyTSN.h"
#include <time.h>

//creating password for USER temp
void newPassword(USER *temp)
{	//to create different passwords each time, use static int as seed
	static int seed = 0;
	srand(seed);
	
	int charNum = rand()%9 + 8; //number of characters in password, between 8 and 16
	int i=0, digit=0, letter=0, symbolNum=0; //for random digits, letters, and symbols
	char symbols[] = {'~', '!', '@', '#', '$', '%', '&', '*'}; //array of special chars to be used in password
	
	letter = rand()%26;
	temp->password[0] = 'A' + letter; //first char is uppercase letter
	
	digit = rand()%10;
	temp->password[1] = '0' + digit; //second char is random digit
		
	symbolNum = rand()%8;
	temp->password[2] =	symbols[symbolNum]; //3rd char is special char
	
	//rest of characters in password are lowercase letters
	for(i=3;i<charNum;i++)
	{	letter = rand()%26;
		temp->password[i] = 'a' + letter;
	}
	
	seed++; //increase seed for next password
}

void printMenu()
{  printf("\n1: search for a user\n"
             "2: list a user's friends\n"
             "3: add a new friend to a user\n"
             "4: add a new user\n"
             "5: remove a friendship\n"
             "6: remove a user\n"
             "7: find a connection\n"
             "0: EXIT\n\n"
             "Enter your choice (0-7):");
}

USER *make_user(int a)
{ 	USER *this;
	
	this = NEW(USER); //allocate memory for new user node (called USER)
	if (this == NULL) 
		exit(0);
	this->uid = a;
	newPassword(this); //create password for USER this
	this->number = 0;
	this->next = NULL;
	this->friend = NULL;

	return this;
}

ROOT *make_root()
{ ROOT *this;
  this = NEW(ROOT);
  if (this == NULL)
    exit(0);
  this->head = NULL;
  this->number = 0;

  return this;
}


NODE *make_node(int id)
{	NODE *this;
	
	this = NEW(NODE); //allocate memory for new NODE pointer
	this->uid = id;
	this->next = NULL;

	return this;
}


PATH *make_path(NODE *s, NODE *t)
{	PATH *this;
	
	this = NEW(PATH); //allocate memory for new PATH 
	if (this == NULL) 
		exit(0);
	this->head = s; //set NODE s as start
	this->tail = t; //set NODE t as tail
	this->length = 0;
	this->next = NULL;
	
	return this;
}

//make new PATH and add it to tail of AllPath linked list
PATH *add_and_make_path(NODE *s, NODE *t, PATH_ROOT *r)
{	PATH *Tom, *temp;

	Tom = make_path(s, t); //make new PATH starting as s and ending at t
	if (Tom == NULL)
		exit(0);
	
	//if inserting first PATH, insert at head
	if(r->number == 0)
	{	Tom->next = r->head;
		r->head = Tom;
		r->number++;
		return Tom;
	}
	
	//otherwise, insert at tail
	//traverse to 1 PATH before tail
	for(temp=r->head; temp->next != NULL; temp = temp->next);
	
	temp->next = Tom;
	Tom->next = NULL;
	r->number++;
	
	return Tom;
}

//make NODE, then add to PATH at NODE after head
void add_and_make_node(int id, PATH *r)
{	NODE *Tom;

	Tom = make_node(id);
	if (Tom == NULL)
		exit(0);
	Tom->next = r->head->next;
	r->head->next = Tom;
	r->length++;

	return;
}

//make PATH_ROOT to AllPaths, linked list with shortest paths asked for during execution
PATH_ROOT *keep_paths()
{	PATH_ROOT *this;
	
	this = NEW(PATH_ROOT); //allocate memory for new PATH_ROOT pointer
	if (this == NULL)
		exit(0);
	this->head = NULL;
	this->number = 0;
	
	return this;
}

/***********************
*** other functions   **
***********************/

void add_user (int id1, ROOT *r)
{ USER *Tom;

  Tom = make_user(id1);
  if (Tom == NULL)
    exit(0);
  Tom->next = r->head;
  r->head = Tom;
  r->number++;

  return;
}

//adding copied USER to tail of copied linked list (CopyUser)
void add_copied_user(USER *u, ROOT *rCopy)
{	USER *Tom, *temp;

	Tom = copy_user(u); //copy user
	
	//if inserting first user, insert at head
	if(rCopy->number == 0)
	{	Tom->next = rCopy->head;
		rCopy->head = Tom;
		rCopy->number++;
		return;
	}
	
	//otherwise, insert user at tail
	//traverse to 1 before tail
	for(temp=rCopy->head; temp->next != NULL; temp = temp->next);
	
	temp->next = Tom;
	Tom->next = NULL;
	rCopy->number++;
	
	return;
}

USER *copy_user (USER *u)
{ USER *this;
  int i;

  this = NEW(USER);
  this->uid = u->uid;
  for (i=0; u->password[i]!='\0'; i++)
    this->password[i] = u->password[i];
  this->password[i] = '\0';
  this->number = u->number; 
  this->next = u->next;
  this->friend = u->friend;

  return this;
}

void add_friendship(int id1, int id2, ROOT * r)
{ USER *Tom, *Jerry, *u1, *u2;
  
  for (Tom = r->head; Tom != NULL; Tom = Tom->next)
  { if (Tom->uid == id1)
    { for (Jerry = r->head; Jerry != NULL; Jerry = Jerry->next)
      { if (Jerry->uid == id2)
          break;
      }
      break;
    }
  }
  
	//if users already friends, print message and exit
	//traverse Tom's friend list to search for id2 (Jerry's id)
	for(u1 = Tom->friend; u1 != NULL; u1=u1->friend)
	{	if(u1->uid == id2)
		{	printf("Users %06d and %06d are already friends.\n", id1, id2);
			return;
		}
	}

  u1 = copy_user(Tom);
  u2 = copy_user(Jerry);

  u2->friend = Tom->friend;
  Tom->friend = u2;
  Tom->number++;

  u1->friend = Jerry->friend;
  Jerry->friend = u1;
  Jerry->number++;
	
  return;
}

void writeOutput(FILE *f, ROOT *r, PATH_ROOT *p)
{	USER *temp, *temp2;
	PATH *temp_path;
	NODE *temp_node;
	int i;
	
	//print out each USER
	for(temp = r->head; temp != NULL; temp = temp->next)
	{	//print UID, password, number of friends
		fprintf(f, "%06d: %16s, %d", temp->uid, temp->password, temp->number);
		
		//if user has no friends
		if(temp->number == 0)
			fprintf(f, ".\n");
		
		else
			fprintf(f, ", ");
		
		//print out each friend
		for(temp2 = temp->friend; temp2 != NULL; temp2=temp2->friend)
		{	//if printing last friend
			if(temp2->friend == NULL)
				fprintf(f, "%06d.\n", temp2->uid);
			
			else
				fprintf(f, "%06d, ", temp2->uid);
		}
	}
	
	//print out all paths asked for during execution
	fprintf(f, "Connections of pairs of users:\n");
	for(temp_path = p->head, i=1; temp_path != NULL; temp_path = temp_path->next, i++)
	{	fprintf(f, "%d. ", i);
		
		//if no path found, length of path is 0
		if(temp_path->length == 0)
			fprintf(f, "%06d and %06d cannot be connected.\n", 
					temp_path->head->uid, temp_path->tail->uid);
		
		else
		{	//print each NODE in path
			for(temp_node = temp_path->head; temp_node != NULL; temp_node = temp_node->next)
			{	//if printing last node
				if(temp_node->next == NULL)
					fprintf(f, "%06d\n", temp_node->uid);
				
				else
					fprintf(f, "%06d, ", temp_node->uid);
			}
		}	
	}
	
	return;
}


USER *search_user(ROOT *r, int uid)
{ USER *this;
  
  for (this = r->head; this != NULL; this = this->next)
  { if (this->uid == uid)
      break;
  }

  return this;
}

//print the friends of USER Tom
void print_Friends(USER *Tom)
{	USER *temp;
		
	temp = Tom->friend;
	
	//if user has no friends
	if(temp == NULL)
	{	printf("User %06d has no friends.\n", Tom->uid);
		return;
	}
	
	//otherwise, traverse friend list and print each UID in USER node
	printf("User %06d's friends: ", Tom->uid);
	for(; temp != NULL; temp=temp->friend)
	{	//if printing last friend
		if(temp->friend == NULL)
			printf("%06d.\n", temp->uid);
		
		else
			printf("%06d, ", temp->uid);
	}
	
	return;
}


void add_a_friend(ROOT *r, int id1, int id2)
{	add_friendship(id1, id2, r); //call add friendship function

	return;
}

//if users not friends, return 0
//if users are no longer friends, return 1
int remove_friendship(ROOT *r, int id1, int id2)
{	USER *u1, *u2, *temp;
	
	u1 = search_user(r, id1);
	u2 = search_user(r, id2);
	
	//traverse u1's friend list to find u2
	//check if current node NULL in case last node in friend list was deleted
	for(temp = u1; temp != NULL && temp->friend != NULL; temp=temp->friend)
	{	//if next USER is u2, delete USER node
		if(temp->friend->uid == id2)
		{	temp->friend = temp->friend->friend;
			u1->number--; //decrease friend number by 1
		}
	}
	
	//traverse u2's friend list to find u1
	for(temp = u2; temp != NULL && temp->friend != NULL; temp=temp->friend)
	{	//if next USER is u1, delete USER node
		if(temp->friend->uid == id1)
		{	temp->friend = temp->friend->friend;
			u2->number--; //decrease friend number by 1
			return 1;
		}
	}
	
	//if users not friends
	return 0;
}


void remove_user(ROOT *r, int id1)
{	USER *u1, *temp;
	
	u1 = search_user(r, id1); //find pointer to USER node with id1
	
	//remove each of u1's friendships
	for(temp = u1->friend; temp != NULL; temp=temp->friend)
		remove_friendship(r, id1, temp->uid);
	
	//traverse linked list to USER node before u1
	//if temp->next is NULL, would mean user does not exist (not possible), but check for safety anyways
	for(temp=r->head; temp != NULL && temp->next != NULL; temp=temp->next)
	{	//if next USER is u1, delete next USER node
		if(temp->next->uid == id1)
		{	temp->next = temp->next->next;
			break; //break from traversion
		}
	}
	
	printf("User %06d has been removed from MyTSN.\n", id1);
}


PATH *find_path(ROOT *r, int id1, int id2, PATH_ROOT *p)
{	PATH *this_path;
	USER *Tom, *Jerry, *temp, *u, *v, *curr_last_node;
	ROOT *CopyUser; //root to linked list, which is copy of AllUser
	NODE *start, *end;
	int path_length = 0; //length of path between id1 and id2
	int jerry_found = 0, node_found = 0;
	
	//make copy of entire list, so that friend number of each USER is saved
	CopyUser = make_root(); //make root to copy list
	for(temp = r->head; temp != NULL; temp = temp->next)
		add_copied_user(temp, CopyUser); //copy user and add to CopyUser at tail
	
	Tom = search_user(CopyUser, id1); //find pointers to USERs with id1 and id2
	Jerry = search_user(CopyUser, id2);
	
	//set distance (number) from Tom to each other node as -1
	for(temp = CopyUser->head; temp != NULL; temp = temp->next)
	{	//set distance from Tom to Tom as 0
		if(temp == Tom)
			temp->number = 0;
		
		else
			temp->number = -1;
	}
	
	do
	{	node_found = 0; //reset indicator checking whether any node with unknown distance found
		
		//find any USER with distance same as current path_length
		for(u = CopyUser->head; u != NULL; u = u->next)
		{	if(u->number == path_length)
			{	//check each friend of node
				for(v = u->friend; v != NULL; v = v->friend)
				{	temp = search_user(CopyUser, v->uid); //find USER v (friend of USER u)
					
					//if distance from Tom to temp (USER v) is -1
					//node with unknown distance found
					if(temp->number == -1)
					{	//set dist from Tom to v as 1 more than path_length
						temp->number = path_length + 1;
						
						//if friend (temp) is Jerry, PATH is complete
						if(temp == Jerry)
							jerry_found = 1; //Jerry has been found
						
						node_found = 1;		
					}
				}
			}			
		}
		
		//if node with unknown distance found
		if(node_found)
			path_length++; //increase length of PATH by 1
	}
	while(!jerry_found && node_found);
	//loop through list while Jerry has NOT been found AND a USER node with unknown distance HAS been found
	
	//make new nodes with id1 and id2
	start = make_node(id1);
	end = make_node(id2);
	
	//add new PATH to tail of list pointed to by PATH_ROOT p, starting at Tom and ending at Jerry
	//this adds NODE start to head of PATH
	this_path = add_and_make_path(start, end, p);
	
	//the last node of the PATH begins as the last node queried (Jerry)
	curr_last_node = Jerry; 
	
	//if Jerry was added to PATH, add all nodes in PATH to this_path
	if(jerry_found)
	{	//while head of PATH hasn't been reached
		while(curr_last_node != Tom)
		{	//find current last node (the next node that will be added to the PATH)
			for(temp = CopyUser->head; temp != NULL; temp = temp->next)
			{	if(temp == curr_last_node)
				{	//find friend of curr_last_node with distance from Tom 1 less than
					//distance from Tom to curr_last_node
					for(u=temp->friend; u != NULL; u=u->friend)
					{	//need USER struct of friend in AllUser linked list
						v = search_user(CopyUser, u->uid);
						
						if(v->number == curr_last_node->number - 1)
						{	//add curr_last_node to PATH
							add_and_make_node(curr_last_node->uid, this_path);
							
							curr_last_node = v; //friend becomes curr_last_node
							
							break; //make sure only 1 friend added at a time
						}
					}
				}
			}
		}
		printf("Path found!\n");
	}
	
	//otherwise USERs cannot be connected
	else
	{	this_path->length = 0; //set length of PATH to 0 if no path exists
		printf("%06d and %06d cannot be connected.\n", id1, id2);
	}
	
	
	return this_path;
}