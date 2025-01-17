
#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>


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
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) &&
            shape.getPosition().x - paddleWidth / 2 > 0) {
            shape.move(-speed * deltaTime, 0.f);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) &&
            shape.getPosition().x + paddleWidth / 2 < windowWidth) {
            shape.move(speed * deltaTime, 0.f);
        }
    }
};

