#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>

const int windowWidth = 800;
const int windowHeight = 600;

const float paddleWidth = 100.f;
const float paddleHeight = 20.f;
const float paddleSpeed = 400.f;

const float ballRadius = 10.f;
const float ballSpeed = 300.f;

const float blockWidth = 60.f;
const float blockHeight = 20.f;
const int blocksPerRow = 10;
const int numRows = 5;

struct ScoreEntry {
    std::string name;
    int score;
};

class Paddle {
public:
    sf::RectangleShape shape;
    float speed;

    Paddle(float x, float y) {
        shape.setSize({ paddleWidth, paddleHeight });
        shape.setPosition(x, y);
        shape.setFillColor(sf::Color::Blue);
        shape.setOrigin(paddleWidth / 2.f, paddleHeight / 2.f);
        speed = paddleSpeed;
    }

    void move(float deltaTime, float windowWidth) {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && shape.getPosition().x - paddleWidth / 2 > 0) {
            shape.move(-speed * deltaTime, 0.f);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && shape.getPosition().x + paddleWidth / 2 < windowWidth) {
            shape.move(speed * deltaTime, 0.f);
        }
    }
};

class Ball {
public:
    sf::CircleShape shape;
    sf::Vector2f velocity;

    Ball(float x, float y) {
        shape.setRadius(ballRadius);
        shape.setPosition(x, y);
        shape.setFillColor(sf::Color::Red);
        shape.setOrigin(ballRadius, ballRadius);
        velocity = { ballSpeed, -ballSpeed };
    }

    void move(float deltaTime) {
        shape.move(velocity * deltaTime);
    }

    void checkCollisionWithWalls(float windowWidth, float windowHeight) {
        if (shape.getPosition().x - ballRadius <= 0 || shape.getPosition().x + ballRadius >= windowWidth) {
            velocity.x = -velocity.x;
        }
        if (shape.getPosition().y - ballRadius <= 0) {
            velocity.y = -velocity.y;
        }
    }
};

class Block {
public:
    sf::RectangleShape shape;
    bool isDestroyed = false;

    Block(float x, float y, const sf::Texture& texture) {
        shape.setSize({ blockWidth, blockHeight });
        shape.setPosition(x, y);
        shape.setTexture(&texture);
        shape.setOrigin(blockWidth / 2.f, blockHeight / 2.f);
    }
};

bool isColliding(const sf::Shape& a, const sf::Shape& b) {
    return a.getGlobalBounds().intersects(b.getGlobalBounds());
}

void showMenu(sf::RenderWindow& window, std::string& playerName, std::vector<ScoreEntry>& highScores);
void showEndScreen(sf::RenderWindow& window, int score, bool isWin, std::string& playerName, std::vector<ScoreEntry>& highScores);
void saveHighScores(const std::vector<ScoreEntry>& highScores);
void loadHighScores(std::vector<ScoreEntry>& highScores);

void saveHighScores(const std::vector<ScoreEntry>& highScores) {
    std::ofstream file("highscores.txt");
    if (!file.is_open()) {
        std::cerr << "Could not open file for saving high scores!" << std::endl;
        return;
    }

    for (const auto& entry : highScores) {
        file << entry.name << " " << entry.score << "\n";
    }
}

void loadHighScores(std::vector<ScoreEntry>& highScores) {
    std::ifstream file("highscores.txt");
    if (!file.is_open()) {
        std::cerr << "Could not open file for loading high scores!" << std::endl;
        return;
    }

    highScores.clear();
    ScoreEntry entry;
    while (file >> entry.name >> entry.score) {
        highScores.push_back(entry);
    }

    std::sort(highScores.begin(), highScores.end(), [](const ScoreEntry& a, const ScoreEntry& b) {
        return a.score > b.score;
        });

    if (highScores.size() > 5) {
        highScores.resize(5);
    }
}

