#include "computer.h"
#include "Card.h"

using namespace std;

list<Card>::iterator draw(list<Card>& hand, Card garbage)
{
	for (auto i = hand.begin();i!=hand.end();i++)
	{ 
		if (garbage.matchesWith(*i))
		{
			return i;
		}
	}
	return hand.end();
}
