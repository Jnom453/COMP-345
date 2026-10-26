#ifndef PLAYER_H
#define PLAYER_H

class Player
{
private:
    int coins;

public:
    Player(int startingCoins);

    int getCoins() const;

    bool spendCoins(int amount);

    void addCoins(int amount);

    bool loseCoins(int amount);
};

#endif