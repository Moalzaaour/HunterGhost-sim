#include "defs.h"

pthread_mutex_t evidenceMutex = PTHREAD_MUTEX_INITIALIZER;
enum EvidenceType collectedEvidence[MAX_COLLECTED_EVIDENCE];


/*
    Dynamically allocates a room and initializes its values.
    in: name - the name of the room.
    out: returns a pointer to the created room.
*/


/*
    Function: createRoom(const char* name)
    Purpose: Dynamically allocates a room and initializes its values.
    
    Parameters:
      in: name - the name of the room.
    
    Returns:
      out: A pointer to the created room.

    Example Usage:
      struct Room* myRoom = createRoom("Living Room");
*/


struct Room* createRoom(const char* name) {
    struct Room* newRoom = (struct Room*)malloc(sizeof(struct Room));
    if (newRoom == NULL) {
        perror("Error creating room");
        exit(EXIT_FAILURE);
    }

    strncpy(newRoom->name, name, MAX_STR - 1);
    newRoom->name[MAX_STR - 1] = '\0'; 
    newRoom->evidenceType = EV_UNKNOWN;
    newRoom->numConnectedRooms = 0;
    pthread_mutex_init(&newRoom->roomMutex, NULL);

    return newRoom;
}




/*
    Connects two rooms in a two-way connection.
    in: room1, room2 - the rooms to connect.
*/


/*
    Function: connectRooms(struct Room* room1, struct Room* room2)
    Purpose: Connects two rooms in a two-way connection.
    
    Parameters:
      in: room1, room2 - the rooms to connect.

    Example Usage:
      struct Room* roomA = createRoom("Room A");
      struct Room* roomB = createRoom("Room B");
      connectRooms(roomA, roomB);
*/


void connectRooms(struct Room* room1, struct Room* room2) {
    if (room1->numConnectedRooms < MAX_CONNECTED_ROOMS && room2->numConnectedRooms < MAX_CONNECTED_ROOMS) {
        room1->connectedRooms[room1->numConnectedRooms++] = room2;
        room2->connectedRooms[room2->numConnectedRooms++] = room1;
    } else {
        fprintf(stderr, "Error connecting rooms: Maximum connected rooms reached.\n");
        exit(EXIT_FAILURE);
    }
}

/*
    Adds a room to the list of rooms in the house.
    in: head - a pointer to the head of the room list.
        room - the room to add to the list.
*/


/*
    Function: addRoom(struct Room** head, struct Room* room)
    Purpose: Adds a room to the list of rooms in the house.
    
    Parameters:
      in/out: head - a pointer to the head of the room list.
      in: room - the room to add to the list.

    Example Usage:
      struct Room* newRoom = createRoom("Bedroom");
      addRoom(&house->rooms, newRoom);
*/


void addRoom(struct Room** head, struct Room* room) {
    if (*head == NULL) {
        *head = room;
        room->nextRoom = NULL;
    } else {
        room->nextRoom = *head;
        *head = room;
    }
}

