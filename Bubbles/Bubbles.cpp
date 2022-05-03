#include <SFML/Graphics.hpp>
#include <cmath>
#include <ctime>
#include <iostream>
#include <sstream>
#include <SFML/Network.hpp>
#include <Windows.h>
#include "message.h"
#include "queue.hpp"
#include "receiver.hpp"
#include "BubbleClass.h"
#include "list.hpp"
#include "accepter.hpp"
#include "UDPreceiver.h"
#include "util.hpp"
#include "server.h"
#include "Player.h"

#define BUBBLE_SIZE 20
#define CANNON_H  60
#define CANNON_W  20
#define MAX_ANGLE 80
#define MIN_ANGLE 280
#define M_PI      3.141692654
#define VELOCITY  7
#define WINDOW_H  600
#define WINDOW_W  1200
#define BUFF_SIZE 1024

int main(int argc, const char* argv[])
{
    srand(time(NULL));
    //Queue<message> queue;
    sf::IpAddress recipient;
    
    Queue<sf::Packet> queue;

    //Broadcast message
    std::string msg;
    sf::Packet packet1;
    packet1 << "msg";
    std::shared_ptr<sf::UdpSocket> testingServer = std::make_shared<sf::UdpSocket>();
    unsigned short remote_port = 55001;
    //sf::Socket::Status status = testingServer.send(msg.c_str(), msg.length(), recipient, 55000);
    
    if (testingServer->send(packet1, sf::IpAddress::Broadcast, 55000) != sf::Socket::Done)
    {
        std::cout << "Problem Sending Broadcast\n";
        return 1;
    }
    else 
    {
        std::cout << "First sent\n";
    }

    char data[BUFF_SIZE];
    std::size_t received;
    std::memset(data, 0, BUFF_SIZE);
    testingServer->setBlocking(false);

    Sleep(1500);
    if (testingServer->receive(data, BUFF_SIZE, received, recipient, remote_port) != sf::Socket::Done)
    {
        Server serv(queue);
        std::cout << "Setting Up Server Now";
        std::thread(&Server::CreateServer, &serv).detach();
        Sleep(1500);
        //sf::Socket::Status status = testingServer.send(msg.c_str(), msg.length(), recipient, 55000);
        if (testingServer->send(packet1, sf::IpAddress::Broadcast, 55000) != sf::Socket::Done)
        {
            std::cout << "Problem Sending Brodcast\n";
            return 1;
        }
        Sleep(1500);
        if (testingServer->receive(data, BUFF_SIZE, received, recipient, remote_port) != sf::Socket::Done)
        {
            std::cout << "Failed to connect to my own server\n";
        }
        else
        {
            std::cout << "Managed to connect to my own server\n";
        }
    }
    else
    {
        std::cout << "Managed to connect to outside server" << std::endl;
    }

    testingServer->setBlocking(true);
    std::shared_ptr<sf::TcpSocket> tcp = std::make_shared<sf::TcpSocket>();
    //status = tcp->connect(sf::IpAddress::getLocalAddress(), 55002);
    //Spawn server if no answer
    if (tcp->connect(sf::IpAddress::getLocalAddress(), 55002) != sf::Socket::Done)
    {
        std::cout << "Unable to Connect to Server\n";
        return 1;
    }
    std::cout << "Connected to the Server\n";

    sf::Packet introP;
    //status = tcp->receive(introP);
    if (tcp->receive(introP) != sf::Socket::Done)
    {
        std::cout << "Error receiving seed\n";
        return 0;
    }
    std::cout << "Seed Recieved\n";

    
    int player_number = 0;
    unsigned int setUp = 0;
    introP >> player_number >> setUp;
    std::cout << "Game setup: " << setUp << " Player Number: " << player_number << std::endl;
    srand(setUp);
    std::shared_ptr<Receiver> receiver = std::make_shared<Receiver>(tcp, queue);
    std::thread(&Receiver::recv_loop, receiver).detach();

    const sf::Color colours[5] = {
        sf::Color::Red,
        sf::Color(127, 192, 127),
        sf::Color::Blue,
        sf::Color(192, 192, 127),
        sf::Color(127, 192, 192),
    };

    std::vector<sf::CircleShape> bubbles;
    for (size_t i = 1; i < 11; i++) {
        for (size_t j = 0; j < (WINDOW_W / 2) / 40 - (i % 2); j++) {
            sf::CircleShape bubble(BUBBLE_SIZE);
            bubble.setPosition(sf::Vector2f(BUBBLE_SIZE * 2 * j + (i % 2) * BUBBLE_SIZE, i * 33));
            int points = rand() % 5;
            bubble.setFillColor(colours[points]);
            bubbles.push_back(bubble);
        }
    }

    std::vector<sf::CircleShape> bubbles2;
    for (size_t i = 1; i < 11; i++) {
        for (size_t j = 0; j < (WINDOW_W / 2) / 40 - (i % 2); j++) {
            sf::CircleShape bubble(BUBBLE_SIZE);
            bubble.setPosition(sf::Vector2f((BUBBLE_SIZE * 2 * j + (i % 2) * BUBBLE_SIZE) + WINDOW_W / 2, i * 33));
            int points = rand() % 5;
            bubble.setFillColor(colours[points]);
            bubbles2.push_back(bubble);
        }
    }

    //Setting Up Player 1
    sf::RectangleShape cannon1(sf::Vector2f(CANNON_W, CANNON_H));
    cannon1.setOrigin(CANNON_W / 2, CANNON_H);
    sf::Vector2f p1_pos(WINDOW_W / 4, (WINDOW_H));
    cannon1.setOrigin(CANNON_W / 2, CANNON_H);
    cannon1.setPosition(p1_pos);
    Player* Player_One = new Player;
    Player_One->angle = 0;
    Player_One->isCannonReady = true;
    Player_One->isCannonShooting = false;
    Player_One->isBallMoving = false;
    Player_One->isBallMovingLeft = false;
    Player_One->velocityX = 0;
    Player_One->velocityY = 0;
    sf::CircleShape ball1(BUBBLE_SIZE);
    ball1.setOrigin(BUBBLE_SIZE, BUBBLE_SIZE);
    ball1.setPosition(p1_pos);
    int ball1color = rand() % 5;
    ball1.setFillColor(colours[ball1color]);

    //Setting Up Player 2
    sf::RectangleShape cannon2(sf::Vector2f(CANNON_W, CANNON_H));
    cannon2.setOrigin(CANNON_W / 2, CANNON_H);
    sf::Vector2f p2_pos(3 * WINDOW_W / 4, (WINDOW_H));
    cannon2.setPosition(p2_pos);
    Player* Player_Two = new Player;
    Player_Two->angle = 0;
    Player_Two->isCannonReady = true;
    Player_Two->isCannonShooting = false;
    Player_Two->isBallMoving = false;
    Player_Two->isBallMovingLeft = false;
    Player_Two->velocityX = 0;
    Player_Two->velocityY = 0;
    sf::CircleShape ball2(BUBBLE_SIZE);
    ball2.setOrigin(BUBBLE_SIZE, BUBBLE_SIZE);
    ball2.setPosition(p2_pos);
    ball2.setFillColor(colours[rand() % 5]);

    sf::RectangleShape wall(sf::Vector2f(1, WINDOW_H));
    wall.setPosition(WINDOW_W / 2, 0);

    sf::Font font;
    if (!font.loadFromFile("PressStart2P.ttf"))
    {
        return 1;
    }

    int score1 = 0;
    int score2 = 0;
    sf::Text score("0 0", font);
    score.setCharacterSize(30);
    score.setPosition(WINDOW_W / 2 - 44, 10);

    float otherAngle = 0;
    float otherVelocityX = 0;
    float otherVelocityY = 0;
    bool otherCannonReady = true;
    bool otherBallMoving = false;
    bool otherCannonShooting = false;
    int otherScore = 0;
    int otherBallColor = rand() %5;

    sf::RenderWindow window(sf::VideoMode(WINDOW_W, WINDOW_H), "Bubble");
    window.setFramerateLimit(60);

    while (window.isOpen())
    {
        int i = 1;
        std::cout << "Times ran: " << i << std::endl;
        i++;

        sf::Event e;
        while (window.pollEvent(e))
        {
            if (e.type == sf::Event::Closed) {
                window.close();
            }
        }

        Player_One->angle = cannon1.getRotation();
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) && (Player_One->angle > MIN_ANGLE + 1 || Player_One->angle < MAX_ANGLE + 1))
            cannon1.rotate(-1);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) && (Player_One->angle > MIN_ANGLE - 1 || Player_One->angle < MAX_ANGLE - 1))
            cannon1.rotate(1);

        Player_Two->angle = cannon2.getRotation();
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && (Player_Two->angle > MIN_ANGLE + 1 || Player_Two->angle < MAX_ANGLE + 1))
            cannon2.rotate(-1);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && (Player_Two->angle > MIN_ANGLE - 1 || Player_Two->angle < MAX_ANGLE - 1))
            cannon2.rotate(1);

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) && Player_One->isCannonReady)
        {
            Player_One->angle = cannon1.getRotation();
            Player_One->velocityX = -cos((Player_One->angle + 90) * M_PI / 180) * VELOCITY;
            Player_One->velocityY = -sin((Player_Two->angle + 90) * M_PI / 180) * VELOCITY;
            Player_One->isCannonReady = false;
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && Player_Two->isCannonReady)
        {
            Player_Two->angle = cannon2.getRotation();
            Player_Two->velocityX = -cos((Player_Two->angle + 90) * M_PI / 180) * VELOCITY;
            Player_Two->velocityY = -sin((Player_Two->angle + 90) * M_PI / 180) * VELOCITY;
            Player_Two->isCannonReady = false;
        }

        std::cout << "Debug\n";


        //Data to be put into the packet
        move_msg message;
        sf::Packet sending;


       if (player_number == 1)
        {
            sf::Vector2f playerpos = ball1.getPosition();
            int colorN = 0;
            message.cannon_angle = Player_One->angle;
            message.isShooting = Player_One->isCannonShooting;
            message.isMoving = Player_One->isBallMoving;
            message.isMovingLeft = Player_One->isBallMovingLeft;
            message.velocityX = Player_One->velocityX;
            message.velocityY = Player_One->velocityY;
            message.score = Player_One->score;
            std::cout << "Sending Player One\n";
        }
        else
        {
            sf::Vector2f playerpos = ball2.getPosition();
            int colorN = 0;
            message.cannon_angle = Player_Two->angle;
            message.isShooting = Player_Two->isCannonShooting;
            message.isMoving = Player_Two->isBallMoving;
            message.isMovingLeft = Player_Two->isBallMovingLeft;
            message.velocityX = Player_Two->velocityX;
            message.velocityY = Player_Two->velocityY;
            message.score = Player_Two->score;
            std::cout << "Sending Player Two\n";

        }
        sending << message;

        if (tcp->send(sending) != sf::Socket::Done)
        {
            std::cout << "Couldnt Send Package" << std::endl;
        }
        else
        {
            std::cout << "Sent Package" << std::endl;

        }

        //auto q = queue.pop();
        
        Sleep(1500);
        sf::Packet q;
        if (queue.pop(q))
        {//sf::Packet reciever = std::get<1>(q);
            q >> message;
            otherAngle = message.cannon_angle;
            otherVelocityX = message.velocityX;
            otherVelocityY = message.velocityY;
            otherAngle = message.cannon_angle;
            otherCannonReady = message.isCannonReady;
            otherBallMoving = message.isMoving;
            otherCannonShooting = message.isShooting;
            otherScore = message.score;
            otherBallColor = message.ballcolour;
            std::cout << "Recieved Package" << std::endl;
        }
        else 
        {
            std::cout << "Couldnt Recieve Package" << std::endl;
        }

        std::cout << otherAngle << "\n" << otherCannonReady << std::endl;

        if (player_number == 1)
        {
            if (Player_One->velocityX != 0 && Player_One->velocityY != 0)
            {
                ball1.move(Player_One->velocityX, Player_One->velocityY);
                sf::Vector2f pos = ball1.getPosition();
                if (pos.x < BUBBLE_SIZE || pos.x > WINDOW_W / 2 - BUBBLE_SIZE)
                {
                    Player_One->velocityX = -Player_One->velocityX;
                }

                bool collided1 = false;

                for (int i = 0; i < bubbles.size(); i++)
                {
                    float x = bubbles[i].getPosition().x + BUBBLE_SIZE - pos.x;
                    float y = bubbles[i].getPosition().y + BUBBLE_SIZE - pos.y;

                    if (size_t(x * x) + y * y <= size_t(BUBBLE_SIZE) * BUBBLE_SIZE * 4)
                    {
                        //std::cout << "Collided\n";

                        collided1 = true;

                        if (bubbles[i].getFillColor().toInteger() == ball1.getFillColor().toInteger())
                        {
                            //std::cout << "SameColour\n";
                        }
                    }
                }

                if (collided1)
                {
                    Player_One->velocityX = 0;
                    Player_One->velocityY = 0;

                    sf::CircleShape bubble(BUBBLE_SIZE);
                    bubble.setPosition(ball1.getPosition().x - BUBBLE_SIZE, ball1.getPosition().y - BUBBLE_SIZE);
                    bubble.setFillColor(ball1.getFillColor());
                    bubbles.push_back(bubble);

                    ball1.setFillColor(colours[rand() % 5]);
                    ball1.setPosition(p1_pos);
                    Player_One->isCannonReady = true;
                }
            }

            if (otherVelocityX != 0 && otherVelocityY != 0)
            {
                ball2.move(otherVelocityX, otherVelocityY);
                sf::Vector2f pos = ball2.getPosition();
                if (pos.x < WINDOW_W / 2 + BUBBLE_SIZE || pos.x > WINDOW_W - BUBBLE_SIZE)
                {
                    otherVelocityX = -otherVelocityY;
                }

                bool collided2 = false;

                for (int i = 0; i < bubbles2.size(); i++)
                {
                    float x = bubbles2[i].getPosition().x + BUBBLE_SIZE - pos.x;
                    float y = bubbles2[i].getPosition().y + BUBBLE_SIZE - pos.y;

                    if (size_t(x * x) + y * y <= size_t(BUBBLE_SIZE) * BUBBLE_SIZE * 4)
                    {
                        //std::cout << "Collided\n";

                        collided2 = true;

                        if (bubbles2[i].getFillColor().toInteger() == ball2.getFillColor().toInteger())
                        {
                            //std::cout << "SameColour\n";
                        }
                    }
                }

                if (collided2)
                {
                    otherVelocityX = 0;
                    otherVelocityY = 0;
                    sf::CircleShape bubble(BUBBLE_SIZE);
                    bubble.setPosition(ball2.getPosition().x - BUBBLE_SIZE, ball2.getPosition().y - BUBBLE_SIZE);
                    bubble.setFillColor(ball2.getFillColor());
                    bubbles2.push_back(bubble);
                    ball2.setFillColor(colours[rand() % 5]);
                    ball2.setPosition(p2_pos);
                    otherCannonReady = true;
                }
            }
            if (player_number > 1)
            {
                if (Player_Two->velocityX != 0 && Player_Two->velocityY != 0)
                {
                    ball2.move(Player_Two->velocityX, Player_Two->velocityY);
                    sf::Vector2f pos = ball2.getPosition();
                    if (pos.x < BUBBLE_SIZE || pos.x > WINDOW_W / 2 - BUBBLE_SIZE)
                    {
                        Player_Two->velocityX = -Player_Two->velocityX;
                    }

                    bool collided2 = false;

                    for (int i = 0; i < bubbles2.size(); i++)
                    {
                        float x = bubbles2[i].getPosition().x + BUBBLE_SIZE - pos.x;
                        float y = bubbles2[i].getPosition().y + BUBBLE_SIZE - pos.y;

                        if (size_t(x * x) + y * y <= size_t(BUBBLE_SIZE) * BUBBLE_SIZE * 4)
                        {
                            //std::cout << "Collided\n";

                            collided2 = true;

                            if (bubbles[i].getFillColor().toInteger() == ball1.getFillColor().toInteger())
                            {
                                //std::cout << "SameColour\n";
                            }
                        }
                    }

                    if (collided2)
                    {
                        Player_Two->velocityX = 0;
                        Player_Two->velocityY = 0;

                        sf::CircleShape bubble(BUBBLE_SIZE);
                        bubble.setPosition(ball2.getPosition().x - BUBBLE_SIZE, ball2.getPosition().y - BUBBLE_SIZE);
                        bubble.setFillColor(ball2.getFillColor());
                        bubbles.push_back(bubble);

                        ball2.setFillColor(colours[rand() % 5]);
                        ball2.setPosition(p2_pos);
                        Player_Two->isCannonReady = true;
                    }
                }

                if (otherVelocityX != 0 && otherVelocityY != 0)
                {
                    ball1.move(otherVelocityX, otherVelocityY);
                    sf::Vector2f pos = ball1.getPosition();
                    if (pos.x < WINDOW_W / 2 + BUBBLE_SIZE || pos.x > WINDOW_W - BUBBLE_SIZE)
                    {
                        otherVelocityX = -otherVelocityY;
                    }

                    bool collided1 = false;

                    for (int i = 0; i < bubbles.size(); i++)
                    {
                        float x = bubbles[i].getPosition().x + BUBBLE_SIZE - pos.x;
                        float y = bubbles[i].getPosition().y + BUBBLE_SIZE - pos.y;

                        if (size_t(x * x) + y * y <= size_t(BUBBLE_SIZE) * BUBBLE_SIZE * 4)
                        {
                            //std::cout << "Collided\n";

                            collided1 = true;

                            if (bubbles[i].getFillColor().toInteger() == ball1.getFillColor().toInteger())
                            {
                                //std::cout << "SameColour\n";
                            }
                        }
                    }

                    if (collided1)
                    {
                        otherVelocityX = 0;
                        otherVelocityY = 0;
                        sf::CircleShape bubble(BUBBLE_SIZE);
                        bubble.setPosition(ball1.getPosition().x - BUBBLE_SIZE, ball1.getPosition().y - BUBBLE_SIZE);
                        bubble.setFillColor(ball1.getFillColor());
                        bubbles.push_back(bubble);
                        ball1.setFillColor(colours[rand() % 5]);
                        ball1.setPosition(p1_pos);
                        otherCannonReady = true;
                    }
                }
            }

            window.clear();
            window.draw(cannon1);
            window.draw(cannon2);
            window.draw(wall);
            window.draw(score);
            window.draw(ball1);
            window.draw(ball2);

            for (auto& b : bubbles) {
                window.draw(b);
            }

            for (auto& b : bubbles2) {
                window.draw(b);
            }

            window.display();
        }
        //return 0;
    }
}
