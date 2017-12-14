/* 
 * File:   unitTests.cpp
 * Author: massimo
 *
 * Created on October 04, 2017, 10:43 AM
 */

#include "../iocolor.h"
#include <chrono>
#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <experimental/string_view>

using namespace ::testing;
using namespace std::experimental;
using namespace std::string_literals;
////////////////////////////////////////////////////////////////////////////////
// a simple function's performance timer
template <typename Time = std::chrono::nanoseconds,
          typename Clock = std::chrono::high_resolution_clock>
struct perftimer
{
  template <typename F, typename... Args>
  static inline Time duration(F&& f, Args&&... args)
  {
    auto start = Clock::now();
    // C++17: not yet available
    //std::invoke(std::forward<F>(f), std::forward<Args>(args)...);
    f(std::forward<Args>(args)...);
    auto end = Clock::now();

    return std::chrono::duration_cast<Time>(end - start);
  }
};

// BEGIN: ignore the warnings listed below when compiled with clang from here
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wexit-time-destructors"
#pragma clang diagnostic ignored "-Wglobal-constructors"
////////////////////////////////////////////////////////////////////////////////
static inline
void testEffects(const enum iocolor::color foreGroundColor,
                 const enum iocolor::color backGroundColor) noexcept
{
  std::cout << iocolor::color::reset
            << iocolor::foreground(foreGroundColor)
            << iocolor::background(backGroundColor)
            << " REGULAR "
            << iocolor::color::reset
            << iocolor::foreground(foreGroundColor)
            << iocolor::background(backGroundColor)
            << iocolor::effects(iocolor::effect::bold)
            << " BOLD "
            << iocolor::color::reset
            << iocolor::foreground(foreGroundColor)
            << iocolor::background(backGroundColor)
            << iocolor::effects(iocolor::effect::underline)
            << " UNDERLINE "
            << iocolor::color::reset
            << iocolor::foreground(foreGroundColor)
            << iocolor::background(backGroundColor)
            << iocolor::effects(iocolor::effect::reverse)
            << " REVERSE "
            << iocolor::color::reset
            << iocolor::foreground(foreGroundColor)
            << iocolor::background(backGroundColor)
            << iocolor::effects(iocolor::effect::strike)
            << " STRIKE "
            << iocolor::color::reset
            << '\n';
}

TEST(ioColor, test_full_1)
{
  const std::vector<iocolor::color> foreGroundVector
  {
    iocolor::color::black,
    iocolor::color::red,
    iocolor::color::green,
    iocolor::color::yellow,
    iocolor::color::blue,
    iocolor::color::magenta,
    iocolor::color::cyan,
    iocolor::color::white,
  };
  const std::vector<iocolor::color> backGroundVector
  {
    iocolor::color::black,
    iocolor::color::red,
    iocolor::color::green,
    iocolor::color::yellow,
    iocolor::color::blue,
    iocolor::color::magenta,
    iocolor::color::cyan,
    iocolor::color::white,
  };
  const std::vector<iocolor::effect> effectVector
  {
    iocolor::effect::none,
    iocolor::effect::bold,
    iocolor::effect::underline,
    iocolor::effect::reverse,
    iocolor::effect::strike,
  };

  auto f1 = [&backGroundVector, &effectVector]
            (const iocolor::color& foreGroundColor) noexcept
            {
              auto f2 = [&effectVector, &foreGroundColor]
                        (const iocolor::color& backGroundColor) noexcept
                        {
                          auto f3 = [&foreGroundColor, &backGroundColor]
                                    (const iocolor::effect& effect) noexcept
                                    {
                                      static const string_view s {" ABCDEFGHabcdefgh012345 "};
                                      std::cout << iocolor::foreground(foreGroundColor)
                                                << iocolor::background(backGroundColor)
                                                << iocolor::effects(effect)
                                                << s
                                                << iocolor::color::reset;
                                    };
                                    std::for_each(std::begin(effectVector),
                                                  std::end(effectVector),
                                                  f3);
                                    std::cout << '\n';
                        };
              std::for_each(std::begin(backGroundVector), std::end(backGroundVector), f2);
            };
  auto for_eachFun = [&f1, &foreGroundVector]()
  {
    std::for_each(std::begin(foreGroundVector), std::end(foreGroundVector), f1);
  };

  auto for_eachTime = perftimer<>::duration(for_eachFun).count();

  std::cout << '\n\n';
  
  // for_eachFun does the same of the following forLoopFun, but faster
  auto forLoopFun = [&foreGroundVector, &backGroundVector, &effectVector]()
  {
    static const string_view s {" ABCDEFGHabcdefgh012345 "};

    for (auto&& fgc : foreGroundVector)
    {
      for (auto&& bgc : backGroundVector)
      {
        for (auto&& eff: effectVector)
        {
          std::cout << iocolor::foreground(fgc)
                    << iocolor::background(bgc)
                    << iocolor::effects(eff)
                    << s
                    << iocolor::color::reset;
        }
        std::cout << '\n';
      }
    }
  };

  auto forLoopTime = perftimer<>::duration(forLoopFun).count();

  std::cout << "for_each took: " << for_eachTime << " nsec" << '\n';
  std::cout << "for loop took: " << forLoopTime << " nsec" << '\n';
  // check that for_each is faster than the manual for loops
  EXPECT_LT(for_eachTime, forLoopTime);
}

