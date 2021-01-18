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
#include "ConnectionIii-Rj3W.hpp"

typedef int Square;

using namespace std;
using namespace sf;

vector<float> distribute(int count, float interval)
{
    vector<float> result;
    float first = -(interval/2)*(count - 1);
    for (int i = 0;i < count;i++)
    {
        result.push_back(first + interval * i);
    }

    return result;
}

int main()
{
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

    Sprite sprite;
    sprite.setTexture(texture);

    RectangleShape card(Vector2f(CWIDTH, CHEIGHT));
    card.setFillColor(Color::Red);
    card.setOrigin(card.getLocalBounds().width / 2, card.getLocalBounds().height);
    card.setPosition(SWIDTH / 2, SHEIGHT + 30);

    RectangleShape cCard(Vector2f(CWIDTH, CHEIGHT));
    card.setFillColor(Color::Blue);
    card.setOrigin(card.getLocalBounds().width / 2, card.getLocalBounds().height);
    card.setPosition(SWIDTH / 2, 0 - 30);

    bool playerTurn;

    random_device rd;
    mt19937 g(rd());

    Font font;
    if (!font.loadFromMemory(ConnectionIii_Rj3W_otf, ConnectionIii_Rj3W_otf_len))
    {
        cout << "Error!" << endl;
    }

    Text comWinText;
    comWinText.setFillColor(Color::White);
    comWinText.setOutlineColor(Color::Black);
    comWinText.setOutlineThickness(5);
    comWinText.setPosition(SWIDTH / 2, SHEIGHT / 2);
    comWinText.setFont(font);
    comWinText.setCharacterSize(50);
    comWinText.setString(L"ezzzzzzzzzzzzzzzzz");
    comWinText.setOrigin(comWinText.getLocalBounds().width / 2, comWinText.getLocalBounds().height / 2);

    Text plyWinText(comWinText);
    plyWinText.setString(L"Y u HaX?");
    plyWinText.setOrigin(plyWinText.getLocalBounds().width / 2, plyWinText.getLocalBounds().height / 2);

    bool playerWon;
    bool computerWon;

    Texture back;
    if (!back.create(SWIDTH, SHEIGHT))
    {
        cout << "Error!" << endl;
    }

    RectangleShape backside(Vector2f(CTWIDTH, CTHEIGHT));
    backside.setFillColor(Color::Blue);

    auto reset = [&]()
    {
        cards = deque<Card>();
        playerHand = list<Card>();
        computerHand = list<Card>();
        garbage = deque<Card>();

        for (int p = 0; p < 3;p++)
        {
            for (int n = 1; n < 14;n++)
            {
                cards.push_front(Card(static_cast<Pattern>(p), static_cast<Number>(n)));
            }
        }
        shuffle(cards.begin(), cards.end(), g);

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
        cards.pop_front();

        playerTurn = true;

        selection = playerHand.begin();

        playerWon = false;
        computerWon = false;
    };

    auto drawCard = [&](Card card)
    {
        auto content = card.getContents();
        sprite.setTextureRect(IntRect(CMWIDTH + CTWIDTH * (static_cast<int>(content.second) - 1), CMHEIGHT + CTHEIGHT * (3 - static_cast<int>(content.first)), CTWIDTH, CTHEIGHT));
        window.draw(sprite);
    };

    auto checkEmpty = [&]()
    {
        if (cards.empty())
        {
            for (int i = 0;i < garbage.size() - 1;i++)
            {
                cards.push_front(garbage.back());
                garbage.pop_back();
            }
            shuffle(cards.begin(), cards.end(), g);
        }
    };

    auto drawHands = [&](list<Card>& hand, bool popup, float Y)
    {
        auto offsets = distribute(hand.size(), CTWIDTH + OFFSET);

        auto currentCard = hand.begin();
        for (int i = 0;i < hand.size();i++)
        {
            if (currentCard->isFacedFront())
            {
                sprite.setPosition((SWIDTH - CTWIDTH) / 2 + offsets[i], Y);

                if (popup && (currentCard == selection))
                {
                    sprite.move(0, -OFFSET);
                }

                drawCard(*currentCard);
            }
            else
            {
                backside.setPosition((SWIDTH - CTWIDTH) / 2 + offsets[i], Y);
                window.draw(backside);
            }
            currentCard++;
        }
    };

    auto playerWonScreen = [&]()
    {
        back.update(window);
        Sprite sprite(back);

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
                    case Keyboard::R:
                        return;
                        break;
                    }
                    break;
                }
            }

            window.clear();

            window.draw(sprite);
            window.draw(plyWinText);

            window.display();
        }
    };

    auto computerWonScreen = [&]()
    {
        back.update(window);
        Sprite sprite(back);

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
                    case Keyboard::R:
                        return;
                        break;
                    }
                    break;
                }
            }

            window.clear();

            window.draw(sprite);
            window.draw(comWinText);

            window.display();
        }
    };

    restart:

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
                            playerTurn = !playerTurn;
                        }
                        if (playerHand.empty())
                        {
                            playerWon = true;
                        }
                    }
                }
                break;
            }
        }

        checkEmpty();

        if (!playerTurn)
        {
            auto cSelection = draw(computerHand, garbage.front());
            if (cSelection == emptyCard)
            {
                computerHand.push_back(cards.front());
                cards.pop_front();
            }

            for (auto i = computerHand.begin();i != computerHand.end();i++)
            {
                if (*i == cSelection)
                {
                    garbage.push_front(*i);
                    computerHand.erase(i);
                    break;
                }
            }
            playerTurn = !playerTurn;

            if (computerHand.empty())
            {
                computerWon = true;
            }
        }

        checkEmpty();

        window.clear();

        sprite.setPosition((SWIDTH - CTWIDTH) / 2, (SHEIGHT - CTHEIGHT) / 2);
        drawCard(garbage.front());

        drawHands(playerHand, true, SHEIGHT - OFFSET - CTHEIGHT);
        drawHands(computerHand, false, OFFSET + CTHEIGHT);

        if (playerWon)
        {
            playerWonScreen();
            goto restart;
        }
        if (computerWon)
        {
            computerWonScreen();
            goto restart;
        }

        window.display();
    }
    return 0;
}