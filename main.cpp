#include "raylib.h"
#include <vector>
#include <string>
#include <ctime>
#include <cstdlib>
#include <algorithm>

using namespace std;

// Card structure
struct Card {
    string suit;
    string rank;
    int value;
};

// Shuffle the deck
void ShuffleDeck(vector<Card>& deck) {
    random_shuffle(deck.begin(), deck.end());
}

// Initialize a full deck of cards
vector<Card> InitializeDeck() {
    vector<Card> deck;
    vector<string> suits = { "Hearts", "Spades", "Clubs", "Diamonds" };
    vector<string> ranks = { "2", "3", "4", "5", "6", "7", "8", "9", "10", "Jack", "Queen", "King", "Ace" };
    vector<int> values = { 2, 3, 4, 5, 6, 7, 8, 9, 10, 10, 10, 10, 11 };

    for (const auto& suit : suits) {
        for (size_t i = 0; i < ranks.size(); ++i) {
            Card card;
            card.suit = suit;
            card.rank = ranks[i];
            card.value = values[i];
            deck.push_back(card);
        }
    }
    return deck;
}

// Calculate the total value of a hand
int CalculateHandValue(const vector<Card>& hand) {
    int total = 0;
    int aces = 0;
    for (const auto& card : hand) {
        total += card.value;
        if (card.rank == "Ace") {
            aces++;
        }
    }
    // Adjust for Aces if total exceeds 21
    while (total > 21 && aces > 0) {
        total -= 10;
        aces--;
    }
    return total;
}

// Draw cards on screen with visuals
void DrawHand(const vector<Card>& hand, int x, int y, bool hideSecondCard = false) {
    for (size_t i = 0; i < hand.size(); ++i) {
        if (i == 1 && hideSecondCard) {
            Rectangle hiddenCard = { x + (float)(i * 120), (float)y, 100, 140 };
            DrawRectangleRounded(hiddenCard, 0.1f, 4, DARKGRAY);
            DrawRectangleRoundedLines(hiddenCard, 0.1f, 4, BLACK);
            DrawText("Hidden", x + (int)(i * 120) + 20, y + 50, 20, RAYWHITE);
        }
        else {
            string cardText = hand[i].rank + "\n" + hand[i].suit;
            Rectangle cardRect = { x + (float)(i * 120), (float)y, 100, 140 };
            DrawRectangleRounded(cardRect, 0.1f, 4, LIGHTGRAY);
            DrawRectangleRoundedLines(cardRect, 0.1f, 4, DARKGRAY);
            DrawText(cardText.c_str(), x + (int)(i * 120) + 10, y + 20, 20, BLACK);
        }
    }
}

// Draw a button on screen
bool DrawButton(Rectangle rect, const string& text) {
    bool isHovered = CheckCollisionPointRec(GetMousePosition(), rect);
    if (isHovered) {
        DrawRectangleRec(rect, DARKGRAY);
    }
    else {
        DrawRectangleRec(rect, LIGHTGRAY);
    }
    DrawRectangleLinesEx(rect, 2, BLACK);
    DrawText(text.c_str(), (int)(rect.x + (rect.width / 2) - MeasureText(text.c_str(), 20) / 2),
        (int)(rect.y + (rect.height / 2) - 10), 20, BLACK);
    return isHovered && IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
}

int main() {
    const int screenWidth = 1000;
    const int screenHeight = 600;
    InitWindow(screenWidth, screenHeight, "Blackjack Game");

    srand(static_cast<unsigned int>(time(nullptr)));

    bool gameStarted = false;
    bool quitGame = false;
    bool nextRound = false;
    int playerWins = 0;

    while (!WindowShouldClose()) {
        // Main menu screen
        if (!gameStarted) {
            Rectangle startButton = { screenWidth / 2 - 100, screenHeight / 2, 200, 50 };
            if (DrawButton(startButton, "Start Game")) {
                gameStarted = true;
                nextRound = true; // Start the game
            }

            BeginDrawing();
            ClearBackground(DARKGREEN);
            DrawText("Blackjack", screenWidth / 2 - MeasureText("Blackjack", 60) / 2, 150, 60, GOLD);
            EndDrawing();
        }
        else if (gameStarted && !quitGame) {
            vector<Card> deck = InitializeDeck();
            ShuffleDeck(deck);

            vector<Card> playerHand;
            vector<Card> dealerHand;

            playerHand.push_back(deck.back()); deck.pop_back();
            dealerHand.push_back(deck.back()); deck.pop_back();
            playerHand.push_back(deck.back()); deck.pop_back();
            dealerHand.push_back(deck.back()); deck.pop_back();

            bool playerTurn = true;
            bool gameOver = false;
            string message;

            while (!WindowShouldClose() && !quitGame) {
                if (!gameOver && playerTurn) {
                    if (IsKeyPressed(KEY_H)) {
                        playerHand.push_back(deck.back());
                        deck.pop_back();
                        if (CalculateHandValue(playerHand) > 21) {
                            message = "You busted! Dealer wins.";
                            gameOver = true;
                        }
                    }
                    else if (IsKeyPressed(KEY_S)) {
                        playerTurn = false;
                        message = "Dealer's turn...";
                    }
                }
                else if (!playerTurn && !gameOver) {
                    int dealerValue = CalculateHandValue(dealerHand);
                    int playerValue = CalculateHandValue(playerHand);

                    if (dealerValue < 17) {
                        dealerHand.push_back(deck.back());
                        deck.pop_back();
                    }
                    else {
                        if (dealerValue > 21 || dealerValue < playerValue) {
                            message = "You win!";
                            playerWins++;
                        }
                        else if (dealerValue > playerValue) {
                            message = "Dealer wins!";
                        }
                        else {
                            message = "It's a tie!";
                        }
                        gameOver = true;
                    }
                }

                BeginDrawing();
                ClearBackground(DARKGREEN);

                if (gameOver) {
                    ClearBackground(DARKGREEN);
                    DrawText(message.c_str(), screenWidth / 2 - MeasureText(message.c_str(), 60) / 2, 200, 60, GOLD);

                    Rectangle nextRoundButton = { screenWidth / 2 - 100, screenHeight / 2 + 50, 200, 50 };
                    Rectangle quitButton = { screenWidth / 2 - 100, screenHeight / 2 + 150, 200, 50 };

                    if (DrawButton(nextRoundButton, "Next Round")) {
                        nextRound = true;
                        break;
                    }

                    if (DrawButton(quitButton, "Quit Game")) {
                        playerWins = 0; // Reset player win count
                        quitGame = true;
                        break;
                    }
                }
                else {
                    DrawText("Blackjack", screenWidth / 2 - MeasureText("Blackjack", 40) / 2, 20, 40, GOLD);
                    DrawText(("Player Wins: " + to_string(playerWins)).c_str(), 20, 60, 20, RAYWHITE);
                    DrawText("Press H (Hit) and S (Stop)", 20, 90, 20, RAYWHITE);

                    DrawText("Your Hand:", 20, 150, 20, RAYWHITE);
                    DrawHand(playerHand, 20, 180);

                    DrawText("Dealer's Hand:", 20, 350, 20, RAYWHITE);
                    DrawHand(dealerHand, 20, 380, playerTurn); // Hide dealer's second card during player turn
                }

                EndDrawing();
            }

            if (nextRound) {
                nextRound = false;
                continue; // Restart the round
            }

            if (quitGame) {
                gameStarted = false;
                quitGame = false;
            }
        }
    }

    CloseWindow();
    return 0;
}