#ifndef COMPUTER_H
#define COMPUTER_H

#include <list>
#include <iterator>
#include "Card.h"
#include <deque>

using namespace std;

list<Card>::iterator draw(list<Card>& hand, Card garbage);

#endif