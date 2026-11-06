#ifndef PLAYER_H
#define PLAYER_H

/**
 * @file Player.h
 * @brief The player's coins.
 */

/**
 * @brief Holds the player's coins, the currency of the game.
 *
 * Game rules:
 * - Coins are spent to buy and upgrade towers.
 * - Coins are earned by killing critters.
 * - Coins are stolen by critters that reach the exit.
 * - The player cannot spend coins he does not have, and the balance never
 *   goes below 0.
 */
class Player
{
private:
    /** @brief Current number of coins. */
    int coins;

public:
    /**
     * @brief Creates a player.
     * @param startingCoins Coins the player starts with.
     */
    Player(int startingCoins);

    /** @brief Returns the current number of coins. */
    int getCoins() const;

    /**
     * @brief Pays an amount, for example to buy or upgrade a tower.
     * @param amount Coins to pay.
     * @return true if the payment was made, false (and no change) if the
     *         amount is negative or larger than the balance.
     */
    bool spendCoins(int amount);

    /**
     * @brief Adds coins, for example the reward for a killed critter.
     * @param amount Coins to add. Zero or negative amounts are ignored.
     */
    void addCoins(int amount);

    /**
     * @brief Removes coins, for example when a critter reaches the exit.
     *
     * The balance never goes below 0.
     *
     * @param amount Coins to remove.
     * @return true if the full amount was removed, false if the player had
     *         less and the balance was set to 0.
     */
    bool loseCoins(int amount);
};

#endif
