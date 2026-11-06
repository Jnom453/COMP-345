#ifndef GAME_H
#define GAME_H

#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "Map.h"
#include "Player.h"
#include "Tower.h"
#include "CritterGroup.h"
#include "CritterGroupGenerator.h"

/**
 * @file Game.h
 * @brief The playable game: rules and game loop, without any graphics.
 */

/**
 * @brief The rules and the game loop of the playable Tower Defense game.
 *
 * Game owns the Map, the Player, the towers the player placed and the current
 * wave of critters. It uses the classes written for the earlier parts of the
 * project (Map, Tower and its subclasses, Critter, CritterGroup,
 * CritterGroupGenerator, Player). It knows nothing about SFML, so the whole
 * game can be tested from the console. The window is drawn by game_main.cpp.
 *
 * Rules:
 * - The player starts with STARTING_COINS coins and STARTING_LIVES lives.
 * - Towers are bought and placed on scenery cells, one tower per cell.
 *   A tower can be upgraded (up to level 3) or sold for its refund value.
 * - startWave() sends the next wave. Critters appear one after the other at
 *   the entry and walk along the path to the exit.
 * - Each tower shoots at critters within its range, at its rate of fire.
 *   A DirectDamageTower hits one critter, an AreaDamageTower hits every
 *   critter in range, and a SlowTower hits one critter and slows it.
 * - Killing a critter gives its reward in coins.
 * - A critter that reaches the exit costs the player one life and steals
 *   coins equal to its strength.
 * - The game is lost when the lives reach 0 and won when the player survives
 *   all MAX_WAVES waves.
 */
class Game
{
public:
    /** @brief Coins the player starts with. */
    static const int STARTING_COINS = 350;
    /** @brief Critters that may reach the exit before the game is lost, plus one. */
    static const int STARTING_LIVES = 10;
    /** @brief Number of waves the player must survive to win. */
    static const int MAX_WAVES = 10;

    /** @brief Seconds between two critters of the same wave appearing. */
    static const double SPAWN_INTERVAL;
    /** @brief Seconds between two moves of the critters. */
    static const double MOVE_INTERVAL;

    /** @brief What the game is currently doing. */
    enum State
    {
        PLANNING,      /**< Between waves: the player can build and start a wave. */
        WAVE_RUNNING,  /**< A wave is walking along the path. */
        GAME_OVER,     /**< The lives reached 0. */
        VICTORY        /**< All waves were survived. */
    };

    /** @brief The tower types the player can buy. */
    enum TowerType
    {
        DIRECT_DAMAGE = 0, /**< DirectDamageTower. */
        AREA_DAMAGE = 1,   /**< AreaDamageTower. */
        SLOW = 2           /**< SlowTower. */
    };

    /** @brief What happened to a critter of the current wave. */
    enum CritterStatus
    {
        WALKING = 0, /**< Still on the map (or not yet released). */
        KILLED = 1,  /**< Killed by a tower; the reward was paid. */
        ESCAPED = 2  /**< Reached the exit. */
    };

    /** @brief A tower placed on the map. */
    struct PlacedTower
    {
        std::unique_ptr<Tower> tower; /**< The tower itself. */
        int row;                      /**< Row of its cell. */
        int column;                   /**< Column of its cell. */
        double cooldown;              /**< Seconds until it can shoot again. */
        int type;                     /**< A TowerType value. */
    };

    /** @brief A short line drawn when a tower shoots, for the display only. */
    struct Shot
    {
        double fromRow;   /**< Row of the tower. */
        double fromColumn;/**< Column of the tower. */
        double toRow;     /**< Row of the target. */
        double toColumn;  /**< Column of the target. */
        double timeLeft;  /**< Seconds before the line disappears. */
    };

    /** @brief Creates a new game on the built-in map. */
    Game();

    /** @brief Starts over: new map, coins, lives, no towers, wave 1. */
    void reset();

