/*
Alain Chen

This class represents the virtual world in which the simulation is set.

Last edited: May 8th, 2014.
*/

#include "StdAfx.h"
#include "World.h"
#include "Country.h"
#include "Disease.h"

#include <fstream>
#include <vector>

using namespace std;

World::World( int yr, int mo )
{
	year = yr;
	month = mo;
	infectionCount = 0;
	numOfCountries = 0;
	timePassed = 0;

	initCountries();
}

World::World(void)
{
	year = 0;
	month = 0;
	infectionCount = 0;
	numOfCountries = 0;
	timePassed = 0;

	initCountries();
}

World::~World(void)
{
}

//Sets the current year of the world and returns true if successful.
bool World::setYear( int yr )
{
	//Set the year only if it is empty and a valid argument is given.
	if( year == 0 && yr > 0 )
	{
		year = yr;
		return true;
	}
	
	return false;
}

//Returns the year the world is in.
int World::getYear()
{
	return year;
}

//Sets the current month the world is in.
bool World::setMonth( int mo )
{
	//Set the month only if it is empty and a valid argument is given.
	if( month == 0 && mo >=1 && mo <=12 )
	{
		month = mo;
		return true;
	}
	
	return false;
}

//Returns the current month the world is in.
int World::getMonth()
{
	return month;
}

//Increase the month the world is in by the amount given.
void World::increaseMonth( int num )
{
	month += num;
	timePassed += num;

	//Wrap around and increase year when December becomes January.
	while( month > 12 )
	{
		month-=12;
		increaseYear();
	}

	//Update infection status of countries whenever time has passed.
	updateInfection();

	return;
}

//Sets the disease in this world to the given disease.
void World::setDisease( Disease d )
{
	disease = d;

	//Update the disease type to change the color of the marks.
	countries.at(numOfCountries-2).setInfected( true, disease.getType() );

	return;
}

//Sets the disease in this world with the given specifications.
void World::setDisease( std::string s, int type, int rating, bool isAirborne,
	bool isTouch, bool isSaliva, bool isCreature )
{
	disease.setName( s );
	disease.setType( type );
	disease.setRR( rating );
	disease.setTransmissionMode( isAirborne, isTouch, isSaliva, isCreature );

	//Update the disease type to change the color of the marks.
	countries.at(numOfCountries-2).setInfected( true, type );

	return;
}

//This function returns a vector of circles that represent 
//the infection status of a country.
void World::drawMarks( RenderWindow* windowPtr )
{
	for( int i=0, size=countries.size(); i < size; i++ )
	{
		windowPtr->draw( countries.at(i).getMark() );
	}
}

//Updates the number of infected countries.
void World::updateInfCt()
{
	int count = 0;
	for( int i=0, size=countries.size(); i < size; i++ )
	{
		if( countries.at(i).getInfected() )
			count++;
	}

	infectionCount = count;

	return;
}

//Returns the number of countries in this world.
int World::getCountriesNum()
{
	return numOfCountries;
}

//Returns the number of infected countries in this world.
int World::getInfectedNum()
{
	return infectionCount;
}

//Clears the state of the world.
void World::reset()
{
	year = 0;
	month = 0;
	infectionCount = 0;
	numOfCountries = 0;
	timePassed = 0;

	countries.clear();

	initCountries();

	return;
}

//Increase the year by one.
void World::increaseYear()
{
	year++;

	return;
}

//Initiates the countries of this world, namely Europe.
void World::initCountries()
{
	fstream file;
	file.open( "Files/countries_list.txt", fstream::in );

	while( file.is_open() )
	{
		while( !file.eof() )
		{
			//Temporary storage for the characters read.
			char name[30], xPosStr[10], yPosStr[10];
			float xPos, yPos;

			//Fill temporary storages with null terminating characters.
			for( int j = 0; j < 30; j++ )
			{
				name[j] = '\0';
			}
			for( int j = 0; j < 10; j++ )
			{
				xPosStr[j] = '\0';
				yPosStr[j] = '\0';
			}
			
			//Read the name.
			file.getline( name, 30, ':' );

			//Skip a space.
			file.seekg( 1, ios_base::cur );

			//Get x coordinate.
			file.getline( xPosStr, 10, ',' );

			//Skip a space.
			file.seekg( 1, ios_base::cur );

			//Get y coordinate.
			file.getline( yPosStr, 10 );

			//Convert string to float.
			xPos = float( atoi(xPosStr) );
			yPos = float( atoi(yPosStr) );

			countries.push_back( Country( name, xPos, yPos ) );
		}
		file.close();
	}

	//Update number of countries.
	countries.shrink_to_fit();
	numOfCountries = countries.size();

	//Sort countries.
	sortCountries();

	//Output to new file.
	outputCountries( "Files/sorted.txt" );

	//Set Asia as infected, to show that the plague did not originate 
	//in Europe. OTHER is defined in "Disease.h".
	countries.at(numOfCountries-2).setInfected( true, OTHER );

	updateInfCt();
	
	return;
}

//This function uses an insertion sort 
//to sort the countries by their x-coordinate.
void World::sortCountries()
{
	bool sorted;

	//Element 0 need not to be compared with itself.
	for ( int elementToSort = 1, size = countries.size(); 
		elementToSort < size ; elementToSort++ )
	{
		sorted=true;
		int numberToSort = elementToSort; 

		while( numberToSort > 0 && sorted )	
		{
			//Switch places if out of order.
			if ( countries.at( numberToSort ).getPos().x < 
				countries.at( numberToSort-1 ).getPos().x )
			{
				Country temp = countries.at(numberToSort);
				countries.at( numberToSort ) = countries.at( numberToSort-1 );
				countries.at( numberToSort-1 ) = temp;
			}
			else
				sorted=false;

			numberToSort--;
		}
	}
	
	return;
}

//This function outputs the contents of countries to a file.
void World::outputCountries( char* fileName )
{
	//Output sorted countries to new file.
	fstream file;
	file.open( fileName, fstream::out );

	for( int i=0, size = countries.size() ; i < size ; i++ )
	{
		file << countries.at(i).getName() << ": " << countries.at(i).getPos().x
			<< ", " << countries.at(i).getPos().y << "\n" ;
	}

	file.close();
	
	return;
}

//Updates the infection status of the world.
void World::updateInfection()
{
	//Iterate over countries.
	for( int i=0, size=countries.size(); i < size; i++ )
	{
		//Could only make changes if countries had not been infected already.
		if( !countries.at(i).getInfected() )
		{
			//The disease has spread to the country.
			if( countries.at(i).getDistance() <= 
				( CONST_OF_SPREAD * timePassed * disease.getRR()/10.0f
				* disease.getTransRating()/10.0f ) )
			{
				countries.at(i).setInfected( true, disease.getType() );
			}
		}
	}

	//Update infection count.
	updateInfCt();

	return;
}