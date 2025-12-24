/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** SfmlGraphics
*/

#include "SfmlGraphics.hpp"

namespace
{
    const std::unordered_map<sf::Keyboard::Key, Engine::Key> sfmlToCoreKeyMap{{sf::Keyboard::Key::A, Engine::Key::A},
        {sf::Keyboard::Key::B, Engine::Key::B}, {sf::Keyboard::Key::C, Engine::Key::C},
        {sf::Keyboard::Key::D, Engine::Key::D}, {sf::Keyboard::Key::E, Engine::Key::E},
        {sf::Keyboard::Key::F, Engine::Key::F}, {sf::Keyboard::Key::G, Engine::Key::G},
        {sf::Keyboard::Key::H, Engine::Key::H}, {sf::Keyboard::Key::I, Engine::Key::I},
        {sf::Keyboard::Key::J, Engine::Key::J}, {sf::Keyboard::Key::K, Engine::Key::K},
        {sf::Keyboard::Key::L, Engine::Key::L}, {sf::Keyboard::Key::M, Engine::Key::M},
        {sf::Keyboard::Key::N, Engine::Key::N}, {sf::Keyboard::Key::O, Engine::Key::O},
        {sf::Keyboard::Key::P, Engine::Key::P}, {sf::Keyboard::Key::Q, Engine::Key::Q},
        {sf::Keyboard::Key::R, Engine::Key::R}, {sf::Keyboard::Key::S, Engine::Key::S},
        {sf::Keyboard::Key::T, Engine::Key::T}, {sf::Keyboard::Key::U, Engine::Key::U},
        {sf::Keyboard::Key::V, Engine::Key::V}, {sf::Keyboard::Key::W, Engine::Key::W},
        {sf::Keyboard::Key::X, Engine::Key::X}, {sf::Keyboard::Key::Y, Engine::Key::Y},
        {sf::Keyboard::Key::Z, Engine::Key::Z},

        {sf::Keyboard::Key::Num0, Engine::Key::Num0}, {sf::Keyboard::Key::Num1, Engine::Key::Num1},
        {sf::Keyboard::Key::Num2, Engine::Key::Num2}, {sf::Keyboard::Key::Num3, Engine::Key::Num3},
        {sf::Keyboard::Key::Num4, Engine::Key::Num4}, {sf::Keyboard::Key::Num5, Engine::Key::Num5},
        {sf::Keyboard::Key::Num6, Engine::Key::Num6}, {sf::Keyboard::Key::Num7, Engine::Key::Num7},
        {sf::Keyboard::Key::Num8, Engine::Key::Num8}, {sf::Keyboard::Key::Num9, Engine::Key::Num9},

        {sf::Keyboard::Key::Escape, Engine::Key::Escape}, {sf::Keyboard::Key::Space, Engine::Key::Space},
        {sf::Keyboard::Key::Enter, Engine::Key::Enter}, {sf::Keyboard::Key::Tab, Engine::Key::Tab},
        {sf::Keyboard::Key::Backspace, Engine::Key::Backspace},

        {sf::Keyboard::Key::Left, Engine::Key::Left}, {sf::Keyboard::Key::Right, Engine::Key::Right},
        {sf::Keyboard::Key::Up, Engine::Key::Up}, {sf::Keyboard::Key::Down, Engine::Key::Down},

        {sf::Keyboard::Key::LShift, Engine::Key::LShift}, {sf::Keyboard::Key::RShift, Engine::Key::RShift},
        {sf::Keyboard::Key::LControl, Engine::Key::LCtrl}, {sf::Keyboard::Key::RControl, Engine::Key::RCtrl}};

    [[nodiscard]] Engine::Key toCoreKey(const sf::Keyboard::Key sfmlKey) noexcept
    {
        if (const auto it = sfmlToCoreKeyMap.find(sfmlKey); it != sfmlToCoreKeyMap.end())
            return it->second;
        return Engine::Key::Unknown;
    }

} // namespace

namespace Graphics
{
    void SfmlGraphics::create(Extent2u size, const std::string &title, const bool fullscreen)
    {
        try {
            _resourceManager = std::make_shared<Resources::EmbeddedResourceManager>();

            const sf::VideoMode videoMode =
                fullscreen ? sf::VideoMode::getDesktopMode() : sf::VideoMode({size.width, size.height});
            constexpr uint32_t style = sf::Style::Titlebar | sf::Style::Close;
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

    void SfmlGraphics::setResolution(Extent2u size)
    {
        if (!isOpen())
            return;

        _window->setSize({size.width, size.height});
        _window->setPosition({0, 0});

        sf::View view;
        view.setSize({static_cast<float>(size.width), static_cast<float>(size.height)});
        view.setCenter({static_cast<float>(size.width) / 2.f, static_cast<float>(size.height) / 2.f});
        _window->setView(view);
    }

    void SfmlGraphics::pollEvents(Engine::EventBus &bus)
    {
        while (const auto event = _window->pollEvent()) {
            if (event->is<sf::Event::Closed>())
                bus.emit(Engine::WindowClosed{});

            if (const auto m = event->getIf<sf::Event::MouseMoved>())
                bus.emit(Engine::MouseMoved(m->position.x, m->position.y));

            if (const auto key = event->getIf<sf::Event::MouseButtonPressed>()) {
                const Engine::Key button =
                    (key->button == sf::Mouse::Button::Left) ? Engine::Key::MouseLeft : Engine::Key::MouseRight;
                bus.emit(Engine::MousePressed(button, key->position.x, key->position.y));
            }

            if (const auto key = event->getIf<sf::Event::MouseButtonReleased>()) {
                const Engine::Key button =
                    (key->button == sf::Mouse::Button::Left) ? Engine::Key::MouseLeft : Engine::Key::MouseRight;
                bus.emit(Engine::MouseReleased(button));
            }

            if (const auto key = event->getIf<sf::Event::KeyPressed>())
                bus.emit(Engine::KeyPressed(toCoreKey(key->code)));

            if (const auto key = event->getIf<sf::Event::KeyReleased>())
                bus.emit(Engine::KeyReleased(toCoreKey(key->code)));
        }
    }

    std::shared_ptr<IRenderer> SfmlGraphics::createRenderer() const noexcept
    {
        return std::make_shared<SfmlRenderer>(_window, _resourceManager);
    }

} // namespace Graphics