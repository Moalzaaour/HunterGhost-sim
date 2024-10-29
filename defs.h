#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>


#define MAX_CONNECTED_ROOMS 5
#define MAX_COLLECTED_EVIDENCE 3
#define MAX_STR         64
#define MAX_RUNS        50
#define BOREDOM_MAX     100
#define C_TRUE          1
#define C_FALSE         0
#define HUNTER_WAIT     5000
#define GHOST_WAIT      600
#define NUM_HUNTERS     4
#define FEAR_MAX        10
#define LOGGING         C_TRUE

typedef enum EvidenceType EvidenceType;
typedef enum GhostClass GhostClass;

enum EvidenceType { EMF, TEMPERATURE, FINGERPRINTS, SOUND, EV_COUNT, EV_UNKNOWN };
enum GhostClass { POLTERGEIST, BANSHEE, BULLIES, PHANTOM, GHOST_COUNT, GH_UNKNOWN };
enum LoggerDetails { LOG_FEAR, LOG_BORED, LOG_EVIDENCE, LOG_SUFFICIENT, LOG_INSUFFICIENT, LOG_UNKNOWN };

// Forward declaration for Hunter
typedef struct Hunter Hunter;

// Forward declaration for Ghost
typedef struct Ghost Ghost;


//type cast stuff
typedef struct Room {
    char name[MAX_STR];
    pthread_mutex_t roomMutex;
    int numConnectedRooms;
    struct Room** connectedRooms;
    enum EvidenceType evidenceType;
    // Add any other room-related information if needed
    Hunter* hunters[NUM_HUNTERS];
    Ghost* ghost;
    struct Room* nextRoom;  // Add this line to include the nextRoom field
} Room;

typedef struct HouseType {
    Room* rooms;
    int numRooms;
} HouseType;

typedef struct Hunter {
    char name[MAX_STR];
    enum EvidenceType equipment;
    Room* currentRoom;
    int fear;
    int boredom;
    pthread_t thread;
    int id;  // Add this line to include the id field
} Hunter;

typedef struct Ghost {
    enum GhostClass type;
    Room* currentRoom;
    int boredom; // Add this line to include the boredom field
    pthread_t thread;
} Ghost;



extern Hunter hunters[NUM_HUNTERS];
extern pthread_mutex_t evidenceMutex;


//declarations 
void* hunterThread(void* arg);
void* ghostThread(void* arg);
void addEvidenceToRoom(Room* room, enum EvidenceType evidenceType);
Room* getRandomConnectedRoom(Room* currentRoom);
void collectEvidence(enum EvidenceType evidenceType);
int reviewEvidence();

struct Room* createRoom(const char* name);
void connectRooms(struct Room* room1, struct Room* room2);
void addRoom(struct Room** head, struct Room* room);
void populateRooms(HouseType* house);

void initHouse(HouseType* house);
enum EvidenceType randomGhostEvidence();
GhostClass identifyGhost(enum EvidenceType evidence[]);



// Helper Utilies
int randInt(int,int);        // Pseudo-random number generator function
float randFloat(float, float);  // Pseudo-random float generator function
enum GhostClass randomGhost();  // Return a randomly selected a ghost type
void ghostToString(enum GhostClass, char*); // Convert a ghost type to a string, stored in output paremeter
void evidenceToString(enum EvidenceType, char*); // Convert an evidence type to a string, stored in output parameter

// Logging Utilities
void l_hunterInit(char* name, enum EvidenceType equipment);
void l_hunterMove(char* name, char* room);
void l_hunterReview(char* name, enum LoggerDetails reviewResult);
void l_hunterCollect(char* name, enum EvidenceType evidence, char* room);
void l_hunterExit(char* name, enum LoggerDetails reason);
void l_ghostInit(enum GhostClass type, char* room);
void l_ghostMove(char* room);
void l_ghostEvidence(enum EvidenceType evidence, char* room);
void l_ghostExit(enum LoggerDetails reason);