    /**
     * @brief Buys a tower and places it on a cell.
     *
     * Fails (and sets the message) if the game is over, the cell is outside
     * the map, is not scenery, already has a tower, or the player cannot pay.
     *
     * @param row    Row of the cell.
     * @param column Column of the cell.
     * @param type   A TowerType value.
     * @return true if the tower was placed.
     */
    bool placeTower(int row, int column, int type);

    /**
     * @brief Upgrades the tower on a cell (see Tower::upgrade()).
     * @return true if the tower was upgraded.
     */
    bool upgradeTower(int row, int column);

    /**
     * @brief Sells the tower on a cell and gives its refund value to the player.
     * @return true if a tower was sold.
     */
    bool sellTower(int row, int column);

    /**
     * @brief Starts the next wave. Only possible while PLANNING.
     * @return true if the wave started.
     */
    bool startWave();

    /**
     * @brief Advances the game by a time step.
     *
     * Releases critters, moves them, makes the towers shoot, pays rewards,
     * takes lives and ends the wave or the game when needed. Does nothing
     * unless a wave is running.
     *
     * @param seconds Time elapsed since the previous call.
     */
    void update(double seconds);

    /** @brief Returns the map. */
    const Map& getMap() const;
    /** @brief Returns the path cells in walking order, as (row, column) pairs. */
    const std::vector<std::pair<int, int> >& getPath() const;

    /** @brief Returns the player's coins. */
    int getCoins() const;
    /** @brief Returns the lives left. */
    int getLives() const;
    /** @brief Returns the number of the next wave, or the running one. */
    int getWave() const;
    /** @brief Returns the current State. */
    State getState() const;
    /** @brief Returns the last message for the player (errors, events). */
    const std::string& getMessage() const;

    /** @brief Returns the number of placed towers. */
    int getTowerCount() const;
    /** @brief Returns a placed tower by index. */
    const PlacedTower& getPlacedTower(int index) const;
    /** @brief Returns the index of the tower on a cell, or -1 if there is none. */
    int findTower(int row, int column) const;

    /** @brief Returns the number of critters in the current wave. */
    int getCritterCount() const;
    /** @brief Returns a critter of the current wave. */
    const Critter& getCritter(int index) const;
    /** @brief Returns the hit points the critter started with. */
    int getCritterMaxHitPoints(int index) const;
    /** @brief Returns a CritterStatus value for a critter of the wave. */
    int getCritterStatus(int index) const;
    /** @brief Tells whether a critter is currently on the map (released, alive, not escaped). */
    bool isCritterOnMap(int index) const;

    /**
     * @brief Returns where a critter is on the map, in cell units.
     *
     * The critter's position (0 to 100) is turned into a point along the
     * path, between two neighbouring path cells.
     *
     * @param index Index of the critter.
     * @param row    Receives the row (can be fractional).
     * @param column Receives the column (can be fractional).
     */
    void getCritterLocation(int index, double& row, double& column) const;

    /** @brief Returns the short lines of the shots to draw. */
    const std::vector<Shot>& getShots() const;

    /** @brief Returns the coins a tower type costs. */
    static int towerCost(int type);
    /** @brief Returns the name of a tower type. */
    static std::string towerName(int type);

private:
    Map map;
    Player player;
    std::vector<std::pair<int, int> > path;

    std::vector<PlacedTower> towers;

    CritterGroupGenerator generator;
    CritterGroup wave;
    std::vector<int> status;
    std::vector<int> maxHitPoints;
    int released;
    double spawnTimer;
    double moveTimer;

    int lives;
    int waveNumber;
    State state;
    std::string message;
    std::vector<Shot> shots;

    void buildMap();
    void fireTowers(double seconds);
    void collectKills();
    void escapeCritter(int index);
    void checkWaveEnd();
    double distanceToCritter(const PlacedTower& t, int critterIndex) const;
};

#endif
