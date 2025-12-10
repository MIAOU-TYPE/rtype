/*
** EPITECH PROJECT, 2025
** real_r_type
** File description:
** SFMLAnimationManager
*/

#include "SFMLAnimationManager.hpp"

using namespace Graphics;

void SFMLAnimationManager::addAnimation(std::shared_ptr<SFMLAnimation> animation)
{
    if (!animation) {
        throw SFMLAnimationManagerError("Cannot add null animation");
    }

    const std::string &animationName = animation->getName();
    if (_animations.find(animationName) != _animations.end()) {
        throw SFMLAnimationManagerError("Animation '" + animationName + "' already exists");
    }

    _animations[animationName] = animation;
}

void SFMLAnimationManager::setCurrentAnimation(const std::string &animationName)
{
    auto it = _animations.find(animationName);
    if (it == _animations.end()) {
        throw SFMLAnimationManagerError("Animation '" + animationName + "' does not exist");
    }

    if (_currentAnimationName != animationName) {
        _currentAnimation = it->second;
        _currentAnimationName = animationName;
        _currentAnimation->reset();
    }
}

void SFMLAnimationManager::update(float deltaTime)
{
    if (_currentAnimation) {
        _currentAnimation->update(deltaTime);
    }
}

const AnimationFrame &SFMLAnimationManager::getCurrentFrame() const
{
    if (!_currentAnimation) {
        throw SFMLAnimationManagerError("No current animation set");
    }

    return _currentAnimation->getCurrentFrame();
}

std::shared_ptr<SFMLAnimation> SFMLAnimationManager::getCurrentAnimation() const
{
    if (!_currentAnimation) {
        throw SFMLAnimationManagerError("No current animation set");
    }

    return _currentAnimation;
}

bool SFMLAnimationManager::hasAnimation(const std::string &animationName) const
{
    return _animations.find(animationName) != _animations.end();
}

const std::string &SFMLAnimationManager::getCurrentAnimationName() const
{
    return _currentAnimationName;
}

void SFMLAnimationManager::resetCurrentAnimation()
{
    if (_currentAnimation) {
        _currentAnimation->reset();
    }
}