/*
    Dynamically allocates several rooms and populates the provided house.
    Note: You may modify this as long as room names and connections are maintained.
        out: house - the house to populate with rooms. Assumes house has been initialized.
*/
void populateRooms(HouseType* house) {
    // First, create each room

    // createRoom assumes that we dynamically allocate a room, initializes the values, and returns a RoomType*
    // create functions are pretty typical, but it means errors are harder to return aside from NULL
    struct Room* van                = createRoom("Van");
    struct Room* hallway            = createRoom("Hallway");
    struct Room* master_bedroom     = createRoom("Master Bedroom");
    struct Room* boys_bedroom       = createRoom("Boy's Bedroom");
    struct Room* bathroom           = createRoom("Bathroom");
    struct Room* basement           = createRoom("Basement");
    struct Room* basement_hallway   = createRoom("Basement Hallway");
    struct Room* right_storage_room = createRoom("Right Storage Room");
    struct Room* left_storage_room  = createRoom("Left Storage Room");
    struct Room* kitchen            = createRoom("Kitchen");
    struct Room* living_room        = createRoom("Living Room");
    struct Room* garage             = createRoom("Garage");
    struct Room* utility_room       = createRoom("Utility Room");

    // This adds each room to each other's room lists
    // All rooms are two-way connections
    connectRooms(van, hallway);
    connectRooms(hallway, master_bedroom);
    connectRooms(hallway, boys_bedroom);
    connectRooms(hallway, bathroom);
    connectRooms(hallway, kitchen);
    connectRooms(hallway, basement);
    connectRooms(basement, basement_hallway);
    connectRooms(basement_hallway, right_storage_room);
    connectRooms(basement_hallway, left_storage_room);
    connectRooms(kitchen, living_room);
    connectRooms(kitchen, garage);
    connectRooms(garage, utility_room);

    // Add each room to the house's room list
    addRoom(&house->rooms, van);
    addRoom(&house->rooms, hallway);
    addRoom(&house->rooms, master_bedroom);
    addRoom(&house->rooms, boys_bedroom);
    addRoom(&house->rooms, bathroom);
    addRoom(&house->rooms, basement);
    addRoom(&house->rooms, basement_hallway);
    addRoom(&house->rooms, right_storage_room);
    addRoom(&house->rooms, left_storage_room);
    addRoom(&house->rooms, kitchen);
    addRoom(&house->rooms, living_room);
    addRoom(&house->rooms, garage);
    addRoom(&house->rooms, utility_room);
}


/*
    Function: initHouseAndRooms(HouseType* house)
    Purpose: Initializes a house and populates it with rooms.

    Parameters:
      in/out: house - a pointer to the HouseType structure to be initialized.

    Example Usage:
      HouseType myHouse;
      initHouseAndRooms(&myHouse);
*/


void initHouseAndRooms(HouseType *house) {
    house->numRooms = 0;
    populateRooms(house); 
}

/*
    Helper Function: addEvidenceToRoom(Room* room, enum EvidenceType evidenceType)
    Purpose: Adds evidence to a room.

    Parameters:
      in/out: room - a pointer to the room to which evidence is added.
      in: evidenceType - the type of evidence to add.

    Example Usage:
      Room* myRoom = createRoom("Library");
      addEvidenceToRoom(myRoom, EV_AUDIO);
*/


void addEvidenceToRoom(Room* room, enum EvidenceType evidenceType) {
    pthread_mutex_lock(&room->roomMutex);
    if (room->evidenceType == EV_UNKNOWN) {
        room->evidenceType = evidenceType;
        l_ghostEvidence(evidenceType, room->name);
    }
    pthread_mutex_unlock(&room->roomMutex);
}


/*
    Helper Function: getRandomConnectedRoom(Room* currentRoom)
    Purpose: Retrieves a random connected room.

    Parameters:
      in: currentRoom - a pointer to the current room.

    Returns:
      out: A pointer to a randomly selected connected room.

    Example Usage:
      Room* currentRoom = getRandomConnectedRoom(myRoom);
*/


Room* getRandomConnectedRoom(Room* currentRoom) {
    int numConnectedRooms = currentRoom->numConnectedRooms;
    if (numConnectedRooms == 0) {
        return currentRoom;
    }
    int randomIndex = randInt(0, numConnectedRooms - 1);
    return currentRoom->connectedRooms[randomIndex];
}


/*
    Helper Function: collectEvidence(enum EvidenceType evidenceType)
    Purpose: Collects evidence.

    Parameters:
      in: evidenceType - the type of evidence to collect.

    Example Usage:
      collectEvidence(EV_VIDEO);
*/


