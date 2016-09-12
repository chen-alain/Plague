#pragma once
#include <string>
#include <SFML\Graphics.hpp>

using namespace sf;

class Country
{
public:
	Country( std::string, float, float );
	Country(void);
	~Country(void);

	bool setName( std::string );
	std::string getName();
	bool setPos( float, float );
	Vector2f getPos();
	void setInfected( bool, int );
	bool getInfected();
	CircleShape getMark();
	float getDistance();

private:
	std::string name;
	Vector2f position;
	bool infected;
	CircleShape mark;
	float distance;//Distance from orginal site of the disease.

	void setMark();
	void calcDistance();
	void modifyMark( const int );
};