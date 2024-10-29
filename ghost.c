// ghost.c
#include "defs.h"

/*
  Function: ghostThread(void* arg)
  Purpose: Simulates the behavior of a ghost in a haunted environment within a ghost-hunting game.

  Parameters:
    in/out arg: A void pointer to a Ghost structure, representing the ghost participating in the game.

  Description:
    This function initializes a ghost and enters into an infinite loop, emulating the actions of a ghost in a haunted environment. The ghost's actions include checking for the presence of hunters in the room, leaving evidence, moving to random adjacent rooms, and managing boredom levels. The function monitors the ghost's boredom level, triggering an exit if it surpasses a predefined threshold.

  Note:
    This function is intended to be executed in a separate thread using pthread.

  
    pthread_t thread;
    Ghost myGhost;
    // Initialize the ghost...
    pthread_create(&thread, NULL, ghostThread, (void*)&myGhost);
    // Wait for the thread to finish...
    pthread_join(thread, NULL);

    return
        none
*/



void* ghostThread(void* arg) {
    Ghost* ghost = (Ghost*)arg;

    // Initialization log
    l_ghostInit(ghost->type, ghost->currentRoom->name);

    while (1) {
        // Check if the Ghost is in the room with a hunter
        int inRoomWithHunter = 0;
        for (int i = 0; i < NUM_HUNTERS; i++) {
            if (hunters[i].currentRoom == ghost->currentRoom) {
                inRoomWithHunter = 1;
                break;
            }
        }

        if (inRoomWithHunter) {
            // Reset boredom timer since Ghost is in the room with a hunter
            ghost->boredom = 0;

            // Randomly choose to leave evidence or do nothing
            int action = randInt(0, 1);
            switch (action) {
                case 0:
                    // Do nothing
                    break;
                case 1:
                    // Leave evidence
                    enum EvidenceType evidenceType = randomGhostEvidence();
                    l_ghostEvidence(evidenceType, ghost->currentRoom->name);
                    addEvidenceToRoom(ghost->currentRoom, evidenceType);
                    break;
            }
        } else {
            // Ghost is not in the room with a hunter
            // Increase the Ghost’s boredom counter by 1
            ghost->boredom++;

            // Randomly choose to move to an adjacent room, leave evidence, or do nothing
            int action = randInt(0, 2);
            switch (action) {
                case 0:
                    // Do nothing
                    break;
                case 1:
                    // Leave evidence
                    enum EvidenceType evidenceType = randomGhostEvidence();
                    l_ghostEvidence(evidenceType, ghost->currentRoom->name);
                    addEvidenceToRoom(ghost->currentRoom, evidenceType);
                    break;
                case 2:
                    // Move to an adjacent room
                    Room* nextRoom = getRandomConnectedRoom(ghost->currentRoom);
                    if (nextRoom != NULL) {
                        l_ghostMove(nextRoom->name);

                        // Update the room’s Ghost pointer
                        ghost->currentRoom->ghost = NULL;
                        nextRoom->ghost = ghost;

                        // Update the Ghost’s Room pointer
                        ghost->currentRoom = nextRoom;
                    }
                    break;
            }
        }

        // Check if the ghost’s boredom counter has reached BOREDOM_MAX
        if (ghost->boredom >= BOREDOM_MAX) {
            l_ghostExit(LOG_UNKNOWN);
            pthread_exit(NULL);
        }

        // Introduce some delay before the next iteration
        usleep(GHOST_WAIT * 1000);
    }
}
