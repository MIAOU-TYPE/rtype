/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** SpriteLoader
*/

#include "SpriteLoader.hpp"

namespace Engine
{

    void SpriteLoader::loadFromFile(const std::string &path, SpriteRegistry &registry)
    {
        std::ifstream file(path);
        if (!file)
            throw SpriteRegistryError("{SpriteLoader::loadFromFile}: cannot open sprite file: " + path);

        nlohmann::json j;
        file >> j;

        SpriteDefinition def;
        def.texturePath = j.at("texture").get<std::string>();
        def.defaultAnimation = j.at("default_animation").get<std::string>();

        for (const auto &[name, animJson] : j.at("animations").items()) {
            Animation anim;
            anim.name = name;
            anim.loop = animJson.at("loop").get<bool>();

            for (const auto &frameJson : animJson.at("frames")) {
                AnimationFrame frame;
                frame.rect = {frameJson.at("x").get<int>(), frameJson.at("y").get<int>(), frameJson.at("w").get<int>(),
                    frameJson.at("h").get<int>()};
                frame.duration = frameJson.at("duration").get<float>();

                anim.frames.push_back(frame);
            }

            def.animations.emplace(name, std::move(anim));
        }

        auto spriteId = j.at("spriteId").get<int>();
        registry.add(spriteId, std::move(def));
    }
} // namespace Engine
