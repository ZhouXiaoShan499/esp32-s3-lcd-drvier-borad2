#include "ui_device_settings.h"
#include "lvgl.h"
#include <stdio.h>

extern const lv_font_t lv_font_simsun_16_cjk;

static lv_obj_t *screen = NULL;
static lv_obj_t *prev_scr = NULL;
static lv_obj_t *label_light_status = NULL;
static lv_obj_t *slider_brightness = NULL;
static int current_brightness = 2;

static void brightness_event_cb(lv_event_t *e)
{
    lv_obj_t *slider = lv_event_get_target(e);
    int val = (int)lv_slider_get_value(slider);
    current_brightness = val;
    char display[64];
    const char *off = (val == 0) ? "●关灯" : "○关灯";
    const char *dim = (val == 1) ? "●调暗" : "○调暗";
    const char *bri = (val == 2) ? "●调亮" : "○调亮";
    snprintf(display, sizeof(display), "%s  %s  %s", off, dim, bri);
    lv_label_set_text(label_light_status, display);
}

static lv_obj_t *create_menu_item(lv_obj_t *parent, const char *text)
{
    lv_obj_t *item = lv_btn_create(parent);
    lv_obj_set_size(item, LV_PCT(95), 44);
    lv_obj_set_style_bg_color(item, lv_color_hex(0x1a2744), LV_PART_MAIN);
    lv_obj_set_style_bg_color(item, lv_color_hex(0x0f3460), LV_STATE_PRESSED);
    lv_obj_set_style_radius(item, 10, LV_PART_MAIN);
    lv_obj_set_style_border_width(item, 0, LV_PART_MAIN);
    lv_obj_set_style_pad_left(item, 12, LV_PART_MAIN);
    lv_obj_t *label = lv_label_create(item);
    lv_label_set_text(label, text);
    lv_obj_set_style_text_color(label, lv_color_hex(0xCCCCCC), LV_PART_MAIN);
    lv_obj_set_style_text_font(label, &lv_font_simsun_16_cjk, LV_PART_MAIN);
    lv_obj_center(label);
    return item;
}

static void menu_click_cb(lv_event_t *e)
{
    lv_obj_t *target = lv_event_get_target(e);
    lv_obj_t *label = lv_obj_get_child(target, 0);
    const char *text = lv_label_get_text(label);
    LV_LOG_USER("Menu clicked: %s", text);
}

static void back_btn_cb(lv_event_t *e)
{
    if (prev_scr) {
        lv_scr_load(prev_scr);
    }
}

