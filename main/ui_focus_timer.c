#include "ui_focus_timer.h"
#include "lvgl.h"
#include <stdio.h>

/* ====== Global UI objects ====== */
static lv_obj_t *screen = NULL;
static lv_obj_t *label_status = NULL;
static lv_obj_t *label_timer = NULL;
static lv_obj_t *label_timer_sub = NULL;
static lv_obj_t *label_daily = NULL;
static lv_obj_t *label_light_status = NULL;

/* Checkbox objects */
static lv_obj_t *cb_study = NULL;
static lv_obj_t *cb_read = NULL;
static lv_obj_t *cb_phone = NULL;
static lv_obj_t *cb_rest = NULL;

/* Brightness control */
static lv_obj_t *slider_brightness = NULL;
static int current_brightness = 2; /* 0=off, 1=dim, 2=bright */

/* ====== Checkbox event callback ====== */
static void checkbox_event_cb(lv_event_t *e)
{
    lv_obj_t *cb = lv_event_get_target(e);
    const char *name = lv_event_get_user_data(e);

    /* Uncheck all */
    lv_obj_clear_state(cb_study, LV_STATE_CHECKED);
    lv_obj_clear_state(cb_read, LV_STATE_CHECKED);
    lv_obj_clear_state(cb_phone, LV_STATE_CHECKED);
    lv_obj_clear_state(cb_rest, LV_STATE_CHECKED);
    /* Check this one */
    lv_obj_add_state(cb, LV_STATE_CHECKED);

    char status[32];
    snprintf(status, sizeof(status), "当前状态：%s", name);
    lv_label_set_text(label_status, status);
}

/* ====== Brightness slider callback ====== */
static void brightness_event_cb(lv_event_t *e)
{
    lv_obj_t *slider = lv_event_get_target(e);
    int val = (int)lv_slider_get_value(slider);
    current_brightness = val;

    char display[64];
    const char *off  = (val == 0) ? "●关灯" : "○关灯";
    const char *dim  = (val == 1) ? "●调暗" : "○调暗";
    const char *bri  = (val == 2) ? "●调亮" : "○调亮";
    snprintf(display, sizeof(display), "%s  %s  %s", off, dim, bri);
    lv_label_set_text(label_light_status, display);
}


