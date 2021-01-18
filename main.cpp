#include <SFML/Graphics.hpp>
#include <iostream>
#include "Card.h"
#include "Animation.h"
#include "consts.h"
#include <deque>
#include <algorithm>
#include <chrono>
#include <random>
#include <cstdlib>
#include <list>
#include <iterator>
#include "computer.h"
#include "texture.hpp"

typedef int Square;

using namespace std;
using namespace sf;

vector<float> distribute(int count, float interval)
{
    vector<float> result;
    float first = -(count * interval * .5f);
    for (int i = 0;i < count;i++)
    {
        result.push_back(first + interval * i);
    }

    return result;
}

vector<float> getFanOrientation(int count)
{
    return distribute(count, FANORIINT);
}

vector<float> getFanOffset(int count)
{
    return distribute(count, FANOFFINT);
}

int main()
{
    unsigned seed = chrono::system_clock::now().time_since_epoch().count();

    srand(time(NULL));

    RenderWindow window(VideoMode(SWIDTH, SHEIGHT), L"¿øÄ«µå");

    Event event;

    Clock clock;

    deque<Card> cards;
    
    list<Card> playerHand;
    list<Card> computerHand;

    deque<Card> garbage;

    list<Card>::iterator selection;

    Texture texture;
    if (!texture.loadFromMemory(texture_png, texture_png_len))
    {
        cout << "Error!" << endl;
    }

    RectangleShape card(Vector2f(CWIDTH, CHEIGHT));
    card.setFillColor(Color::Red);
    card.setOrigin(card.getLocalBounds().width / 2, card.getLocalBounds().height);
    card.setPosition(SWIDTH / 2, SHEIGHT + 30);

    RectangleShape cCard(Vector2f(CWIDTH, CHEIGHT));
    card.setFillColor(Color::Blue);
    card.setOrigin(card.getLocalBounds().width / 2, card.getLocalBounds().height);
    card.setPosition(SWIDTH / 2, 0 - 30);

    bool playerTurn;

    auto reset = [&]()
    {
        for (int p = 0; p < 3;p++)
        {
            for (int n = 1; n < 14;n++)
            {
                cards.push_front(Card(static_cast<Pattern>(p), static_cast<Number>(n)));
            }
        }
        shuffle(cards.begin(), cards.end(), default_random_engine(seed));

        for (int i = 0;i < CARDCNT;i++)
        {
            cards.front().flip();
            playerHand.push_back(cards.front());
            cards.pop_front();

            computerHand.push_back(cards.front());
            cards.pop_front();
        }

        cards.front().flip();
        garbage.push_front(cards.front());

        playerTurn = true;

        selection = playerHand.begin();
    };

    reset();

    while (window.isOpen()) 
    {
        while (window.pollEvent(event))
        {
            switch (event.type)
            {
            case Event::Closed:
                window.close();
                break;
            case Event::KeyPressed:
                switch (event.key.code)
                {
                case Keyboard::Left:
                    if (selection == playerHand.begin())
                    {
                        selection = playerHand.end();
                        selection--;
                    }
                    else
                    {
                        selection--;
                    }
                    break;
                case Keyboard::Right:
                    selection++;
                    if (selection == playerHand.end())
                    {
                        selection = playerHand.begin();
                    }
                    break;
                case Keyboard::Up:
                {
                    bool noDrawble = true;
                    for (auto& elem : playerHand)
                    {
                        if (elem.matchesWith(garbage.front()))
                        {
                            noDrawble = false;
                        }
                    }

                    if (noDrawble)
                    {
                        cards.front().flip();
                        playerHand.push_back(cards.front());
                        cards.pop_front();
                        playerTurn = !playerTurn;
                    }
                }
                    break;
                case Keyboard::Enter:
                    if (playerTurn)
                    {
                        if (selection->matchesWith(garbage.front()))
                        {
                            garbage.push_front(*selection);
                            playerHand.erase(selection);
                            selection = playerHand.begin();
                        }
                        if (playerHand.empty())
                        {

                        }
                    }
                }
                break;
            }
        }

        if (!playerTurn)
        {
            if (computerHand.empty())
            {

            }
            else
            {
                auto selection = draw(computerHand, garbage.front());
                if (selection == computerHand.end())
                {
                    cards.front().flip();
                    computerHand.push_back(cards.front());
                    cards.pop_front();
                    playerTurn = !playerTurn;
                }

                if (selection->matchesWith(garbage.front()))
                {
                    garbage.push_front(*selection);
                    computerHand.erase(selection);
                    selection = playerHand.begin();
                }
            }
        }

        if (cards.empty())
        {
            for (int i = 0;i < garbage.size() - 1;i++)
            {
                cards.push_front(garbage.back());
                garbage.pop_back();
            }
            shuffle(cards.begin(), cards.end(), default_random_engine(seed));
        }

        window.clear();

        

        window.display();
    }
    return 0;
}