void showMenu(sf::RenderWindow& window, std::string& playerName, std::vector<ScoreEntry>& highScores) {
    sf::Font font;
    if (!font.loadFromFile("arial.ttf")) {
        std::cerr << "Could not load font!" << std::endl;
        return;
    }

    sf::Text titleText("ARKANOID", font, 40);
    titleText.setPosition(windowWidth / 2.f - 100, 50);
    titleText.setFillColor(sf::Color::White);

    sf::Text playText("GRAJ", font, 30);
    playText.setPosition(windowWidth / 2.f - 50, 150);
    playText.setFillColor(sf::Color::White);

    sf::Text highScoresText("NAJLEPSZE WYNIKI", font, 30);
    highScoresText.setPosition(windowWidth / 2.f - 150, 200);
    highScoresText.setFillColor(sf::Color::White);

    sf::Text exitText("WYJSCIE", font, 30);
    exitText.setPosition(windowWidth / 2.f - 50, 250);
    exitText.setFillColor(sf::Color::White);

    sf::Text inputPrompt("Podaj swoj nick: ", font, 20);
    inputPrompt.setPosition(50, windowHeight - 100);
    inputPrompt.setFillColor(sf::Color::White);

    sf::Text playerInput(playerName, font, 20);
    playerInput.setPosition(250, windowHeight - 100);
    playerInput.setFillColor(sf::Color::White);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            if (event.type == sf::Event::MouseButtonPressed) {
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                if (playText.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                    return;
                }
                if (highScoresText.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                    sf::Text scoresDisplay("Najlepsze wyniki:", font, 30);
                    scoresDisplay.setPosition(50, 50);
                    scoresDisplay.setFillColor(sf::Color::White);

                    sf::Text backText("POWRÓT", font, 30);
                    backText.setPosition(windowWidth / 2.f - 50, windowHeight - 100);
                    backText.setFillColor(sf::Color::White);

                    while (window.isOpen()) {
                        while (window.pollEvent(event)) {
                            if (event.type == sf::Event::Closed) {
                                window.close();
                            }
                            if (event.type == sf::Event::MouseButtonPressed) {
                                mousePos = sf::Mouse::getPosition(window);
                                if (backText.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                                    return;
                                }
                            }
                        }

                        window.clear();
                        window.draw(scoresDisplay);

                        for (size_t i = 0; i < highScores.size(); ++i) {
                            sf::Text scoreText(std::to_string(i + 1) + ". " + highScores[i].name + ": " + std::to_string(highScores[i].score), font, 20);
                            scoreText.setPosition(50, 100 + i * 30);
                            scoreText.setFillColor(sf::Color::White);
                            window.draw(scoreText);
                        }

                        window.draw(backText);
                        window.display();
                    }
                }
                if (exitText.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                    window.close();
                }
            }
            if (event.type == sf::Event::TextEntered) {
                if (event.text.unicode == 8 && !playerName.empty()) {
                    playerName.pop_back();
                }
                else if (event.text.unicode < 128 && playerName.size() < 15) {
                    playerName += static_cast<char>(event.text.unicode);
                }
                playerInput.setString(playerName);
            }
        }

        window.clear();
        window.draw(titleText);
        window.draw(playText);
        window.draw(highScoresText);
        window.draw(exitText);
        window.draw(inputPrompt);
        window.draw(playerInput);
        window.display();
    }
}

void showEndScreen(sf::RenderWindow& window, int score, bool isWin, std::string& playerName, std::vector<ScoreEntry>& highScores) {
    sf::Font font;
    if (!font.loadFromFile("arial.ttf")) {
        std::cerr << "Could not load font!" << std::endl;
        return;
    }

    std::string endMessage = isWin ? "Gratulacje! Wygrales!" : "Koniec gry! Przegrales!";

    sf::Text messageText(endMessage, font, 30);
    messageText.setPosition(windowWidth / 2.f - 150, windowHeight / 2.f - 100);
    messageText.setFillColor(sf::Color::White);

    sf::Text scoreText("Wynik: " + std::to_string(score), font, 30);
    scoreText.setPosition(windowWidth / 2.f - 100, windowHeight / 2.f - 50);
    scoreText.setFillColor(sf::Color::White);

    sf::Text retryText("Zagraj jeszcze raz", font, 20);
    retryText.setPosition(windowWidth / 2.f - 100, windowHeight / 2.f);
    retryText.setFillColor(sf::Color::White);

    sf::Text menuText("Powrot do MENU", font, 20);
    menuText.setPosition(windowWidth / 2.f - 100, windowHeight / 2.f + 50);
    menuText.setFillColor(sf::Color::White);

    sf::Text exitText("Wyjscie", font, 20);
    exitText.setPosition(windowWidth / 2.f - 100, windowHeight / 2.f + 100);
    exitText.setFillColor(sf::Color::White);

    highScores.push_back({ playerName, score });
    std::sort(highScores.begin(), highScores.end(), [](const ScoreEntry& a, const ScoreEntry& b) {
        return a.score > b.score;
        });
    if (highScores.size() > 5) {
        highScores.resize(5);
    }

    saveHighScores(highScores);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            if (event.type == sf::Event::MouseButtonPressed) {
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                if (retryText.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                    return;
                }
                if (menuText.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                    showMenu(window, playerName, highScores);
                    return;
                }
                if (exitText.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                    window.close();
                }
            }
        }

        window.clear();
        window.draw(messageText);
        window.draw(scoreText);
        window.draw(retryText);
        window.draw(menuText);
        window.draw(exitText);
        window.display();
    }
}

