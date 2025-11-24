/*
** EPITECH PROJECT, 2025
** real_r_type
** File description:
** main
*/

#include "game/GameClient.hpp"
#include "exceptions/GameExceptions.hpp"
#include <iostream>
#include <exception>

/**
 * @brief Main entry point of the R-Type client application.
 *
 * Initializes the GameClient and starts the game loop with proper exception handling.
 *
 * @return Exit status: 0 on success, 1 on error.
 */
int main() {
    try {
        GameClient client;

        client.init();
        client.run();

        return 0;
    } catch (const InitializationException& e) {
        std::cerr << "R-Type Client Initialization Error: " << e.what() << std::endl;
        return 1;
    } catch (const RenderException& e) {
        std::cerr << "R-Type Client Rendering Error: " << e.what() << std::endl;
        return 1;
    } catch (const InputException& e) {
        std::cerr << "R-Type Client Input Error: " << e.what() << std::endl;
        return 1;
    } catch (const GameLoopException& e) {
        std::cerr << "R-Type Client Game Loop Error: " << e.what() << std::endl;
        return 1;
    } catch (const GameException& e) {
        std::cerr << "R-Type Client Game Error: " << e.what() << std::endl;
        return 1;
    } catch (const std::exception& e) {
        std::cerr << "R-Type Client System Error: " << e.what() << std::endl;
        return 1;
    } catch (...) {
        std::cerr << "R-Type Client: An unknown error occurred" << std::endl;
        return 1;
    }
}