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

#include "gui/mainbar/app_tile/app_tile.h"
#include "gui/mainbar/main_tile/main_tile.h"
#include "gui/mainbar/mainbar.h"
#include "gui/statusbar.h"
#include "gui/widget_factory.h"

lv_obj_t *scoreboard_app_main_tile = NULL;
// Styles
static lv_style_t scoreboard_app_main_style;
static lv_style_t large_label_style;
static lv_style_t transparent_style;

LV_FONT_DECLARE(Ubuntu_48px);

// buttons
static void enter_scoreboard_app_team1_score_event_cb( lv_obj_t * obj, lv_event_t event );
static void enter_scoreboard_app_team2_score_event_cb( lv_obj_t * obj, lv_event_t event );
static void enter_scoreboard_app_undo_event_cb( lv_obj_t * obj, lv_event_t event );
static void enter_scoreboard_app_reset_event_cb( lv_obj_t * obj, lv_event_t event );
static void exit_scoreboard_app_main_event_cb( lv_obj_t * obj, lv_event_t event );

// Labels
lv_obj_t *team1_score_btn;
lv_obj_t *team2_score_btn;
lv_obj_t *team1_games_label;
lv_obj_t *team2_games_label;
lv_obj_t *team1_sets_label;
lv_obj_t *team2_sets_label;

void initialize_styles(uint32_t tile_num) {
    scoreboard_app_main_tile = mainbar_get_tile_obj( tile_num );
    lv_style_copy( &scoreboard_app_main_style, APP_STYLE );

    lv_style_init(&large_label_style);
    lv_style_copy(&large_label_style, APP_STYLE);
    lv_style_set_text_font(&large_label_style, LV_STATE_DEFAULT, &Ubuntu_48px);
    lv_style_set_text_color(&large_label_style, LV_STATE_DEFAULT, LV_COLOR_WHITE);

    lv_style_init(&transparent_style);
    lv_style_copy(&transparent_style, APP_STYLE);
    lv_style_set_bg_opa(&transparent_style, LV_STATE_DEFAULT, LV_OPA_TRANSP);
    lv_style_set_border_opa(&transparent_style, LV_STATE_DEFAULT, LV_OPA_TRANSP);
}

void update_labels() {
    lv_label_set_text(lv_obj_get_child(team1_score_btn, NULL), to_score(team1.score.points));
    lv_label_set_text(lv_obj_get_child(team2_score_btn, NULL), to_score(team2.score.points));
    lv_label_set_text_fmt(team1_games_label, "%d", team1.score.games);
    lv_label_set_text_fmt(team2_games_label, "%d", team2.score.games);
    lv_label_set_text_fmt(team1_sets_label, "%d", team1.score.sets);
    lv_label_set_text_fmt(team2_sets_label, "%d", team2.score.sets);
}

void points_setup(lv_obj_t * parent) {
    lv_obj_t * points_container = wf_add_container(parent, LV_LAYOUT_PRETTY_MID, LV_FIT_PARENT, LV_FIT_TIGHT, false);
    lv_obj_align(points_container, NULL, LV_ALIGN_IN_TOP_LEFT, 0, 0);

    // Team 1 scored (Top left)
    team1_score_btn = wf_add_button_c( points_container, "0", LV_HOR_RES/4, LV_VER_RES/2, enter_scoreboard_app_team1_score_event_cb);
    lv_obj_add_style(lv_obj_get_child(team1_score_btn, NULL), LV_BTN_PART_MAIN, &large_label_style);
    lv_obj_add_style(team1_score_btn, LV_BTN_PART_MAIN, &transparent_style);


    // Points label (Top center)
    lv_obj_t * points_label = wf_add_label(points_container, "Points");
    lv_obj_set_width(points_label, LV_HOR_RES/4);

    // Team 2 scored (Top right)
    team2_score_btn = wf_add_button_c( points_container, "0", LV_HOR_RES/4, LV_VER_RES/2, enter_scoreboard_app_team2_score_event_cb);
    lv_obj_add_style(lv_obj_get_child(team2_score_btn, NULL), LV_BTN_PART_MAIN, &large_label_style);
    lv_obj_add_style(team2_score_btn, LV_BTN_PART_MAIN, &transparent_style);
}

void games_setup(lv_obj_t * parent) {
    lv_obj_t * games_container = wf_add_container(parent, LV_LAYOUT_PRETTY_MID, LV_FIT_PARENT, LV_FIT_TIGHT, false);
    lv_obj_align(games_container, NULL, LV_ALIGN_IN_BOTTOM_LEFT, 0, 0);

    // Team 1 Games won (Center left)
    team1_games_label = wf_add_label( games_container, "0" );

    // Games label (Center center)
    wf_add_label( games_container, "Games" );

    // Team 2 Games won (Center right)
    team2_games_label = wf_add_label( games_container, "0" );
}

