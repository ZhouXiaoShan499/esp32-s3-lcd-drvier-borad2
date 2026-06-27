#ifndef UI_FOCUS_TIMER_H
#define UI_FOCUS_TIMER_H

#ifdef __cplusplus
extern "C" {
#endif

#include "lvgl.h"

/**
 * @brief Create the focus timer UI screen
 * @return The created screen object
 */
lv_obj_t *ui_focus_timer_create(void);

/**
 * @brief Update the timer display
 * @param hours hours
 * @param minutes minutes
 * @param seconds seconds
 */
void ui_focus_timer_update_time(uint8_t hours, uint8_t minutes, uint8_t seconds);

/**
 * @brief Update the current status text
 * @param status status string (e.g. "电脑办公", "学习", "看书")
 */
void ui_focus_timer_set_status(const char *status);

/**
 * @brief Update daily stats
 * @param focus_h focus hours
 * @param focus_m focus minutes
 * @param rest_h rest hours
 * @param rest_m rest minutes
 */
void ui_focus_timer_set_daily_stats(uint8_t focus_h, uint8_t focus_m, uint8_t rest_h, uint8_t rest_m);

#ifdef __cplusplus
}
#endif

#endif /* UI_FOCUS_TIMER_H */
