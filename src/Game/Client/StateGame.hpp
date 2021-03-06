#ifndef GAME_STATE_GAME_HPP_INCLUDEGUARD
#define GAME_STATE_GAME_HPP_INCLUDEGUARD

#include "CircularBuffer.hpp"
#include "EnemyState.hpp"
#include "ExplosionManager.hpp"
#include "ExplosionState.hpp"
#include "GameState.hpp"
#include "NetworkClient.hpp"
#include "Parallax.hpp"
#include "Player.hpp"
#include "PowerupState.hpp"
#include "PredictedMove.hpp"
#include "common.hpp"
#include <cstddef>
#include <memory>

// fwd decls
namespace jt {
class Sound;
class Shape;
class Sprite;
class Animation;
} // namespace jt

class b2World;
class Hud;

class StateGame final : public jt::GameState {
public:
    StateGame() = default;

private:
    std::shared_ptr<jt::Shape> m_background;
    std::shared_ptr<jt::Shape> m_overlay;
    std::shared_ptr<jt::Sprite> m_vignette;
    std::shared_ptr<Hud> m_hud;
    std::shared_ptr<b2World> m_world { nullptr };
    std::shared_ptr<NetworkClient> m_client { nullptr };
    std::shared_ptr<Player> m_localPlayer { nullptr };
    std::map<int, std::shared_ptr<Player>> m_remotePlayers;

    std::vector<ShotState> m_shots;
    std::vector<EnemyState> m_enemies;
    std::vector<PowerupState> m_powerups;
    std::shared_ptr<jt::Animation> mutable m_shotSprite { nullptr };
    std::shared_ptr<jt::Sprite> mutable m_enemySprite { nullptr };
    std::shared_ptr<jt::Animation> mutable m_powerupAnimation { nullptr };
    std::shared_ptr<ExplosionManager> m_explosionManager { nullptr };
    std::shared_ptr<Parallax> m_parallax { nullptr };
    std::shared_ptr<jt::SoundGroup> m_shotSounds { nullptr };
    std::shared_ptr<jt::Sound> m_powerupSound { nullptr };
    bool m_running { false };
    bool m_hasEnded { false };
    float m_secondsSincelastDataReceived { 0.0f };

    int m_localPlayerId { -1 };
    PlayerState m_currentPlayerState;
    std::size_t m_currentPredictionId { 0 };

    jt::CircularBuffer<PredictedMove, Network::NetworkProperties::clientNetworkBufferSize()>
        m_predictedMoves;
    jt::CircularBuffer<PlayerState, Network::NetworkProperties::clientNetworkBufferSize()>
        m_predictedMoveResults;

    void doInternalCreate() override;
    void updateActivePlayerPositionFromServer(
        std::shared_ptr<Player> player, PlayerMap playerPositions);
    void spawnNewPlayer(int newPlayerId);
    void updateRemotePlayers(PayloadServer2Client payload);
    void removeLocalOnlyPlayers(PayloadServer2Client payload);
    void checkLocalPlayerId(int payloadPlayerId);
    void doInternalUpdate(float const elapsed) override;
    void drawPlayerShots() const;
    void drawEnemyShots() const;
    void toggleMuteOnButtonPress();
    void doInternalDraw() const override;

    void endGame();
};

#endif
