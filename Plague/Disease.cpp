/*
Alain Chen

This class represents the disease the simulation is based around.

Last edited: May 8th, 2014.
*/

#include "StdAfx.h"
#include "Disease.h"

Disease::Disease( std::string s, int typeGiven, int rating, bool isAirborne,
	bool isTouch, bool isSaliva, bool isCreature )
{
	name = "";
	type = -1;
	reproductionRating = 0;
	for( int i=0; i<4; i++ )
		transmissionMode[i] = false;
	transmissionRating = 20;

	setName( s );
	setType( typeGiven );
	setRR( rating );
	setTransmissionMode( isAirborne, isTouch, isSaliva, isCreature );

}

Disease::Disease( std::string s, int typeGiven, int rating, bool modes[] )
{
	name = "";
	type = -1;
	reproductionRating = 0;
	for( int i=0; i<4; i++ )
		transmissionMode[i] = false;
	transmissionRating = 20;

	setName( s );
	setType( typeGiven );
	setRR( rating );

	//In case an array size less than 4 is provided.
	int size = sizeof( modes ), j = 0;
	bool temp[4];
	for( int i=0; i < 4; i++ )
		temp[i] = false;
	while( j < 4 && j < size )
	{
		temp[j] = modes[j];
		j++;
	}

	setTransmissionMode( temp[0], temp[1], temp[2], temp[3] );

}

Disease::Disease(void)
{
	name = "";
	type = -1;
	reproductionRating = 0;
	transmissionRating = 20;
	for( int i=0; i<4; i++ )
		transmissionMode[i] = false;
}

Disease::~Disease(void)
{
}

//Sets the name of the disease and returns true if successful.
bool Disease::setName( std::string s )
{
	//Set the disease's name only if it is empty.
	if( name.empty() )
	{
		name = s;
		return true;
	}
	
	return false;
}

//Returns the name of the disease.
std::string Disease::getName()
{
	return name;
}

//Sets the type of the disease and returns true if successful.
bool Disease::setType( int option )
{
	//Set the disease's type only if it is empty.
	if( type == -1 )
	{
		type = option;
		return true;
	}
	
	return false;
}

//Returns the type of the disease.
int Disease::getType()
{
	return type;
}

//Sets the reproduction rating of the disease and returns true if successful.
bool Disease::setRR( int rate )
{
	//Set the disease's reproduction rating only if it is the default.
	if( reproductionRating == 0 )
	{
		reproductionRating = rate;
		return true;
	}
	
	return false;
}

//Returns the reproduction rating of the disease.
int Disease::getRR()
{
	return reproductionRating;
}

//Returns the transmission rating of the disease.
int Disease::getTransRating()
{
	return transmissionRating;
}

//Sets whether or not that particular transmission mode is one that the
//disease utilizes and update the transmission rating.
void Disease::setTransmissionMode( bool isAirborne, bool isTouch, 
	bool isSaliva, bool isCreature )
{
	transmissionMode[AIRBORNE] = isAirborne;
	transmissionMode[TOUCH] = isTouch;
	transmissionMode[SALIVA] = isSaliva;
	transmissionMode[CREATURE] = isCreature;

	//Set transmission rating.
	//20 + 29 + 23 + 17 + 11 == 100, which is the highest possible rating.
	transmissionRating = 20;
	if( isAirborne )
	{
		transmissionRating += 29;
	}
	
	if( isTouch )
	{
		transmissionRating += 23;
	}

	if( isSaliva )
	{
		transmissionRating += 17;
	}

	if( isCreature )
	{
		transmissionRating += 11;
	}
}

//Returns false if an invalid index is given or if the disease does 
//not use that mode of transmission to spread.
bool Disease::getTransmissionMode( int index )
{
	//Invalid index, return false.
	if( index < 0 || index >= 4 )
	{
		return false;
	}

	//Return whether or not the mode is used.
	return transmissionMode[index];
}
