#include "StateMenu.hpp"
#include "ClientProperties.hpp"
#include "Color.hpp"
#include "DrawableHelpers.hpp"
#include "GameInterface.hpp"
#include "GameProperties.hpp"
#include "InputManager.hpp"
#include "KeyCodes.hpp"
#include "MathHelper.hpp"
#include "Shape.hpp"
#include "Sprite.hpp"
#include "StateGame.hpp"
#include "Text.hpp"
#include "TweenAlpha.hpp"
#include "TweenPosition.hpp"
#include "TweenScale.hpp"
#include <algorithm>

StateMenu::StateMenu() = default;

void StateMenu::doInternalCreate()
{
    createMenuText();
    createShapes();
    createVignette();

    createTweens();
    getGame()->getMusicPlayer()->PlayMusic("assets/music.ogg");
}

void StateMenu::createVignette()
{
    m_vignette = jt::dh::createVignette(Game::GameProperties::displayScreenSize());
    m_vignette->setColor({ 255, 255, 255, 110 });
}

void StateMenu::createLogo()
{
    float half_width = Game::GameProperties::displayScreenSize().x() / 2 - 77;
    m_logo = std::make_shared<jt::Sprite>();
    m_logo->loadSprite("assets/logo.png");
    m_logo->setPosition({ half_width, 20 });
}

void StateMenu::createShapes()
{
    m_background
        = jt::dh::createRectShape(Game::GameProperties::displayScreenSize(), GP::PaletteColor1());
    m_overlay
        = jt::dh::createRectShape(Game::GameProperties::displayScreenSize(), jt::colors::Black);
}

void StateMenu::createMenuText()
{
    createLogo();
    createTextExplanation();
    createTextCredits();
}

void StateMenu::createTextCredits()
{
    m_text_Credits = jt::dh::createText(getGame()->getRenderTarget(),
        "Created by " + GP::AuthorName() + " for " + "\n" + GP::JamName() + " in " + GP::JamDate()
            + "\nThis game uses OpenAl, licensed under LGPL v2.\nPlease see https://openal.org/ "
              "for details"
            + "\nThe Soundtrack 'Tomorrow Belongs to Cats' belongs to Doxent Zsigmond (c)\n"
              "copyright 2020 Licensed under a Creative Commons Attribution (3.0) license. ",
        8U, GP::PaletteColor5());
    m_text_Credits->SetTextAlign(jt::Text::TextAlign::LEFT);
    m_text_Credits->setPosition({ 8, Game::GameProperties::displayScreenSize().y() - 75 });
    m_text_Credits->setShadow(GP::PaletteFontShadow(), jt::Vector2 { 1, 1 });
}

void StateMenu::createTextExplanation()
{
    float half_width = Game::GameProperties::displayScreenSize().x() / 2;
    m_text_Explanation = jt::dh::createText(
        getGame()->getRenderTarget(), "Press Space to start the game", 16U, GP::PaletteColor8());
    m_text_Explanation->setPosition({ half_width, 150 });
    m_text_Explanation->setShadow(GP::PaletteFontShadow(), jt::Vector2 { 1, 1 });
}

void StateMenu::createTweens()
{
    createTweenOverlayAlpha();
    createTweenCreditsPosition();
    createTweenExplanationScale();
}

void StateMenu::createTweenExplanationScale()
{
    auto s2 = m_text_Explanation->getPosition() + jt::Vector2 { -1000, 0 };
    auto e2 = m_text_Explanation->getPosition();

    auto tween = jt::TweenPosition<jt::Text>::create(m_text_Explanation, 0.5f, s2, e2);
    tween->setStartDelay(0.3f);
    tween->setSkipFrames();

    tween->addCompleteCallback([this]() {
        auto ts = jt::TweenScale<jt::Text>::create(
            m_text_Explanation, 0.75f, jt::Vector2 { 1.0f, 1.0f }, jt::Vector2 { 1.05f, 1.05f });
        ts->setRepeat(true);
        ts->setAgePercentConversion([](float age) {
            return jt::Lerp::cosine(0.0f, 1.0f, jt::MathHelper::clamp(age, 0.0f, 1.0f));
        });
        add(ts);
    });
    add(tween);
}

void StateMenu::createTweenOverlayAlpha()
{
    auto tween = jt::TweenAlpha<jt::Shape>::create(
        m_overlay, 0.5f, std::uint8_t { 255 }, std::uint8_t { 0 });
    tween->setSkipFrames();
    add(tween);
}

void StateMenu::createTweenCreditsPosition()
{
    auto s3 = m_text_Credits->getPosition() + jt::Vector2 { 0, 100 };
    auto e3 = m_text_Credits->getPosition();

    auto tween = jt::TweenPosition<jt::Text>::create(m_text_Credits, 0.35f, s3, e3);
    tween->setStartDelay(0.8f);
    tween->setSkipFrames();
    add(tween);
}

void StateMenu::toggleMusicMuteOnButtonPress()
{
    if (getGame()->input()->keyboard()->justPressed(jt::KeyCode::M)) {
        float volume = getGame()->getMusicPlayer()->GetMusicVolume();

        if (volume == 0.0f) {
            getGame()->getMusicPlayer()->SetMusicVolume(100.0f);
        } else {
            getGame()->getMusicPlayer()->SetMusicVolume(0.0f);
        }
    }
}

void StateMenu::doInternalUpdate(float const elapsed)
{
    updateDrawables(elapsed);
    checkForTransitionToStateGame();
    toggleMusicMuteOnButtonPress();
}

void StateMenu::updateDrawables(const float& elapsed)
{
    m_background->update(elapsed);
    m_text_Explanation->update(elapsed);
    m_text_Credits->update(elapsed);
    m_overlay->update(elapsed);
    m_vignette->update(elapsed);
    m_logo->update(elapsed);
}

void StateMenu::checkForTransitionToStateGame()
{
    auto const keysToTriggerTransition = { jt::KeyCode::Space, jt::KeyCode::Enter };

    if (std::any_of(keysToTriggerTransition.begin(), keysToTriggerTransition.end(),
            [this](auto const k) { return getGame()->input()->keyboard()->justPressed(k); })) {
        startTransitionToStateGame();
    }
}

void StateMenu::startTransitionToStateGame()
{
    if (!m_started) {
        m_started = true;

        createTweenTransition();
    }
}

void StateMenu::createTweenTransition()
{
    auto tw = jt::TweenAlpha<jt::Shape>::create(
        m_overlay, 0.5f, std::uint8_t { 0 }, std::uint8_t { 255 });
    tw->setSkipFrames();
    tw->addCompleteCallback([this]() { getGame()->switchState(std::make_shared<StateGame>()); });
    add(tw);
}

void StateMenu::doInternalDraw() const
{
    m_background->draw(getGame()->getRenderTarget());

    m_text_Explanation->draw(getGame()->getRenderTarget());
    m_text_Credits->draw(getGame()->getRenderTarget());

    m_overlay->draw(getGame()->getRenderTarget());
    m_vignette->draw(getGame()->getRenderTarget());
    m_logo->draw(getGame()->getRenderTarget());
}
