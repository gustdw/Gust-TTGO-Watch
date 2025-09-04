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

LV_IMG_DECLARE(exit_32px);
LV_IMG_DECLARE(button_64px);
LV_IMG_DECLARE(refresh_32px);
LV_IMG_DECLARE(prev_32px);
LV_FONT_DECLARE(Ubuntu_72px);

// buttons
static void enter_scoreboard_app_team1_score_event_cb( lv_obj_t * obj, lv_event_t event );
static void enter_scoreboard_app_team2_score_event_cb( lv_obj_t * obj, lv_event_t event );
static void enter_scoreboard_app_undo_event_cb( lv_obj_t * obj, lv_event_t event );
static void enter_scoreboard_app_reset_event_cb( lv_obj_t * obj, lv_event_t event );
static void exit_scoreboard_app_main_event_cb( lv_obj_t * obj, lv_event_t event );

// Labels
lv_obj_t *team1_score_label;
lv_obj_t *team2_score_label;
lv_obj_t *team1_games_label;
lv_obj_t *team2_games_label;
lv_obj_t *team1_sets_label;
lv_obj_t *team2_sets_label;
lv_obj_t *head_label;

void initialize_styles(uint32_t tile_num) {
    scoreboard_app_main_tile = mainbar_get_tile_obj( tile_num );
    lv_style_copy( &scoreboard_app_main_style, APP_STYLE );

    lv_style_init(&large_label_style);
    lv_style_copy(&large_label_style, APP_STYLE);
    lv_style_set_text_font(&large_label_style, LV_STATE_DEFAULT, &Ubuntu_72px);
    lv_style_set_text_color(&large_label_style, LV_STATE_DEFAULT, LV_COLOR_WHITE);

    lv_style_init(&transparent_style);
    lv_style_copy(&transparent_style, APP_STYLE);
    lv_style_set_bg_opa(&transparent_style, LV_STATE_DEFAULT, LV_OPA_TRANSP);

    lv_style_set_border_opa(&transparent_style, LV_STATE_DEFAULT, LV_OPA_TRANSP);
    lv_style_set_border_color(&transparent_style, LV_STATE_PRESSED, LV_COLOR_WHITE);
    lv_style_set_border_opa(&transparent_style, LV_STATE_PRESSED, LV_OPA_COVER);
}

void update_labels() {
    lv_label_set_text(team1_score_label, to_score(team1.score.points));
    lv_label_set_text(team2_score_label, to_score(team2.score.points));
    lv_label_set_text_fmt(team1_games_label, "%d", team1.score.games);
    lv_label_set_text_fmt(team2_games_label, "%d", team2.score.games);
    lv_label_set_text_fmt(team1_sets_label, "%d", team1.score.sets);
    lv_label_set_text_fmt(team2_sets_label, "%d", team2.score.sets);
    lv_label_set_text_fmt(head_label, "%d", score_history.head);
}