lv_obj_t *ui_device_settings_create(lv_obj_t *previous_screen)
{
    prev_scr = previous_screen;
    screen = lv_obj_create(NULL);
    lv_obj_set_style_bg_color(screen, lv_color_hex(0x1a1a2e), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(screen, LV_OPA_COVER, LV_PART_MAIN);

    lv_obj_t *cont = lv_obj_create(screen);
    lv_obj_set_size(cont, 400, 420);
    lv_obj_align(cont, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_bg_color(cont, lv_color_hex(0x16213e), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(cont, LV_OPA_COVER, LV_PART_MAIN);
    lv_obj_set_style_radius(cont, 20, LV_PART_MAIN);
    lv_obj_set_style_border_width(cont, 0, LV_PART_MAIN);
    lv_obj_set_style_pad_all(cont, 16, LV_PART_MAIN);
    lv_obj_set_flex_flow(cont, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(cont, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_set_style_pad_row(cont, 8, LV_PART_MAIN);

    /* Title bar */
    lv_obj_t *title_row = lv_obj_create(cont);
    lv_obj_set_size(title_row, LV_PCT(100), LV_SIZE_CONTENT);
    lv_obj_set_style_bg_opa(title_row, LV_OPA_TRANSP, LV_PART_MAIN);
    lv_obj_set_style_border_width(title_row, 0, LV_PART_MAIN);
    lv_obj_set_style_pad_all(title_row, 0, LV_PART_MAIN);

    lv_obj_t *back_btn = lv_btn_create(title_row);
    lv_obj_set_size(back_btn, 28, 28);
    lv_obj_align(back_btn, LV_ALIGN_LEFT_MID, 0, 0);
    lv_obj_set_style_bg_color(back_btn, lv_color_hex(0x1a2744), LV_PART_MAIN);
    lv_obj_set_style_radius(back_btn, 6, LV_PART_MAIN);
    lv_obj_set_style_border_width(back_btn, 0, LV_PART_MAIN);
    lv_obj_add_event_cb(back_btn, back_btn_cb, LV_EVENT_CLICKED, NULL);
    lv_obj_t *back_label = lv_label_create(back_btn);
    lv_label_set_text(back_label, LV_SYMBOL_LEFT);
    lv_obj_set_style_text_color(back_label, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
    lv_obj_center(back_label);

    lv_obj_t *title = lv_label_create(title_row);
    lv_label_set_text(title, "设备设置");
    lv_obj_set_style_text_color(title, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
    lv_obj_set_style_text_font(title, &lv_font_simsun_16_cjk, LV_PART_MAIN);
    lv_obj_align(title, LV_ALIGN_CENTER, 0, 0);

    /* Divider */
    lv_obj_t *sp1 = lv_obj_create(cont);
    lv_obj_set_size(sp1, LV_PCT(100), 4);
    lv_obj_set_style_bg_opa(sp1, LV_OPA_TRANSP, LV_PART_MAIN);
    lv_obj_set_style_border_width(sp1, 0, LV_PART_MAIN);

    lv_obj_t *div_top = lv_obj_create(cont);
    lv_obj_set_size(div_top, LV_PCT(90), 1);
    lv_obj_set_style_bg_color(div_top, lv_color_hex(0x333355), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(div_top, LV_OPA_COVER, LV_PART_MAIN);
    lv_obj_set_style_border_width(div_top, 0, LV_PART_MAIN);



    /* Menu items */
    lv_obj_t *item1 = create_menu_item(cont, "▶ 自定义行为标签（新增/删除）");
    lv_obj_add_event_cb(item1, menu_click_cb, LV_EVENT_CLICKED, NULL);
    lv_obj_t *item2 = create_menu_item(cont, "▶ 自动识别传感器开关");
    lv_obj_add_event_cb(item2, menu_click_cb, LV_EVENT_CLICKED, NULL);
    lv_obj_t *item3 = create_menu_item(cont, "▶ 屏幕亮度调节");
    lv_obj_add_event_cb(item3, menu_click_cb, LV_EVENT_CLICKED, NULL);
    lv_obj_t *item4 = create_menu_item(cont, "▶ 本地数据导出");
    lv_obj_add_event_cb(item4, menu_click_cb, LV_EVENT_CLICKED, NULL);
    lv_obj_t *item5 = create_menu_item(cont, "▶ 设备恢复出厂");
    lv_obj_add_event_cb(item5, menu_click_cb, LV_EVENT_CLICKED, NULL);

    /* Spacer to push brightness to bottom */
    lv_obj_t *sp_fill = lv_obj_create(cont);
    lv_obj_set_size(sp_fill, LV_PCT(100), 1);
    lv_obj_set_style_bg_opa(sp_fill, LV_OPA_TRANSP, LV_PART_MAIN);
    lv_obj_set_style_border_width(sp_fill, 0, LV_PART_MAIN);
    lv_obj_set_flex_grow(sp_fill, 1);

    /* Divider */
    lv_obj_t *div = lv_obj_create(cont);
    lv_obj_set_size(div, LV_PCT(90), 2);
    lv_obj_set_style_bg_color(div, lv_color_hex(0x333355), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(div, LV_OPA_COVER, LV_PART_MAIN);
    lv_obj_set_style_border_width(div, 0, LV_PART_MAIN);

    /* Brightness bar */
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
    lv_obj_set_style_text_font(label_light_status, &lv_font_simsun_16_cjk, LV_PART_MAIN);

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
