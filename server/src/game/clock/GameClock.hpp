/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** GameClock
*/

#pragma once
#include <chrono>

namespace Game
{
    /**
     * @brief A simple game clock to measure elapsed time.
     */
    class GameClock {
      public:
        /**
         * @brief Construct a new Game Clock object and initializes the last time point.
         */
        GameClock();

        /**
         * @brief Restart the clock and return the elapsed time in seconds since the last restart.
         *
         * @return double Elapsed time in seconds.
         */
        double restart();

        /**
         * @brief Get the elapsed time in seconds since the last restart without resetting the clock.
         *
         * @return double Elapsed time in seconds.
         */
        double elapsed() const;

      private:
        std::chrono::steady_clock::time_point _last; ///> The last recorded time point.
    };

} // namespace Game