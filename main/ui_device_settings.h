#ifndef UI_DEVICE_SETTINGS_H
#define UI_DEVICE_SETTINGS_H

#ifdef __cplusplus
extern "C" {
#endif

#include "lvgl.h"

/**
 * @brief Create the device settings screen
 * @param prev_screen The screen to return to (e.g. main timer screen)
 * @return The created settings screen object
 */
lv_obj_t *ui_device_settings_create(lv_obj_t *prev_screen);

#ifdef __cplusplus
}
#endif

#endif /* UI_DEVICE_SETTINGS_H */
