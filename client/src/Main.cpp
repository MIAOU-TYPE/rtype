/*
** EPITECH PROJECT, 2025
** real_r_type
** File description:
** Main
*/

#include <exception>
#include <iostream>
#include "ClientRuntime.hpp"
#include "GameClient.hpp"
#include "NetClient.hpp"

using namespace Game;

int main()
{
    try {
        Thread::ClientRuntime runtime(std::make_shared<Network::NetClient>());
        runtime.start();
        return 0;
    } catch (const std::exception &e) {
        std::cerr << "R-Type Client System Error: " << e.what() << std::endl;
        return 1;
    }
}