void sets_setup(lv_obj_t * parent) {
    lv_obj_t * sets_container = wf_add_container(parent, LV_LAYOUT_PRETTY_MID, LV_FIT_PARENT, LV_FIT_TIGHT, false);
    lv_obj_align(sets_container, NULL, LV_ALIGN_IN_BOTTOM_LEFT, 0, 0);

    // Team 1 Sets won (Bottom left)
    team1_sets_label = wf_add_label( sets_container, "0" );

    // Sets label (Bottom center)
    wf_add_label( sets_container, "Sets" );
    
    // Team 2 Sets won (Bottom right)
    team2_sets_label = wf_add_label( sets_container, "0" );
}

void score_setup(lv_obj_t * parent) {
    lv_obj_t * score_container = wf_add_container(parent, LV_LAYOUT_COLUMN_MID, LV_FIT_PARENT, LV_FIT_TIGHT, false);
    lv_obj_align(score_container, NULL, LV_ALIGN_IN_TOP_LEFT, 0, 0 );
    points_setup(score_container);
    games_setup(score_container);
    sets_setup(score_container);
}

void reset_setup(lv_obj_t * parent) {
    // Reset score
    wf_add_refresh_button(parent, enter_scoreboard_app_reset_event_cb);
}

void undo_setup(lv_obj_t * parent) {
    // Undo last action
    wf_add_left_button(parent, enter_scoreboard_app_undo_event_cb);
}

void exit_setup(lv_obj_t * parent) {
    // Exit button
    wf_add_exit_button(parent, exit_scoreboard_app_main_event_cb);
}

void actionbar_setup(lv_obj_t * parent) {
    lv_obj_t * actionbar_container = wf_add_container( parent, LV_LAYOUT_PRETTY_MID, LV_FIT_PARENT, LV_FIT_TIGHT, false );
    lv_obj_align(actionbar_container, parent, LV_ALIGN_IN_BOTTOM_LEFT, 0, 0 );

    reset_setup(actionbar_container);
    undo_setup(actionbar_container);
    exit_setup(actionbar_container);
}

/*
    ____________________________
    |        |        |        |
    | T1 pts | Points | T2 pts |
    |________|________|________|
    |        |        |        |
    |T1 games|  Games |T2 games|
    |________|________|________|
    |        |        |        |
    | T1 sets|  Sets  | T2 sets|
    |________|________|________|
    |        |        |        |
    | Reset  |  Undo  |  Exit  |
    |________|________|________|

*/

// GUI setup
void scoreboard_app_main_setup( uint32_t tile_num ) {
    initialize_styles(tile_num);
    lv_obj_t * scoreboard_app_container = wf_add_container( scoreboard_app_main_tile, LV_LAYOUT_COLUMN_MID, LV_FIT_PARENT, LV_FIT_TIGHT, false );
    score_setup(scoreboard_app_container);
    actionbar_setup(scoreboard_app_container);

    score_init();
}

static void enter_scoreboard_app_team1_score_event_cb( lv_obj_t * obj, lv_event_t event ) {
    switch( event ) {
        case( LV_EVENT_CLICKED ):
            score_save_snapshot();
            score_add(&team1, &team2);
            update_labels();
            break;
    }
}

static void enter_scoreboard_app_team2_score_event_cb( lv_obj_t * obj, lv_event_t event ) {
    switch( event ) {
        case( LV_EVENT_CLICKED ):      
            score_save_snapshot();
            score_add(&team2, &team1);
            update_labels();
            break;
    }
}

static void enter_scoreboard_app_undo_event_cb( lv_obj_t * obj, lv_event_t event ) {
    switch( event ) {
        case( LV_EVENT_CLICKED ):      
            score_undo();
            update_labels();
            break;
    }
}

static void enter_scoreboard_app_reset_event_cb( lv_obj_t * obj, lv_event_t event ) {
    switch( event ) {
        case( LV_EVENT_CLICKED ):
            score_save_snapshot();
            score_reset();
            update_labels();
            break;
    }
}

static void exit_scoreboard_app_main_event_cb( lv_obj_t * obj, lv_event_t event ) {
    switch( event ) {
        case( LV_EVENT_CLICKED ):       mainbar_jump_back();
                                        break;
    }
}