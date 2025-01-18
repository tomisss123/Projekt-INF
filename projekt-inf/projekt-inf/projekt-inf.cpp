
#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include <string>

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

void showMenu(sf::RenderWindow& window);
void showEndScreen(sf::RenderWindow& window, int score, bool isWin);

void showMenu(sf::RenderWindow& window) {
    sf::Font font;
    if (!font.loadFromFile("arial.ttf")) {
        std::cerr << "Could not load font!" << std::endl;
        return;
    }

    sf::Text playText("GRAJ", font, 30);
    playText.setPosition(windowWidth / 2.f - 50, windowHeight / 2.f - 50);
    playText.setFillColor(sf::Color::White);

    sf::Text helpText("POMOC", font, 30);
    helpText.setPosition(windowWidth / 2.f - 50, windowHeight / 2.f);
    helpText.setFillColor(sf::Color::White);

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
                if (helpText.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                    sf::Text helpMessage("Przykładowy tekst", font, 20);
                    helpMessage.setPosition(50, 50);
                    helpMessage.setFillColor(sf::Color::White);

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
                        window.draw(helpMessage);
                        window.draw(backText);
                        window.display();
                    }
                }
            }
        }

        window.clear();
        window.draw(playText);
        window.draw(helpText);
        window.display();
    }
}

void showEndScreen(sf::RenderWindow& window, int score, bool isWin) {
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
                    showMenu(window);
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

    showMenu(window);

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

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        float deltaTime = clock.restart().asSeconds();

        paddle.move(deltaTime, windowWidth);

        ball.move(deltaTime);
        ball.checkCollisionWithWalls(windowWidth, windowHeight);

        if (isColliding(ball.shape, paddle.shape)) {
            ball.velocity.y = -ballSpeed;
        }

        for (auto& block : blocks) {
            if (!block.isDestroyed && isColliding(ball.shape, block.shape)) {
                ball.velocity.y = -ball.velocity.y;
                block.isDestroyed = true;
                score += 10;
            }
        }

        blocks.erase(std::remove_if(blocks.begin(), blocks.end(), [](const Block& block) {
            return block.isDestroyed;
            }), blocks.end());

        if (ball.shape.getPosition().y - ballRadius > windowHeight) {
            showEndScreen(window, score, false);
            return 0;
        }

        window.clear();
        window.draw(paddle.shape);
        window.draw(ball.shape);
        for (const auto& block : blocks) {
            window.draw(block.shape);
        }

        scoreText.setString("Punkty: " + std::to_string(score));
        window.draw(scoreText);

        window.display();

        if (blocks.empty()) {
            showEndScreen(window, score, true);
            return 0;
        }
    }

    return 0;
}
