/*
  CrazyUncleBurton's M5Unified + LVGL demo
  by Bryan A. "CrazyUncleBurton" Thompson
  Last Updated 02/19/2026

*/

#include <Arduino.h>
#include <M5Unified.h>
#include <lvgl.h>

static lv_color_t* buf1 = nullptr;
static lv_color_t* buf2 = nullptr;
static uint32_t draw_buf_pixels = 0;
static lv_obj_t* counter_label = nullptr;
static lv_obj_t* slider_label = nullptr;
static lv_obj_t* arc = nullptr;
static lv_obj_t* arc_value_label = nullptr;
static lv_obj_t* slider_obj = nullptr;
static int tap_count = 0;
static volatile bool frame_dirty = false;

/* LVGL -> M5GFX flush */
static void lvgl_flush_cb(lv_display_t* disp, const lv_area_t* area, uint8_t* px_map)
{
  const int32_t w = area->x2 - area->x1 + 1;
  const int32_t h = area->y2 - area->y1 + 1;

  M5.Display.startWrite();
  M5.Display.setAddrWindow(area->x1, area->y1, w, h);

  // Use blocking transfer to avoid LVGL reusing draw buffers before DMA completes.
  M5.Display.pushPixels((uint16_t*)px_map, (uint32_t)w * (uint32_t)h, false);
  M5.Display.endWrite();

  frame_dirty = true;

  lv_display_flush_ready(disp);
}

/* M5Unified Touch -> LVGL pointer device */
static void lvgl_touch_read_cb(lv_indev_t* indev, lv_indev_data_t* data)
{
  // M5.Touch is Touch_Class; getCount/getDetail are the main calls. :contentReference[oaicite:2]{index=2}
  if (M5.Touch.getCount() > 0) {
    const auto& t = M5.Touch.getDetail(0); // includes x/y and state helpers :contentReference[oaicite:3]{index=3}
    data->state = LV_INDEV_STATE_PRESSED;
    data->point.x = t.x;
    data->point.y = t.y;
  } else {
    data->state = LV_INDEV_STATE_RELEASED;
  }
}

static uint32_t last_ms = 0;

static void update_counter_label()
{
  lv_label_set_text_fmt(counter_label, "Taps: %d", tap_count);
}

static void set_demo_value(int value)
{
  if (value < 0) value = 0;
  if (value > 100) value = 100;

  lv_slider_set_value(slider_obj, value, LV_ANIM_OFF);
  lv_arc_set_value(arc, value);
  lv_label_set_text_fmt(slider_label, "Slider: %d", value);
  lv_label_set_text_fmt(arc_value_label, "%d", value);
  lv_obj_center(arc_value_label);
}

static void on_tap_button(lv_event_t* e)
{
  if (lv_event_get_code(e) == LV_EVENT_CLICKED) {
    tap_count++;
    update_counter_label();

    const int current = lv_arc_get_value(arc);
    int next = current + 10;
    if (next > 100) next = 0;
    set_demo_value(next);
  }
}

static void on_slider_change(lv_event_t* e)
{
  if (lv_event_get_code(e) == LV_EVENT_VALUE_CHANGED) {
    lv_obj_t* slider = (lv_obj_t*)lv_event_get_target(e);
    const int value = lv_slider_get_value(slider);
    set_demo_value(value);
  }
}

static void on_arc_change(lv_event_t* e)
{
  if (lv_event_get_code(e) == LV_EVENT_VALUE_CHANGED) {
    lv_obj_t* arc_obj = (lv_obj_t*)lv_event_get_target(e);
    const int value = lv_arc_get_value(arc_obj);
    set_demo_value(value);
  }
}

static bool init_draw_buffers(uint32_t scr_w, uint32_t scr_h)
{
  uint32_t buf_lines = 120;
  if (buf_lines > scr_h) {
    buf_lines = scr_h;
  }

  while (buf_lines >= 24) {
    const uint32_t buf_pixels = scr_w * buf_lines;
    const size_t buf_bytes = (size_t)buf_pixels * sizeof(lv_color_t);

    buf1 = (lv_color_t*)heap_caps_malloc(buf_bytes, MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT);
    buf2 = (lv_color_t*)heap_caps_malloc(buf_bytes, MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT);

    if (buf1 && buf2) {
      draw_buf_pixels = buf_pixels;
      return true;
    }

    if (buf1) {
      heap_caps_free(buf1);
      buf1 = nullptr;
    }
    if (buf2) {
      heap_caps_free(buf2);
      buf2 = nullptr;
    }

    buf_lines /= 2;
  }

  return false;
}

