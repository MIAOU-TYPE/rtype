/*
** EPITECH PROJECT, 2025
** real_r_type
** File description:
** SFMLInputHandler
*/

#include "SFMLInputHandler.hpp"

using namespace Input;

void SFMLInputHandler::handleEvent(const sf::Event &event)
{
    std::cout << "Handling event of type: " << event.type << std::endl;
    // TODO: Implement event handling logic if needed
}

bool SFMLInputHandler::isKeyPressed(Key key) const
{
    sf::Keyboard::Key sfmlKey;
    switch (key) {
        case Key::A: sfmlKey = sf::Keyboard::A; break;
        case Key::B: sfmlKey = sf::Keyboard::B; break;
        case Key::C: sfmlKey = sf::Keyboard::C; break;
        case Key::D: sfmlKey = sf::Keyboard::D; break;
        case Key::E: sfmlKey = sf::Keyboard::E; break;
        case Key::F: sfmlKey = sf::Keyboard::F; break;
        case Key::G: sfmlKey = sf::Keyboard::G; break;
        case Key::H: sfmlKey = sf::Keyboard::H; break;
        case Key::I: sfmlKey = sf::Keyboard::I; break;
        case Key::J: sfmlKey = sf::Keyboard::J; break;
        case Key::K: sfmlKey = sf::Keyboard::K; break;
        case Key::L: sfmlKey = sf::Keyboard::L; break;
        case Key::M: sfmlKey = sf::Keyboard::M; break;
        case Key::N: sfmlKey = sf::Keyboard::N; break;
        case Key::O: sfmlKey = sf::Keyboard::O; break;
        case Key::P: sfmlKey = sf::Keyboard::P; break;
        case Key::Q: sfmlKey = sf::Keyboard::Q; break;
        case Key::R: sfmlKey = sf::Keyboard::R; break;
        case Key::S: sfmlKey = sf::Keyboard::S; break;
        case Key::T: sfmlKey = sf::Keyboard::T; break;
        case Key::U: sfmlKey = sf::Keyboard::U; break;
        case Key::V: sfmlKey = sf::Keyboard::V; break;
        case Key::W: sfmlKey = sf::Keyboard::W; break;
        case Key::X: sfmlKey = sf::Keyboard::X; break;
        case Key::Y: sfmlKey = sf::Keyboard::Y; break;
        case Key::Z: sfmlKey = sf::Keyboard::Z; break;
        case Key::Num0: sfmlKey = sf::Keyboard::Num0; break;
        case Key::Num1: sfmlKey = sf::Keyboard::Num1; break;
        case Key::Num2: sfmlKey = sf::Keyboard::Num2; break;
        case Key::Num3: sfmlKey = sf::Keyboard::Num3; break;
        case Key::Num4: sfmlKey = sf::Keyboard::Num4; break;
        case Key::Num5: sfmlKey = sf::Keyboard::Num5; break;
        case Key::Num6: sfmlKey = sf::Keyboard::Num6; break;
        case Key::Num7: sfmlKey = sf::Keyboard::Num7; break;
        case Key::Num8: sfmlKey = sf::Keyboard::Num8; break;
        case Key::Num9: sfmlKey = sf::Keyboard::Num9; break;
        case Key::Escape: sfmlKey = sf::Keyboard::Escape; break;
        case Key::LControl: sfmlKey = sf::Keyboard::LControl; break;
        case Key::LShift: sfmlKey = sf::Keyboard::LShift; break;
        case Key::LAlt: sfmlKey = sf::Keyboard::LAlt; break;
        case Key::LSystem: sfmlKey = sf::Keyboard::LSystem; break;
        case Key::RControl: sfmlKey = sf::Keyboard::RControl; break;
        case Key::RShift: sfmlKey = sf::Keyboard::RShift; break;
        case Key::RAlt: sfmlKey = sf::Keyboard::RAlt; break;
        case Key::RSystem: sfmlKey = sf::Keyboard::RSystem; break;
        case Key::Menu: sfmlKey = sf::Keyboard::Menu; break;
        case Key::LBracket: sfmlKey = sf::Keyboard::LBracket; break;
        case Key::RBracket: sfmlKey = sf::Keyboard::RBracket; break;
        case Key::Semicolon: sfmlKey = sf::Keyboard::Semicolon; break;
        case Key::Comma: sfmlKey = sf::Keyboard::Comma; break;
        case Key::Period: sfmlKey = sf::Keyboard::Period; break;
        case Key::Quote: sfmlKey = sf::Keyboard::Quote; break;
        case Key::Slash: sfmlKey = sf::Keyboard::Slash; break;
        case Key::Backslash: sfmlKey = sf::Keyboard::Backslash; break;
        case Key::Tilde: sfmlKey = sf::Keyboard::Tilde; break;
        case Key::Equal: sfmlKey = sf::Keyboard::Equal; break;
        case Key::Hyphen: sfmlKey = sf::Keyboard::Hyphen; break;
        case Key::Space: sfmlKey = sf::Keyboard::Space; break;
        case Key::Enter: sfmlKey = sf::Keyboard::Enter; break;
        case Key::Backspace: sfmlKey = sf::Keyboard::Backspace; break;
        case Key::Tab: sfmlKey = sf::Keyboard::Tab; break;
        case Key::PageUp: sfmlKey = sf::Keyboard::PageUp; break;
        case Key::PageDown: sfmlKey = sf::Keyboard::PageDown; break;
        case Key::End: sfmlKey = sf::Keyboard::End; break;
        case Key::Home: sfmlKey = sf::Keyboard::Home; break;
        case Key::Insert: sfmlKey = sf::Keyboard::Insert; break;
        case Key::Delete: sfmlKey = sf::Keyboard::Delete; break;
        case Key::Add: sfmlKey = sf::Keyboard::Add; break;
        case Key::Subtract: sfmlKey = sf::Keyboard::Subtract; break;
        case Key::Multiply: sfmlKey = sf::Keyboard::Multiply; break;
        case Key::Divide: sfmlKey = sf::Keyboard::Divide; break;
        case Key::Left: sfmlKey = sf::Keyboard::Left; break;
        case Key::Right: sfmlKey = sf::Keyboard::Right; break;
        case Key::Up: sfmlKey = sf::Keyboard::Up; break;
        case Key::Down: sfmlKey = sf::Keyboard::Down; break;
        case Key::Numpad0: sfmlKey = sf::Keyboard::Numpad0; break;
        case Key::Numpad1: sfmlKey = sf::Keyboard::Numpad1; break;
        case Key::Numpad2: sfmlKey = sf::Keyboard::Numpad2; break;
        case Key::Numpad3: sfmlKey = sf::Keyboard::Numpad3; break;
        case Key::Numpad4: sfmlKey = sf::Keyboard::Numpad4; break;
        case Key::Numpad5: sfmlKey = sf::Keyboard::Numpad5; break;
        case Key::Numpad6: sfmlKey = sf::Keyboard::Numpad6; break;
        case Key::Numpad7: sfmlKey = sf::Keyboard::Numpad7; break;
        case Key::Numpad8: sfmlKey = sf::Keyboard::Numpad8; break;
        case Key::Numpad9: sfmlKey = sf::Keyboard::Numpad9; break;
        case Key::F1: sfmlKey = sf::Keyboard::F1; break;
        case Key::F2: sfmlKey = sf::Keyboard::F2; break;
        case Key::F3: sfmlKey = sf::Keyboard::F3; break;
        case Key::F4: sfmlKey = sf::Keyboard::F4; break;
        case Key::F5: sfmlKey = sf::Keyboard::F5; break;
        case Key::F6: sfmlKey = sf::Keyboard::F6; break;
        case Key::F7: sfmlKey = sf::Keyboard::F7; break;
        case Key::F8: sfmlKey = sf::Keyboard::F8; break;
        case Key::F9: sfmlKey = sf::Keyboard::F9; break;
        case Key::F10: sfmlKey = sf::Keyboard::F10; break;
        case Key::F11: sfmlKey = sf::Keyboard::F11; break;
        case Key::F12: sfmlKey = sf::Keyboard::F12; break;
        case Key::F13: sfmlKey = sf::Keyboard::F13; break;
        case Key::F14: sfmlKey = sf::Keyboard::F14; break;
        case Key::F15: sfmlKey = sf::Keyboard::F15; break;
        case Key::Pause: sfmlKey = sf::Keyboard::Pause; break;
        default: return false;
    }
    return sf::Keyboard::isKeyPressed(sfmlKey);
}