#include "defs.h"

int main() {
    srand(time(NULL));

    HouseType house;
    initHouse(&house);
    populateRooms(&house);

    // Create and initialize hunters
    Hunter hunters[NUM_HUNTERS];
    char hunterNames[NUM_HUNTERS][MAX_STR];

    for (int i = 0; i < NUM_HUNTERS; i++) {
        printf("Enter name for Hunter %d: ", i + 1);
        fgets(hunterNames[i], MAX_STR, stdin);
        strtok(hunterNames[i], "\n"); // Remove newline character
        hunters[i].fear = 0;
        hunters[i].boredom = 0;
        strcpy(hunters[i].name, hunterNames[i]);
        hunters[i].equipment = randomGhost(); // Assign random equipment for demonstration purposes
        hunters[i].currentRoom = &house.rooms[0]; // Start in the Van room
        pthread_create(&hunters[i].thread, NULL, hunterThread, (void*)&hunters[i]);
    }

    // Create and initialize the ghost
    Ghost ghost;
    ghost.type = randomGhost();
    ghost.currentRoom = &house.rooms[randInt(1, house.numRooms - 1)]; // Random room (not the Van)
    pthread_create(&ghost.thread, NULL, ghostThread, (void*)&ghost);

    // Add code to wait for threads to finish (pthread_join)

    return 0;
}

