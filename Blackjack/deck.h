#ifndef DECK_H
#define DECK_H
#include <vector>


class Deck
{
    //generally, blackjack is played with 6 decks shuffled together
    const static int deckCount = 6;
    //the threshhold for which the deck will be reshuffled, although there is no exact standard, generally the deck is reshuffled
    //when around 1.5-2 decks are left, 78 is 1.5 so that is the threshhold for reshuffling
    const static int shuffleThreshold = 78;
    std::vector<char> deck;
    void shuffle();


public:
    Deck();
    char getNextCard();
    int getDeckCount();
    //checks if the deck should be shuffled, and if so it shuffles. Should be called in between hands
    void shuffleCheck();

};

#endif // DECK_H
