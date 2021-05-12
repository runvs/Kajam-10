#ifndef GAME_STATE_GAME_HPP_INCLUDEGUARD
#define GAME_STATE_GAME_HPP_INCLUDEGUARD

#include "GameState.hpp"
#include "NetworkClient.hpp"
#include "Player.hpp"
#include <array>
#include <iostream>
#include <memory>
#include <vector>

// fwd decls
namespace jt {
class Shape;
class Sprite;
} // namespace jt
class b2World;

class Hud;

struct PredictedMove {
    float dt;
    InputState input;
};

constexpr std::size_t c_buffer_size = 512;
constexpr std::size_t c_buffer_mask = c_buffer_size - 1;

class StateGame : public jt::GameState {
private:
    std::shared_ptr<jt::Shape> m_background;
    std::shared_ptr<jt::Shape> m_overlay;
    std::shared_ptr<jt::Sprite> m_vignette;
    std::shared_ptr<Hud> m_hud;
    std::shared_ptr<b2World> m_world { nullptr };
    std::shared_ptr<NetworkClient> m_client { nullptr };
    std::shared_ptr<Player> m_player { nullptr };
    std::map<int, std::shared_ptr<Player>> m_players;
    bool m_running { false };
    bool m_hasEnded { false };

    std::array<PredictedMove, c_buffer_size> predicted_move;
    std::array<PlayerState, c_buffer_size> predicted_move_result;

    PlayerState player_state;
    uint32_t current_prediction_id;

    void doInternalCreate() override;
    void updateActivePlayerPositionFromServer(
        int playerID, std::shared_ptr<Player> player, PlayerMap playerPositions);
    void spawnNewPlayer(int newPlayerId);
    void UpdateAllPlayerPositionsFromServer(PayloadServer2Client payload);
    void removeLocalOnlyPlayers(PayloadServer2Client payload);
    void doInternalUpdate(float const elapsed) override;
    void doInternalDraw() const override;

    void endGame();
};

#endif