void setup()
{
  Serial.begin(115200);

  // M5Unified init
  auto cfg = M5.config();
  M5.begin(cfg);
  M5.Display.setAutoDisplay(false);

  M5.Display.setRotation(1);
  M5.Display.fillScreen(TFT_BLACK);

  // LVGL init
  lv_init();

  const uint32_t scr_w = M5.Display.width();
  const uint32_t scr_h = M5.Display.height();

  if (!init_draw_buffers(scr_w, scr_h)) {
    Serial.println("LVGL draw buffer allocation failed");
    while (true) {
      delay(1000);
    }
  }

  lv_display_t* display = lv_display_create(scr_w, scr_h);
  lv_display_set_flush_cb(display, lvgl_flush_cb);
  lv_display_set_buffers(display, buf1, buf2,
                         (uint32_t)(draw_buf_pixels * sizeof(lv_color_t)),
                         LV_DISPLAY_RENDER_MODE_PARTIAL);

  lv_indev_t* indev = lv_indev_create();
  lv_indev_set_type(indev, LV_INDEV_TYPE_POINTER);
  lv_indev_set_read_cb(indev, lvgl_touch_read_cb);

  lv_obj_set_style_bg_color(lv_scr_act(), lv_color_black(), LV_PART_MAIN);
  lv_obj_set_style_bg_opa(lv_scr_act(), LV_OPA_COVER, LV_PART_MAIN);

  const int title_top = (int)(scr_h * 0.03f);
  const int title_to_controls_gap = (int)(scr_h * 0.11f);
  const int side_margin = (int)(scr_w * 0.06f);
  const int bottom_margin = (int)(scr_h * 0.07f);
  const int button_w = (int)(scr_w * 0.16f);
  const int button_h = (int)(scr_h * 0.11f);
  const int slider_w = (int)(scr_w * 0.34f);
  const int arc_size = (int)(scr_h * 0.50f);

  lv_obj_t* title = lv_label_create(lv_scr_act());
  lv_label_set_text(title, "M5Unified + LVGL demo");
  lv_obj_set_style_text_color(title, lv_color_white(), LV_PART_MAIN);
  lv_obj_set_style_text_font(title, &lv_font_montserrat_32, LV_PART_MAIN);
  lv_obj_align(title, LV_ALIGN_TOP_MID, 0, title_top);

  lv_obj_t* btn = lv_btn_create(lv_scr_act());
  lv_obj_set_size(btn, button_w, button_h);
  lv_obj_align(btn, LV_ALIGN_TOP_LEFT, side_margin, title_to_controls_gap);
  lv_obj_add_event_cb(btn, on_tap_button, LV_EVENT_CLICKED, nullptr);

  lv_obj_t* btn_label = lv_label_create(btn);
  lv_label_set_text(btn_label, "Tap me");
  lv_obj_set_style_text_font(btn_label, &lv_font_montserrat_28, LV_PART_MAIN);
  lv_obj_center(btn_label);

  counter_label = lv_label_create(lv_scr_act());
  lv_obj_set_style_text_color(counter_label, lv_color_white(), LV_PART_MAIN);
  lv_obj_set_style_text_font(counter_label, &lv_font_montserrat_24, LV_PART_MAIN);
  lv_obj_align_to(counter_label, btn, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 14);
  update_counter_label();

  slider_obj = lv_slider_create(lv_scr_act());
  lv_obj_set_size(slider_obj, slider_w, 28);
  lv_obj_align(slider_obj, LV_ALIGN_BOTTOM_LEFT, side_margin, -bottom_margin);
  lv_slider_set_range(slider_obj, 0, 100);
  lv_slider_set_value(slider_obj, 20, LV_ANIM_OFF);
  lv_obj_add_event_cb(slider_obj, on_slider_change, LV_EVENT_VALUE_CHANGED, nullptr);

  slider_label = lv_label_create(lv_scr_act());
  lv_obj_set_style_text_color(slider_label, lv_color_white(), LV_PART_MAIN);
  lv_obj_set_style_text_font(slider_label, &lv_font_montserrat_24, LV_PART_MAIN);
  lv_label_set_text(slider_label, "Slider: 20");
  lv_obj_align_to(slider_label, slider_obj, LV_ALIGN_OUT_TOP_LEFT, 0, -18);

  lv_obj_t* hint = lv_label_create(lv_scr_act());
  lv_label_set_text(hint, "Drag slider, drag arc, or tap button (+10)");
  lv_obj_set_style_text_color(hint, lv_palette_lighten(LV_PALETTE_GREY, 2), LV_PART_MAIN);
  lv_obj_set_style_text_font(hint, &lv_font_montserrat_20, LV_PART_MAIN);
  lv_obj_align_to(hint, slider_obj, LV_ALIGN_OUT_TOP_LEFT, 0, -52);

  arc = lv_arc_create(lv_scr_act());
  lv_obj_set_size(arc, arc_size, arc_size);
  lv_obj_align(arc, LV_ALIGN_RIGHT_MID, -side_margin, 24);
  lv_arc_set_range(arc, 0, 100);
  lv_arc_set_value(arc, 20);
  lv_obj_add_event_cb(arc, on_arc_change, LV_EVENT_VALUE_CHANGED, nullptr);
  lv_obj_set_style_arc_color(arc, lv_palette_main(LV_PALETTE_BLUE), LV_PART_INDICATOR);
  lv_obj_set_style_arc_width(arc, 28, LV_PART_MAIN);
  lv_obj_set_style_arc_width(arc, 32, LV_PART_INDICATOR);

  arc_value_label = lv_label_create(arc);
  lv_obj_set_style_text_color(arc_value_label, lv_color_white(), LV_PART_MAIN);
  lv_obj_set_style_text_font(arc_value_label, &lv_font_montserrat_32, LV_PART_MAIN);
  lv_label_set_text(arc_value_label, "20");
  lv_obj_center(arc_value_label);

  last_ms = millis();
}

void loop()
{
  // Important: M5.update() keeps touch state fresh
  M5.update();

  // Feed LVGL tick
  const uint32_t now = millis();
  lv_tick_inc(now - last_ms);
  last_ms = now;

  // Let LVGL run rendering/events at ~30 Hz to reduce visible tearing
  static uint32_t last_handler_ms = 0;
  if ((now - last_handler_ms) >= 33) {
    lv_timer_handler();
    if (frame_dirty) {
      M5.Display.display();
      frame_dirty = false;
    }
    last_handler_ms = now;
  }

  delay(1);

}
