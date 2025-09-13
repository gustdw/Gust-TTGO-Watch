#include "scoreboard_app_setup.h"
#include "scoreboard_app_main.h"

#include "gui/mainbar/app_tile/app_tile.h"
#include "gui/mainbar/main_tile/main_tile.h"
#include "gui/mainbar/mainbar.h"
#include "gui/statusbar.h"
#include "gui/widget_factory.h"
#include "lvgl.h"

lv_obj_t *team1_score_btn = NULL;
lv_obj_t *team2_score_btn = NULL;
lv_obj_t *team1_games_label = NULL;
lv_obj_t *team2_games_label = NULL;
lv_obj_t *team1_sets_label = NULL;
lv_obj_t *team2_sets_label = NULL;
lv_obj_t *team1_turn_arrow = NULL;
lv_obj_t *team2_turn_arrow = NULL;
LV_IMG_DECLARE(up_16px);

lv_obj_t *scoreboard_app_main_tile = NULL;

// Styles
static lv_style_t scoreboard_app_main_style;
static lv_style_t large_label_style;
static lv_style_t medium_label_style;
static lv_style_t transparent_style;

LV_FONT_DECLARE(Ubuntu_48px);
LV_FONT_DECLARE(Ubuntu_32px);

void initialize_styles(uint32_t tile_num) {
    scoreboard_app_main_tile = mainbar_get_tile_obj( tile_num );
    lv_style_copy( &scoreboard_app_main_style, APP_STYLE );

    lv_style_init(&large_label_style);
    lv_style_copy(&large_label_style, APP_STYLE);
    lv_style_set_text_font(&large_label_style, LV_STATE_DEFAULT, &Ubuntu_48px);
    lv_style_set_text_color(&large_label_style, LV_STATE_DEFAULT, LV_COLOR_WHITE);

    lv_style_init(&medium_label_style);
    lv_style_copy(&medium_label_style, APP_STYLE);
    lv_style_set_text_font(&medium_label_style, LV_STATE_DEFAULT, &Ubuntu_32px);
    lv_style_set_text_color(&medium_label_style, LV_STATE_DEFAULT, LV_COLOR_WHITE);

    lv_style_init(&transparent_style);
    lv_style_copy(&transparent_style, APP_STYLE);
    lv_style_set_bg_opa(&transparent_style, LV_STATE_DEFAULT, LV_OPA_TRANSP);
    lv_style_set_border_opa(&transparent_style, LV_STATE_DEFAULT, LV_OPA_TRANSP);
}

void points_setup(lv_obj_t * parent) {
    lv_obj_t * points_container = wf_add_container(parent, LV_LAYOUT_PRETTY_MID, LV_FIT_PARENT, LV_FIT_TIGHT, false);
    lv_obj_align(points_container, NULL, LV_ALIGN_IN_TOP_LEFT, 0, 0);

    team1_score_btn = wf_add_button_c( points_container, "0", LV_HOR_RES/4, LV_VER_RES/2, enter_scoreboard_app_team1_score_event_cb);
    lv_obj_add_style(lv_obj_get_child(team1_score_btn, NULL), LV_LABEL_PART_MAIN, &large_label_style);
    lv_obj_add_style(team1_score_btn, LV_BTN_PART_MAIN, &transparent_style);

    lv_obj_t * points_label = wf_add_label(points_container, "Points");
    lv_obj_set_width(points_label, LV_HOR_RES/4);

    team2_score_btn = wf_add_button_c( points_container, "0", LV_HOR_RES/4, LV_VER_RES/2, enter_scoreboard_app_team2_score_event_cb);
    lv_obj_add_style(lv_obj_get_child(team2_score_btn, NULL), LV_LABEL_PART_MAIN, &large_label_style);
    lv_obj_add_style(team2_score_btn, LV_BTN_PART_MAIN, &transparent_style);
}

void games_setup(lv_obj_t * parent) {
    lv_obj_t * games_container = wf_add_container(parent, LV_LAYOUT_PRETTY_MID, LV_FIT_PARENT, LV_FIT_TIGHT, false);
    lv_obj_align(games_container, NULL, LV_ALIGN_IN_BOTTOM_LEFT, 0, 0);

    team1_games_label = wf_add_label( games_container, "0" );
    lv_obj_add_style(team1_games_label, LV_LABEL_PART_MAIN, &medium_label_style);

    wf_add_label( games_container, "Games" );
    team2_games_label = wf_add_label( games_container, "0" );
    lv_obj_add_style(team2_games_label, LV_LABEL_PART_MAIN, &medium_label_style);
}

void sets_setup(lv_obj_t * parent) {
    lv_obj_t * sets_container = wf_add_container(parent, LV_LAYOUT_PRETTY_MID, LV_FIT_PARENT, LV_FIT_TIGHT, false);
    lv_obj_align(sets_container, NULL, LV_ALIGN_IN_BOTTOM_LEFT, 0, 0);

    team1_sets_label = wf_add_label( sets_container, "0" );
    wf_add_label( sets_container, "Sets" );
    team2_sets_label = wf_add_label( sets_container, "0" );
}

void turn_setup(lv_obj_t * parent) {
    // Add up arrow images below each team's score button
    // Create arrows as children of parent, but set their parent after creation to ensure correct z-order
    team1_turn_arrow = lv_img_create(scoreboard_app_main_tile, NULL);
    lv_img_set_src(team1_turn_arrow, &up_16px);
    lv_obj_align(team1_turn_arrow, team1_score_btn, LV_ALIGN_CENTER, 0, 30);
    lv_obj_set_style_local_opa_scale(team1_turn_arrow, LV_IMG_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_TRANSP); // Initially hidden

    team2_turn_arrow = lv_img_create(scoreboard_app_main_tile, NULL);
    lv_img_set_src(team2_turn_arrow, &up_16px);
    lv_obj_align(team2_turn_arrow, team2_score_btn, LV_ALIGN_CENTER, 0, 30);
    lv_obj_set_style_local_opa_scale(team2_turn_arrow, LV_IMG_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_TRANSP); // Initially hidden
}

void score_setup(lv_obj_t * parent) {
    lv_obj_t * score_container = wf_add_container(parent, LV_LAYOUT_COLUMN_MID, LV_FIT_PARENT, LV_FIT_TIGHT, false);
    lv_obj_align(score_container, NULL, LV_ALIGN_IN_TOP_LEFT, 0, 0 );
    points_setup(score_container);
    games_setup(score_container);
    sets_setup(score_container);
    turn_setup(parent);
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
    |        |        |        |
    | T1 pts | Points | T2 pts |
    |        |        |        |
    |        |        |        |
    |________|________|________|
    |        |        |        |
    |T1 games|  Games |T2 games|
    |        |        |        |
    | T1 sets|  Sets  | T2 sets|
    |________|________|________|
    |        |        |        |
    | Reset  |  Undo  |  Exit  |
    |________|________|________|

*/

void scoreboard_app_gui_setup( uint32_t tile_num ) {
    initialize_styles(tile_num);
    lv_obj_t * scoreboard_app_container = wf_add_container( scoreboard_app_main_tile, LV_LAYOUT_COLUMN_MID, LV_FIT_PARENT, LV_FIT_TIGHT, false );
    score_setup(scoreboard_app_container);
    actionbar_setup(scoreboard_app_container);
}