void collectEvidence(enum EvidenceType evidenceType) {
    int evidenceAlreadyCollected = 0;
    int numCollectedEvidence= 0;
    pthread_mutex_lock(&evidenceMutex);
    for (int i = 0; i < numCollectedEvidence; i++) {
        if (collectedEvidence[i] == evidenceType) {
            evidenceAlreadyCollected = 1;
            break;
        }
    }
    if (!evidenceAlreadyCollected && numCollectedEvidence < MAX_COLLECTED_EVIDENCE) {
        collectedEvidence[numCollectedEvidence++] = evidenceType;
        l_hunterCollect("Shared", evidenceType, "Shared");
    }
    pthread_mutex_unlock(&evidenceMutex);
}
/*
    Helper Function: reviewEvidence()
    Purpose: Reviews evidence and checks if there are at least three unique pieces.

    Returns:
      out: 1 if there are at least three unique pieces; otherwise, 0.

    Example Usage:
      int result = reviewEvidence();
*/

int reviewEvidence() {
    int uniqueEvidenceCount = 0;
    int numCollectedEvidence = 0;
    pthread_mutex_lock(&evidenceMutex);

    for (int i = 0; i < numCollectedEvidence; i++) {
        int isUnique = 1;
        for (int j = i + 1; j < numCollectedEvidence; j++) {
            if (collectedEvidence[i] == collectedEvidence[j]) {
                isUnique = 0;
                break;
            }
        }

        if (isUnique) {
            uniqueEvidenceCount++;
        }
    }

    pthread_mutex_unlock(&evidenceMutex);

    return (uniqueEvidenceCount >= 3) ? 1 : 0;
}


/*
    Function: finalizeResults(const HouseType* house, const Hunter hunters[NUM_HUNTERS], const Ghost* ghost)
    Purpose: Prints the final results of the ghost-hunting game.

    Parameters:
      in: house - a pointer to the HouseType structure containing information about the house.
      in: hunters - an array of Hunter structures containing information about each hunter.
      in: ghost - a pointer to the Ghost structure containing information about the ghost.

    Example Usage:
      finalizeResults(&myHouse, myHunters, &myGhost);
*/


void finalizeResults(const HouseType* house, const Hunter hunters[NUM_HUNTERS], const Ghost* ghost) {
    int numCollectedEvidence = 0;
    printf("Hunters with fear >= FEAR_MAX:\n");
    for (int i = 0; i < NUM_HUNTERS; i++) {
        if (hunters[i].fear >= FEAR_MAX) {
            printf("- %s\n", hunters[i].name);
        }
    }
    printf("\nHunters with boredom >= BOREDOM_MAX:\n");
    for (int i = 0; i < NUM_HUNTERS; i++) {
        if (hunters[i].boredom >= BOREDOM_MAX && hunters[i].fear < FEAR_MAX) {
            printf("- %s\n", hunters[i].name);
        }
    }
    int allHuntersInactive = 1;
    for (int i = 0; i < NUM_HUNTERS; i++) {
        if (hunters[i].fear < FEAR_MAX && hunters[i].boredom < BOREDOM_MAX) {
            allHuntersInactive = 0;
            break;
        }
    }

    if (allHuntersInactive) {
        printf("\nThe ghost has won! All hunters are inactive.\n");
    }

    printf("\nEvidence collected by hunters:\n");
    for (int i = 0; i < numCollectedEvidence; i++) {
        char evidenceTypeStr[MAX_STR];
        evidenceToString(collectedEvidence[i], evidenceTypeStr);
        printf("- %s\n", evidenceTypeStr);
    }

    if (numCollectedEvidence >= 3) {
        GhostClass identifiedGhost = identifyGhost(collectedEvidence);
        char identifiedGhostStr[MAX_STR];
        ghostToString(identifiedGhost, identifiedGhostStr);
        printf("\nIdentified Ghost Type: %s\n", identifiedGhostStr);
        if (identifiedGhost == ghost->type) {
            printf("The pieces of evidence correctly identified the ghost!\n");
        } else {
            printf("The pieces of evidence did not correctly identify the ghost.\n");
        }
    }
}