void points_setup() {
    lv_obj_t * points_container = wf_add_container(scoreboard_app_main_tile, LV_LAYOUT_ROW_MID, LV_FIT_TIGHT, LV_FIT_TIGHT, true);

    // Points label (Top center)
    lv_obj_t * points_label = wf_add_label(points_container, "Points");

    lv_obj_t * points_label = lv_label_create( points_container, NULL );
    lv_label_set_text( points_label, "Points" );
    lv_obj_add_style( points_label, LV_LABEL_PART_MAIN, &scoreboard_app_main_style );
    lv_obj_set_size( points_label, LV_HOR_RES/3, LV_VER_RES/2 );
    lv_obj_align( points_label, NULL, LV_ALIGN_CENTER, 0, -30 );

    // Team 1 scored (Top left)
    lv_obj_t * team1_score_btn = lv_btn_create( points_label, NULL);
    lv_obj_add_style(team1_score_btn, LV_BTN_PART_MAIN, &transparent_style );
    lv_obj_set_size(team1_score_btn, LV_HOR_RES/3, LV_VER_RES/2 );
    lv_obj_align_x(team1_score_btn, NULL, LV_ALIGN_CENTER, LV_HOR_RES/3);
    lv_obj_set_event_cb( team1_score_btn, enter_scoreboard_app_team1_score_event_cb );

    team1_score_label = lv_label_create( team1_score_btn, NULL );
    lv_label_set_text_fmt(team1_score_label, "%d", POINTS_0);
    lv_obj_set_size( team1_score_label, LV_HOR_RES/3, LV_VER_RES/2 );
    lv_obj_add_style( team1_score_label, LV_LABEL_PART_MAIN, &large_label_style);
    lv_obj_align( team1_score_label, NULL, LV_ALIGN_CENTER, 0, 0 );

    // Team 2 scored (Top right)
    lv_obj_t * team2_score_btn = lv_btn_create( points_label, NULL);
    lv_obj_add_style(team2_score_btn, LV_BTN_PART_MAIN, &transparent_style );
    lv_obj_set_size(team2_score_btn, LV_HOR_RES/3, LV_VER_RES/2 );
    lv_obj_align_x(team2_score_btn, NULL, LV_ALIGN_CENTER, 0);
    lv_obj_set_event_cb( team2_score_btn, enter_scoreboard_app_team2_score_event_cb );

    team2_score_label = lv_label_create( team2_score_btn, NULL );
    lv_label_set_text_fmt(team2_score_label, "%d", POINTS_0);
    lv_obj_set_size( team2_score_label, LV_HOR_RES/3, LV_VER_RES/2 );
    lv_obj_reset_style_list( team2_score_label, LV_LABEL_PART_MAIN);
    lv_obj_add_style( team2_score_label, LV_LABEL_PART_MAIN, &large_label_style );
    lv_obj_align( team2_score_label, team2_score_btn, LV_ALIGN_CENTER, 0, 0 );
}

void games_setup() {
    // Games label (Center center)
    lv_obj_t * games_label = lv_label_create( scoreboard_app_main_tile, NULL );
    lv_label_set_text( games_label, "Games" );
    lv_obj_align( games_label, scoreboard_app_main_tile, LV_ALIGN_IN_BOTTOM_MID, 0, -60 );

    // Team 1 Games won (Center left)
    team1_games_label = lv_label_create( games_label, NULL );
    lv_label_set_text_fmt(team1_games_label, "%d", 0);
    lv_obj_align_x( team1_games_label, NULL, LV_ALIGN_OUT_LEFT_MID, 0);

    // Team 2 Games won (Center right)
    team2_games_label = lv_label_create( games_label, NULL );
    lv_label_set_text_fmt(team2_games_label, "%d", 0);
    lv_obj_align_x( team2_games_label, NULL, LV_ALIGN_OUT_RIGHT_MID, 0);
}

void sets_setup() {
    // Sets label (Bottom center)
    lv_obj_t * sets_label = lv_label_create( scoreboard_app_main_tile, NULL );
    lv_label_set_text( sets_label, "Sets" );
    lv_obj_align( sets_label, NULL, LV_ALIGN_IN_BOTTOM_MID, 0, -40 );

    // Team 1 Sets won (Bottom left)
    team1_sets_label = lv_label_create( sets_label, NULL );
    lv_label_set_text_fmt(team1_sets_label, "%d", 0);
    lv_obj_align_x( team1_sets_label, NULL, LV_ALIGN_OUT_LEFT_MID, 0 );

    // Team 2 Sets won (Bottom right)
    team2_sets_label = lv_label_create( sets_label, NULL );
    lv_label_set_text_fmt(team2_sets_label, "%d", 0);
    lv_obj_align_x( team2_sets_label, NULL, LV_ALIGN_OUT_RIGHT_MID, 0 );
}

void score_setup() {
    points_setup();
    games_setup();
    sets_setup();
}

