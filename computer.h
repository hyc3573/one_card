#ifndef COMPUTER_H
#define COMPUTER_H

#include <list>
#include <iterator>
#include "Card.h"
#include <deque>

using namespace std;

Card draw(list<Card>& hand, Card garbage);

#endif