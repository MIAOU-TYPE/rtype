/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** SfmlGraphics
*/

#include "SfmlGraphics.hpp"

namespace
{
    const std::unordered_map<sf::Keyboard::Key, Core::Key> sfmlToCoreKeyMap{{sf::Keyboard::Key::A, Core::Key::A},
        {sf::Keyboard::Key::B, Core::Key::B}, {sf::Keyboard::Key::C, Core::Key::C},
        {sf::Keyboard::Key::D, Core::Key::D}, {sf::Keyboard::Key::E, Core::Key::E},
        {sf::Keyboard::Key::F, Core::Key::F}, {sf::Keyboard::Key::G, Core::Key::G},
        {sf::Keyboard::Key::H, Core::Key::H}, {sf::Keyboard::Key::I, Core::Key::I},
        {sf::Keyboard::Key::J, Core::Key::J}, {sf::Keyboard::Key::K, Core::Key::K},
        {sf::Keyboard::Key::L, Core::Key::L}, {sf::Keyboard::Key::M, Core::Key::M},
        {sf::Keyboard::Key::N, Core::Key::N}, {sf::Keyboard::Key::O, Core::Key::O},
        {sf::Keyboard::Key::P, Core::Key::P}, {sf::Keyboard::Key::Q, Core::Key::Q},
        {sf::Keyboard::Key::R, Core::Key::R}, {sf::Keyboard::Key::S, Core::Key::S},
        {sf::Keyboard::Key::T, Core::Key::T}, {sf::Keyboard::Key::U, Core::Key::U},
        {sf::Keyboard::Key::V, Core::Key::V}, {sf::Keyboard::Key::W, Core::Key::W},
        {sf::Keyboard::Key::X, Core::Key::X}, {sf::Keyboard::Key::Y, Core::Key::Y},
        {sf::Keyboard::Key::Z, Core::Key::Z},

        {sf::Keyboard::Key::Num0, Core::Key::Num0}, {sf::Keyboard::Key::Num1, Core::Key::Num1},
        {sf::Keyboard::Key::Num2, Core::Key::Num2}, {sf::Keyboard::Key::Num3, Core::Key::Num3},
        {sf::Keyboard::Key::Num4, Core::Key::Num4}, {sf::Keyboard::Key::Num5, Core::Key::Num5},
        {sf::Keyboard::Key::Num6, Core::Key::Num6}, {sf::Keyboard::Key::Num7, Core::Key::Num7},
        {sf::Keyboard::Key::Num8, Core::Key::Num8}, {sf::Keyboard::Key::Num9, Core::Key::Num9},

        {sf::Keyboard::Key::Escape, Core::Key::Escape}, {sf::Keyboard::Key::Space, Core::Key::Space},
        {sf::Keyboard::Key::Enter, Core::Key::Enter}, {sf::Keyboard::Key::Tab, Core::Key::Tab},
        {sf::Keyboard::Key::Backspace, Core::Key::Backspace},

        {sf::Keyboard::Key::Left, Core::Key::Left}, {sf::Keyboard::Key::Right, Core::Key::Right},
        {sf::Keyboard::Key::Up, Core::Key::Up}, {sf::Keyboard::Key::Down, Core::Key::Down},

        {sf::Keyboard::Key::LShift, Core::Key::LShift}, {sf::Keyboard::Key::RShift, Core::Key::RShift},
        {sf::Keyboard::Key::LControl, Core::Key::LCtrl}, {sf::Keyboard::Key::RControl, Core::Key::RCtrl}};

    [[nodiscard]] Core::Key toCoreKey(const sf::Keyboard::Key sfmlKey) noexcept
    {
        if (const auto it = sfmlToCoreKeyMap.find(sfmlKey); it != sfmlToCoreKeyMap.end())
            return it->second;
        return Core::Key::Unknown;
    }

} // namespace

namespace Graphics
{
    void SfmlGraphics::create(
        const unsigned int width, const unsigned int height, const std::string &title, const bool fullscreen)
    {
        try {
            _resourceManager = std::make_shared<Resources::EmbeddedResourceManager>();

            const sf::VideoMode videoMode =
                fullscreen ? sf::VideoMode::getDesktopMode() : sf::VideoMode({width, height});
            constexpr uint32_t style = sf::Style::Default;
            const sf::State state = fullscreen ? sf::State::Fullscreen : sf::State::Windowed;
            _window = std::make_shared<sf::RenderWindow>();
            _window->create(videoMode, title, style, state);
            if (!isOpen())
                throw;
        } catch (...) {
            throw IGraphicsError("{SfmlGraphics::create}: Failed to create SFML window");
        }
    }

    void SfmlGraphics::close() noexcept
    {
        if (isOpen())
            _window->close();
    }

    bool SfmlGraphics::isOpen() const noexcept
    {
        return _window && _window->isOpen();
    }

    void SfmlGraphics::pollEvents(Core::EventBus &bus)
    {
        while (const auto event = _window->pollEvent()) {
            if (event->is<sf::Event::Closed>())
                bus.emit(Core::WindowClosed{});

            if (const auto key = event->getIf<sf::Event::MouseButtonPressed>()) {
                const Core::Key button =
                    (key->button == sf::Mouse::Button::Left) ? Core::Key::MouseLeft : Core::Key::MouseRight;
                bus.emit(Core::MousePressed(button, key->position.x, key->position.y));
            }

            if (const auto key = event->getIf<sf::Event::MouseButtonReleased>()) {
                const Core::Key button =
                    (key->button == sf::Mouse::Button::Left) ? Core::Key::MouseLeft : Core::Key::MouseRight;
                bus.emit(Core::MouseReleased(button));
            }

            if (const auto key = event->getIf<sf::Event::KeyPressed>())
                bus.emit(Core::KeyPressed(toCoreKey(key->code)));

            if (const auto key = event->getIf<sf::Event::KeyReleased>())
                bus.emit(Core::KeyReleased(toCoreKey(key->code)));
        }
    }

    std::shared_ptr<IRenderer> SfmlGraphics::createRenderer() const noexcept
    {
        return std::make_shared<SfmlRenderer>(_window, _resourceManager);
    }

} // namespace Graphics