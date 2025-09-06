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
#include "gui/widget_factory.h"


void update_labels() {
    lv_label_set_text(lv_obj_get_child(team1_score_btn, NULL), to_score(team1.score.points));
    lv_label_set_text(lv_obj_get_child(team2_score_btn, NULL), to_score(team2.score.points));
    lv_label_set_text_fmt(team1_games_label, "%d", team1.score.games);
    lv_label_set_text_fmt(team2_games_label, "%d", team2.score.games);
    lv_label_set_text_fmt(team1_sets_label, "%d", team1.score.sets);
    lv_label_set_text_fmt(team2_sets_label, "%d", team2.score.sets);
}

// GUI setup
void scoreboard_app_main_setup( uint32_t tile_num ) {
    scoreboard_app_setup(tile_num);

    score_init();
}

void enter_scoreboard_app_team1_score_event_cb( lv_obj_t * obj, lv_event_t event ) {
    switch( event ) {
        case( LV_EVENT_CLICKED ):       score_save_snapshot();
                                        score_add(&team1, &team2);
                                        update_labels();
                                        break;
    }
}

void enter_scoreboard_app_team2_score_event_cb( lv_obj_t * obj, lv_event_t event ) {
    switch( event ) {
        case( LV_EVENT_CLICKED ):       score_save_snapshot();
                                        score_add(&team2, &team1);
                                        update_labels();
                                        break;
    }
}

void enter_scoreboard_app_undo_event_cb( lv_obj_t * obj, lv_event_t event ) {
    switch( event ) {
        case( LV_EVENT_CLICKED ):       score_undo();
                                        update_labels();
                                        break;
    }
}

void enter_scoreboard_app_reset_event_cb( lv_obj_t * obj, lv_event_t event ) {
    switch( event ) {
        case( LV_EVENT_CLICKED ):       score_save_snapshot();
                                        score_reset();
                                        update_labels();
                                        break;
    }
}

void exit_scoreboard_app_main_event_cb( lv_obj_t * obj, lv_event_t event ) {
    switch( event ) {
        case( LV_EVENT_CLICKED ):       mainbar_jump_back();
                                        break;
    }
}