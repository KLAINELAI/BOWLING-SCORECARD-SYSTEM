#include <iostream>
#include <vector>
#include <iomanip>
#include <algorithm> // for sorting

class Player {
private:
    std::string name;
    std::vector<int> rolls;

public:
    Player(const std::string& playerName) : name(playerName) {}

    const std::string& getName() const {
        return name;
    }

    void roll(int pins) {
        rolls.push_back(pins);
    }

    const std::vector<int>& getRolls() const {
        return rolls;
    }
};

class BowlingGame {
private:
    std::vector<Player> players;

public:
    void addPlayer(const std::string& playerName) {
        players.emplace_back(playerName);
    }

    const std::vector<Player>& getPlayers() const {
        return players;
    }

    void roll(int pins) {
        for (auto& player : players) {
            player.roll(pins);
        }
    }

    void displayGameProgress() const {
        std::cout << "Game Progress\n";
        for (const auto& player : players) {
            std::cout << player.getName() << ":\t";
            for (int roll : player.getRolls()) {
                std::cout << std::setw(2) << roll << " ";
            }
            std::cout << "\n";
        }
        std::cout << "\n";
    }

    void calculateAndDisplayScores() const {
        std::cout << "Current Scores\n";
        for (const auto& player : players) {
            int totalScore = 0;
            size_t rollIndex = 0;

            std::cout << player.getName() << ":\t";

            for (int frame = 1; frame <= 10; ++frame) {
                int frameScore = player.getRolls()[rollIndex];

                if (frameScore == 10) {
                    // Strike
                    frameScore += player.getRolls()[rollIndex + 1] + player.getRolls()[rollIndex + 2];
                    rollIndex += 1;
                } else {
                    frameScore += player.getRolls()[rollIndex + 1];

                    if (frameScore == 10) {
                        // Spare
                        frameScore += player.getRolls()[rollIndex + 2];
                    }

                    rollIndex += 2;
                }

                totalScore += frameScore;
                std::cout << std::setw(3) << totalScore << " ";
            }

            std::cout << "\n";
        }
        std::cout << "\n";
    }

    void displayGameSummary() const {
        std::cout << "Game Summary\n";
        for (const auto& player : players) {
            std::cout << player.getName() << ":\t";
            displayRolls(player.getRolls());
            std::cout << "\n";
        }
        std::cout << "\n";

        calculateAndDisplayScores();  // Display the total scores directly

        // Add player ranking
        std::vector<std::pair<std::string, int>> playerRanking;
        for (const auto& player : players) {
            // Calculate total score
            int totalScore = 0;
            size_t rollIndex = 0;

            for (int frame = 1; frame <= 10; ++frame) {
                int frameScore = player.getRolls()[rollIndex];

                if (frameScore == 10) {
                    frameScore += player.getRolls()[rollIndex + 1] + player.getRolls()[rollIndex + 2];
                    rollIndex += 1;
                } else {
                    frameScore += player.getRolls()[rollIndex + 1];

                    if (frameScore == 10) {
                        frameScore += player.getRolls()[rollIndex + 2];
                    }

                    rollIndex += 2;
                }

                totalScore += frameScore;
            }

            playerRanking.emplace_back(player.getName(), totalScore);
        }

        // Sort and display player ranking
        std::sort(playerRanking.begin(), playerRanking.end(), [](const auto& a, const auto& b) {
            return a.second > b.second;
        });

        std::cout << "Player Ranking\n";
        for (size_t i = 0; i < playerRanking.size(); ++i) {
            std::cout << i + 1 << ". " << playerRanking[i].first << ": " << playerRanking[i].second << " points\n";
        }
    }

private:
    void displayRolls(const std::vector<int>& rolls) const {
        for (int roll : rolls) {
            std::cout << std::setw(2) << roll << " ";
        }
    }
};

int main() {
    BowlingGame game;

    // Menu for Player Management
    int option;
    do {
        std::cout << "Menu:\n";
        std::cout << "1. Add Player\n";
        std::cout << "2. Enter Scores\n";
        std::cout << "3. Display Game Progress\n";
        std::cout << "4. Display Game Summary\n";
        std::cout << "0. Exit\n";
        std::cout << "Choose an option: ";
        std::cin >> option;

        switch (option) {
            case 1: {
                if (game.getPlayers().size() < 5) {
                    std::string playerName;
                    std::cout << "Enter player name: ";
                    std::cin.ignore(); // Clear newline character from buffer
                    std::getline(std::cin, playerName);
                    game.addPlayer(playerName);
                    std::cout << "Player added.\n";
                } else {
                    std::cout << "Maximum number of players reached (5).\n";
                }
                break;
            }
            case 2: {
                if (game.getPlayers().empty()) {
                    std::cout << "No players added yet. Please add players first.\n";
                } else {
                    // Frame and Roll Input
                    for (int frame = 1; frame <= 10; ++frame) {
                        for (const auto& player : game.getPlayers()) {
                            std::cout << "Enter scores for " << player.getName() << ", Frame " << frame << "\n";
                            int pins1, pins2;
                            std::cout << "  Roll 1: ";
                            std::cin >> pins1;

                            // Check for valid input
                            if (pins1 < 0 || pins1 > 10) {
                                std::cout << "Invalid input. Pins should be between 0 and 10.\n";
                                continue;
                            }

                            if (pins1 != 10) {
                                std::cout << "  Roll 2: ";
                                std::cin >> pins2;

                                // Check for valid input
                                if (pins2 < 0 || pins2 > 10 || pins1 + pins2 > 10) {
                                    std::cout << "Invalid input. Pins should be between 0 and 10, and the total should not exceed 10.\n";
                                    continue;
                                }
                            } else {
                                pins2 = 0; // If it's a strike, the second roll is automatically 0
                            }

                            game.roll(pins1);
                            game.roll(pins2);

                            // Display the next player's name
                            auto playerIterator = std::find_if(game.getPlayers().begin(), game.getPlayers().end(),
                                [&player](const Player& p) { return p.getName() != player.getName(); });

                            if (playerIterator != game.getPlayers().end()) {
                                std::cout << "Next player: " << playerIterator->getName() << "\n";
                            }

                            game.displayGameProgress();
                        }
                    }
                }
                break;
            }
            case 3:
                game.displayGameProgress();
                break;
            case 4:
                game.displayGameSummary();
                break;
            case 0:
                std::cout << "Exiting the program.\n";
                break;
            default:
                std::cout << "Invalid option. Please try again.\n";
        }

    } while (option != 0);

    return 0;
}