int main() {
    sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "Arkanoid");
    window.setFramerateLimit(60);

    std::string playerName;
    std::vector<ScoreEntry> highScores;
    loadHighScores(highScores);

    showMenu(window, playerName, highScores);

    sf::Texture blockTexture;
    if (!blockTexture.loadFromFile("block_texture.png")) {
        std::cerr << "Could not load block texture!" << std::endl;
        return -1;
    }

    Paddle paddle(windowWidth / 2.f, windowHeight - 50.f);
    Ball ball(windowWidth / 2.f, windowHeight / 2.f);

    std::vector<Block> blocks;
    float startX = (windowWidth - blocksPerRow * blockWidth) / 2.f;
    float startY = 50.f;

    for (int i = 0; i < numRows; ++i) {
        for (int j = 0; j < blocksPerRow; ++j) {
            float x = startX + j * blockWidth;
            float y = startY + i * blockHeight;
            blocks.emplace_back(x + blockWidth / 2, y + blockHeight / 2, blockTexture);
        }
    }

    int score = 0;
    sf::Font font;
    if (!font.loadFromFile("arial.ttf")) {
        std::cerr << "Could not load font!" << std::endl;
        return -1;
    }

    sf::Text scoreText;
    scoreText.setFont(font);
    scoreText.setCharacterSize(20);
    scoreText.setFillColor(sf::Color::White);
    scoreText.setPosition(windowWidth - 150, 10);

    sf::Clock clock;
    bool isPaused = false;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent (event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::P) {
                    isPaused = !isPaused; // Pauza po naciśnięciu klawisza 'P'
                }
            }
        }

        if (isPaused) {
            sf::Text pauseText("PAUZA - Nacisnij P aby kontynuowac", font, 20);
            pauseText.setPosition(windowWidth / 2.f - 200, windowHeight / 2.f);
            pauseText.setFillColor(sf::Color::White);
            window.clear();
            window.draw(pauseText);
            window.display();
            continue;
        }

        float deltaTime = clock.restart().asSeconds();

        // Poruszanie paddle
        paddle.move(deltaTime, windowWidth);

        // Ruch piłki
        ball.move(deltaTime);

        // Kolizje piłki z ścianami
        ball.checkCollisionWithWalls(windowWidth, windowHeight);

        // Kolizja piłki z paddle
        if (isColliding(ball.shape, paddle.shape)) {
            ball.velocity.y = -std::abs(ball.velocity.y);
        }

        // Kolizje piłki z blokami
        for (auto& block : blocks) {
            if (!block.isDestroyed && isColliding(ball.shape, block.shape)) {
                block.isDestroyed = true;
                ball.velocity.y = -ball.velocity.y;
                score += 10;
            }
        }

        // Usuwanie zniszczonych bloków
        blocks.erase(std::remove_if(blocks.begin(), blocks.end(),
            [](const Block& block) { return block.isDestroyed; }),
            blocks.end());

        // Sprawdzenie warunków wygranej/przegranej
        if (ball.shape.getPosition().y - ballRadius > windowHeight) {
            showEndScreen(window, score, false, playerName, highScores);
            return 0; // Zakończenie gry po przegranej
        }

        if (blocks.empty()) {
            showEndScreen(window, score, true, playerName, highScores);
            return 0; // Zakończenie gry po wygranej
        }

        // Wyświetlanie wyniku
        scoreText.setString("Wynik: " + std::to_string(score));

        // Rysowanie
        window.clear();
        window.draw(paddle.shape);
        window.draw(ball.shape);
        for (const auto& block : blocks) {
            window.draw(block.shape);
        }
        window.draw(scoreText);
        window.display();
    }

    return 0;
}
