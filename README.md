# ENEE150
This repository contains the projects completed in the class ENEE150: Intermediate Programming Concepts for Engineers.

## Project 1
This project will implement and compare the efficiencies of 4 sorting algorithims: bubble sort, insertion sort, merge sort, and quick sort.
The efficiency of a given algorithim is measured by the average number of comparisons of array elements and the average number of copies to and from the array(s) in use.

## Project 2
This project generates a simplified social network. It uses an input file to read in the user IDs and friendships. These files are labeled "in_num1_num2", where num1
represents the number of users and num2 represents the number of connections. The program user will be able to manage the network by: searching for a user, 
listing a user's friends, adding a friend to a user, adding a new user, removing a friendship, and removing a user. After the program ends, it prints the updated user 
information to an output file in the following format:
<UID># //if the user has no friends
<UID>: <Friend 1 UID>, <Friend 2 UID>, ... 

## Project 3
This project is an extension of the Tiny Social Network developed in project 2. In this version, each user is given a random password, and the user is given an additional
management option: finding a connection between users. The input file follows the same format as project 2, but the output file now follows the following format:
  
<UID>: <password>, <no. of friends>, <Friend 1 UID>, <Friend 2 UID>, ...
  ...
<UID>: <password>, <no. of friends>, <Friend 1 UID>, <Friend 2 UID>, ...
Connections of pairs of users:
1. <UID 1>, <connecting UID 1>, <connecting UID 2>, ..., <UID 2>
2. <UID 1> and <UID 2> cannot be connected
  ...
