#ifndef GAME_STATE_GAME_HPP_INCLUDEGUARD
#define GAME_STATE_GAME_HPP_INCLUDEGUARD

#include "GameState.hpp"
#include "NetworkClient.hpp"
#include "Player.hpp"
#include "PredictedMove.hpp"
#include "common.hpp"
#include <array>
#include <memory>
#include <vector>

// fwd decls
namespace jt {
class Shape;
class Sprite;
} // namespace jt
class b2World;

class Hud;

class StateGame : public jt::GameState {
private:
    std::shared_ptr<jt::Shape> m_background;
    std::shared_ptr<jt::Shape> m_overlay;
    std::shared_ptr<jt::Sprite> m_vignette;
    std::shared_ptr<Hud> m_hud;
    std::shared_ptr<b2World> m_world { nullptr };
    std::shared_ptr<NetworkClient> m_client { nullptr };
    std::shared_ptr<Player> m_localPlayer { nullptr };
    std::map<int, std::shared_ptr<Player>> m_remotePlayers;
    bool m_running { false };
    bool m_hasEnded { false };

    int m_localPlayerId { -1 };

    std::array<Predicted_Move, Network::NetworkProperties::c_buffer_size()> predicted_move;
    std::array<PlayerState, Network::NetworkProperties::c_buffer_size()> predicted_move_result;

    PlayerState player_state;
    std::size_t current_prediction_id { 0 };

    void doInternalCreate() override;
    void updateActivePlayerPositionFromServer(
        std::shared_ptr<Player> player, PlayerMap playerPositions);
    void spawnNewPlayer(int newPlayerId);
    void UpdateRemotePlayerPositions(PayloadServer2Client payload);
    void removeLocalOnlyPlayers(PayloadServer2Client payload);
    void checkLocalPlayerId(int payloadPlayerId);
    void doInternalUpdate(float const elapsed) override;
    void doInternalDraw() const override;

    void endGame();
};

#endif
