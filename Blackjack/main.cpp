#include <iostream>
#include "deck.h"
#include <cmath>

using namespace std;

const int BJ = 21;
int playHand(Deck  &d,int chipStack);
bool isBlackjack(char c1, char c2);
int getHandMaxScore(vector<char> cards);
int getHandMinScore(vector<char> cards);

int main()
{
    Deck d;
    int chips = 500;

    while (true) {
        chips += playHand(d, chips);

        if (chips < 1) {
            cout << "You're broke! Game over!\n";
            exit(0);
        }
        cout << "\nYou have " << chips << " chips. Enter q to quit, or anything else to continue playing\n";
        char c;
        cin >> c;
        if (c == 'q' || c == 'Q') {
            exit(0);
        }
    }

}

int playHand(Deck &deck, int chipStack) {
    int betSize;
    vector<char> pHand;
    vector<char> dHand;
    //dealer stands on 17
    const int dealerStand = 17;


    cout << "How many chips would you like to bet?\n";
    //verify input please
    cin >> betSize;

    if (cin.fail() || betSize < 0 || betSize > chipStack) {
        cout << "please enter valid input!";
        cin.clear();
        return 0;
    }

    //deal the player and dealer 2 cards each
    pHand.push_back(deck.getNextCard());
    pHand.push_back(deck.getNextCard());
    dHand.push_back(deck.getNextCard());
    dHand.push_back(deck.getNextCard());

    bool dealerBJ = isBlackjack(dHand.at(0), dHand.at(1));
    bool playerBJ = isBlackjack(pHand.at(0), pHand.at(1));

    if (dealerBJ || playerBJ) {
        cout <<"\nYou both have blackjack. Its a push";
        return 0;
    }
    else if (dealerBJ) {
        cout << "\nDealer has blackjack. You lose!";
        return -1 * betSize;
    } else if (playerBJ) {
        int chipsWon;
        double chipsWonDb = 1.5 * betSize;
        chipsWon = round(chipsWonDb);

        cout <<"\nYou have blackjack! You win " + std::to_string(chipsWon);
        return chipsWon;
    }

    //if your cards are the same, you may split once
    bool split = false;
    //after the split option, you have the option to hit, stand or double down
    //if you hit, if you reach 21 you stand, if you exceed 21 you bust, otherwise the option repeats
    //if you double down, you double your bet, get 1 more card, and then stand
    //once you stand, you move onto your next hand if split, and the process repeats, otherwise it is the dealers turn


    int playerScore;
    int pMaxScore = getHandMaxScore(pHand);
    int pMinScore = getHandMinScore(pHand);
    while(true) {
        string cards = "";

        for (int i = 0; i < pHand.size(); i++) {
            cards += pHand[i];
            if (i + 1 < pHand.size()) {
                cards += ", ";
            }
        }

        cout << "\nyou have " + cards;
        if (pMaxScore == pMinScore) {
            cout << ", your score is " << std::to_string(pMaxScore);
        }
        else {
            cout << ", your score is " << std::to_string(pMaxScore) << "/" << std::to_string(pMinScore);
        }

        cout << "\nEnter d to double down, h to hit, or anything else to stand\n";
        char input;
        cin >> input;

        if (input == 'd' || input == 'D') {
            betSize*= 2;
            cout << "\nYour bet is now " << std::to_string((betSize));
            pHand.push_back(deck.getNextCard());
            pMaxScore = getHandMaxScore(pHand);
            pMinScore = getHandMinScore(pHand);

            string cards = "";

            for (int i = 0; i < pHand.size(); i++) {
                cards += pHand[i];
                if (i + 1 < pHand.size()) {
                    cards += ", ";
                }
            }

            int score;

            if (pMaxScore > BJ) {
                if (pMinScore > BJ) {
                    cout << "\nyou bust!";
                    return -1 * betSize;
                } else if (pMinScore == BJ) {
                    cout << "\nYour score is 21, you stand";
                    playerScore = BJ;
                    break;
                } else {
                    score = pMinScore;
                }
            } else {
                score = pMaxScore;
            }

            cout << "\nyou have " + cards;
            if (score == pMinScore) {
                cout << ", your score is " << std::to_string(score);
            }
            else {
                cout << ", your score is " << std::to_string(score) << "/" << std::to_string(pMinScore);
            }

            cout << "\nsince you double down'd, your turn is over\n";
            playerScore = score;
            break;


        }
        else if (input == 'h' || input == 'H') {
            pHand.push_back(deck.getNextCard());
            pMaxScore = getHandMaxScore(pHand);
            pMinScore = getHandMinScore(pHand);

            string cards = "";

            for (int i = 0; i < pHand.size(); i++) {
                cards += pHand[i];
                if (i + 1 < pHand.size()) {
                    cards += ", ";
                }
            }

            int score;

            if (pMaxScore > BJ) {
                if (pMinScore > BJ) {
                    cout << "\nyou bust!";
                    return -1 * betSize;
                } else if (pMinScore == BJ) {
                    cout << "\nYour score is 21, you stand";
                    playerScore = BJ;
                    break;
                } else {
                    score = pMinScore;
                }
            } else {
                score = pMaxScore;
            }
        }
        else {
            //stand
            int score;
            if (pMaxScore <= BJ) {
                score = pMaxScore;
            }
            else {
                score = pMinScore;
            }
            cout << "\nyou stand, your score is " << std::to_string(score);
            playerScore = score;
            break;
        }
    }

    //the dealer reveals their score, if their score is 17 or greater, they stand
    //if their score is below 17, they draw another card
    //if their new score is above 21 they bust and their score is set to -1, if they didnt bust and are still below 17 they continue

    string cards = "";

    for (int i = 0; i < dHand.size(); i++) {
        cards += dHand[i];
        if (i + 1 < dHand.size()) {
            cards += ", ";
        }
    }

    cout << "\nthe dealer's holdings are " << cards;

    int dMaxScore = getHandMaxScore(dHand);
    int dMinScore = getHandMinScore(dHand);
    int dealerScore;
    while (true) {
        if (dMaxScore > BJ) {
            if (dMinScore > BJ) {
                cout << "\nThe dealer busts, you win!";
                return 2 * betSize;
            } else if (dMinScore >= dealerStand) {
                //stand
                dealerScore = dMinScore;
                break;
            } else {
                //dealer hits
                char c = deck.getNextCard();
                cout << "\nthe dealer hit, he drew a(n) " << c;
                dHand.push_back(c);
                dMaxScore = getHandMaxScore(dHand);
                dMinScore = getHandMinScore(dHand);
                continue;
            }
        } else if (dMaxScore >= dealerStand) {
            //stand
            dealerScore = dMaxScore;
            break;

        } else {
            //dealer hits
            char c = deck.getNextCard();
            cout << "\nthe dealer hit, he drew a(n) " << c;
            dHand.push_back(c);
            dMaxScore = getHandMaxScore(dHand);
            dMinScore = getHandMinScore(dHand);
            continue;
        }
    }

    if (playerScore == dealerScore) {
        cout << "\nits a push\n";
        return 0;
    } else if (playerScore > dealerScore) {
        cout << "\nyou win!\n";
        return betSize;
    }
    else {
        cout << "\nSorry, you lose.\n";
        return -1 * betSize;
    }

}

