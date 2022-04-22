/*
Lucas Kaplan
ENEE150
0101
3/24/20

Project 2

This project will create a simplified social network. It will use an input file for the current network data and will allow the program user to manage the network by: searching for a user, listing a user's friends, adding a friend to a user, adding a new user, removing a friendship, and removing a user. After the program ends, it will print the updated user information to an output file.
*/

#include <stdio.h>
#include <stdlib.h>

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//                GLOBAL VARIABLES
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
int userCnt=0; //number of rows with a user (one less than total number of users)

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//                FUNCTION PROTOTYPE 
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
int menu();
int search(int, int**); //search for UID
void list(int, int, int**, int*); //list users friends
void newFriends(int, int, int***, int**); //create new friendship
void newUser(int***, int**); //create new user
int removeFriend(int, int, int***, int **); //remove friendship
void removeUser(int, int***, int**); //remove user
void writeFile(FILE**, int***, int**); //exit program and write to outputFile

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//                MAIN FUNCTION 
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
int main(int argc, char *argv[])
{	int **data; //2D array - 1st column of each row stores user ID; each column after stores ID of friendship
	int *column; //array to store number of columns in each row (number of friends + 1)
	int id, id2, row_id; //used to scan in user ID and find their row in the data array
	char extra; //used to distinguish between frienship and user declaration
	int i, j, notExit=1;
	
	if(argc != 3)
	{	printf("Incorrect number of command line arguments.\n");
		printf("Usage: a.out user_database.txt update.txt\n");
		exit(0);
	}
	
	//file safety check
	FILE *inputFile = fopen(argv[1], "r"); //opening input user database
	FILE *outputFile = fopen(argv[2], "wx"); //opening final user database	
	
	if(inputFile == NULL)
	{	printf("%s could not be opened properly.\n", argv[1]);
		exit(0);
	}
	
	if(outputFile == NULL)
	{	printf("%s could not be opened properly.\n", argv[2]);
		exit(0);
	}
	
	//scanning in inputFile
	while(fscanf(inputFile, "%d%c", &id, &extra) != EOF)
	{	//if char after UID is #, allocate 1 new row and 1 new column with UID
		if(extra == '#')
		{	//if first UID, need to allocate memory
			if(userCnt == 0)
			{	//allocating memory for data array
				data = (int **) malloc(sizeof(int *)); //creating 0th row (an int pointer)
				*data = (int *) malloc(sizeof(int)); //creating 0th column (an int)
				
				//allocating memory for column[0] (a.k.a. int *column)
				column = calloc(1, sizeof(int)); //creates 1 column with value 0

				data[userCnt][0] = id; //value of 0th column in current row (userCnt) is UID
			}
			
			//otherwise, need to RE-allocate memory
			else
			{	//add 1 user row
				data = (int **) realloc(data, (userCnt+1)*sizeof(int *)); 
				//allocate memory for 1 column in data[userCnt]
				*(data+userCnt) = (int *) malloc(sizeof(int));
				
				//increase size of column counter by 1 for new UID
				column = (int *) realloc(column, (userCnt+1)*sizeof(int));
				column[userCnt] = 0; //new user only has 0th column (UID) in data array
				
				data[userCnt][0] = id; //value of 0th column in current row (userCnt) is UID
			}
			
			userCnt++; //increase number of users
		}
		
		//if char after UID is space, create new friendship between ID1 and ID2
		if(extra == ' ')
		{	fscanf(inputFile, "%d", &id2);
			
			newFriends(id, id2, &data, &column); //call new friendship function
		}
		
		//scan in newline char
		fscanf(inputFile, "%c", &extra);
	}
	
	int attempts = 0; //number of attempts for certain options
	
	printf("\nWelcome to MyTSN!\n");
	while(notExit)
	{	switch(menu()) //menu function returns user choice as int
		{	//search for user
			case 1:
				printf("\nEnter the user ID: ");
				scanf("%d", &id);
				
				//if search() finds user, it returns UID's row number, which is >= 0
				if(search(id, data) >= 0)
					printf("User %06d found.\n", id);
				
				else
					printf("User %06d not found.\n", id);
				
				break;
			
			//list user's friends
			case 2:
				//user has 3 attempts to enter valid UID
				while(attempts < 3)
				{	printf("\nEnter the user ID: ");
					scanf("%d", &id);
					
					row_id = search(id, data); //find row with UID in data array
					
					//if UID exists
					if(row_id >= 0)
					{	list(id, row_id, data, column);
						break;
					}
					
					
					//if UID doesn't exist
					else
					{	printf("User %06d does not exist.\n", id);
						attempts++; //increase attempt count
					}
				}
				
				attempts = 0; //reset attempt count
				
				break;
			
			//add new frienship
			case 3:
				printf("\nEnter the user IDs of the two friends: ");
				scanf("%d %d", &id, &id2);
				
				newFriends(id, id2, &data, &column); //call new frienship fxn
				
				break;
			
			//add new user
			case 4: 
				newUser(&data, &column);
				
				break;
			
			//remove friendship
			case 5:
				printf("\nEnter the user IDs of the two friends: ");
				scanf("%d %d", &id, &id2);
				
				//if removeFriend function returns true, UIDs are no longer friends
				if(removeFriend(id, id2, &data, &column))
					printf("Users %06d and %06d are no longer friends.\n", id, id2);
				
				//otherwise, UIDs were already not friends (friendship can't be removed)
				else
					printf("Users %06d and %06d are not friends.\n", id, id2);
				
				break;
				
			//remove user
			case 6:
				printf("\nEnter the user ID: ");
				scanf("%d", &id);
			
				removeUser(id, &data, &column); //call remove user function
				
				break;
				
			//exit
			case 0:
				writeFile(&outputFile, &data, &column);
				notExit = 0; //exit while loop
				
				break;
		}
	}
	
	//closing files
	fclose(inputFile);
	fclose(outputFile);
	
	return 0;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//              FUNCTION IMPLEMENTATION 
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//main menu
int menu()
{	int choice;
	char enter;
	
	printf("\n1: search for a user\n2: list a user’s friends\n");
	printf("3: add a new friend to a user\n4: add a new user\n");
	printf("5: remove a friendship\n6: remove a user\n");
	printf("0: EXIT\n");
	printf("\nEnter your choice (0-6):\n");
	
	scanf("%d%c", &choice, &enter);
	return choice;
}

//search for UID; need UID and data array
int search(int id, int **data)
{	int i=0;
	
	//check if data[i][0] is UID, since every UID declared in first column of data array
	for(i=0;i<userCnt;i++)
	{	//if UID found, return current row (i)
		if(data[i][0] == id)
			return i;
	}
	
	//if no UID found, return negative int
	return -1;
}

//list users friends; need UID, row with UID in data array, data array, and array with number of friends per user
void list(int id, int row_id, int **data, int *column)
{	int i;
	
	//if user has no friends
	if(column[row_id] == 0)
		printf("User %06d has no friends.\n", id);
		
	//else, user has at least 1 friend
	else
	{	printf("User %06d's friends: ", id);
		for(i=1; i<=column[row_id]; i++)
		{	printf("%06d", data[row_id][i]);
			
			if(i<column[row_id])
				printf(", ");
			
			//if printing last friend
			else
				printf(".\n");
		}
	}
}

//create new friendship; need 2 UIDS, pointer to data array, and pointer to number of columns in each row of data array
void newFriends(int id, int id2, int ***data, int **column)
{	int i, col_id=0, col_id2=0; //stores number of columns in data array in rows with ID1 and ID2
	
	//find separate rows in data array with UID1 and UID2
	int row_id = search(id, *data);
	int row_id2 = search(id2, *data);
	
	//check if ID1 and ID2 already friends
	for(i=0;i<=(*column)[row_id];i++)
	{	//if ID2 in any of columns of row with ID1, ID1 and ID2 are already friends
		if(id2 == (*data)[row_id][i])
		{	printf("Users %06d and %06d are already friends.\n", id, id2);
			return;
		}
	}
	
	//otherwise, ID1 and ID2 NOT friends yet
	//~~~ 1st ID ~~~
	col_id = ++(*column)[row_id]; //increase column counter of ID1 by 1
	//increase number of columns in row with ID1 by 1
	(*data)[row_id] = (int *) realloc((*data)[row_id], (col_id+1)*sizeof(int));
	
	(*data)[row_id][col_id] = id2; //put ID2 into new column of row with ID1
	
	//~~~ 2nd ID ~~~
	col_id2 = ++(*column)[row_id2]; //increase column counter of ID2 by 1
	//increase number of columns in row with ID2 by 1
	(*data)[row_id2] = (int *) realloc((*data)[row_id2], (col_id2+1)*sizeof(int));
	
	(*data)[row_id2][col_id2] = id; //put ID1 into new column of row with ID2
}

//add new user; need pointer to data array, pointer to number of columns in each row of data array
//use pointer to data and column array so that original array can be changed
void newUser(int ***data, int **column)
{	int id=0, i=0;
	int flag=0; //flag indicates whether UID valid or not
	char idString[99];
	int attempts=0;
	
	//check if UID valid
	while(attempts < 3)
	{	printf("\nEnter the user ID: ");
		scanf("%s", idString);
			
		//reset counter and flag
		i=0; flag=0;
		
		//check id until newline char or end of string
		while(idString[i] != '\n' && idString[i] != '\0')
		{	//if a non-digit or non-negative sign entered
			if(!(idString[i] >= '0' && idString[i] <= '9') && idString[i] != '-')
			{	printf("%s can only have up to 6 digits (0-9).\n", idString);
				flag = 1; //turn flag on to indicate problem
				attempts++;		
				break;
			}
			
			//if negative sign entered in middle of number
			else if(idString[i] == '-' && i != 0)
			{	printf("%s can only have up to 6 digits (0-9).\n", idString);
				flag = 1;
				attempts++;
				break;
			}
			
			//if there more than six digits entered
			else if(i>5)
			{	printf("%s can only have up to 6 digits (0-9).\n", idString);
				flag = 1;
				attempts++;
				break;
			}
			
			i++; //otherwise, increase counter
		}
			
		//if UID valid (set of digits), i>0 and flag is off
		if(i>0 && !flag)
		{	id = atoi(idString); //convert string to int
	
			//check if UID already exists
			if(search(id, *data) >= 0)
			{	printf("%06d already exists.\n", id);
				attempts++;
			}
			
			//if UID doesn't exist, add to data array and exit function
			else
			{	//add row for new user
				*data = (int **) realloc(*data, (userCnt+1)*sizeof(int *)); 
				//allocate memory for 1 column in data[userCnt]
				(*data)[userCnt] = (int *) malloc(sizeof(int));
				
				//increase size of column counter by 1 for new UID
				*column = (int *) realloc(*column, (userCnt+1)*sizeof(int));
				(*column)[userCnt] = 0; //new user only has 0th column (UID) in data array
				
				(*data)[userCnt][0] = id; //value of 0th column in current row (userCnt) is UID
				
				userCnt++; //increase number of users
				
				return;
			}
		}
	}
}

//remove frienship; need both UIDs, pointers to data array and column array (stores number of columns in each row)
int removeFriend(int id, int id2, int ***data, int **column)
{	int i=0, j=0;
	
	//find separate rows in data array with UID1 and UID2
	int row_id = search(id, *data);
	int row_id2 = search(id2, *data);
	
	//check row with ID1 for ID2
	for(i=0;i<=(*column)[row_id];i++)
	{	//if ID2 in any of columns, remove friendship
		if(id2 == (*data)[row_id][i])
		{	//move all friends right of ID2 left one space
			for(j=i+1;j<=(*column)[row_id];j++)
			{	(*data)[row_id][j-1] = (*data)[row_id][j];
			}
			
			//decrease number of columns in data[row_id] by 1
			(*data)[row_id] = (int *) realloc((*data)[row_id], (*column)[row_id]*sizeof(int));
			//decrease column counter by 1
			(*column)[row_id]--;
		}
	}
	
	//check row with ID2 for ID1
	for(i=0;i<=(*column)[row_id2];i++)
	{	//if ID1 in any of columns, remove friendship
		if(id == (*data)[row_id2][i])
		{	//move all friends right of ID1 left one space
			for(j=i+1;j<=(*column)[row_id2];j++)
			{	(*data)[row_id2][j-1] = (*data)[row_id2][j];
			}
			
			//decrease number of columns in data[row_id2] by 1
			(*data)[row_id2] = (int *) realloc((*data)[row_id2], (*column)[row_id2]*sizeof(int));
			//decrease column counter by 1
			(*column)[row_id2]--;
			
			return 1; //return true
		}
	}
	
	//if ID1 and ID2 are NOT friends, return false
	return 0;
}

//remove user; need UID, pointers to data array and column array (stores number of columns in each row) 
void removeUser(int id, int ***data, int **column)
{	int i=0;
	
	//find row in data array with UID
	int row_id = search(id, *data);
	
	//remove each friendship in data[row_id], starting with last friend
	for(i=(*column)[row_id];i>=1;i--)
	{	removeFriend(id, (*data)[row_id][i], data, column);
	}
	
	//move all rows below row_id up 1 in data array
	//move all columns right of row_id left 1 in column array
	for(i=row_id+1;i<userCnt;i++)
	{	(*data)[i-1] = (*data)[i];
		(*column)[i-1] = (*column)[i];
	}
	
	userCnt--; //decrease userCnt by 1
	//reduce number of rows in data array by 1
	(*data) = (int **) realloc((*data), userCnt*sizeof(int *));
	//reduce number of columns in column array by 1
	(*column) = (int *) realloc((*column), userCnt*sizeof(int));
	
	printf("User %06d has been removed from MyTSN.\n", id);
}

//exit program and write to output file; need pointers to outputFile, data array, and column array
void writeFile(FILE **outputFile, int ***data, int **column)
{	int row=0; //tracks row in data array
	int i=0, j=0, temp=0, row_id=0;
	int idArray[userCnt]; //used to sort UIDs
	//place all UIDs (0th column of all rows) into idArray
	for(i=0;i<userCnt;i++)
		idArray[i] = (*data)[i][0];
	
	//loop through entire data array
	for(row=0;row<userCnt;row++)		
	{	//sort one element of idArray
		//j starts as last element in array and goes down by 1
		for(j=userCnt-1;j>row;j--)
		{	//if element to left is smaller, swap elements
			if(idArray[j] < idArray[j-1])
			{	temp = idArray[j];    
				idArray[j] = idArray[j-1];   
				idArray[j-1] = temp;
			}
		}
		
		//find row with sorted UID
		row_id = search(idArray[row], *data);
		
		//if a user has no friends
		if((*column)[row_id] == 0)
			fprintf(*outputFile, "%06d#\n", (*data)[row_id][0]);
		
		//if a user has friends, sort them in ascending order
		else
		{	fprintf(*outputFile, "%06d: ", (*data)[row_id][0]); //print first UID
			
			//loop through each column with friend
			for(i=1;i<=(*column)[row_id];i++)		
			{	for(j=(*column)[row_id];j>i;j--)
				{	//if friend to left smaller in value numerically, swap places
					if((*data)[row_id][j] < (*data)[row_id][j-1]) 
					{	temp = (*data)[row_id][j];    
						(*data)[row_id][j] = (*data)[row_id][j-1];   
						(*data)[row_id][j-1] = temp;				
					}
				}
				
				//if its the last friend
				if(i == (*column)[row_id])
					fprintf(*outputFile, "%06d.\n", (*data)[row_id][i]);
				
				else
					fprintf(*outputFile, "%06d, ", (*data)[row_id][i]);
			}
		}
	}
	
	printf("\nThank you for using MyTSN!\n\n");
}