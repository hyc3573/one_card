#include "Card.h"

bool Card::operator<(const Card& rhs) const
{
	return rhs.card.first < this->card.first || (rhs.card.first == this->card.first && rhs.card.second < this->card.second);
}

bool Card::operator==(const Card& rhs) const
{
	return rhs.card == this->card;
}

bool Card::operator>(const Card& rhs) const
{
	return !(*this < rhs || *this == rhs);
}

bool Card::operator<=(const Card& rhs) const
{
	return !(*this > rhs);
}

bool Card::operator>=(const Card& rhs) const 
{
	return !(*this < rhs);
}

bool Card::matchesWith(const Card& rhs) const 
{
	return rhs.card.first == this->card.first || rhs.card.second == this->card.second;
}

void Card::flip()
{
	face = static_cast<Face>(!static_cast<int>(face));
}

bool Card::isFacedFront() const
{
	return face == Face::Front;
}

pair<Pattern, Number> Card::getContents() const
{
	return card;
}
