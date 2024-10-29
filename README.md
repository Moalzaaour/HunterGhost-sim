Name: Mohammad Alzaaour 
student number: 101267268

#Final project- Threads, and Putting it All Together

This assignment is simliar to a game called Phasmaphobia where A house is used to hold all of the information about the current hunt. It contains the, a collection of hunters, a linked list of all rooms, and the shared evidence list that all hunters will reference. The first room in
every house is always the ghost hunter’s vehicle, the Van.


#Purpose: 

the goal of this assignment is to have a functioning, multi-threaded simulation, which follows the rules of the assignmnet, and which executes without crashing, without valgrind warnings, and without memory leaks.


#list of files

README.md : this file
ChatGPT.txt : the chat between the AI and I
main.c: only has the main function 
house.c: contains house related stuff, such as intilizaing the house, adding rooms and finding evidence
ghost.c:contains the functions necessary functions to create a ghost
hunter.c:contains the functions necessary functions to create a hunter
utils.c: the provided code 
logger.c: contains the logging info for hunters and ghosts (also didn't change this at all)


#Instructions for compiling the program

to compile this program simply type make 

#Instructions for running the program

to run this program simply use the command ./ghost_hunter_game
or if you are checking for memoery leak, then use valgrind --leak-check=full ./ghost_hunter_game

#Instructions for how to use the program once it is running,
you dont have to do anything, the game runs by it selfs. 

#Generative AI 
the conversation between chatGPT and I is in the chatGPT file
