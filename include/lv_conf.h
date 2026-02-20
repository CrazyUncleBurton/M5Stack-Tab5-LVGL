#pragma once

/* Keep this minimal. LVGL defaults are fine for a first bring-up. */
#define LV_COLOR_DEPTH 16
#define LV_COLOR_16_SWAP 0
#define LV_ANTIALIAS 0

/* Optional: reduce memory a bit */
#define LV_MEM_SIZE (128U * 1024U)

/* Fonts used by the demo UI */
#define LV_FONT_MONTSERRAT_20 1
#define LV_FONT_MONTSERRAT_24 1
#define LV_FONT_MONTSERRAT_28 1
#define LV_FONT_MONTSERRAT_32 1

/* Optional: enable logs while debugging */
#define LV_USE_LOG 1
#if LV_USE_LOG
  #define LV_LOG_LEVEL LV_LOG_LEVEL_WARN
#endif