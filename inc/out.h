/*
  This driver manages the output to a specific buffer
*/
#pragma once
#include <stdint.h>
#include <decl.h>

typedef enum
{
  STRAPV2_CONSOLECOLOR_black,
  STRAPV2_CONSOLECOLOR_blue,
  STRAPV2_CONSOLECOLOR_green,
  STRAPV2_CONSOLECOLOR_cyan,
  STRAPV2_CONSOLECOLOR_red,
  STRAPV2_CONSOLECOLOR_magenta,
  STRAPV2_CONSOLECOLOR_brown,
  STRAPV2_CONSOLECOLOR_light_gray,
  STRAPV2_CONSOLECOLOR_dark_gray,
  STRAPV2_CONSOLECOLOR_light_blue,
  STRAPV2_CONSOLECOLOR_light_green,
  STRAPV2_CONSOLECOLOR_light_cyan,
  STRAPV2_CONSOLECOLOR_light_red,
  STRAPV2_CONSOLECOLOR_light_magenta,
  STRAPV2_CONSOLECOLOR_yellow,
  STRAPV2_CONSOLECOLOR_white
} console_color_t;

typedef struct
{
  console_color_t fg : 4;
  console_color_t bg : 4;
} packed__ attribute_t;
typedef struct
{
  char data;
  attribute_t attrib;
} packed__ console_char_t;

typedef struct
{
  console_char_t *framebuffer;
  uint16_t x, y;
  uint16_t max_x, max_y;
  console_color_t fg, bg;
} standard_output;

extern standard_output *current_std;

/**
 * @brief change current std
 * @param std std to set as current
*/
void __STRAPV2_switch_std(standard_output *std);

/**
 * @brief put a string to a specific std
 * @param std the std to write to
 * @param str the string to put
*/
void __STRAPV2_puts_std(standard_output *std, char *str);
/**
 * @brief put a string to the current std
 * @param str the string to put
*/
void __STRAPV2_puts(char *str);
/**
 * @brief print to a specific std
 * @param std the std to write to
 * @param text the message to write
*/
void __STRAPV2_print_std(standard_output *std, char *text);
/**
 * @brief print to the current std
 * @param text the message to write
*/
void __STRAPV2_print(char *text);

/**
 * @brief put a character on a specific std
 * @param std the std to put the char to
 * @param c the character
*/
void __STRAPV2_putc_std(standard_output *std, char c);
/**
 * @brief put a character on the current std
 * @param c the character
*/
void __STRAPV2_putc(char c);

/**
 * @brief clear a specific std
 * @param std the std to clear
*/
void __STRAPV2_clear_std(standard_output *std);
/**
 * @brief clear the current std
*/
void __STRAPV2_clear();

/**
 * @brief clear a specific std with some foreground color
 * @param std the std to clear
 * @param fg the foreground color to apply
*/
void __STRAPV2_clear_fg_std(standard_output *std, console_color_t fg);
/**
 * @brief clear the current std with some foreground color
 * @param fg the foreground color to apply
*/
void __STRAPV2_clear_fg(console_color_t fg);

/**
 * @brief clear a specific std with some background color
 * @param std the std to clear
 * @param bg the background color to apply
*/
void __STRAPV2_clear_bg_std(standard_output *std, console_color_t bg);
/**
 * @brief clear the current std with some background color
 * @param bg the background color to apply
*/
void __STRAPV2_clear_bg(console_color_t bg);

/**
 * @brief clear a specific std with some background and foreground colors
 * @param std the std to clear
 * @param fg the foreground color to apply
 * @param bg the background color to apply
*/
void __STRAPV2_clear_colors_std(standard_output *std, console_color_t fg, console_color_t bg);
/**
 * @brief clear the current std with some background and foreground colors
 * @param fg the foreground color to apply
 * @param bg the background color to apply
*/
void __STRAPV2_clear_colors(console_color_t fg, console_color_t bg);

/**
 * @brief render the current std
*/
void __STRAPV2_render();

