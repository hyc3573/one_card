#include <SFML/Graphics.hpp>
#include <iostream>
#include <cstdlib>
#include <Windows.h>

#define SWIDTH 800
#define SHEIGHT 800

typedef int Square;

using namespace std;
using namespace sf;

int main()
{
    unsigned int t = GetTickCount64();
    srand(t);

    RenderWindow window(VideoMode(SWIDTH, SHEIGHT), L"크고♂아름다운♂게이ㅁ");

    Event event;

    Clock clock;

    while (window.isOpen()) 
    {
        while (window.pollEvent(event))
        {
            switch (event.type)
            {
            case Event::Closed:
                window.close();
                break;
            }
        }

        window.clear();

        window.display();
    }
    return 0;
}