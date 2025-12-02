/*
** EPITECH PROJECT, 2025
** real_r_type
** File description:
** SFMLAnimation
*/

#include "SFMLAnimation.hpp"

using namespace Graphics;

SFMLAnimation::SFMLAnimation(const std::string &name, const std::vector<AnimationFrame> &frames, bool loop)
    : _name(name), _frames(frames), _currentFrameIndex(0), _currentFrameTime(0.0f), _loop(loop), _finished(false)
{
    if (_frames.empty()) {
        throw SFMLAnimationError("Cannot create animation '" + name + "' with no frames");
    }
}

void SFMLAnimation::update(float deltaTime)
{
    if (_finished && !_loop) {
        return;
    }

    if (_frames.empty()) {
        throw SFMLAnimationError("Cannot update animation '" + _name + "' with no frames");
    }

    _currentFrameTime += deltaTime;

    const AnimationFrame &currentFrame = _frames[_currentFrameIndex];
    if (_currentFrameTime >= currentFrame.duration) {
        _currentFrameTime = 0.0f;
        _currentFrameIndex++;

        if (_currentFrameIndex >= _frames.size()) {
            if (_loop) {
                _currentFrameIndex = 0;
            } else {
                _currentFrameIndex = _frames.size() - 1;
                _finished = true;
            }
        }
    }
}

const AnimationFrame &SFMLAnimation::getCurrentFrame() const
{
    if (_frames.empty()) {
        throw SFMLAnimationError("Cannot get current frame from animation '" + _name + "' with no frames");
    }

    if (_currentFrameIndex >= _frames.size()) {
        throw SFMLAnimationError("Current frame index out of bounds in animation '" + _name + "'");
    }

    return _frames[_currentFrameIndex];
}

void SFMLAnimation::reset()
{
    _currentFrameIndex = 0;
    _currentFrameTime = 0.0f;
    _finished = false;
}

bool SFMLAnimation::isFinished() const
{
    return _finished;
}

const std::string &SFMLAnimation::getName() const
{
    return _name;
}

void SFMLAnimation::setLoop(bool loop)
{
    _loop = loop;
    if (_loop && _finished) {
        reset();
    }
}

size_t SFMLAnimation::getFrameCount() const
{
    return _frames.size();
}