void reset_setup(lv_obj_t * parent) {
    // Reset score
    lv_obj_t * reset_btn = lv_imgbtn_create( parent, NULL);
    lv_imgbtn_set_src(reset_btn, LV_BTN_STATE_RELEASED, &refresh_32px);
    lv_imgbtn_set_src(reset_btn, LV_BTN_STATE_PRESSED, &refresh_32px);
    lv_imgbtn_set_src(reset_btn, LV_BTN_STATE_CHECKED_RELEASED, &refresh_32px);
    lv_imgbtn_set_src(reset_btn, LV_BTN_STATE_CHECKED_PRESSED, &refresh_32px);
    lv_obj_add_style(reset_btn, LV_IMGBTN_PART_MAIN, &scoreboard_app_main_style );
    lv_obj_align_x(reset_btn, NULL, LV_ALIGN_OUT_LEFT_MID, 0 );
    lv_obj_set_event_cb( reset_btn, enter_scoreboard_app_reset_event_cb );
}

void undo_setup(lv_obj_t * parent) {
    // Undo last action
    lv_obj_t * undo_btn = lv_imgbtn_create( parent, NULL);
    lv_imgbtn_set_src(undo_btn, LV_BTN_STATE_RELEASED, &prev_32px);
    lv_imgbtn_set_src(undo_btn, LV_BTN_STATE_PRESSED, &prev_32px);
    lv_imgbtn_set_src(undo_btn, LV_BTN_STATE_CHECKED_RELEASED, &prev_32px);
    lv_imgbtn_set_src(undo_btn, LV_BTN_STATE_CHECKED_PRESSED, &prev_32px);
    lv_obj_add_style(undo_btn, LV_IMGBTN_PART_MAIN, &scoreboard_app_main_style );
    lv_obj_align_x(undo_btn, NULL, LV_ALIGN_CENTER, 0);
    lv_obj_set_event_cb( undo_btn, enter_scoreboard_app_undo_event_cb );
}

void exit_setup(lv_obj_t * parent) {
    // Exit button
    lv_obj_t * exit_btn = lv_imgbtn_create( parent, NULL);
    lv_imgbtn_set_src(exit_btn, LV_BTN_STATE_RELEASED, &exit_32px);
    lv_imgbtn_set_src(exit_btn, LV_BTN_STATE_PRESSED, &exit_32px);
    lv_imgbtn_set_src(exit_btn, LV_BTN_STATE_CHECKED_RELEASED, &exit_32px);
    lv_imgbtn_set_src(exit_btn, LV_BTN_STATE_CHECKED_PRESSED, &exit_32px);
    lv_obj_add_style(exit_btn, LV_IMGBTN_PART_MAIN, &scoreboard_app_main_style );
    lv_obj_align_x(exit_btn, NULL, LV_ALIGN_OUT_RIGHT_MID, 0);
    lv_obj_set_event_cb( exit_btn, exit_scoreboard_app_main_event_cb );
}

void actionbar_setup() {
    lv_obj_t * actionbar = lv_obj_create( scoreboard_app_main_tile, NULL );
    lv_obj_add_style(actionbar, LV_OBJ_PART_MAIN, &scoreboard_app_main_style );
    lv_obj_align(actionbar, scoreboard_app_main_tile, LV_ALIGN_IN_BOTTOM_MID, 0, 0 );

    reset_setup(actionbar);
    undo_setup(actionbar);
    exit_setup(actionbar);
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
    score_setup();
    actionbar_setup();

    // HEAD LABEL
    head_label = lv_label_create( scoreboard_app_main_tile, NULL );
    lv_label_set_text_fmt( head_label, "%d", score_history.head );
    lv_obj_align( head_label, scoreboard_app_main_tile, LV_ALIGN_IN_TOP_MID, 0, 10 );

    score_init();
}

static void enter_scoreboard_app_team1_score_event_cb( lv_obj_t * obj, lv_event_t event ) {
    switch( event ) {
        case( LV_EVENT_CLICKED ):
            score_add(&team1, &team2);
            score_save_snapshot();
            update_labels();
            break;
    }
}

static void enter_scoreboard_app_team2_score_event_cb( lv_obj_t * obj, lv_event_t event ) {
    switch( event ) {
        case( LV_EVENT_CLICKED ):      
            score_add(&team2, &team1);
            score_save_snapshot();
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
            score_reset();
            // score_save_snapshot();
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