#include <out.h>
#include <stddef.h>
#include <memory.h>
#include <hal/ports.h>

standard_output *current_std = NULL;
__STAPV2_port_t __STRAPV2_INTERNAL_out_cmd_port = (__STAPV2_port_t) {
  .address = 0x3D4,
  .flags = {
    .write = true
  },
  .unit = STRAPV2_PORT_BYTE
};
__STAPV2_port_t __STRAPV2_INTERNAL_out_dat_port = (__STAPV2_port_t) {
  .address = 0x3D5,
  .flags = {
    .write = true
  },
  .unit = STRAPV2_PORT_BYTE
};

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
    if (std->y == 0) return;
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
void __STRAPV2_INTERNAL_tab(standard_output *)
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

void __STRAPV2_clear_std(standard_output *std)
{
  console_char_t dummy = {
    .attrib = {
      .fg = std->fg,
      .bg = std->bg
    },
    .data = '\0'
  };

  memsetw(
    (uint16_t *)std->framebuffer,
    *(uint16_t*)&dummy,
    std->max_x * std->max_y
  );

  std->x = 0, std->y = 0;
}
void __STRAPV2_clear() { __STRAPV2_clear_std(current_std); }

void __STRAPV2_clear_fg_std(
  standard_output *std,
  console_color_t fg
)
{
  std->fg = fg;
  __STRAPV2_clear_std(std);
}
void __STRAPV2_clear_fg(console_color_t fg)
{
  current_std->fg = fg;
  __STRAPV2_clear();
}

void __STRAPV2_clear_bg_std(
  standard_output *std,
  console_color_t bg
)
{
  std->bg = bg;
  __STRAPV2_clear_std(std);
}
void __STRAPV2_clear_bg(console_color_t bg)
{
  current_std->bg = bg;
  __STRAPV2_clear();
}

void __STRAPV2_clear_colors_std(
  standard_output *std,
  console_color_t fg,
  console_color_t bg
)
{
  std->fg = fg;
  std->bg = bg;
  __STRAPV2_clear_std(std);
}
void __STRAPV2_clear_colors(
  console_color_t fg,
  console_color_t bg
)
{
  current_std->fg = fg;
  current_std->bg = bg;
  __STRAPV2_clear_std(current_std);
}

void __STRAPV2_render()
{
  uint16_t offset = current_std->x + (current_std->y*current_std->max_x);

  __STRAPV2_port_write(
    __STRAPV2_INTERNAL_out_cmd_port,
    0x0f
  );
  __STRAPV2_port_write(
    __STRAPV2_INTERNAL_out_dat_port,
    offset & 0xFF
  );

  __STRAPV2_port_write(
    __STRAPV2_INTERNAL_out_cmd_port,
    0x0e
  );
  __STRAPV2_port_write(
    __STRAPV2_INTERNAL_out_dat_port,
    (offset >> 8) & 0xFF
  );

  memcpy(
    (void *)0xb8000,
    current_std->framebuffer,
    (current_std->max_x * current_std->max_y)*2
  );
}

void __STRAPV2_init_output_system()
{
  __STRAPV2_port_write(
    __STRAPV2_INTERNAL_out_cmd_port,
    0x09
  );
  __STRAPV2_port_write(
    __STRAPV2_INTERNAL_out_dat_port,
    0x0f
  );

  __STRAPV2_port_write(
    __STRAPV2_INTERNAL_out_cmd_port,
    0x0b
  );
  __STRAPV2_port_write(
    __STRAPV2_INTERNAL_out_dat_port,
    0x0f
  );

  __STRAPV2_port_write(
    __STRAPV2_INTERNAL_out_cmd_port,
    0x0a
  );
  __STRAPV2_port_write(
    __STRAPV2_INTERNAL_out_dat_port,
    0x0e
  );
}