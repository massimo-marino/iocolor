/* 
 * File:   iocolor.cpp
 * Author: massimo
 *
 * Created on October 18, 2017, 4:29 PM
 */

#include "iocolor.h"
////////////////////////////////////////////////////////////////////////////////
std::ostream&
operator << (std::ostream& os, const iocolor::color_definition& descriptor) noexcept
{
  if ( !iocolor::is_tty(os) )
  {
    return os;
  }

  if (descriptor.effects != 0)
  {
    if (descriptor.effects & static_cast<iocolor::effect_t>(iocolor::effect::bold))
    {
      os << "\033[1m";
    }

    if (descriptor.effects & static_cast<iocolor::effect_t>(iocolor::effect::underline))
    {
      os << "\033[4m";
    }

    if (descriptor.effects & static_cast<iocolor::effect_t>(iocolor::effect::blink))
    {
      os << "\033[5m";
    }

    if (descriptor.effects & static_cast<iocolor::effect_t>(iocolor::effect::reverse))
    {
      os << "\033[7m";
    }

     if (descriptor.effects & static_cast<iocolor::effect_t>(iocolor::effect::concealed))
    {
      os << "\033[8m";
    }

     if (descriptor.effects & static_cast<iocolor::effect_t>(iocolor::effect::strike))
    {
      os << "\033[9m";
    }
  }

  if (descriptor.foreground != -1)
  {
    if (descriptor.foreground < 8)
    {
      os << "\033[" << descriptor.foreground + 30 << "m";
    }
    else
    {
      os << "\033[38;5;" << descriptor.foreground << "m";
    }
  }

  if (descriptor.background != -1)
  {
    if (descriptor.background < 8)
    {
      os << "\033[" << descriptor.background + 40 << "m";
    }
    else
    {
      os << "\033[48;5;" << descriptor.background << "m";
    }
  }

  return os;
}

std::ostream&
operator << (std::ostream& os, const enum iocolor::color c) noexcept
{
  if (c == iocolor::color::reset)
  {
    return os << "\033[0m";
  }

  return os << iocolor::foreground(c);
}

std::ostream&
operator << (std::ostream& os, const enum iocolor::effect ef) noexcept
{
  if (ef == iocolor::effect::none)
  {
    return os << "\033[0m";
  }

  return os << iocolor::effects(ef);
}
