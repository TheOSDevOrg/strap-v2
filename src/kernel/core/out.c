#include <out.h>
#include <stddef.h>
#include <memory.h>

standard_output *current_std = NULL;

int __STRAPV2_INTERNAL_get_std_ind(standard_output *std)
{
  int index = 0;
  while (std->framebuffer[index].data != '\0') index++;
  return index;
}
int __STRAPV2_INTERNAL_get_offset(
  uint16_t x,
  uint16_t y,
  uint16_t max_x
)
{ return x+(y*max_x); }

void __STRAPV2_INTERNAL_move_up(standard_output *std)
{
  for (int y = 1; y < std->max_y; y++)
    memcpy(
      (void *)&std->framebuffer[(y-1)*std->max_x],
      (void *)&std->framebuffer[y*std->max_x],
      std->max_x*sizeof(console_char_t)
    );

  console_char_t dummy = (console_char_t){
    .attrib = {
      .fg = std->fg,
      .bg = std->bg
    },
    .data = '\0'
  };

  std->y = std->max_y-1;
  std->x = 0;

  memsetw(
    (uint16_t *)&std->framebuffer[__STRAPV2_INTERNAL_get_offset(
      std->x,
      std->y,
      std->max_x
    )],
    *((uint16_t*)&dummy),
    sizeof(console_char_t)*std->max_x
  );
}
void __STRAPV2_INTERNAL_new_line(standard_output *std)
{
  std->x = 0, std->y++;
  if (__STRAPV2_INTERNAL_get_offset(
    std->x, std->y,
    std->max_x
  ) >= std->max_x * std->max_y) __STRAPV2_INTERNAL_move_up(std);
}
void __STRAPV2_INTERNAL_backspace(standard_output *std)
{
  if (std->x == 0)
  {
    std->y--;
    for (; std->x < std->max_x; std->x++)
      if (std->framebuffer[__STRAPV2_INTERNAL_get_offset(
        std->x, std->y,
        std->max_x
      )].data == '\0') break;
  }
  else std->x--;

  console_char_t c = {
    .attrib = {
      .fg = std->fg,
      .bg = std->bg
    },
    .data = '\0'
  };
  std->framebuffer[__STRAPV2_INTERNAL_get_offset(
    std->x, std->y,
    std->max_x
  )] = c;
}
void __STRAPV2_INTERNAL_tab(standard_output *std)
{

}

void __STRAPV2_INTERNAL_parse_special_char(
  standard_output *std,
  char c
)
{
  switch (c)
  {
  case '\n':
  case '\r':
    __STRAPV2_INTERNAL_new_line(std);
    break;
  case '\b':
    __STRAPV2_INTERNAL_backspace(std);
    break;
  case '\t':
    __STRAPV2_INTERNAL_tab(std);
    break;
  default:
    break;
  }
}

void __STRAPV2_switch_std(standard_output *std) { current_std = std; }

void __STRAPV2_puts_std(standard_output *std, char *str)
{
  while (*str) __STRAPV2_putc_std(std, *str++);
}
void __STRAPV2_puts(char *str)
{ __STRAPV2_print_std(current_std, str); }

void __STRAPV2_print_std(standard_output *std, char *text)
{ __STRAPV2_puts_std(std, text); }
void __STRAPV2_print(char *text)
{ __STRAPV2_puts_std(current_std, text); }

void __STRAPV2_putc_std(standard_output *std, char c)
{
  if (c < ' ' || c > '~') return __STRAPV2_INTERNAL_parse_special_char(
    std,
    c
  );

  int offset = __STRAPV2_INTERNAL_get_offset(
    std->x,
    std->y,
    std->max_x
  );

  std->framebuffer[offset].data = c;
  std->framebuffer[offset].attrib.fg = std->fg;
  std->framebuffer[offset].attrib.bg = std->bg;

  std->x++;
  if (std->x > std->max_x) __STRAPV2_INTERNAL_new_line(std);
}
/**
 * @brief put a character on the current std
 * @param c the character
*/
void __STRAPV2_putc(char c)
{ __STRAPV2_putc_std(current_std, c); }

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
void __STRAPV2_render()
{
  memcpy(
    (void *)0xb8000,
    current_std->framebuffer,
    (current_std->max_x * current_std->max_y)*2
  );
}