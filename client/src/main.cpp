/*
** EPITECH PROJECT, 2025
** real_r_type
** File description:
** main
*/

#include <exception>
#include <iostream>
#include "game/GameClient.hpp"

using namespace Game;

int main()
{
    try {
        GameClient client;

        client.init(800, 600);
        client.run();

        return 0;
    } catch (const std::exception &e) {
        std::cerr << "R-Type Client System Error: " << e.what() << std::endl;
        return 1;
    }
}