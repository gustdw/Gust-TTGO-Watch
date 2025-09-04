#include <stdint.h>

// Points enum
typedef enum {
    POINTS_0 = 0,
    POINTS_15 = 1,
    POINTS_30 = 2,
    POINTS_40 = 3,
    POINTS_AD = 4,
    POINTS_GAME = 5
} points_t;

// Convert points_t to string representation
static inline const char* to_score(points_t p) {
    switch(p) {
        case POINTS_0:  return "0";
        case POINTS_15: return "15";
        case POINTS_30: return "30";
        case POINTS_40: return "40";
        case POINTS_AD: return "AD";
        default:        return "?";
    }
}

// Score struct
typedef struct {
    points_t points;
    uint8_t games;
    uint8_t sets;
} score_t;

// Team structs
typedef struct {
    score_t score;
} team_t;

extern team_t team1;
extern team_t team2;

// Snapshot of the current score
typedef struct {
    team_t team1;
    team_t team2;
} score_snapshot_t;

typedef struct {
    #define HISTORY_BUFF_SIZE 100
    score_snapshot_t snapshots[HISTORY_BUFF_SIZE];
    uint8_t head;
} score_history_t;

extern score_history_t score_history;

// Callable functions
void score_reset_team(team_t *team);
void score_add(team_t *won, team_t *lost);
void score_reset();
void score_save_snapshot();
void score_undo();
void score_init();