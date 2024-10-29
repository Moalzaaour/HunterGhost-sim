// hunter.c
#include "defs.h"

/*
  Function: hunterThread(void* arg)
  Purpose: Simulates the behavior of a hunter in a ghost-hunting game.

  Parameters:
    in/out arg: A void pointer to a Hunter structure, representing the hunter participating in the game.

  Description:
    This function initializes a hunter and enters into an infinite loop, emulating the actions of a ghost hunter in a haunted environment. The hunter's actions include checking for the presence of ghosts, collecting evidence, moving to random connected rooms, and reviewing evidence. The function also monitors the hunter's fear and boredom levels, triggering an exit if either surpasses predefined thresholds.

    pthread_t thread;
    Hunter myHunter;
    // Initialize the hunter...
    pthread_create(&thread, NULL, hunterThread, (void*)&myHunter);
    // Wait for the thread to finish...
    pthread_join(thread, NULL);
  
  Note:
    This function is intended to be executed in a separate thread using pthread.

  return
    none
*/


void* hunterThread(void* arg) {
    Hunter* hunter = (Hunter*)arg;

    // Initialization log
    l_hunterInit(hunter->name, hunter->equipment);

    while (1) {
        // Check if the hunter is in a room with a ghost
        int inRoomWithGhost = (hunter->currentRoom->ghost != NULL);

        if (inRoomWithGhost) {
            // Increase fear field of the hunter by 1 and reset boredom timer
            hunter->fear++;
            hunter->boredom = 0;
        } else {
            // Hunter is not in a room with a ghost
            // Increase boredom by 1
            hunter->boredom++;
        }

        // Randomly choose to collect evidence, move, or review evidence
        int action = randInt(0, 2);
        switch (action) {
            case 0:
                // Collect evidence
                if (inRoomWithGhost) {
                    enum EvidenceType evidenceType = hunter->currentRoom->evidenceType;
                    if (evidenceType != EV_UNKNOWN && evidenceType == hunter->equipment) {
                        l_hunterCollect(hunter->name, evidenceType, hunter->currentRoom->name);
                        collectEvidence(evidenceType);
                        hunter->currentRoom->evidenceType = EV_UNKNOWN;
                    }
                }
                break;
            case 1:
                // Move to a random, connected room
                Room* nextRoom = getRandomConnectedRoom(hunter->currentRoom);
                if (nextRoom != NULL) {
                    l_hunterMove(hunter->name, nextRoom->name);

                    // Update the room pointer in the hunter
                    hunter->currentRoom->hunters[hunter->id] = NULL;
                    nextRoom->hunters[hunter->id] = hunter;

                    // Update the hunter's current room
                    hunter->currentRoom = nextRoom;
                }
                break;
            case 2:
                // Review evidence
                if (reviewEvidence()) {
                    l_hunterReview(hunter->name, LOG_SUFFICIENT);
                    pthread_exit(NULL);
                }
                break;
        }

        // Check if the fear of the hunter is greater than or equal to FEAR_MAX
        if (hunter->fear >= FEAR_MAX) {
            l_hunterExit(hunter->name, LOG_UNKNOWN);
            pthread_exit(NULL);
        }

        // Check if the hunter's boredom is greater than or equal to BOREDOM_MAX
        if (hunter->boredom >= BOREDOM_MAX) {
            l_hunterExit(hunter->name, LOG_UNKNOWN);
            pthread_exit(NULL);
        }

        // Introduce some delay before the next iteration
        usleep(HUNTER_WAIT * 1000);
    }
}
