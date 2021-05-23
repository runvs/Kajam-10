﻿#include "main.hpp"
#include "../common/common.hpp"
#include "ClientProperties.hpp"
#include "Game.hpp"
#include "GameProperties.hpp"
#include "InputManager.hpp"
#include "KeyboardInput.hpp"
#include "MouseInput.hpp"
#include "MusicPlayer.hpp"
#include "Random.hpp"
#include "RenderWindow.hpp"
#include "StateMenu.hpp"
#include <iostream>

std::shared_ptr<jt::GameInterface> game;

void gameloop()
{
    if (game) {
        game->run();
    }
}

int main()
{
    // hideConsoleInRelease();

    jt::Random::useTimeAsRandomSeed();
    auto const mouse = std::make_shared<jt::MouseInput>();
    auto const keyboard = std::make_shared<jt::KeyboardInput>();
    auto input = std::make_shared<jt::InputManager>(mouse, keyboard);

    game = std::make_shared<jt::Game>(
        std::make_shared<jt::RenderWindow>(
            static_cast<unsigned int>(Game::GameProperties::displayWindowSize().x()),
            static_cast<unsigned int>(Game::GameProperties::displayWindowSize().y()),
            GP::GameName()),
        Game::GameProperties::displayZoom(), input, std::make_shared<jt::MusicPlayer>());
    game->setupRenderTarget();

    game->startGame(std::make_shared<StateMenu>(), gameloop);

    std::cout << "thread ended\n";
    return 0;
}
