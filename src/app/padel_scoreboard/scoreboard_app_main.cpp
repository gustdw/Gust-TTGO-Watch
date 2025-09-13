/*
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#include "config.h"
#include <TTGO.h>

#include "scoreboard_app.h"
#include "scoreboard_app_main.h"
#include "scoreboard_app_score.h"
#include "scoreboard_app_setup.h"

#include "gui/mainbar/app_tile/app_tile.h"
#include "gui/mainbar/main_tile/main_tile.h"
#include "gui/mainbar/mainbar.h"
#include "gui/statusbar.h"

// #include <hardware/ble/gadgetbridge.h>


void update_labels() {
    lv_label_set_text(lv_obj_get_child(team1_score_btn, NULL), to_score(team1.score.points));
    lv_label_set_text(lv_obj_get_child(team2_score_btn, NULL), to_score(team2.score.points));
    lv_label_set_text_fmt(team1_games_label, "%d", team1.score.games);
    lv_label_set_text_fmt(team2_games_label, "%d", team2.score.games);
    lv_label_set_text_fmt(team1_sets_label, "%d", team1.score.sets);
    lv_label_set_text_fmt(team2_sets_label, "%d", team2.score.sets);
    // Update turn indicators
    if (turn == 1) {
        lv_obj_set_style_local_opa_scale(team1_turn_arrow, LV_IMG_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_COVER);
        lv_obj_set_style_local_opa_scale(team2_turn_arrow, LV_IMG_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_TRANSP);
    } else if (turn == 2) {
        lv_obj_set_style_local_opa_scale(team1_turn_arrow, LV_IMG_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_TRANSP);
        lv_obj_set_style_local_opa_scale(team2_turn_arrow, LV_IMG_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_COVER);
    } else { // No turn yet
        lv_obj_set_style_local_opa_scale(team1_turn_arrow, LV_IMG_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_TRANSP);
        lv_obj_set_style_local_opa_scale(team2_turn_arrow, LV_IMG_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_TRANSP);
    }
}

// void gadgetbridge_send_score(score_snapshot_t score) {
//     // Send score via BLE to connected device
//     // Format: "Turn;T1:points,games,sets;T2:points,games,sets"
//     #define SCORE_BUFFER_SIZE 32 // enough for "Turn;T1:AD,10,5;T2:AD,10,5" and null terminator
//     char score_str[SCORE_BUFFER_SIZE];
//     snprintf(score_str, sizeof(score_str), "%d;T1:%s,%d,%d;T2:%s,%d,%d",
//              score.turn,
//              to_score(score.team1.score.points), score.team1.score.games, score.team1.score.sets,
//              to_score(score.team2.score.points), score.team2.score.games, score.team2.score.sets);
//     gadgetbridge_send_msg(score_str);
// }

void update_score() {
    update_labels();
    //gadgetbridge_send_score(score_snapshot_t{team1, team2, turn});
}

// GUI setup
void scoreboard_app_main_setup( uint32_t tile_num ) {
    scoreboard_app_gui_setup(tile_num); // Ensure setup is called to initialize styles

    score_init();
}

void enter_scoreboard_app_team1_score_event_cb( lv_obj_t * obj, lv_event_t event ) {
    switch( event ) {
        case( LV_EVENT_CLICKED ):       score_save_snapshot();
                                        score_add(&team1, &team2);
                                        update_score();
                                        break;
    }
}

void enter_scoreboard_app_team2_score_event_cb( lv_obj_t * obj, lv_event_t event ) {
    switch( event ) {
        case( LV_EVENT_CLICKED ):       score_save_snapshot();
                                        score_add(&team2, &team1);
                                        update_score();
                                        break;
    }
}

void enter_scoreboard_app_undo_event_cb( lv_obj_t * obj, lv_event_t event ) {
    switch( event ) {
        case( LV_EVENT_CLICKED ):       score_undo();
                                        update_score();
                                        break;
    }
}

void enter_scoreboard_app_reset_event_cb( lv_obj_t * obj, lv_event_t event ) {
    switch( event ) {
        case( LV_EVENT_CLICKED ):       score_save_snapshot();
                                        score_reset();
                                        update_score();
                                        break;
    }
}

void exit_scoreboard_app_main_event_cb( lv_obj_t * obj, lv_event_t event ) {
    switch( event ) {
        case( LV_EVENT_CLICKED ):       mainbar_jump_back();
                                        break;
    }
}