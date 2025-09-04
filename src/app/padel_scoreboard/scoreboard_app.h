#ifndef _SCOREBOARD_APP_H
    #define _SCOREBOARD_APP_H

    /**
     * @brief setup scoreboard app
     * 
     */
    void scoreboard_app_setup( void );
    /**
     * @brief get to scoreboard app tile number
     * 
     * @return uint32_t tilenumber
     */
    uint32_t scoreboard_app_get_app_main_tile_num( void );
    /**
     * @brief call back function when enter the app
     * 
     * @param obj           object
     * @param event         event
     */
    void enter_scoreboard_app_event_cb( lv_obj_t * obj, lv_event_t event );
    /**
     * @brief call back function when exit the app
     * 
     * @param obj           object
     * @param event         event
     */
    void exit_scoreboard_app_event_cb( lv_obj_t * obj, lv_event_t event );

#endif // _SCOREBOARD_APP_H