TEST(ioColor, test_full_2)
{
  const std::vector<iocolor::color> foreGroundColorVector
  {
    iocolor::color::black,
    iocolor::color::red,
    iocolor::color::green,
    iocolor::color::yellow,
    iocolor::color::blue,
    iocolor::color::magenta,
    iocolor::color::cyan,
    iocolor::color::white,
  };
  const std::vector<iocolor::color> backGroundColorVector
  {
    iocolor::color::black,
    iocolor::color::red,
    iocolor::color::green,
    iocolor::color::yellow,
    iocolor::color::blue,
    iocolor::color::magenta,
    iocolor::color::cyan,
    iocolor::color::white,
  };
  
  auto f1 = [&backGroundColorVector]
            (const iocolor::color& foreGroundColor) noexcept
            {
              auto f2 = [&foreGroundColor]
                        (const iocolor::color& backGroundColor) noexcept
                        {
                          testEffects(foreGroundColor, backGroundColor);  
                        };
              std::for_each(std::begin(backGroundColorVector),
                            std::end(backGroundColorVector),
                            f2);
            };
  auto for_eachFun = [&f1, &foreGroundColorVector]()
  {
    std::for_each(std::begin(foreGroundColorVector), std::end(foreGroundColorVector), f1);
  };

  auto for_eachTime = perftimer<>::duration(for_eachFun).count();

  std::cout << '\n\n';

  // for_eachFun does the same of the following forLoopFun
  auto forLoopFun = [&foreGroundColorVector, &backGroundColorVector]()
  {
    for (auto&& foreGroundColor : foreGroundColorVector)
    {
      for (auto&& backGroundColor : backGroundColorVector)
      {
        testEffects(foreGroundColor, backGroundColor);
      }
    }
  };

  auto forLoopTime = perftimer<>::duration(forLoopFun).count();

  // on my pc forLoopTime is much better than for_eachTime
  std::cout << "for_each took: " << for_eachTime << " nsec" << '\n';
  std::cout << "for loop took: " << forLoopTime << " nsec" << '\n';
  EXPECT_GT(for_eachTime, forLoopTime);
}

TEST(ioColor, test_full_3)
{
  std::cout << iocolor::color::reset
            << iocolor::foreground(iocolor::color::black)
            << iocolor::background(iocolor::color::black)
            << "          "
            << iocolor::foreground(iocolor::color::red)
            << iocolor::background(iocolor::color::red)
            << "          "
            << iocolor::foreground(iocolor::color::green)
            << iocolor::background(iocolor::color::green)
            << "          "
            << iocolor::foreground(iocolor::color::yellow)
            << iocolor::background(iocolor::color::yellow)
            << "          "
            << iocolor::foreground(iocolor::color::blue)
            << iocolor::background(iocolor::color::blue)
            << "          "
            << iocolor::foreground(iocolor::color::magenta)
            << iocolor::background(iocolor::color::magenta)
            << "          "
            << iocolor::foreground(iocolor::color::cyan)
            << iocolor::background(iocolor::color::cyan)
            << "          "
            << iocolor::foreground(iocolor::color::white)
            << iocolor::background(iocolor::color::white)
            << "          "
            << iocolor::color::reset
            << '\n';
}

static
const std::string s = " ABCDEFGHIJKLMNOPQRSTUVWXYZ-0123456789-_\\|!\"£$%&/()=?^{}[]@#,.;: "s;

TEST(ioColor, testMakeColor_1)
{
  short color {};

  for (color = 0; color <= 255; ++color)
  {
    std::cout << iocolor::color::reset
              << iocolor::make_color(color)
              << "fg "
              << color
              << s
              << iocolor::color::reset
              << '\n';
  }
}

TEST(ioColor, testMakeColor_2)
{
  short foreGroundColor {};

  for (foreGroundColor = 0; foreGroundColor <= 255; ++foreGroundColor)
  {
    std::cout << iocolor::color::reset
              << iocolor::foreground(foreGroundColor)
              << "fg "
              << foreGroundColor
              << s
              << iocolor::color::reset
              << '\n';
  }
}

TEST(ioColor, testMakeColor_3)
{
  short backGroundColor {};

  for (backGroundColor = 0; backGroundColor <= 255; ++backGroundColor)
  {
    std::cout << iocolor::color::reset
              << iocolor::background(backGroundColor)
              << "bg "
              << backGroundColor
              << s
              << iocolor::color::reset
              << '\n';
  }
}

// very long output: commented out
// uncomment if wishing to run it
//TEST(ioColor, testMakeColor_4)
//{
//  short foreGroundColor {};
//  short backGroundColor {};
//
//  for (foreGroundColor = 0; foreGroundColor <= 255; ++foreGroundColor)
//  {
//    for (backGroundColor = 0; backGroundColor <= 255; ++backGroundColor)
//    {
//      std::cout << iocolor::color::reset
//                << iocolor::foreground(foreGroundColor)
//                << iocolor::background(backGroundColor)
//                << "fg "
//                << foreGroundColor
//                << " bg "
//                << backGroundColor
//                << s
//                << iocolor::color::reset
//                << '\n';
//    }
//  }
//}
////////////////////////////////////////////////////////////////////////////////
#pragma clang diagnostic pop
// END: ignore the warnings when compiled with clang up to here
