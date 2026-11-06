// The rules of the playable game. There is no drawing code here: game_main.cpp
// draws what this file computes. The big picture is:
//   - the player places towers (placeTower, upgradeTower, sellTower),
//   - startWave() asks CritterGroupGenerator for a wave of critters,
//   - update() is called many times per second and, each time, releases critters, moves them, lets towers shoot, pays rewards and takes lives.
#include "Game.h"

#include <cmath>
#include <sstream>

#include "Cell.h"
#include "DirectDamageTower.h"
#include "AreaDamageTower.h"
#include "SlowTower.h"

// One critter of a wave appears every second.
const double Game::SPAWN_INTERVAL = 1.0;
// Critters move one step every 0.2 seconds.
const double Game::MOVE_INTERVAL = 0.2;

Game::Game()
    : map(12, 10),
      player(STARTING_COINS),
      released(0),
      spawnTimer(0.0),
      moveTimer(0.0),
      lives(STARTING_LIVES),
      waveNumber(1),
      state(PLANNING)
{
    reset();
}

// Builds the one map of the game: a snake-shaped path with the entry on the
// left and the exit on the right. The path cells are also saved, in walking
// order, in the "path" list so a critter's position can be turned into a cell.
void Game::buildMap()
{
    map = Map(12, 10);
    path.clear();

    // A snake-shaped path from the entry (left) to the exit (right).
    for (int c = 0; c <= 10; ++c) path.push_back(std::make_pair(1, c));
    for (int r = 2; r <= 4; ++r)  path.push_back(std::make_pair(r, 10));
    for (int c = 9; c >= 1; --c)  path.push_back(std::make_pair(4, c));
    for (int r = 5; r <= 7; ++r)  path.push_back(std::make_pair(r, 1));
    for (int c = 2; c <= 11; ++c) path.push_back(std::make_pair(7, c));

    for (std::size_t i = 0; i < path.size(); ++i)
        map.setCell(path[i].first, path[i].second, PATH);

    map.setEntry(path.front().first, path.front().second);
    map.setExit(path.back().first, path.back().second);
}

// Puts the game back in its starting state (used by the constructor and by R).
void Game::reset()
{
    buildMap();
    player = Player(STARTING_COINS);
    towers.clear();
    wave = CritterGroup();
    status.clear();
    maxHitPoints.clear();
    shots.clear();
    released = 0;
    spawnTimer = 0.0;
    moveTimer = 0.0;
    lives = STARTING_LIVES;
    waveNumber = 1;
    state = PLANNING;
    message = "Place towers on the green cells, then press SPACE to start wave 1.";
}

// The cost and name of a tower type come from the tower classes themselves,
// so the numbers are only written once (in DirectDamageTower, AreaDamageTower, SlowTower).
int Game::towerCost(int type)
{
    switch (type)
    {
    case AREA_DAMAGE: return AreaDamageTower().getBuyingCost();
    case SLOW:        return SlowTower().getBuyingCost();
    default:          return DirectDamageTower().getBuyingCost();
    }
}

std::string Game::towerName(int type)
{
    switch (type)
    {
    case AREA_DAMAGE: return AreaDamageTower().getName();
    case SLOW:        return SlowTower().getName();
    default:          return DirectDamageTower().getName();
    }
}

// Returns the index of the tower on a cell, or -1 if the cell has none.
int Game::findTower(int row, int column) const
{
    for (std::size_t i = 0; i < towers.size(); ++i)
        if (towers[i].row == row && towers[i].column == column)
            return static_cast<int>(i);
    return -1;
}

// Rules for placing a tower: the game must be running, the cell must be
// scenery (Cell rule: towers only go on scenery), the cell must be empty,
// and the player must be able to pay the buying cost.
bool Game::placeTower(int row, int column, int type)
{
    if (state == GAME_OVER || state == VICTORY)
        return false;

    if (!map.isInside(row, column) || map.getCellType(row, column) != SCENERY)
    {
        message = "Towers can only be placed on green (scenery) cells.";
        return false;
    }
    if (findTower(row, column) >= 0)
    {
        message = "There is already a tower on that cell.";
        return false;
    }

    PlacedTower placed;
    switch (type)
    {
    case AREA_DAMAGE: placed.tower.reset(new AreaDamageTower()); break;
    case SLOW:        placed.tower.reset(new SlowTower()); break;
    default:          placed.tower.reset(new DirectDamageTower()); type = DIRECT_DAMAGE; break;
    }

    if (!player.spendCoins(placed.tower->getBuyingCost()))
    {
        message = "Not enough coins for a " + placed.tower->getName() + ".";
        return false;
    }

    placed.row = row;
    placed.column = column;
    placed.cooldown = 0.0;
    placed.type = type;
    message = "Placed a " + placed.tower->getName() + ".";
    towers.push_back(std::move(placed));
    return true;
}

