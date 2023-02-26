#include <iostream>
#include "deck.h"
#include <cmath>

using namespace std;

const int BJ = 21;
int playHand(Deck  &d,int chipStack);
bool isBlackjack(char c1, char c2);
int getHandMaxScore(vector<char> cards);
int getHandMinScore(vector<char> cards);
int playPlayerHand(char c1, char c2, int betSize, Deck &deck, int chipStack, bool &DD);
void outputHand(vector<char> hand);
bool firstHand = true;

int main()
{
    Deck d;
    int chips = 500;
    int delta;

    cout << "Would you like to play the tutorial? Enter 1 for yes, or anything else for no\n";
    char input;
    cin >> input;

    if (input == '1') {
        cout << "The first step is to choose how much to bet. For now, we will bet 10 each hand";
        cout << "\nYou have a K and a 2 so your score is 12, the dealer shows a 7. Now you can decide if you want to double down, hit or stand\n";
        cout << "If you double down, your bet doubles and you draw 1 more card\n";
        cout << "If you hit, you draw another card\n";
        cout << "If you stand, your turn is over\n";
        cout << "Be careful, you want to score as much as possible to beat the dealer, but if you go past 21, you automatically lose (bust)\n";
        cout << "Enter 'h' to hit\n";
        cin >> input;
        cout << "You drew an 8. Your score is 20 which is really good, go ahead and enter any value except h, so you can stand\n";
        cout << "While normally it is allowed to hit, its not a good idea because most likely you will go past 21 and bust\n";
        cin >> input;
        cout << "The dealer's 2nd card is a 6, so he has 13 points. He will hit to see if he can beat you.\n";
        cout << "The dealer draws a 6, he now has 19 points. The rules state if he has 17 or more he must stand\n";
        cout << "since your score is higher, you win! That's +10 chips for you!\n";
        cout << "Onto the next hand, you have a Q and a Q , and the dealer is showing a 2. This is great, go ahead and enter 1 to split\n";
        cin >> input;
        cout << "Your first hand you have a Q and a 2, go ahead and enter D to double down\n";
        cin >> input;
        cout << "You drew a 9, that's a perfect 21 so you automatically stand\n";
        cout <<"Your second hand you have a Q and a J, thats 20 so enter anything except D or H to stand\n";
        cin >> input;
        cout << "the dealer's next card is a K, he has 12 points\n";
        cout << "he draws a K, he has 22 points and he busts! You win both your hands, thats +20 chips!\n";
        cout << "Lets play another, you have a J and a 3, and...oh no the dealer has an ace and a king, thats blackjack\n";
        cout << "Since he has blackjack you lose, thats -10\n";
        cout << "next hand the dealer has a 3, and you have an ace and a ten, thats blackjack!\n";
        cout << "blackjack pays 3:2 so thats +15 chips!\n";
        cout << "1 final hand now, you have an 8 and a 2, the dealer has as 7. Go ahead and enter d to double down\n";
        cin >> input;
        cout << "you draw a 7 and since you doubled down your turn is over\n";
        cout << "the dealer has a 5, so he hits\n";
        cout << "the dealer draws a J and busts, thats plus 20 for you!\n";
        cout << "that is everything you need to know, good luck!\n\n\n\n\n";
    }

    while (true) {
        delta = playHand(d, chips);
        firstHand = false;
        chips += delta;

        if (delta < 0) {
            cout << "\nyou lost " << delta << " chips";
        }
        else if (delta == 0) {
            cout << "\nits a push";
        }
        else {
            cout << "\nyou won " << delta << " chips";
        }

        if (chips < 1) {
            cout << "\nYou're broke! Game over!\n";
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
    if (!firstHand) {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    cin >> betSize;

    if (cin.fail() || betSize < 0 || betSize > chipStack) {
        cout << "please enter valid input!";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return 0;
    }

    //deal the player and dealer 2 cards each


    //rigs for pair for testing ONLY
    //This was helpful to make sure splitting works
    //If you need to test splitting uncomment this and comment out the 4 lines below
    /*
    char c = deck.getNextCard();
    pHand.push_back(c);
    pHand.push_back(c);
    dHand.push_back(deck.getNextCard());
    dHand.push_back(deck.getNextCard());
    */

    pHand.push_back(deck.getNextCard());
    pHand.push_back(deck.getNextCard());
    dHand.push_back(deck.getNextCard());
    dHand.push_back(deck.getNextCard());


    bool dealerBJ = isBlackjack(dHand.at(0), dHand.at(1));
    bool playerBJ = isBlackjack(pHand.at(0), pHand.at(1));

    cout << "\nyou have " << pHand[0] << ", " << pHand[1];
    cout << "\ndealer shows " << dHand[0] << endl;

    int pMinScore = getHandMinScore(pHand);
    int pMaxScore = getHandMaxScore(pHand);
    if (pMaxScore == pMinScore) {
        cout << "your score is " << std::to_string(pMaxScore);
    }
    else {
        cout << "your score is " << std::to_string(pMaxScore) << "/" << std::to_string(pMinScore);
    }

    if (dealerBJ && playerBJ) {
        cout << "\nDealer shows his other card is " << dHand[1] << ". You both have blackjack. Its a push";
        return 0;
    }
    else if (dealerBJ) {
        cout << "\nDealer shows his other card is " << dHand[1] << ". Dealer has blackjack. You lose!";
        return -1 * betSize;
    } else if (playerBJ) {
        int chipsWon = betSize * 1.5;
        cout <<"\nYou have blackjack! You win " + std::to_string(chipsWon);
        return chipsWon;
    }

    bool split = false;

    if (pHand[0] == pHand[1] && chipStack >= betSize * 2) {
        cout << "\nWould you like to split? Enter 1 for yes, or anything else for no\n";
        char input;
        cin >> input;

        split = (input == '1');

        if (split) {
            pHand.push_back(deck.getNextCard());
            pHand.push_back(deck.getNextCard());

            //take the second card of the first hand and swap it with the first card of the second hand to properly split
            //for example if first hand is 88, and the next is KQ, it will become 8K and 8Q
            //It doesn't matter which card swaps with which, so long as each hand has 1 of the old cards and 1 of the new
            std::swap(pHand[1], pHand[2]);
            }
    }


    int playerScore1;
    int playerScore2 = -1;
    bool hand1DD;
    bool hand2DD;

    if (split) {
        cout << "\nFirst play hand 1\n";
        vector<char> newHand;
        newHand.push_back(pHand[0]);
        newHand.push_back(pHand[1]);

        int nMinScore = getHandMinScore(newHand);
        int nMaxScore = getHandMaxScore((newHand));
        outputHand(newHand);
        if (nMaxScore == nMinScore) {
            cout << ", your score is " << std::to_string(nMinScore);
        }
        else {
            cout << ", your score is " << std::to_string(nMinScore) << "/" << std::to_string(nMaxScore);
        }


    }
        playerScore1 = playPlayerHand(pHand[0], pHand[1], betSize, deck, chipStack, hand1DD);
    if (split) {
        cout << "\nNow play hand 2\n";
        vector<char> newHand;
        newHand.push_back(pHand[2]);
        newHand.push_back(pHand[3]);

        int nMinScore = getHandMinScore(newHand);
        int nMaxScore = getHandMaxScore((newHand));
        outputHand(newHand);
        if (nMaxScore == nMinScore) {
            cout << ", your score is " << std::to_string(nMinScore);
        }
        else {
            cout << ", your score is " << std::to_string(nMinScore) << "/" << std::to_string(nMaxScore);
        }
        playerScore2 = playPlayerHand(pHand[2], pHand[3], betSize, deck, chipStack, hand2DD);
    }

    //the dealer reveals their score, if their score is 17 or greater, they stand
    //if their score is below 17, they draw another card
    //if their new score is above 21 they bust and their score is set to -1, if they didnt bust and are still below 17 they continue
    cout << "\ndealer holdings: ";
    outputHand(dHand);
    cout << endl;

    int dMaxScore = getHandMaxScore(dHand);
    int dMinScore = getHandMinScore(dHand);
    int dealerScore;

    //if the player has at least 1 hand that did not bust
    bool dealerDraw = (playerScore1 > 1 || (split && playerScore2 > 1));

    while (true) {
        if (!dealerDraw) {
            dealerScore = 2;
            break;
        }

        if (dMaxScore > BJ)
        {
            if (dMinScore > BJ) {
                cout << "\nThe dealer busts";
                dealerScore = -1;
                break;

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


    //at this point the dealer has either stood or busted
    int winnings = 0;
    int betSize1;
    int betSize2;

    if (hand1DD) {
        betSize1 = betSize * 2;
    }
    else {
        betSize1 = betSize;
    }

    if (hand2DD) {
        betSize2 = betSize * 2;
    }
    else {
        betSize2 = betSize;
    }

    if (playerScore1 < 1 || playerScore1 < dealerScore) {
        winnings = -1 * betSize1;
    }
    //if the player scored higher
    else if (playerScore1 > dealerScore) {
        //BJ pays 3:2, or 1.5x
        if (playerScore1 == BJ) {
            winnings+= betSize1 * 1.5;
        }
        else {
            winnings+= betSize1;
        }
    }

    if (split) {
        if (playerScore2 < 1 || playerScore2 < dealerScore) {
            winnings = -1 * betSize2;
        }
        //if the player scored higher
        else if (playerScore2 > dealerScore) {
            //BJ pays 3:2, or 1.5x
            if (playerScore2 == BJ) {
                winnings+= betSize2 * 1.5;
            }
            else {
                winnings+= betSize2;
            }
        }
    }

    return winnings;
}

void outputHand(vector<char> hand) {
    string cards = "";

    for (int i = 0; i < hand.size(); i++) {
        cards += hand[i];
        if (i + 1 < hand.size()) {
            cards += ", ";
        }
    }
    cout << cards;
}
int playPlayerHand(char c1, char c2, int betSize, Deck &deck, int chipStack, bool &DD) {
    int pMinScore;
    int pMaxScore;
    int playerScore;
    vector<char> pHand;
    pHand.push_back(c1);
    pHand.push_back(c2);

    cout << "\nbet size  " << betSize << endl;
    while(true) {
        string message;
        bool canDouble = (chipStack >= betSize * 2 && pHand.size() == 2);

        if (canDouble) {
            message = "\nEnter d to double down, h to hit, or anything else to stand\n";
        }
        else {
            message = "\nEnter h to hit, or anything else to stand\n";
        }

        cout << message;
        char input;
        cin >> input;

        if ( (input == 'd' || input == 'D') && canDouble) {
            betSize*= 2;
            cout << "\nYour bet is now " << std::to_string((betSize)) << endl;
            pHand.push_back(deck.getNextCard());
            pMaxScore = getHandMaxScore(pHand);
            pMinScore = getHandMinScore(pHand);
            DD = true;

            outputHand(pHand);

            if (pMaxScore > BJ) {
                if (pMinScore > BJ) {
                    cout << "\nyou bust!";
                    playerScore = -1;
                    break;
                }
                else if (pMinScore == BJ) {
                    cout << "\nYour score is 21, you stand";
                    playerScore = BJ;
                    break;
                }
                else {
                    playerScore = pMinScore;
                }
            }
            else {
                playerScore = pMaxScore;
            }

            cout << "\nyou have ";
            outputHand(pHand);
            if (playerScore == pMinScore) {
                cout << ", your score is " << std::to_string(playerScore);
            }
            else {
                cout << ", your score is " << std::to_string(playerScore) << "/" << std::to_string(pMinScore);
            }

            cout << "\nsince you double down'd, your turn is over\n";
            break;


        }
        else if (input == 'h' || input == 'H') {
            pHand.push_back(deck.getNextCard());
            pMaxScore = getHandMaxScore(pHand);
            pMinScore = getHandMinScore(pHand);

            outputHand(pHand);

            if (pMaxScore == pMinScore) {
                cout << ", your score is " << std::to_string(pMaxScore);
            }
            else {
                cout << ", your score is " << std::to_string(pMaxScore) << "/" << std::to_string(pMinScore);
            }


            int score;

            if (pMaxScore > BJ) {
                if (pMinScore > BJ) {
                    cout << "\nyou bust!";
                    playerScore = -1;
                    break;
                } else if (pMinScore == BJ) {
                    cout << "\nYour score is 21, you stand";
                    playerScore = BJ;
                    break;
                }
            }
        }
        else {
            //stand
            int score;
            pMaxScore = getHandMaxScore(pHand);
            pMinScore = getHandMinScore(pHand);
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


    return playerScore;
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