/* ====== Create the focus timer UI ====== */
lv_obj_t *ui_focus_timer_create(void)
{
    screen = lv_obj_create(NULL);
    lv_obj_set_style_bg_color(screen, lv_color_hex(0x1a1a2e), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(screen, LV_OPA_COVER, LV_PART_MAIN);

    /* Card container */
    lv_obj_t *cont = lv_obj_create(screen);
    lv_obj_set_size(cont, 400, 420);
    lv_obj_align(cont, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_bg_color(cont, lv_color_hex(0x16213e), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(cont, LV_OPA_COVER, LV_PART_MAIN);
    lv_obj_set_style_radius(cont, 20, LV_PART_MAIN);
    lv_obj_set_style_border_width(cont, 0, LV_PART_MAIN);
    lv_obj_set_style_pad_all(cont, 20, LV_PART_MAIN);
    lv_obj_set_flex_flow(cont, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(cont, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_set_style_pad_row(cont, 10, LV_PART_MAIN);

    /* Row 1: Status title + badge */
    lv_obj_t *row1 = lv_obj_create(cont);
    lv_obj_set_size(row1, LV_PCT(100), LV_SIZE_CONTENT);
    lv_obj_set_style_bg_opa(row1, LV_OPA_TRANSP, LV_PART_MAIN);
    lv_obj_set_style_border_width(row1, 0, LV_PART_MAIN);
    lv_obj_set_style_pad_all(row1, 0, LV_PART_MAIN);
    lv_obj_set_flex_flow(row1, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(row1, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

    label_status = lv_label_create(row1);
    lv_label_set_text(label_status, "当前状态：电脑办公");
    lv_obj_set_style_text_color(label_status, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
    lv_obj_set_style_text_font(label_status, &lv_font_montserrat_16, LV_PART_MAIN);

    lv_obj_t *badge = lv_label_create(row1);
    lv_label_set_text(badge, "自动识别");
    lv_obj_set_style_text_color(badge, lv_color_hex(0x00FF88), LV_PART_MAIN);
    lv_obj_set_style_text_font(badge, &lv_font_montserrat_12, LV_PART_MAIN);
    lv_obj_set_style_bg_color(badge, lv_color_hex(0x0a3d2e), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(badge, LV_OPA_COVER, LV_PART_MAIN);
    lv_obj_set_style_radius(badge, 8, LV_PART_MAIN);
    lv_obj_set_style_pad_hor(badge, 8, LV_PART_MAIN);
    lv_obj_set_style_pad_ver(badge, 2, LV_PART_MAIN);

    /* Spacer */
    lv_obj_t *sp1 = lv_obj_create(cont);
    lv_obj_set_size(sp1, LV_PCT(100), 6);
    lv_obj_set_style_bg_opa(sp1, LV_OPA_TRANSP, LV_PART_MAIN);
    lv_obj_set_style_border_width(sp1, 0, LV_PART_MAIN);

    /* Timer display */
    lv_obj_t *timer_box = lv_obj_create(cont);
    lv_obj_set_size(timer_box, 220, 120);
    lv_obj_set_style_bg_color(timer_box, lv_color_hex(0x0f3460), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(timer_box, LV_OPA_COVER, LV_PART_MAIN);
    lv_obj_set_style_radius(timer_box, 16, LV_PART_MAIN);
    lv_obj_set_style_border_width(timer_box, 0, LV_PART_MAIN);
    lv_obj_set_flex_flow(timer_box, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(timer_box, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_set_style_pad_row(timer_box, 4, LV_PART_MAIN);

    label_timer = lv_label_create(timer_box);
    lv_label_set_text(label_timer, "01:28:46");
    lv_obj_set_style_text_color(label_timer, lv_color_hex(0x00FF88), LV_PART_MAIN);
    lv_obj_set_style_text_font(label_timer, &lv_font_montserrat_36, LV_PART_MAIN);

    label_timer_sub = lv_label_create(timer_box);
    lv_label_set_text(label_timer_sub, "本次专注时长");
    lv_obj_set_style_text_color(label_timer_sub, lv_color_hex(0xAAAAAA), LV_PART_MAIN);
    lv_obj_set_style_text_font(label_timer_sub, &lv_font_montserrat_14, LV_PART_MAIN);

    /* Daily stats */
    label_daily = lv_label_create(cont);
    lv_label_set_text(label_daily, "今日累计：4h12m  休息：45m");
    lv_obj_set_style_text_color(label_daily, lv_color_hex(0xCCCCCC), LV_PART_MAIN);
    lv_obj_set_style_text_font(label_daily, &lv_font_montserrat_14, LV_PART_MAIN);
    lv_obj_set_style_text_align(label_daily, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN);

    /* Activity checkboxes */
    lv_obj_t *row_cb = lv_obj_create(cont);
    lv_obj_set_size(row_cb, LV_PCT(100), LV_SIZE_CONTENT);
    lv_obj_set_style_bg_opa(row_cb, LV_OPA_TRANSP, LV_PART_MAIN);
    lv_obj_set_style_border_width(row_cb, 0, LV_PART_MAIN);
    lv_obj_set_style_pad_all(row_cb, 0, LV_PART_MAIN);
    lv_obj_set_flex_flow(row_cb, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(row_cb, LV_FLEX_ALIGN_SPACE_EVENLY, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

    cb_study = lv_checkbox_create(row_cb);
    lv_checkbox_set_text(cb_study, "学习");
    lv_obj_set_style_text_color(cb_study, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
    lv_obj_add_event_cb(cb_study, checkbox_event_cb, LV_EVENT_VALUE_CHANGED, (void *)"学习");

    cb_read = lv_checkbox_create(row_cb);
    lv_checkbox_set_text(cb_read, "看书");
    lv_obj_set_style_text_color(cb_read, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
    lv_obj_add_event_cb(cb_read, checkbox_event_cb, LV_EVENT_VALUE_CHANGED, (void *)"看书");

    cb_phone = lv_checkbox_create(row_cb);
    lv_checkbox_set_text(cb_phone, "手机");
    lv_obj_set_style_text_color(cb_phone, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
    lv_obj_add_event_cb(cb_phone, checkbox_event_cb, LV_EVENT_VALUE_CHANGED, (void *)"手机");

    cb_rest = lv_checkbox_create(row_cb);
    lv_checkbox_set_text(cb_rest, "休息");
    lv_obj_set_style_text_color(cb_rest, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
    lv_obj_add_event_cb(cb_rest, checkbox_event_cb, LV_EVENT_VALUE_CHANGED, (void *)"休息");

    /* Divider */
    lv_obj_t *div = lv_obj_create(cont);
    lv_obj_set_size(div, LV_PCT(90), 2);
    lv_obj_set_style_bg_color(div, lv_color_hex(0x333355), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(div, LV_OPA_COVER, LV_PART_MAIN);
    lv_obj_set_style_border_width(div, 0, LV_PART_MAIN);

    /* Brightness controls */
    lv_obj_t *row_light = lv_obj_create(cont);
    lv_obj_set_size(row_light, LV_PCT(100), LV_SIZE_CONTENT);
    lv_obj_set_style_bg_opa(row_light, LV_OPA_TRANSP, LV_PART_MAIN);
    lv_obj_set_style_border_width(row_light, 0, LV_PART_MAIN);
    lv_obj_set_style_pad_all(row_light, 0, LV_PART_MAIN);
    lv_obj_set_flex_flow(row_light, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(row_light, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_set_style_pad_row(row_light, 6, LV_PART_MAIN);

    label_light_status = lv_label_create(row_light);
    lv_label_set_text(label_light_status, "○关灯  ○调暗  ●调亮");
    lv_obj_set_style_text_color(label_light_status, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
    lv_obj_set_style_text_font(label_light_status, &lv_font_montserrat_16, LV_PART_MAIN);

    slider_brightness = lv_slider_create(row_light);
    lv_obj_set_size(slider_brightness, LV_PCT(80), 8);
    lv_slider_set_range(slider_brightness, 0, 2);
    lv_slider_set_value(slider_brightness, 2, LV_ANIM_OFF);
    lv_obj_set_style_bg_color(slider_brightness, lv_color_hex(0x333355), LV_PART_MAIN);
    lv_obj_set_style_bg_color(slider_brightness, lv_color_hex(0x00FF88), LV_PART_INDICATOR);
    lv_obj_add_event_cb(slider_brightness, brightness_event_cb, LV_EVENT_VALUE_CHANGED, NULL);

    lv_scr_load(screen);
    return screen;
}

/* ====== API: Update timer ====== */
void ui_focus_timer_update_time(uint8_t h, uint8_t m, uint8_t s)
{
    if (label_timer) {
        char buf[16];
        snprintf(buf, sizeof(buf), "%02d:%02d:%02d", h, m, s);
        lv_label_set_text(label_timer, buf);
    }
}

/* ====== API: Update status ====== */
void ui_focus_timer_set_status(const char *status)
{
    if (label_status) {
        char buf[64];
        snprintf(buf, sizeof(buf), "当前状态：%s", status);
        lv_label_set_text(label_status, buf);
    }
}

/* ====== API: Update daily stats ====== */
void ui_focus_timer_set_daily_stats(uint8_t fh, uint8_t fm, uint8_t rh, uint8_t rm)
{
    if (label_daily) {
        char buf[64];
        snprintf(buf, sizeof(buf), "今日累计：%dh%02dm  休息：%dh%02dm", fh, fm, rh, rm);
        lv_label_set_text(label_daily, buf);
    }
}