// The upgrade itself (cost = buying cost x level, +5 power, +1 range, max level 3)
// is done by Tower::upgrade(). This function only finds the tower and reports
// why an upgrade failed.
bool Game::upgradeTower(int row, int column)
{
    if (state == GAME_OVER || state == VICTORY)
        return false;

    int index = findTower(row, column);
    if (index < 0)
    {
        message = "Select a tower first.";
        return false;
    }
    Tower& tower = *towers[index].tower;
    if (tower.getLevel() >= 3)
    {
        message = "That tower is already at the maximum level.";
        return false;
    }
    if (!tower.upgrade(player))
    {
        message = "Not enough coins to upgrade.";
        return false;
    }
    message = "Upgraded to level " + std::to_string(tower.getLevel()) + ".";
    return true;
}

// Removes the tower and gives the player its refund value.
bool Game::sellTower(int row, int column)
{
    if (state == GAME_OVER || state == VICTORY)
        return false;

    int index = findTower(row, column);
    if (index < 0)
    {
        message = "Select a tower first.";
        return false;
    }
    int refund = towers[index].tower->getRefundValue();
    player.addCoins(refund);
    towers.erase(towers.begin() + index);
    message = "Sold the tower for " + std::to_string(refund) + " coins.";
    return true;
}

// Asks the generator for the critters of the current wave and remembers their
// starting hit points (for the health bars). No critter is on the map yet:
// update() releases them one by one.
bool Game::startWave()
{
    if (state != PLANNING)
        return false;

    wave = generator.generateWave(waveNumber);
    status.assign(wave.getSize(), WALKING);
    maxHitPoints.clear();
    for (int i = 0; i < wave.getSize(); ++i)
        maxHitPoints.push_back(wave.getCritter(i).getHitPoints());

    released = 0;
    spawnTimer = SPAWN_INTERVAL; // first critter appears immediately
    moveTimer = 0.0;
    state = WAVE_RUNNING;
    message = "Wave " + std::to_string(waveNumber) + " started!";
    return true;
}

// A critter is "on the map" if it was released, is still alive and has not escaped.
bool Game::isCritterOnMap(int index) const
{
    return index >= 0 && index < released
        && status[index] == WALKING
        && wave.getCritter(index).isAlive();
}

// A critter stores its progress as a number from 0 (entry) to 100 (exit).
// This turns that number into a point between two cells of the path,
// so the window can draw it and the towers can measure the distance to it.
void Game::getCritterLocation(int index, double& row, double& column) const
{
    double position = wave.getCritter(index).getPosition();
    if (position < 0.0) position = 0.0;
    if (position > 100.0) position = 100.0;

    double t = position / 100.0 * (static_cast<double>(path.size()) - 1.0);
    std::size_t i = static_cast<std::size_t>(t);
    if (i >= path.size() - 1)
    {
        row = path.back().first;
        column = path.back().second;
        return;
    }
    double f = t - static_cast<double>(i);
    row = path[i].first + (path[i + 1].first - path[i].first) * f;
    column = path[i].second + (path[i + 1].second - path[i].second) * f;
}

// Straight-line distance, in cells, between a tower and a critter.
double Game::distanceToCritter(const PlacedTower& t, int critterIndex) const
{
    double r, c;
    getCritterLocation(critterIndex, r, c);
    double dr = r - t.row;
    double dc = c - t.column;
    return std::sqrt(dr * dr + dc * dc);
}

// Rule: a critter that reaches the exit costs one life and steals coins equal
// to its strength. At 0 lives the game is over.
void Game::escapeCritter(int index)
{
    wave.getCritter(index).reachExit();
    status[index] = ESCAPED;
    --lives;
    player.loseCoins(wave.getCritter(index).getStrength());
    message = "A critter reached the exit! You lost a life.";
    if (lives <= 0)
    {
        lives = 0;
        state = GAME_OVER;
        message = "GAME OVER - press R to restart.";
    }
}

// Rule: killing a critter gives its reward in coins (paid once per critter).
void Game::collectKills()
{
    for (int i = 0; i < wave.getSize(); ++i)
    {
        if (status[i] == WALKING && i < released && !wave.getCritter(i).isAlive())
        {
            status[i] = KILLED;
            player.addCoins(wave.getCritter(i).getReward());
        }
    }
}

