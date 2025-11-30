/*
** EPITECH PROJECT, 2025
** real_r_type
** File description:
** SFMLInputHandler
*/

#include "SFMLInputHandler.hpp"

using namespace Input;

void SFMLInputHandler::handleEvent(const sf::Event &)
{
    std::cout << "Handling SFML event\n";
    // TODO: Implement event handling logic if needed
}

bool SFMLInputHandler::isKeyPressed(Key key) const
{
    sf::Keyboard::Key sfmlKey;
    switch (key) {
        case Key::A: sfmlKey = sf::Keyboard::Key::A; break;
        case Key::B: sfmlKey = sf::Keyboard::Key::B; break;
        case Key::C: sfmlKey = sf::Keyboard::Key::C; break;
        case Key::D: sfmlKey = sf::Keyboard::Key::D; break;
        case Key::E: sfmlKey = sf::Keyboard::Key::E; break;
        case Key::F: sfmlKey = sf::Keyboard::Key::F; break;
        case Key::G: sfmlKey = sf::Keyboard::Key::G; break;
        case Key::H: sfmlKey = sf::Keyboard::Key::H; break;
        case Key::I: sfmlKey = sf::Keyboard::Key::I; break;
        case Key::J: sfmlKey = sf::Keyboard::Key::J; break;
        case Key::K: sfmlKey = sf::Keyboard::Key::K; break;
        case Key::L: sfmlKey = sf::Keyboard::Key::L; break;
        case Key::M: sfmlKey = sf::Keyboard::Key::M; break;
        case Key::N: sfmlKey = sf::Keyboard::Key::N; break;
        case Key::O: sfmlKey = sf::Keyboard::Key::O; break;
        case Key::P: sfmlKey = sf::Keyboard::Key::P; break;
        case Key::Q: sfmlKey = sf::Keyboard::Key::Q; break;
        case Key::R: sfmlKey = sf::Keyboard::Key::R; break;
        case Key::S: sfmlKey = sf::Keyboard::Key::S; break;
        case Key::T: sfmlKey = sf::Keyboard::Key::T; break;
        case Key::U: sfmlKey = sf::Keyboard::Key::U; break;
        case Key::V: sfmlKey = sf::Keyboard::Key::V; break;
        case Key::W: sfmlKey = sf::Keyboard::Key::W; break;
        case Key::X: sfmlKey = sf::Keyboard::Key::X; break;
        case Key::Y: sfmlKey = sf::Keyboard::Key::Y; break;
        case Key::Z: sfmlKey = sf::Keyboard::Key::Z; break;

        case Key::Escape: sfmlKey = sf::Keyboard::Key::Escape; break;
        case Key::Space:  sfmlKey = sf::Keyboard::Key::Space; break;
        case Key::Enter:  sfmlKey = sf::Keyboard::Key::Enter; break;

        case Key::Left:  sfmlKey = sf::Keyboard::Key::Left; break;
        case Key::Right: sfmlKey = sf::Keyboard::Key::Right; break;
        case Key::Up:    sfmlKey = sf::Keyboard::Key::Up; break;
        case Key::Down:  sfmlKey = sf::Keyboard::Key::Down; break;

        default:
            return false;
    }
    return sf::Keyboard::isKeyPressed(sfmlKey);
}