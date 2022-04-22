/*
Lucas Kaplan
ENEE150
0101
2/24/20

Project 1

This project will implement and compare the efficiencies of 4 sorting algorithims: bubble sort, insertion sort, merge sort, and quick sort. The efficiency of a given algorithim will be measured by the average number of comparisons of array elements and the average number of copies to and from the array(s) in use.
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

float comp_cnt = 0, copy_cnt = 0; //counts number of comparisons and number of copies in sorting algorithim  

// ******************************************************
// *************** FUNCTION PROTOTYPE *******************
// ******************************************************
void bubble_sort(int []);
void insertion_sort(int []);
void merge(int [], int, int, int);
void merge_sort(int [], int, int);
int partition(int [], int, int, int); 
void quick_sort(int [], int, int, int);
void swap(int*, int*);

// ******************************************************
// *************** MAIN FUNCTION ************************
// ******************************************************
int main(int argc, char *argv[])
{	int list[25][25]; //list of integers to be used
	int i=0, j=0; //counters
	srand(time(NULL)); //generates actual random numbers each run
	
	//command line safety check
	if(argc != 4)
	{	printf("Incorrect number of command line arguments.\n");
		printf("Usage: a.out inputFile.txt algorithim_number outputFile.txt\n"); 
		exit(0);
	}
	
	//file safety check
	FILE *inputFile = fopen(argv[1], "r"); //opening inputFile
	FILE *outputFile = fopen(argv[3], "wx"); //opening outputFile	
	
	if(inputFile == NULL)
	{	printf("%s could not be opened properly.\n", argv[1]);
		exit(0);
	}
	
	if(outputFile == NULL)
	{	printf("%s could not be opened properly.\n", argv[3]);
		exit(0);
	}
	
	//scanning in list of numbers
	for(i=0;i<25;i++)
	{	for(j=0;j<25;j++)
		{	fscanf(inputFile, "%d ", &list[i][j]);
			copy_cnt++;
		}
	}
	
	int alg = atoi(argv[2]); //scans in which algorithim to use
	
	switch(alg)
	{	//bubble sort
		case 1:
			for(i=0;i<25;i++)
			{	bubble_sort(list[i]);
				for(j=0;j<24;j++)
					fprintf(outputFile, "%d ", list[i][j]);
				
				fprintf(outputFile, "%d", list[i][j]);
				fprintf(outputFile, "\n");
			}
			
			fprintf(outputFile, "The average number of comparisons is %0.2f.\n", comp_cnt/25);
			fprintf(outputFile, "The average number of copies is %0.2f.\n", copy_cnt/25);
			break;
		
		//insertion sort
		case 2:
			for(i=0;i<25;i++)
			{	insertion_sort(list[i]);
				for(j=0;j<24;j++)
					fprintf(outputFile, "%d ", list[i][j]);
				
				fprintf(outputFile, "%d", list[i][j]);
				fprintf(outputFile, "\n");
			}
			
			fprintf(outputFile, "The average number of comparisons is %0.2f.\n", comp_cnt/25);
			fprintf(outputFile, "The average number of copies is %0.2f.\n", copy_cnt/25);
			break;
		
		//merge sort
		case 3:
			for(i=0;i<25;i++)
			{	merge_sort(list[i], 0, 24);
				for(j=0;j<24;j++)
					fprintf(outputFile, "%d ", list[i][j]);
				
				fprintf(outputFile, "%d", list[i][j]);
				fprintf(outputFile, "\n");
			}
			
			fprintf(outputFile, "The average number of comparisons is %0.2f.\n", comp_cnt/25);
			fprintf(outputFile, "The average number of copies is %0.2f.\n", copy_cnt/25);
			break;
		
		//quick sort with last array element as pivot value
		case 4:
			for(i=0;i<25;i++)
			{	quick_sort(list[i], 0, 24, 4);
				for(j=0;j<24;j++)
					fprintf(outputFile, "%d ", list[i][j]);
				
				fprintf(outputFile, "%d", list[i][j]);
				fprintf(outputFile, "\n");
			}
			
			fprintf(outputFile, "The average number of comparisons is %0.2f.\n", comp_cnt/25);
			fprintf(outputFile, "The average number of copies is %0.2f.\n", copy_cnt/25);
			break;
			
		//quick sort with first array element as pivot value
		case 5:
			for(i=0;i<25;i++)
			{	quick_sort(list[i], 0, 24, 5);
				for(j=0;j<24;j++)
					fprintf(outputFile, "%d ", list[i][j]);
				
				fprintf(outputFile, "%d", list[i][j]);
				fprintf(outputFile, "\n");
			}
			
			fprintf(outputFile, "The average number of comparisons is %0.2f.\n", comp_cnt/25);
			fprintf(outputFile, "The average number of copies is %0.2f.\n", copy_cnt/25);
			break;
		
		//quick sort with random array element as pivot value
		case 6:
			for(i=0;i<25;i++)
			{	quick_sort(list[i], 0, 24, 6);
				for(j=0;j<24;j++)
					fprintf(outputFile, "%d ", list[i][j]);
				
				fprintf(outputFile, "%d", list[i][j]);
				fprintf(outputFile, "\n");
			}
			
			fprintf(outputFile, "The average number of comparisons is %0.2f.\n", comp_cnt/25);
			fprintf(outputFile, "The average number of copies is %0.2f.\n", copy_cnt/25);
			break;
		
		default: 
			printf("Invalid value for requested sorting algorithim.\n");
			exit(0);
	}
	
	//closing files
	fclose(inputFile);
	fclose(outputFile);
	
	return 0;
}

// ***********************************************************
// *************** FUNCTION IMPLEMENTATION *******************
// ***********************************************************
//bubble sort
void bubble_sort(int list[])
{	int i=0, j=0, tmp=0; 
	for(i=0;i<25;i++)		
	{	for(j=24;j>=i+1;j--)
		{	comp_cnt++; // this cannot be put inside the if() block
			if(list[j] < list[j-1]) 
			{	tmp = list[j];    
				list[j] = list[j-1];   
				list[j-1] = tmp; 
				copy_cnt += 3; //ideally, copy_cnt++; should be used for each of the above three assignment statements
			}				
		}
	}
}	

//insertion sort
void insertion_sort(int A[])
{	int i=0, j=0, key=0;
	
	for(i=1;i<25;i++)
	{	key = A[i]; copy_cnt++; //key value being inserted into sorted section
		j = i;
		
		//move sorted subarray so that all numbers to right of key value will be greater
		while(j>0 && key<A[j-1])
		{	comp_cnt++; //comparison between key and A[j-1] will always occur if loop entered
			A[j] = A[j-1]; copy_cnt++;
			j--;
		}
		
		//if loop ended due to comparison, comp_cnt must increase by 1
		if(j>0)
			comp_cnt++;
		
		A[j] = key; copy_cnt++; //place key value in correct position within sorted array
	}
}

//MERGE SORT
//merge function
void merge(int A[], int start, int middle, int end) 
{	int i, j, k; 
	int left_length = middle - start + 1; 
	int right_length =  end - middle; 
    int L[left_length], R[right_length]; //sub-array for left and right side
  
    //copy original array to temp arrays L[] and R[] 
	for(i = 0; i < left_length; i++)
	{	L[i] = A[start+i]; copy_cnt++;
	}
	for (j = 0; j < right_length; j++) 
	{	R[j] = A[middle+1+j]; copy_cnt++;
	}
	
    i = 0; //initial index of left subarray 
    j = 0; //initial index of right subarray 
    k = start; //initial index of merged subarray 
    
	//merge temp arrays back into original array, from A[start] to A[end]
	//merge while counters still at valid array index
	while (i < left_length && j < right_length) 
    {	//if left array smaller, copy it into merged array
		if (L[i] <= R[j]) 
		{	A[k] = L[i];
            i++; 
        } 
        //otherwise copy right array into merged array
		else
        {	A[k] = R[j];
            j++; 
        } 
        
		k++; //always increase counter for merged array
		comp_cnt++; //if loop entered, comparison occurred
		copy_cnt++; //if loop entered, copy occurred
    } 
  
    //copy any remaining elements of L[]
    while (i < left_length) 
    {	A[k] = L[i]; copy_cnt++;
        i++; 
        k++; 
    } 
  
    //copy any remaining elements of R[]
    while (j < right_length) 
    {	A[k] = R[j]; copy_cnt++;
        j++; 
        k++; 
    } 
}

//recursive merge sort function
void merge_sort(int A[], int start, int end)
{	//base case is when array is only one element (i.e. start < end)
	if(start < end)
	{	int middle = (start+end)/2;
		
		//recursively call function 
        merge_sort(A, start, middle); 
        merge_sort(A, middle+1, end); 
		
		//merge subarrays
		merge(A, start, middle, end);
	}
}

//QUICK SORT
//swap function
void swap(int* a, int* b) 
{	int temp = *a; 
	*a = *b; 
	*b = temp; 
}

//partition function
int partition(int list[], int low, int high, int pValue) 
{	int pivot;
	int pIndex = 0; //index of pivot value
	int pivot_cnt = low - 1; //counter to track elements smaller than pivot
	
	//pivot value depends on user choice, dictated by pvalue
	switch(pValue)
	{	case 4: //pivot is last array element
			pivot = list[high]; 
			break;
		
		case 5: //pivot is first array element
			swap(&list[low], &list[high]); //swap first and last array element
			copy_cnt += 3; //swap increases copy counter by 3
			pivot = list[high];
			break;
		
		case 6: //pivot is random array element
			pIndex = rand()%(high-low+1) + low; //random index between low and high
												//need to add 1 to generate random numbers
												//between low and high, since subtraction not inclusive
			swap(&list[pIndex], &list[high]); //swap random array element with last array element
			copy_cnt += 3;
			pivot = list[high];
			break;
			
		default:
			printf("Invalid value for requested sorting algorithim.\n");
			exit(0);
	}
		
	int j; //counter going through all of subarray
	
	//don't have to check last element since it's always pivot value
	for(j = low; j < high; j++) 
    {	//if current element is smaller than the pivot 
		if(list[j] < pivot) 
        {	pivot_cnt++; //increment index of smaller elements
			swap(&list[pivot_cnt], &list[j]); //swap current element with element at pivot counter
			copy_cnt += 3;
        }
		comp_cnt++; //comparison occurs anytime loop entered	
    } 
    
	swap(&list[pivot_cnt + 1], &list[high]); //place pivot in correct place by swapping it with
											 //element in index 1 after pivot_cnt
											 //index of pivot always = high due to earlier swaps 
	copy_cnt += 3; 
    
	return (pivot_cnt + 1); //return index of pivot
}

//recursive quicksort function
void quick_sort(int list[], int low, int high, int pValue) 
{	//recursively call function until array has size 1 (low = high) or is invalid (low < high)
	if(low < high) 
    {	//find index of partition
		int pIndex = partition(list, low, high, pValue); 
  
        //separately sort elements before and after partition 
        quick_sort(list, low, pIndex - 1, pValue); 
        quick_sort(list, pIndex + 1, high, pValue); 
    } 
}