// Each tower has a cooldown (1 / rate of fire). When it is ready it looks for
// critters inside its range:
//   - AreaDamageTower: hits every critter in range (attackMultiple),
//   - the other towers: hit the critter closest to the exit (the one furthest
//     along the path); SlowTower::attack also slows it.
void Game::fireTowers(double seconds)
{
    for (std::size_t k = 0; k < towers.size(); ++k)
    {
        PlacedTower& t = towers[k];
        t.cooldown -= seconds;
        if (t.cooldown > 0.0)
            continue;

        // Critters of the wave that are within range of this tower.
        std::vector<int> inRange;
        for (int i = 0; i < wave.getSize(); ++i)
            if (isCritterOnMap(i) && distanceToCritter(t, i) <= t.tower->getRange())
                inRange.push_back(i);

        if (inRange.empty())
        {
            t.cooldown = 0.0; // ready as soon as something comes in range
            continue;
        }

        // The critter closest to the exit is the main target.
        int target = inRange[0];
        for (std::size_t j = 1; j < inRange.size(); ++j)
            if (wave.getCritter(inRange[j]).getPosition() >
                wave.getCritter(target).getPosition())
                target = inRange[j];

        AreaDamageTower* area = dynamic_cast<AreaDamageTower*>(t.tower.get());
        if (area != NULL)
        {
            std::vector<Critter*> targets;
            for (std::size_t j = 0; j < inRange.size(); ++j)
            {
                targets.push_back(&wave.getCritter(inRange[j]));
                double r, c;
                getCritterLocation(inRange[j], r, c);
                Shot s = { static_cast<double>(t.row), static_cast<double>(t.column), r, c, 0.12 };
                shots.push_back(s);
            }
            area->attackMultiple(targets);
        }
        else
        {
            t.tower->attack(wave.getCritter(target));
            double r, c;
            getCritterLocation(target, r, c);
            Shot s = { static_cast<double>(t.row), static_cast<double>(t.column), r, c, 0.12 };
            shots.push_back(s);
        }

        t.cooldown = 1.0 / t.tower->getRateOfFire();
    }
}

// A wave is over when every critter has been killed or has escaped.
// Then the game is won (after the last wave) or goes back to PLANNING.
void Game::checkWaveEnd()
{
    if (state != WAVE_RUNNING || released < wave.getSize())
        return;
    for (int i = 0; i < wave.getSize(); ++i)
        if (status[i] == WALKING)
            return;

    if (waveNumber >= MAX_WAVES)
    {
        state = VICTORY;
        message = "VICTORY! You survived all waves. Press R to play again.";
        return;
    }
    ++waveNumber;
    state = PLANNING;
    message = "Wave cleared! Build or upgrade, then press SPACE for wave "
              + std::to_string(waveNumber) + ".";
}

// Called every frame with the time since the last frame. Order of work:
// 1) fade old shot lines, 2) release the next critter, 3) move the critters,
// 4) towers shoot, 5) pay rewards for kills, 6) check whether the wave ended.
void Game::update(double seconds)
{
    for (std::size_t i = 0; i < shots.size();)
    {
        shots[i].timeLeft -= seconds;
        if (shots[i].timeLeft <= 0.0) shots.erase(shots.begin() + i);
        else ++i;
    }

    if (state != WAVE_RUNNING)
        return;

    // Release the next critter of the wave.
    if (released < wave.getSize())
    {
        spawnTimer += seconds;
        if (spawnTimer >= SPAWN_INTERVAL)
        {
            spawnTimer = 0.0;
            ++released;
        }
    }

    // Move the critters.
    moveTimer += seconds;
    while (moveTimer >= MOVE_INTERVAL && state == WAVE_RUNNING)
    {
        moveTimer -= MOVE_INTERVAL;
        for (int i = 0; i < released; ++i)
        {
            if (!isCritterOnMap(i))
                continue;
            wave.getCritter(i).move();
            if (wave.getCritter(i).getPosition() >= 100.0)
            {
                escapeCritter(i);
                if (state != WAVE_RUNNING)
                    return;
            }
        }
    }

    fireTowers(seconds);
    collectKills();
    checkWaveEnd();
}

// The functions below are simple getters used by game_main.cpp to draw the game.
const Map& Game::getMap() const { return map; }
const std::vector<std::pair<int, int> >& Game::getPath() const { return path; }
int Game::getCoins() const { return player.getCoins(); }
int Game::getLives() const { return lives; }
int Game::getWave() const { return waveNumber; }
Game::State Game::getState() const { return state; }
const std::string& Game::getMessage() const { return message; }
int Game::getTowerCount() const { return static_cast<int>(towers.size()); }
const Game::PlacedTower& Game::getPlacedTower(int index) const { return towers.at(index); }
int Game::getCritterCount() const { return wave.getSize(); }
const Critter& Game::getCritter(int index) const { return wave.getCritter(index); }
int Game::getCritterMaxHitPoints(int index) const { return maxHitPoints.at(index); }
int Game::getCritterStatus(int index) const { return status.at(index); }
const std::vector<Game::Shot>& Game::getShots() const { return shots; }