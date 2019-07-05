//
// Created by liyicheng on 2019/7/3.
//

#ifndef Z2_WORLD_H
#define Z2_WORLD_H

#include "Tile.h"
#include "Player.h"
#include "Point.h"
#include "../entity/GameUnit.h"
#include "../core/Message.h"
#include "../core/messages/GameMessage.h"
#include <vector>
namespace z2{
/**
 * 记录游戏中的世界，左下角坐标为(0,0)。
 */
class World{
private:
    int width = 1;
    int height = 1;
    /**
     * The map of the world.
     */
    Tile** data;
    /**
     * The players of this game.
     */
    vector<Player> players;

    /**
     * The current player of this world.
     */
    int currentPlayer = 0;
    /**
     * The unique object id counter.
     *
     * Note: If this world is created from saves, this field must be set.
     */
    int objectUID = 0;

    void initPlainDataFrom(const World &world);

public:
    World(int width_,int height_);

    World(const World& world);

    World& operator=(const World& world);

    World(World&& world) noexcept;

    World& operator=(World&& world) noexcept;

    ~World();



    const vector<Player> &getPlayers() const;

    void setPlayers(const vector<Player> &players_);

    int getPlayerCount() const;

    Tile& getTile(int x,int y) const;

    Tile& getTile(const Point& pos) const;

    bool isInside(int x, int y) const;

    bool isOccupied(int x, int y) const;

    /**
     * Gets the next object id of this world. Multiple calls of this method will
     * return different unique ids.
     *
     *
     */
    int getNextObjectId();

    int getCurrentPlayer() const;

    /**
     * Resets the current player to the next player.
     */
    int nextPlayer();

    Player& getPlayer(int playerId);

    /**
     * Called when the current player starts his turn.
     */
    void onPlayerTurnStart();
    /**
     * Called when the current player finishes his turn.
     */
    void onPlayerTurnFinish();



    void dealWithMessage(const shared_ptr<GameMessage>& message);

    /**
     * Moves the entity at the tile of the point `from` to `dest`, if `dest` is not occupied.
     * Otherwise, returns `false`
     */
    bool moveEntity(const Point& from, const Point& dest);

    void onEntityMoved(const Point& from, const Point& dest, const shared_ptr<GameUnit>& entity);

    /**
     * Creates an entity at the given position.
     */
    shared_ptr<Entity> createEntity(const Point& pos, const string& entityId, const Properties& prop);
    /**
     * Creates an entity at the given position with no properties.
     */
    shared_ptr<Entity> createEntity(const Point& pos, const string& entityId);
};
}

#endif //Z2_WORLD_H
