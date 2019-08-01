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
#include "../entity/MeleeUnit.h"
#include "../entity/RangeUnit.h"
#include <vector>
#include <unordered_map>
#include "event/EventDispatcher.h"
namespace z2 {
class CommandLineGameGui;

/**
 * 记录游戏中的世界，左下角坐标为(0,0)。
 */
class World : public Serializable {
private:
    int width = 1;
    int height = 1;


    /**
     * The number of players in this game.
     */
    int playerCount = 1;

    /**
     * The current player of this world.
     */
    int currentPlayer = 0;

    /**
     * The players of this game.
     */
    vector<Player> players;
    /**
     * The unique object id counter.
     *
     * Note: If this world is created from saves, this field must be set.
     */
    unsigned int objectUID = 1;

    unordered_map<int, shared_ptr<Entity>> entityMap;

    /**
     * The map of the world.
     */
    Tile **data;

    /**
     * volatile
     */
    EventDispatcher dispatcher;

    // NOTE: updates `initPlainDataFrom` if new fields are added!



    void initPlainDataFrom(const World &world);

    void resetVisibility(int playerId);

    void setEntityVisibility(int x, int y, const shared_ptr<Entity> &en, int playerId);

    void onEntityMoved(const Point &from, const Point &dest, const shared_ptr<GameUnit> &entity);

    void onEntityCreated(const Point &pos, const shared_ptr<Entity> &entity, const string &entityName, int playerId);

//    void onEntityBought()
    /**
     *
     * @param damage it can be modified
     */
    void onEntityDamaging(const Point &from, const Point &dest, const shared_ptr<Entity> &attacker,
                          const shared_ptr<EntityWithHealth> &receiver,
                          int &damage);

    void removeEntity(const Point &pos);

    void onEntityRemoved(const Point &pos, const shared_ptr<Entity> &entity);
//    void removeEntity(int entityId);

    void onEntityPerformed(const Point &pos, const shared_ptr<Entity> & entityPtr);

    /**
     *
     * @return `true` if the victim is dead
     */
    bool performMeleeAttack(const Point &from, const Point &dest, const shared_ptr<MeleeUnit> &melee,
                            const shared_ptr<EntityWithHealth> &victim);

    bool checkPlayerLostAllUnit(int playerId);

    /**
     * May call `onPlayerGroupWon`.
     */
    void checkPlayerGroupWin();

    void onPlayerDefeated(int playerId);

    void onPlayerGroupWon(int groupId);

public:
    World(int width_, int height_, int playerCount);

    World(const World &world);

    World &operator=(const World &world);

    World(World &&world) noexcept;

    World &operator=(World &&world) noexcept;

    ~World();

    const string &getClassName()const  override;

    int getWidth() const;

    int getHeight() const;


    const vector<Player> &getPlayers() const;

    Player &getPlayer(int playerId);

    Player& getCurrentAsPlayer();

    bool checkReady();

    int getPlayerCount() const;

    /**
     * Gets the tile at the given position. This method won't check whether the position
     * is valid.
     */
    Tile &getTile(int x, int y) const;

    /**
     * Gets the tile at the given position. This method won't check whether the position
     * is valid.
     */
    Tile &getTile(const Point &pos) const;

    shared_ptr<Entity> getEntity(const Point &point);

    /**
     * Determines whether the given coordinate is inside the map.
     */
    bool isInside(int x, int y) const;

    /**
     * Determines whether the given point is inside the map.
     */
    bool isInside(const Point &pos) const;

    bool isOccupied(int x, int y) const;

    /**
     * Computes the path length from `start` to `dest`.
     */
    int pathLength(const Point &start, const Point &dest,shared_ptr<GameUnit>& unit) const;


    /**
     * Gets an adjacent empty tile from the point.
     */
    Tile *getAdjacentEmptyTile(const Point &pos) const;

    /**
     * Gets the position of an adjacent empty tile from the point, if there is
     * no such point, returns a point (-1,-1).
     */
    Point getAdjacentEmptyPos(const Point &pos) const;

    /**
     * Gets the next object id of this world. Multiple calls of this method will
     * return different unique ids.
     */
    int getNextObjectId();

    int getCurrentPlayer() const;

    /**
     * Finds the next available player and then resets the current player to the next player.
     */
    int nextPlayer();

    /**
     * Finds the next available player, taking the current player as candidate as well.
     */
    int nextPlayerFromCurrent();

    /**
     * Called when the current player starts his turn.
     */
    void onPlayerTurnStart();
    
    void onPlayerTurnStart(int playerId);
    
    /**
     * Called when the current player finishes his turn.
     */
    void onPlayerTurnFinish();

    /**
     * Applies the given function to all the entities of the player.
     * @param f a function which accepts the x,y coordinates and the entity in order.
     */
    void forEachEntitiesOf(int playerId, const function<void(int, int, shared_ptr<Entity>&)> &f);

    /**
     * Searches the map for the first entity of the given `entityName`.
     */
    Point searchFor(int playerId, const string& entityName);

    /**
     * Determines whether the two entities are of the same group.
     */
    bool isOfSameGroup(const shared_ptr<Entity>& e1, const shared_ptr<Entity>& e2);

    /**
     * Updates the visibility of a player. If the player id is not valid, this method will do nothing.
     */
    void updateVisibility(int playerId);

    /**
     * Refreshes all the units of the given player.
     */
    void refreshMoves(int playerId);

    /**
     * Deals with the given message.
     */
    void dealWithMessage(const shared_ptr<GameMessage> &message);

    void publishEvent(shared_ptr<GameEvent> &event);


    bool canMove(const Point &from, const Point &dest, int playerId = Player::NO_PLAYER);

    /**
     * Gets the entities that the player can choose to buy, not considering the price.
     */
    vector<string> getAvailableEntitiesFor(int playerId);

    bool canAttack(const Point &from, const Point &dest, int playerId = Player::NO_PLAYER);

    bool canPerform(const Point& target, int playerId = Player::NO_PLAYER);

    /**
     * Moves the entity at the tile of the point `from` to `dest`, if `dest` is not occupied.
     * Otherwise, returns `false`
     */
    bool moveEntity(const Point &from, const Point &dest);

    void buyEntity(int playerId, const Point &pos, const string &entityName);



    void attackEntity(const Point &from, const Point &dest);

    /**
     * Performs a melee attack.
     */
    void attackEntityMelee(const Point &from, const Point &dest, const shared_ptr<MeleeUnit> &range,
                           const shared_ptr<EntityWithHealth> &victim);

    /**
     * Performs a range attack.
     */
    void attackEntityRange(const Point &from, const Point &dest, const shared_ptr<RangeUnit> &range,
                           const shared_ptr<EntityWithHealth> &victim);

    /**
     * Performs the entity at the target position.
     */
    void performEntity(const Point& target);

    /**
     * Creates an entity at the given tile.
     */
    shared_ptr<Entity> createEntity(const Point &, const string &entityId, int playerId);

    /**
     * Creates an entity at the given tile with no properties.
     */
    shared_ptr<Entity> createEntity(const Point &, const string &entityId);

    void addEventListener(const EventListener &listener);

public:


    friend class CommandLineGameGui;


public:

    static const string &className();

    /**
    * Serialize this world to the stream.
    */
    void serializeTo(ostream &output) override;

    /**
     * Loads and creates a new world from an `istream`.
     */
    static World *loadFrom(istream &input);

private:
    void saveTileData(Tile &t, ostream &out);

//    int objectCount()
//    void se(ostream& output);
    void loadTileData(Tile &t, istream &input);

};
}

#endif //Z2_WORLD_H
