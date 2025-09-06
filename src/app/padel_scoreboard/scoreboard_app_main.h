#ifndef _SCOREBOARD_APP_MAIN_H
    #define _SCOREBOARD_APP_MAIN_H

    void enter_scoreboard_app_team1_score_event_cb( lv_obj_t * obj, lv_event_t event );
    void enter_scoreboard_app_team2_score_event_cb( lv_obj_t * obj, lv_event_t event );
    void enter_scoreboard_app_undo_event_cb( lv_obj_t * obj, lv_event_t event );
    void enter_scoreboard_app_reset_event_cb( lv_obj_t * obj, lv_event_t event );
    void exit_scoreboard_app_main_event_cb( lv_obj_t * obj, lv_event_t event );

    extern lv_obj_t *team1_score_btn;
    extern lv_obj_t *team2_score_btn;
    extern lv_obj_t *team1_games_label;
    extern lv_obj_t *team2_games_label;
    extern lv_obj_t *team1_sets_label;
    extern lv_obj_t *team2_sets_label;
    
    /**
     * @brief setup main scoreboard main tile
     * 
     * @param tile_num return tile for scoreboard main tile
     */
    void scoreboard_app_main_setup( uint32_t tile_num );

#endif // _SCOREBOARD_APP_MAIN_H