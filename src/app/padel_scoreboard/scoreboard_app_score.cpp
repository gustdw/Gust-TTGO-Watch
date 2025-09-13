#include "scoreboard_app_score.h"

team_t team1 = { .score = { .points = POINTS_0, .games = 0, .sets = 0 } };
team_t team2 = { .score = { .points = POINTS_0, .games = 0, .sets = 0 } };
uint8_t turn = 0; // 1 for team1, 2 for team2, 0 for no turn yet
score_history_t score_history;

void score_init() {
    score_history.head = 0;
    score_history.size = 0;
}

void score_add(team_t *won, team_t *lost) {
    if (turn == 0) {
        turn = (won == &team1) ? 1 : 2;
        return;
    }

    won->score.points = static_cast<points_t>(static_cast<int>(won->score.points) + 1);

    if (won->score.points == POINTS_AD || won->score.points == POINTS_GAME) {
        if ((won->score.points - lost->score.points) > 1) {
            won->score.points = POINTS_0;
            won->score.games += 1;
            lost->score.points = POINTS_0;
            turn = (turn == 1) ? 2 : 1; // Switch turn to the other team

            if (won->score.games >= 6 && (won->score.games - lost->score.games) > 1) {
                won->score.sets += 1;
                won->score.games = 0;
                lost->score.games = 0;
            }
        } else if ((won->score.points - lost->score.points) == 0) {
            lost->score.points = POINTS_40;
            won->score.points = POINTS_40;
        }
    }
}

void score_reset_team(team_t *team) {
    team->score.points = POINTS_0;
    team->score.games = 0;
    team->score.sets = 0;
}

void score_reset() {
    score_reset_team(&team1);
    score_reset_team(&team2);
    turn = 0; // No turn yet
}

void score_save_snapshot() {
    score_snapshot_t score_snapshot = { .team1 = team1, .team2 = team2, .turn = turn };
    score_history.snapshots[score_history.head] = score_snapshot;
    score_history.head = (score_history.head + 1) % HISTORY_BUFF_SIZE;
    if (score_history.size < HISTORY_BUFF_SIZE) score_history.size++;
}   

void score_undo() {
    if (score_history.size == 0) return;

    score_history.size--;
    score_history.head = (score_history.head + HISTORY_BUFF_SIZE - 1) % HISTORY_BUFF_SIZE;
    score_snapshot_t *snapshot = &score_history.snapshots[score_history.head];

    team1 = snapshot->team1;
    team2 = snapshot->team2;
    turn = snapshot->turn;
}