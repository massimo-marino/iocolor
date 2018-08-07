/* 
 * File:   iocolor.h
 * Author: massimo
 *
 * Created on October 17, 2017, 1:50 PM
 */
#pragma once

#include <iostream>
#include <unistd.h>
////////////////////////////////////////////////////////////////////////////////
namespace iocolor
{
// chosen uint16_t for padding color_definition
using effect_t = uint16_t;

struct color_definition
{
  short    foreground;
  short    background;
  effect_t effects;
};

enum class color : short
{
  reset   = -2, // "\033[0m"
  none    = -1,

  black   = 0,  // "\033[30m"
  red     = 1,  // "\033[31m"
  green   = 2,  // "\033[32m"
  yellow  = 3,  // "\033[33m"
  blue    = 4,  // "\033[34m"
  magenta = 5,  // "\033[35m"
  cyan    = 6,  // "\033[36m"
  white   = 7,  // "\033[37m"
};

enum class effect : effect_t
{
  none      = 0,  // "\033[0m"

  bold      = 1,  // "\033[1m"
  underline = 2,  // "\033[4m"
  blink     = 4,  // "\033[5m"
  reverse   = 8,  // "\033[7m"
  concealed = 16, // "\033[8m"
  strike    = 32, // "\033[9m"
};

static
bool
is_tty (const int fd) noexcept
{
  return static_cast<bool>(isatty(fd));
}

[[maybe_unused]]
static
bool
is_tty (const std::ostream& s) noexcept
{
  if (s.rdbuf() == std::cout.rdbuf())
  {
    return is_tty(STDOUT_FILENO);
  }
  else if ( (s.rdbuf() == std::cerr.rdbuf()) ||
            (s.rdbuf() == std::clog.rdbuf()) )
  {
    return is_tty(STDERR_FILENO);
  }
  return false;
}

effect
operator| (enum effect a, enum effect b) noexcept;

effect
operator| (const enum effect a, const enum effect b) noexcept
{
  return static_cast<effect>(static_cast<effect_t>(a) | static_cast<effect_t>(b));
}

color_definition
make_color (enum color fg,
            enum color bg = color::none,
            enum effect ef = effect::none) noexcept;

color_definition
make_color (const enum color fg,
            const enum color bg,
            const enum effect ef) noexcept
{
  return { static_cast<short>(fg),
           static_cast<short>(bg),
           static_cast<effect_t>(ef) };
}

color_definition
make_color (short fg,
            short bg = -1,
            effect_t ef = 0) noexcept;

color_definition
make_color (const short fg,
            const short bg,
            const effect_t ef) noexcept
{
  return { fg, bg, ef };
}

color_definition
foreground (enum color fg) noexcept;

color_definition
foreground (const enum color fg) noexcept
{
  return make_color(fg);
}

color_definition
foreground (short fg) noexcept;

color_definition
foreground (const short fg) noexcept
{
  return make_color(fg);
}

color_definition
background (enum color bg) noexcept;

color_definition
background (const enum color bg) noexcept
{
  return make_color(color::none, bg);
}

color_definition
background (short bg) noexcept;

color_definition
background (const short bg) noexcept
{
  return make_color(-1, bg);
}

color_definition
effects (enum effect ef) noexcept;

color_definition
effects (const enum effect ef) noexcept
{
  return make_color(color::none, color::none, ef);
}

color_definition
effects (effect_t ef) noexcept;

color_definition
effects (const effect_t ef) noexcept
{
  return make_color(-1, -1, ef);
}
}  // namespace iocolor
////////////////////////////////////////////////////////////////////////////////
std::ostream&
operator<< (std::ostream& os, const iocolor::color_definition& descriptor) noexcept;

std::ostream&
operator<< (std::ostream& os, enum iocolor::color c) noexcept;

std::ostream&
operator<< (std::ostream& os, enum iocolor::effect ef) noexcept;
