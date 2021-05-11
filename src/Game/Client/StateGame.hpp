#ifndef GAME_STATE_GAME_HPP_INCLUDEGUARD
#define GAME_STATE_GAME_HPP_INCLUDEGUARD

#include "GameState.hpp"
#include "NetworkClient.hpp"
#include "Player.hpp"

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

    void doInternalCreate() override;
    void updateActivePlayerPositionFromServer(
        int playerID, std::shared_ptr<Player> player, std::map<int, jt::Vector2> playerPositions);
    void spawnNewPlayer(int newPlayerId);
    void UpdateAllPlayerPositionsFromServer(PayloadServer2Client payload);
    void InterpolateActivePlayer(float elapsed);
    void removeLocalOnlyPlayers(PayloadServer2Client payload);
    void doInternalUpdate(float const elapsed) override;
    void doInternalDraw() const override;

    void endGame();
};

#endif
