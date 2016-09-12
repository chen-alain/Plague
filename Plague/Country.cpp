/*
Alain Chen

This class represents the individual countries 
or regions the disease might invade.

Last edited: May 8th, 2014.
*/

#include "StdAfx.h"
#include "Country.h"
#include "Disease.h"
#include <cmath>


Country::Country( std::string s, float xPos, float yPos )
{
	name = "";
	position.x = 0.0;
	position.y = 0.0;
	distance = 0.0;
	infected = false;

	setName( s );
	setPos( xPos, yPos );
	infected = false;
	calcDistance();
}

Country::Country(void)
{
	name = "";
	position.x = 0.0;
	position.y = 0.0;
	distance = 0.0;
	infected = false;
}

Country::~Country(void)
{
}

//Sets the name of the country and returns true if successful.
bool Country::setName( std::string s )
{
	//Set the country's name only if it is empty.
	if( name.empty() )
	{
		name = s;
		return true;
	}
	
	return false;
}

//Returns the name of the country.
std::string Country::getName()
{
	return name;
}

//Sets the position of the country relative to the window 
//and returns true if successful.
bool Country::setPos( float xPos, float yPos )
{
	//Set the country's position only if it is empty.
	if( position.x ==  0.0 && position.y ==  0.0 )
	{
		position.x = xPos;
		position.y = yPos;

		setMark();
		calcDistance();
		return true;
	}

	return false;
}

//Returns the position of the country in vector form.
Vector2f Country::getPos()
{
	return position;
}

//Sets whether the country has been infected by the disease or not.
void Country::setInfected( bool option, int diseaseType )
{
	infected = option;

	modifyMark( diseaseType );

	return;
}

//Returns whether a country is infected or not.
bool Country::getInfected()
{
	return infected;
}

//Returns the circle that represents the status of infection.
CircleShape Country::getMark()
{
	return mark;
}

//Returns the distance the country is away from where the disease
//first appeared.
float Country::getDistance()
{
	return distance;
}

//Sets the circle that represents the status of infection.
void Country::setMark()
{
	mark.setRadius( 10 );

	//Center origin at the middle of circle.
	FloatRect textRect = mark.getGlobalBounds();
	mark.setOrigin( textRect.left + textRect.width/2.0f,
		textRect.top + textRect.height/2.0f );

	mark.setPosition( position );
	mark.setOutlineColor( Color::Transparent );
	mark.setFillColor( Color::Transparent );

	return;
}

//Calculates the distance from the original site of the disease.
void Country::calcDistance()
{
	distance = std::sqrtf( std::powf( 900-position.x, 2.0 ) 
		+ std::powf( 650-position.y, 2.0 ) );

	return;
}

//Modifies the color of the circle that represents the status of infection,
//according to the type of disease give.
void Country::modifyMark( const int diseaseType )
{
	if( infected )
	{
		if( diseaseType == BACTERIUM )
			mark.setFillColor( Color::Red );
		else if( diseaseType == FUNGUS )
			mark.setFillColor( Color( 43, 100, 200 ) );
		else
			mark.setFillColor( Color::Black );
	}
	else
		mark.setFillColor( Color::Transparent );

	return;
}