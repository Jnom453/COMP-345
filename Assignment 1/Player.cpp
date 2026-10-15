#include "Player.h"

Player::Player(int startingCoins)
{
    coins = startingCoins;
}

int Player::getCoins() const
{
    return coins;
}

bool Player::spendCoins(int amount)
{
    if (amount < 0)
    {
        return false;
    }

    if (coins < amount)
    {
        return false;
    }

    coins = coins - amount;

    return true;
}

void Player::addCoins(int amount)
{
    if (amount > 0)
    {
        coins = coins + amount;
    }
}

bool Player::loseCoins(int amount)
{
    if (amount <= 0)
    {
        return true;
    }

    if (coins < amount)
    {
        coins = 0;
        return false;
    }

    coins = coins - amount;

    return true;
}