int getHandMaxScore(vector<char> cards) {
    int score = 0;

    for (int i = 0; i < cards.size(); i++) {
        if (cards[i] == 'A') {
            score+= 11;
        }
        else if (cards[i] == 'T' || cards[i] == 'J' || cards[i] == 'Q' || cards[i] == 'K') {
            score += 10;
        }
        else {
            //cards[i] - '0' is effectively getting cards[i] as an int, but where '0' is 0, '1' is one etc
            //regular casting would give us the ascii value which is no good
            score += cards[i] - '0';
        }
    }

    return score;
}

int getHandMinScore(vector<char> cards) {
    int score = 0;

    for (int i = 0; i < cards.size(); i++) {
        if (cards[i] == 'A') {
            score+= 1;
        }
        else if (cards[i] == 'T' || cards[i] == 'J' || cards[i] == 'Q' || cards[i] == 'K') {
            score += 10;
        }
        else {
            //cards[i] - '0' is effectively getting cards[i] as an int, but where '0' is 0, '1' is one etc
            //regular casting would give us the ascii value which is no good
            score += cards[i] - '0';
        }
    }

    return score;
}

bool isBlackjack(char c1, char c2) {
    if (c1 == 'A') {
        if (c2 == 'T' || c2 == 'J' || c2 == 'Q' || c2 == 'K' ) {
            return true;
        }
    }

    if (c2 == 'A') {
        if (c1 == 'T' || c1 == 'J' || c1 == 'Q' || c1 == 'K' ) {
            return true;
        }
    }

    return false;
}
