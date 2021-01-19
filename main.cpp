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

    auto getYPos = [&](bool isPlayer) -> float
    {
        return isPlayer ? SHEIGHT - OFFSET - CTHEIGHT : OFFSET + CTHEIGHT;
    };

    auto getHandCardPos = [&](list<Card>& hand,int index, bool isPlayer, int count = -1) -> Vector2f
    {
        if (count == -1)
        {
            count = hand.size();
        }

        float offset = distribute(count, CTWIDTH + OFFSET)[index];
        return Vector2f((SWIDTH - CTWIDTH) / 2 + offset, getYPos(isPlayer));
    };

    auto getGarbageCardPos = [&]() -> Vector2f
    {
        return Vector2f((SWIDTH - CTWIDTH) / 2, (SHEIGHT - CTHEIGHT) / 2);
    };

    auto getCardStackPos = [&]() -> Vector2f
    {
        return getGarbageCardPos() - Vector2f(OFFSET + CTWIDTH, 0);
    };

    auto getTextureRect = [&](Card card) -> IntRect
    {
        auto content = card.getContents();
        return IntRect(CMWIDTH + CTWIDTH * (static_cast<int>(content.second) - 1), CMHEIGHT + CTHEIGHT * (3 - static_cast<int>(content.first)), CTWIDTH, CTHEIGHT);
    };

    auto drawCard = [&](Card card)
    {
        sprite.setTextureRect(getTextureRect(card));
        window.draw(sprite);
    };

    auto checkEmpty = [&]()
    {
        if (cards.empty())
        {
            for (int i = 0;i < garbage.size() - 1;i++)
            {
                garbage.back().flip();
                cards.push_front(garbage.back());
                garbage.pop_back();
            }
            shuffle(cards.begin(), cards.end(), g);
        }
    };

    auto drawHands = [&](list<Card>& hand, bool popup, bool isPlayer)
    {
        auto offsets = distribute(hand.size(), CTWIDTH + OFFSET);

        auto currentCard = hand.begin();
        for (int i = 0;i < hand.size();i++)
        {
            if (currentCard->isFacedFront())
            {
                sprite.setPosition(getHandCardPos(hand, i, isPlayer));

                if (popup && (currentCard == selection))
                {
                    sprite.move(0, -OFFSET);
                }

                drawCard(*currentCard);
            }
            else
            {
                backside.setPosition(getHandCardPos(hand, i, isPlayer));
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

    auto drawAllHands = [&]()
    {
        drawHands(playerHand, true, true);
        drawHands(computerHand, false, false);
    };

    auto drawGarbage = [&]()
    {
        sprite.setPosition(getGarbageCardPos());
        drawCard(garbage.front());
    };

    auto drawCardStack = [&]()
    {
        backside.setPosition(getCardStackPos());
        window.draw(backside);
    };

    auto drawAll = [&]()
    {
        drawGarbage();
        drawCardStack();

        drawAllHands();
    };

    auto popCard = [&](deque<Card>& cardStack, list<Card>& hand, list<Card>& opponentCard, bool flip)
    {
        window.clear();

        drawGarbage();
        drawCardStack();

        drawHands(opponentCard, !flip, !flip);

        if (flip)
        {
            cardStack.front().flip();
        }
        hand.push_back(cardStack.front());
        cardStack.pop_front();

        Animation ani(window);

        vector<Sprite> sprites(hand.size(), sprite);
        vector<RectangleShape> rects(hand.size(), backside);

        auto iter = hand.begin();
        for (int i = 0;i < hand.size() - 1;i++)
        {
            if (flip)
            {
                sprites[i].setTextureRect(getTextureRect(*iter));
                sprites[i].setPosition(getHandCardPos(hand, i, flip, hand.size() - 1));
                sprites[i].setTextureRect(getTextureRect(*iter));
                ani.addTarget(AnimTarget(&sprites[i], sprites[i], getHandCardPos(hand, i, flip), speed, accel));
            }
            else
            {
                rects[i].setPosition(getHandCardPos(hand, i, flip, hand.size() - 1));
                ani.addTarget(AnimTarget(&rects[i], rects[i], getHandCardPos(hand, i, flip), speed, accel));
            }

            iter++;
        }

        if (flip)
        {
            sprites.back().setPosition(getCardStackPos());
            sprites.back().setTextureRect(getTextureRect(*iter));
            ani.addTarget(AnimTarget(&sprites.back(), sprites.back(), getHandCardPos(hand, hand.size() - 1, flip), speed, accel));
        }
        else
        {
            rects.back().setPosition(getCardStackPos());
            ani.addTarget(AnimTarget(&rects.back(), rects.back(), getHandCardPos(hand, hand.size() - 1, flip), speed, accel));
        }

        ani.start();
        
        playerTurn = !playerTurn;
    };

    auto drawCardTo = [&](list<Card>& hand, list<Card>& opponentCard, Card card, deque<Card>& cardStack, bool flip, bool isPlayer)
    {
        window.clear();

        drawGarbage();
        drawCardStack();
        drawHands(opponentCard, !isPlayer, !isPlayer);

        Animation ani(window);

        if (flip)
        {
            card.flip();
        }

        int index = 0;
        for (auto i = hand.begin();i != hand.end();i++)
        {
            if (card == *i)
            {
                break;
            }
            index++;
        }

        Sprite spr(sprite);
        spr.setPosition(getHandCardPos(hand, index, isPlayer));
        spr.setTextureRect(getTextureRect(card));

        hand.remove(card);
        cardStack.push_front(card);

        vector<Sprite> sprites(hand.size(), sprite);
        vector<RectangleShape> rects(hand.size(), backside);
        auto carditer = hand.begin();
        for (int i = 0;i < hand.size();i++)
        {
            if (!flip)
            {
                sprites[i].setPosition(getHandCardPos(hand, (i < index) ? i : i + 1, isPlayer, hand.size() + 1));
                sprites[i].setTextureRect(getTextureRect(*carditer));
                ani.addTarget(AnimTarget(&sprites[i], sprites[i], getHandCardPos(hand, i, isPlayer), speed, accel));
            }
            else
            {
                rects[i].setPosition(getHandCardPos(hand, i, isPlayer, hand.size() + 1));
                ani.addTarget(AnimTarget(&rects[i], rects[i], getHandCardPos(hand, i, isPlayer), speed, accel));
            }
            carditer++;
        }

        ani.addTarget(AnimTarget(&spr, spr, getGarbageCardPos(), speed, accel));

        ani.start();

        playerTurn = !playerTurn;
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
                        popCard(cards, playerHand, computerHand, true);
                        selection = playerHand.begin();
                    }
                }
                    break;
                case Keyboard::Enter:
                    if (playerTurn)
                    {
                        if (selection->matchesWith(garbage.front()))
                        {
                            drawCardTo(playerHand, computerHand, *selection, garbage, false, true);
                            selection = playerHand.begin();
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

        if (!(playerTurn || playerWon))
        {
            auto cSelection = draw(computerHand, garbage.front());
            if (cSelection == emptyCard)
            {
                popCard(cards, computerHand, playerHand, false);
            }
            else
            {
                drawCardTo(computerHand, playerHand, cSelection, garbage, true, false );
            }

            if (computerHand.empty())
            {
                computerWon = true;
            }
        }

        checkEmpty();

        window.clear();

        drawAll();

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