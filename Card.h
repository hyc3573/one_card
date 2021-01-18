#ifndef CARD_H
#define CARD_H

#include <utility>
#include <string>

using namespace std;

typedef enum class Pattern { Spade = 0, Heart, Diamond, Club } Pattern;
typedef enum class Number { Ace = 1, Two, Three, Four, Five, Six, Seven, Eight, Nine, Ten, Jack, Queen, King } Number;
typedef enum class Face { Back = 0, Front } Face;

class Card
{
private:
	pair<Pattern, Number> card;
	Face face;

public:
	Card() : card(pair<Pattern, Number>(Pattern::Spade, Number::Ace)), face(Face::Back) {};
	Card(Pattern pattern, Number number) : card(make_pair(pattern, number)), face(Face::Back) {};
	
	bool operator==(const Card& rhs) const;
	bool operator< (const Card& rhs) const;
	bool operator> (const Card& rhs) const;
	bool operator<= (const Card& rhs) const;
	bool operator>= (const Card& rhs) const;
	
	bool matchesWith(const Card& rhs) const;
	
	void flip();
	bool isFacedFront() const;

	pair<Pattern, Number> getContents() const;
};

#endif