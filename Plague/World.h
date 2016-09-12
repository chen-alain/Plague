#pragma once
#include "Country.h"
#include "Disease.h"
#include <vector>
#include <string>
#include <SFML\Graphics.hpp>
#include <SFML\Window.hpp>

using namespace sf;

/*
Used to control the ratye at which a disease spreads.
It can be thought as different diseases having the same
outside factors other than the ones implemented in this class.

950.0 is the maximum distance from the original site.
80.0 is the reproduction rating for the black death.
100.0 is the transmission rating for the black death.
120.0 is approximately the number of months it
took the black death to take over Europe.
*/
const float CONST_OF_SPREAD = 950.0f/( 80.0f/10.0f * 100.0f/10.0f * 120.0f );


class World
{
public:
	World( int, int );
	World(void);
	~World(void);
	
	bool setYear( int );
	int getYear();
	bool setMonth( int );
	int getMonth();
	void increaseMonth( int );
	void setDisease( Disease );
	void setDisease( std::string, int, int, bool, bool, bool, bool );
	void drawMarks( RenderWindow* );
	void updateInfCt();
	int getCountriesNum();
	int getInfectedNum();
	void reset();

private:
	std::vector<Country> countries;
	int year;
	int month;
	int timePassed;//In months.
	int infectionCount;
	int numOfCountries;
	Disease disease;

	void increaseYear();
	void initCountries();
	void sortCountries();
	void outputCountries( char* );
	void updateInfection();
};