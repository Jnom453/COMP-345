#include "Game.h"

#include <cmath>
#include <sstream>

#include "Cell.h"
#include "DirectDamageTower.h"
#include "AreaDamageTower.h"
#include "SlowTower.h"

const double Game::SPAWN_INTERVAL = 1.0;
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

int Game::findTower(int row, int column) const
{
    for (std::size_t i = 0; i < towers.size(); ++i)
        if (towers[i].row == row && towers[i].column == column)
            return static_cast<int>(i);
    return -1;
}

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

bool Game::isCritterOnMap(int index) const
{
    return index >= 0 && index < released
        && status[index] == WALKING
        && wave.getCritter(index).isAlive();
}

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

double Game::distanceToCritter(const PlacedTower& t, int critterIndex) const
{
    double r, c;
    getCritterLocation(critterIndex, r, c);
    double dr = r - t.row;
    double dc = c - t.column;
    return std::sqrt(dr * dr + dc * dc);
}

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
