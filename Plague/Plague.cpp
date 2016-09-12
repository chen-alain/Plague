/*
Alain Chen

This program, PLAGUE, contains two main parts. 
The first part simulates the actual spreading of the black death, 
which is also known as the bubonic plague or the Great Plague.
The second part allows the user to modify variables that affect the 
spreading of diseases and simulates that.

Created: Apr 8th, 2014.
Last edited: May 8th, 2014.

I hereby certify that the program below represents my work and the design,
content, and logic was completed without outside assistance.
*/

#include "stdafx.h"
#include "Country.h"
#include "Disease.h"
#include "World.h"

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <windows.h>
#include <string>
#include <stdio.h>
#include <time.h>

using namespace sf;

const int WINDOW_HEIGHT = 800;
const int WINDOW_WIDTH = 1200;

//Simple data structure to keep track of user input.
struct Info
{
	int type;
	int rating;
	bool transmissionModes[4];
};

void displayMenu( int&, RenderWindow*, bool, Info* );
bool input( int&, RenderWindow*, bool, Info* );
void simulation( int&, RenderWindow*, bool, World* );
bool intersects( Vector2i*, RectangleShape* );
std::string monthToString( const int );
template <class T>
void centerOrigin( T* );

enum STATE{ MENU, BLACK_DEATH, USER, SIM };

int _tmain(int argc, _TCHAR* argv[])
{
	FreeConsole();

	RenderWindow window( VideoMode( WINDOW_WIDTH, WINDOW_HEIGHT ), "PLAGUE" );

	Event e;
	int state = MENU;
	World world;
	bool pressed = false;
	Info userProvided;


	//Default values for user provided info.
	userProvided.type = OTHER;//Defined in "Disease.h".
	userProvided.rating = 50;
	for( int i = 0; i < 4; i++ )
		userProvided.transmissionModes[i] = false;

	//Load map and return error if not able to load file.
	Image original;
	if(	!original.loadFromFile( "Files/Europe.jpg" ) )
		return -1;
	Texture texture;
	texture.loadFromImage( original );
	RectangleShape map;
	map.setSize( Vector2f( float( original.getSize().x ),
		float( original.getSize().y ) ) );
	map.setTexture( &texture );
	map.setPosition( 0, 0 );

	while(window.isOpen())
	{
		//Event loop.
		while(window.pollEvent(e))
		{
			pressed = false;

			if( e.type == Event::MouseButtonPressed &&
				e.mouseButton.button == Mouse::Left )
				pressed = true;

			//User closes the window.
			if( e.type == Event::Closed )
				window.close();
			
			//Program is at the menu page.
			if( state == MENU )
			{
				//Display menu.
				window.clear( Color( 20, 20, 20 ) );
				displayMenu( state, &window, pressed, &userProvided );
				window.display();
			}
			
			//User wants to simulate the black death.
			if( state == BLACK_DEATH )
			{
				world.setDisease( Disease( "PLAGUE", BACTERIUM, 80,
					true, true, true, true ) );
				world.setYear( 1344 );
				world.setMonth( 1 );
				state = SIM;
			}

			//User wants to input own variables.
			if( state == USER )
			{
				window.clear( Color( 150, 170, 195 ) );

				//User submitted information, so set world and disease.
				if( input( state, &window, pressed, &userProvided ) )
				{
					world.setDisease( Disease( "USER", 
						userProvided.type, userProvided.rating, 
						userProvided.transmissionModes ) );

					srand(time(NULL));
					int randYear = rand() % 5000 + 1;
					int randMonth = rand() % 12 + 1;

					world.setYear( randYear );
					world.setMonth( randMonth );
					state = SIM;
				}
				window.display();
			}

			//The current state is simulating.
			if( state == SIM )
			{
				window.clear( Color( 200, 200, 200 ) );
				window.draw(map);
				simulation( state, &window, pressed, &world );
				window.display();
			}
		}
		
	}

	return 0;
}

//This function displays the menu of Plague and modifies the state
//of the program as the user clicks on objects.
void displayMenu( int& state, RenderWindow* windowPtr, bool pressed, 
	Info* userInfoPtr )
{
	Font font;
	Text plagueText, blackDeathText, diseaseText;
	RectangleShape blackDeathButton, diseaseButton;
	Vector2i mousePos = Mouse::getPosition( *windowPtr );

	//Set font of texts.
	if( font.loadFromFile( "Files/ARLRDBD.TTF" ) )
	{
		plagueText.setFont( font );
		blackDeathText.setFont( font );
		diseaseText.setFont( font );
	}
		
	//Appearane of the text containing plague.
	plagueText.setString( "PLAGUE" );	
	plagueText.setColor( Color::Red );
	plagueText.setCharacterSize( 100 );
	centerOrigin( &plagueText );
	plagueText.setPosition( WINDOW_WIDTH/2.0f, WINDOW_HEIGHT/5.0f );
		
	//Appearane of the text containing the black death.
	blackDeathText.setString( "The Black Death" );	
	blackDeathText.setColor( Color::Black );
	blackDeathText.setCharacterSize( 30 );
	centerOrigin( &blackDeathText );
	blackDeathText.setPosition( WINDOW_WIDTH/2.0f, WINDOW_HEIGHT/2.2f );

	//Appearane of the text containing the disease simulator.
	diseaseText.setString( "Disease Simulator" );	
	diseaseText.setColor( Color::Blue );
	diseaseText.setCharacterSize( 30 );
	centerOrigin( &diseaseText );
	diseaseText.setPosition( WINDOW_WIDTH/2.0f, 
		blackDeathText.getPosition().y + 150 );

	//Appearance of the button of the black death.
	blackDeathButton.setSize( 
		Vector2f( blackDeathText.getGlobalBounds().width + 10,
		blackDeathText.getGlobalBounds().height + 10 ) );
	centerOrigin( &blackDeathButton );
	blackDeathButton.setPosition( blackDeathText.getPosition() );
	blackDeathButton.setOutlineColor( Color::Magenta );
	blackDeathButton.setOutlineThickness( 3 );
	
	//Change color if mouse is hovered over and 
	//change state if clicked on.
	if( intersects( &mousePos, &blackDeathButton ) )
	{
		blackDeathButton.setFillColor( Color::Magenta );

		if( pressed )
		{
			state = BLACK_DEATH;
		}
	}
	else
		blackDeathButton.setFillColor( Color::Yellow );

	//Appearance of the button of the disease simulator.
	diseaseButton.setSize( Vector2f( diseaseText.getGlobalBounds().width + 10,
		diseaseText.getGlobalBounds().height + 10 ) );
	centerOrigin( &diseaseButton );
	diseaseButton.setPosition( diseaseText.getPosition() );
	diseaseButton.setOutlineColor( Color::Magenta );
	diseaseButton.setOutlineThickness( 3 );

	//Change color if mouse is hovered over and 
	//change state if clicked on.
	if( intersects( &mousePos, &diseaseButton ) )
	{
		diseaseButton.setFillColor( Color::Magenta );

		if( pressed )
		{
			state = USER;

			//Go back to original settings.
			userInfoPtr->type = OTHER;//Defined in "Disease.h".
			userInfoPtr->rating = 50;
			for( int i = 0; i < 4; i++ )
				userInfoPtr->transmissionModes[i] = false;
		}
	}
	else
		diseaseButton.setFillColor( Color::Green );
	
	//Draw objects.
	windowPtr->draw( plagueText );
	windowPtr->draw( blackDeathButton );
	windowPtr->draw( diseaseButton );
	windowPtr->draw( blackDeathText );
	windowPtr->draw( diseaseText );


	//Button that allows the user to exit the program.
	Text quitText;
	RectangleShape quitButton;
	if( font.loadFromFile( "Files/ARLRDBD.TTF" ) )
	{
		quitText.setFont( font );
	}

	//Set text of quit button.
	quitText.setString( "QUIT" );
	quitText.setColor( Color::Black );
	quitText.setCharacterSize( 38 );
	centerOrigin( &quitText );
	quitText.setPosition( 1100, 100 );

	//Appearance of the quit button.
	quitButton.setSize( Vector2f( quitText.getGlobalBounds().width + 10,
		quitText.getGlobalBounds().height + 10 ) );
	centerOrigin( &quitButton );
	quitButton.setPosition( quitText.getPosition() );
	quitButton.setOutlineColor( Color::Yellow );
	quitButton.setOutlineThickness( 3 );
	quitButton.setFillColor( Color( 70, 240, 250 ) );

	//Change color if mouse is hovered over and 
	//return to quit if clicked on.
	if( intersects( &mousePos, &quitButton ) )
	{
		quitButton.setFillColor( Color( 250, 240, 70 ) );

		if( pressed )
		{
			windowPtr->close();
		}
	}

	//Draw objects.
	windowPtr->draw( quitButton );
	windowPtr->draw( quitText );

}

//This function allows the user to input the desired values as variables.
//It returns whether the user has submitted information or not.
bool input( int& state, RenderWindow* windowPtr, bool pressed,
	Info* userInfoPtr )
{
	//Position of mouse.
	Vector2i mousePos = Mouse::getPosition( *windowPtr );
	Font font;


	//Disease information.
	Text typeTitle, typesText[3], transmissionTitle, transmissionsText[4],
		reproductionTitle, highTitle, lowTitle;
	RectangleShape typeButton[3], transmissionsButton[4], reproductionSlider;

	if( font.loadFromFile( "Files/ARLRDBD.TTF" ) )
	{
		typeTitle.setFont( font );
		transmissionTitle.setFont( font ); 
		reproductionTitle.setFont( font ); 
		highTitle.setFont( font ); 
		lowTitle.setFont( font );

		for( int i = 0; i < 3; i++ )
			typesText[i].setFont( font );

		for( int i = 0; i < 4; i++ )
			transmissionsText[i].setFont( font );
	}

	//The title for the types of diseases.
	typeTitle.setString( "TYPE OF ORGANISM" );
	typeTitle.setColor( Color::Blue );
	typeTitle.setStyle( Text::Underlined );
	typeTitle.setCharacterSize( 36 );
	typeTitle.setPosition( 20, 100 );

	//The title for the modes of transportation.
	transmissionTitle.setString( "TRANSMISSION MODES" );
	transmissionTitle.setColor( Color::Blue );
	transmissionTitle.setStyle( Text::Underlined );
	transmissionTitle.setCharacterSize( 36 );
	transmissionTitle.setPosition( 20, 300 );

	//The title for reprodution rating.
	reproductionTitle.setString( "RATE OF REPRODUCTION" );
	reproductionTitle.setColor( Color::Blue );
	reproductionTitle.setStyle( Text::Underlined );
	reproductionTitle.setCharacterSize( 36 );
	reproductionTitle.setPosition( 20, 500 );

	//Draw objects.
	windowPtr->draw( typeTitle );
	windowPtr->draw( transmissionTitle );
	windowPtr->draw( reproductionTitle );

	//The text and button of the types of diseases.
	typesText[0].setString( "BACTERIUM" );
	typesText[1].setString( "FUNGUS" );
	typesText[2].setString( "OTHER" );
	for( int i = 0; i < 3; i++ )
	{
		typesText[i].setColor( Color::Blue );
		typesText[i].setCharacterSize( 20 );
		centerOrigin( &typesText[i] );
		typesText[i].setPosition( 100.0f + 150.0f*i, 200.0f );

		//The size of typesText[0] is used so each button would have same size.
		typeButton[i].setSize( 
			Vector2f( typesText[0].getGlobalBounds().width + 10,
			typesText[0].getGlobalBounds().height + 10 ) );

		centerOrigin( &typeButton[i] );
		typeButton[i].setPosition( typesText[i].getPosition() );
		typeButton[i].setOutlineColor( Color::Black );
		typeButton[i].setOutlineThickness( 3 );

		if( userInfoPtr->type == i )
			typeButton[i].setFillColor( Color( Color::Red ) );
		else
			typeButton[i].setFillColor( Color( Color::Cyan ) );

		//Change color if selected
		if( intersects( &mousePos, &typeButton[i] ) && pressed )
		{
			userInfoPtr->type = i;
		}

		//Draw objects.
		windowPtr->draw( typeButton[i] );
		windowPtr->draw( typesText[i] );
	}

	//The text and button of the modes of transmission.
	transmissionsText[0].setString( "AIRBORNE" );
	transmissionsText[1].setString( "BY TOUCH" );
	transmissionsText[2].setString( "BY SALIVA" );
	transmissionsText[3].setString( "BY CREATURES" );
	for( int i = 3; i >= 0 ; i-- )
	{
		transmissionsText[i].setColor( Color::Blue );
		transmissionsText[i].setCharacterSize( 20 );
		centerOrigin( &transmissionsText[i] );
		transmissionsText[i].setPosition( 100.0f + 200.0f*i, 400.0f );

		//The size of transmissionsText[3] is used 
		//so each button would have same size.
		transmissionsButton[i].setSize( 
			Vector2f( transmissionsText[3].getGlobalBounds().width + 10,
			transmissionsText[3].getGlobalBounds().height + 10 ) );

		centerOrigin( &transmissionsButton[i] );
		transmissionsButton[i].setPosition( 
			transmissionsText[i].getPosition() );
		transmissionsButton[i].setOutlineColor( Color::Black );
		transmissionsButton[i].setOutlineThickness( 3 );

		if( userInfoPtr->transmissionModes[i] )
			transmissionsButton[i].setFillColor( Color::Red );
		else
			transmissionsButton[i].setFillColor( Color::Cyan );

		//Change color if selected
		if( intersects( &mousePos, &transmissionsButton[i] ) && pressed )
		{
			bool option = userInfoPtr->transmissionModes[i];
			userInfoPtr->transmissionModes[i] = (!option);
		}

		//Draw objects.
		windowPtr->draw( transmissionsButton[i] );
		windowPtr->draw( transmissionsText[i] );
	}


	//Slider for reproduction rating.
	RectangleShape line, slider, addButton, subtractButton;
	Text addText, subtractText, ratingText;

	if( font.loadFromFile( "Files/ARLRDBD.TTF" ) )
	{
		addText.setFont( font );
		subtractText.setFont( font );
		ratingText.setFont( font );
	}

	//The line the slider is on.
	line.setSize( Vector2f( 400, 0 ) );
	line.setOutlineThickness( 2 );
	line.setOutlineColor( Color::Black );
	line.setFillColor( Color::Black );
	centerOrigin( &line );
	line.setPosition( reproductionTitle.getPosition().x + 30 + 
		line.getSize().x/2.0f, reproductionTitle.getPosition().y + 150 );

	//The slider itself.
	slider.setSize( Vector2f( 10, 45 ) );
	slider.setOutlineThickness( 1 );
	slider.setOutlineColor( Color( 156, 140, 30 ) );
	slider.setFillColor( Color( 120, 100, 2 ) );
	centerOrigin( &slider );
	slider.setPosition( line.getPosition().x - line.getSize().x / 2.0f
		+ userInfoPtr->rating * line.getSize().x / 99.0f,
		line.getPosition().y );

	//Button to increase rating.
	addButton.setSize( Vector2f( 28, 28 ) );
	addButton.setOutlineThickness( 2 );
	addButton.setOutlineColor( Color::Black );
	addButton.setFillColor( Color::Cyan );
	centerOrigin( &addButton );
	addButton.setPosition( line.getPosition().x + line.getSize().x/2.0f + 30,
		line.getPosition().y );

	//Plus sign on the button to increase rating.
	addText.setString( "+" );
	addText.setColor( Color::Black );
	addText.setCharacterSize( 28 );
	centerOrigin( &addText );
	addText.setPosition( addButton.getPosition() );

	//Button to decrease rating.
	subtractButton.setSize( Vector2f( 28, 28 ) );
	subtractButton.setOutlineThickness( 2 );
	subtractButton.setOutlineColor( Color::Black );
	subtractButton.setFillColor( Color::Cyan );
	centerOrigin( &subtractButton );
	subtractButton.setPosition( line.getPosition().x - 
		line.getSize().x/2.0f - 30, line.getPosition().y );
	
	//Minus sign on the button to decrease rating.
	subtractText.setString( "-" );
	subtractText.setColor( Color::Black );
	subtractText.setCharacterSize( 30 );
	centerOrigin( &subtractText );
	subtractText.setPosition( subtractButton.getPosition() );

	//A number that informs the user of the current rating the
	//slider is on.
	char buffer[5];
	sprintf_s( buffer, "%d", userInfoPtr->rating );
	ratingText.setString( buffer );
	ratingText.setColor( Color( 125, 25, 255 ) );
	ratingText.setStyle( Text::Bold );
	ratingText.setCharacterSize( 40 );
	centerOrigin( &ratingText );
	ratingText.setPosition( slider.getPosition().x,
		slider.getPosition().y - 50 );

	//Modify the slider if the button to increase is pressed.
	if( intersects( &mousePos, &addButton ) && pressed )
	{
		if( userInfoPtr->rating < 100 )
			userInfoPtr->rating++;
	}

	//Modify the slider if the button to decrease is pressed.
	if( intersects( &mousePos, &subtractButton ) && pressed )
	{
		if( userInfoPtr->rating > 1 )
			userInfoPtr->rating--;
	}

	//Draw objects.
	windowPtr->draw( line );
	windowPtr->draw( slider );
	windowPtr->draw( addButton );
	windowPtr->draw( subtractButton );
	windowPtr->draw( addText );
	windowPtr->draw( subtractText );
	windowPtr->draw( ratingText );

	//Return to menu.
	Text menuText;
	RectangleShape menuButton;
	if( font.loadFromFile( "Files/ARLRDBD.TTF" ) )
	{
		menuText.setFont( font );
	}

	//Set text of menu button.
	menuText.setString( "QUIT" );
	menuText.setColor( Color::Black );
	menuText.setCharacterSize( 38 );
	centerOrigin( &menuText );
	menuText.setPosition( 1100, 100 );

	//Appearance of the menu button.
	menuButton.setSize( Vector2f( menuText.getGlobalBounds().width + 10,
		menuText.getGlobalBounds().height + 10 ) );
	centerOrigin( &menuButton );
	menuButton.setPosition( menuText.getPosition() );
	menuButton.setOutlineColor( Color::Yellow );
	menuButton.setOutlineThickness( 3 );
	menuButton.setFillColor( Color( 70, 240, 250 ) );

	//Change color if mouse is hovered over and 
	//return to menu if clicked on.
	if( intersects( &mousePos, &menuButton ) )
	{
		menuButton.setFillColor( Color( 250, 240, 70 ) );

		if( pressed )
		{
			state = MENU;
		}
	}

	//Draw objects.
	windowPtr->draw( menuButton );
	windowPtr->draw( menuText );


	//Submit info.
	Text submitText;
	RectangleShape submitButton;
	if( font.loadFromFile( "Files/ARLRDBD.TTF" ) )
	{
		submitText.setFont( font );
	}

	//Set text of submit button.
	submitText.setString( "-BEGIN-" );
	submitText.setColor( Color::White );
	submitText.setStyle( Text::Bold );
	submitText.setCharacterSize( 60 );
	centerOrigin( &submitText );
	submitText.setPosition( WINDOW_WIDTH - 250, WINDOW_HEIGHT - 100 );

	//Appearance of the submit button.
	submitButton.setSize( Vector2f( submitText.getGlobalBounds().width + 15,
		submitText.getGlobalBounds().height + 15 ) );
	centerOrigin( &submitButton );
	submitButton.setPosition( submitText.getPosition() );
	submitButton.setOutlineColor( Color::Black );
	submitButton.setOutlineThickness( 8.0 );
	submitButton.setFillColor( Color( 100, 100, 20 ) );

	//Change color if mouse is hovered over and return true if clicked on,
	//signifying a submit.
	if( intersects( &mousePos, &submitButton ) )
	{
		submitButton.setFillColor( Color( 250, 240, 70 ) );
		submitText.setColor( Color::Magenta );

		if( pressed )
		{
			return true;
		}
	}

	//Draw objects.
	windowPtr->draw( submitButton );
	windowPtr->draw( submitText );

	return false;
}

//This function simulates the spread of a disease.
void simulation( int& state, RenderWindow* windowPtr, bool pressed,
	World* worldPtr )
{
	//Position of mouse.
	Vector2i mousePos = Mouse::getPosition( *windowPtr );

	//Time.
	Font font;
	Text year, month;
	if( font.loadFromFile( "Files/ARLRDBD.TTF" ) )
	{
		year.setFont( font );
		month.setFont( font );
	}

	char buffer[10];

	//Display year.
	//Convert int to a string.
	sprintf_s( buffer, "%d", worldPtr->getYear() );
	year.setString( buffer );
	year.setColor( Color( 50, 100, 200 ) );
	year.setCharacterSize( 65 );
	centerOrigin( &year );
	year.setPosition( 1100, 200 );

	//Display month.
	month.setString( monthToString( worldPtr->getMonth() ) );
	month.setColor( Color( 50, 100, 200 ) );
	month.setCharacterSize( 40 );
	centerOrigin( &month );
	month.setPosition( 1100, 250 );

	//Draw objects.
	windowPtr->draw( year );
	windowPtr->draw( month );
	worldPtr->drawMarks( windowPtr );


	//Current stats, i.e. the number of countries infected vs total countries.
	Text current, stats;
	if( font.loadFromFile( "Files/ARLRDBD.TTF" ) )
	{
		current.setFont( font );
		stats.setFont( font );
	}

	current.setString( "Countries infected" );
	current.setColor( Color( 100, 20, 170 ) );
	current.setCharacterSize( 20 );
	centerOrigin( &current );
	current.setPosition( 1100, 315 );

	//Convert from int to a string.
	char buffer1[5], buffer2[5];
	sprintf_s( buffer1, "%d", worldPtr->getInfectedNum() );
	sprintf_s( buffer2, "%d", worldPtr->getCountriesNum() );

	std::string statString( buffer1 );
	statString.append( "/" );
	statString.append( buffer2 );

	stats.setString( statString );
	stats.setColor( Color( 100, 20, 170 ) );
	stats.setCharacterSize( 50 );
	centerOrigin( &stats );
	stats.setPosition( 1100, 360 );

	//Draw objects.
	windowPtr->draw(current);
	windowPtr->draw(stats);


	//Options.
	RectangleShape oneMonthButton, oneYearButton, 
		tenYearsButton, fiftyYearsButton;
	Text oneMoText, oneYrText, tenYrText, fiftyYrText, optionsText;

	//Set font of text.
	if( font.loadFromFile( "Files/ARLRDBD.TTF" ) )
	{
		oneMoText.setFont( font );
		oneYrText.setFont( font );
		tenYrText.setFont( font );
		fiftyYrText.setFont( font );
		optionsText.setFont( font );
	}

	//Appearane of the text of 1 month.
	oneMoText.setString( "1 MONTH" );
	oneMoText.setColor( Color::Red );
	oneMoText.setCharacterSize( 20 );
	centerOrigin( &oneMoText );
	oneMoText.setPosition( 1100, 550 );

	//Appearane of the text of 1 year.
	oneYrText.setString( "1 YEAR" );
	oneYrText.setColor( Color::Red );
	oneYrText.setCharacterSize( 20 );
	centerOrigin( &oneYrText );
	oneYrText.setPosition( 1100, 600 );

	//Appearane of the text of 10 years.
	tenYrText.setString( "10 YEARs" );
	tenYrText.setColor( Color::Red );
	tenYrText.setCharacterSize( 20 );
	centerOrigin( &tenYrText );
	tenYrText.setPosition( 1100, 650 );

	//Appearane of the text of 50 years.
	fiftyYrText.setString( "50 YEARs" );
	fiftyYrText.setColor( Color::Red );
	fiftyYrText.setCharacterSize( 20 );
	centerOrigin( &fiftyYrText );
	fiftyYrText.setPosition( 1100, 700 );

	//Appearance of the title.
	optionsText.setString( "INCREASE BY:" );
	optionsText.setColor( Color( 30, 30, 250 ) );
	optionsText.setStyle( Text::Bold );
	optionsText.setCharacterSize( 24 );
	centerOrigin( &optionsText );
	optionsText.setPosition( 1100, 500 );

	//Size of the buttons so each is the same.
	Vector2f maxSize( fiftyYrText.getGlobalBounds().width + 10,
		fiftyYrText.getGlobalBounds().height + 10 );

	//Appearance of the button of 1 month.
	oneMonthButton.setSize( maxSize );
	centerOrigin( &oneMonthButton );
	oneMonthButton.setPosition( oneMoText.getPosition() );
	oneMonthButton.setOutlineColor( Color::Magenta );
	oneMonthButton.setOutlineThickness( 3 );
	oneMonthButton.setFillColor( Color::Green );

	//Appearance of the button of 1 year.
	oneYearButton.setSize( maxSize );
	centerOrigin( &oneYearButton );
	oneYearButton.setPosition( oneYrText.getPosition() );
	oneYearButton.setOutlineColor( Color::Magenta );
	oneYearButton.setOutlineThickness( 3 );
	oneYearButton.setFillColor( Color::Green );

	//Appearance of the button of 10 years.
	tenYearsButton.setSize( maxSize );
	centerOrigin( &tenYearsButton );
	tenYearsButton.setPosition( tenYrText.getPosition() );
	tenYearsButton.setOutlineColor( Color::Magenta );
	tenYearsButton.setOutlineThickness( 3 );
	tenYearsButton.setFillColor( Color::Green );

	//Appearance of the button of 50 years.
	fiftyYearsButton.setSize( maxSize );
	centerOrigin( &fiftyYearsButton );
	fiftyYearsButton.setPosition( fiftyYrText.getPosition() );
	fiftyYearsButton.setOutlineColor( Color::Magenta );
	fiftyYearsButton.setOutlineThickness( 3 );
	fiftyYearsButton.setFillColor( Color::Green );
	
	//Change color if mouse is hovered over and 
	//increase time if clicked on.
	if( intersects( &mousePos, &oneMonthButton ) )
	{
		oneMonthButton.setFillColor( Color( 25, 100, 200 ) );		

		if( pressed )
		{
			worldPtr->increaseMonth( 1 );
		}
	}

	//Change color if mouse is hovered over and 
	//increase time if clicked on.
	if( intersects( &mousePos, &oneYearButton ) )
	{
		oneYearButton.setFillColor( Color( 25, 100, 200 ) );

		if( pressed )
		{
			worldPtr->increaseMonth( 12 );
		}
	}

	//Change color if mouse is hovered over and 
	//increase time if clicked on.
	if( intersects( &mousePos, &tenYearsButton ) )
	{
		tenYearsButton.setFillColor( Color( 25, 100, 200 ) );

		if( pressed )
		{
			worldPtr->increaseMonth( 120 );
		}
	}

	//Change color if mouse is hovered over and 
	//increase time if clicked on.
	if( intersects( &mousePos, &fiftyYearsButton ) )
	{
		fiftyYearsButton.setFillColor( Color( 25, 100, 200 ) );

		if( pressed )
		{
			worldPtr->increaseMonth( 600 );
		}
	}

	//Draw buttons.
	windowPtr->draw( oneMonthButton );
	windowPtr->draw( oneYearButton );
	windowPtr->draw( tenYearsButton );
	windowPtr->draw( fiftyYearsButton );

	//Draw texts.
	windowPtr->draw( oneMoText );
	windowPtr->draw( oneYrText );
	windowPtr->draw( tenYrText );
	windowPtr->draw( fiftyYrText );
	windowPtr->draw( optionsText );


	//Return to menu.
	Text menuText;
	RectangleShape menuButton;
	if( font.loadFromFile( "Files/ARLRDBD.TTF" ) )
	{
		menuText.setFont( font );
	}

	//Set text of menu button.
	menuText.setString( "QUIT" );
	menuText.setColor( Color::Black );
	menuText.setCharacterSize( 38 );
	centerOrigin( &menuText );
	menuText.setPosition( 1100, 100 );

	//Appearance of the menu button.
	menuButton.setSize( Vector2f( menuText.getGlobalBounds().width + 10,
		menuText.getGlobalBounds().height + 10 ) );
	centerOrigin( &menuButton );
	menuButton.setPosition( menuText.getPosition() );
	menuButton.setOutlineColor( Color::Yellow );
	menuButton.setOutlineThickness( 3 );
	menuButton.setFillColor( Color( 70, 240, 250 ) );

	//Change color if mouse is hovered over and 
	//return to menu if clicked on.
	if( intersects( &mousePos, &menuButton ) )
	{
		menuButton.setFillColor( Color( 250, 240, 70 ) );

		if( pressed )
		{
			state = MENU;
			worldPtr->reset();
		}
	}

	//Draw objects.
	windowPtr->draw( menuButton );
	windowPtr->draw( menuText );

	return;
}

//Returns true if the position of the mouse intersects the given rectangle.
bool intersects( Vector2i* mousePtr, RectangleShape* shapePtr )
{
	//The mouse lies within the boundaries of the rectangle, so return true.
	if( mousePtr->x > shapePtr->getPosition().x - shapePtr->getSize().x/2.0f &&
		mousePtr->x < shapePtr->getPosition().x + shapePtr->getSize().x/2.0f &&
		mousePtr->y > shapePtr->getPosition().y - shapePtr->getSize().y/2.0f &&
		mousePtr->y < shapePtr->getPosition().y + shapePtr->getSize().y/2.0f )
	{
		return true;
	}

	return false;
}

/*
This function accepts an integer value and returns the corresponding
abbreviation for that month. An empty string is retured if an 
invalid value is provided.
*/
std::string monthToString( const int month )
{
	switch( month )
	{
	case 1:
		return "JAN";
	case 2:
		return "FEB";
	case 3:
		return "MAR";
	case 4:
		return "APR";
	case 5:
		return "MAY";
	case 6:
		return "JUN";
	case 7:
		return "JUL";
	case 8:
		return "AUG";
	case 9:
		return "SEP";
	case 10:
		return "OCT";
	case 11:
		return "NOV";
	case 12:
		return "DEC";
	default:
		return "";
	}
}

/*
This function centers the origin of the given object.
Only the Shape, Sprite, Text classes and its child classes
can be supplied as T.
*/
template <class T>
void centerOrigin( T* objectPtr )
{
	FloatRect textRect = objectPtr->getGlobalBounds();
	objectPtr->setOrigin( textRect.left + textRect.width/2.0f,
		textRect.top + textRect.height/2.0f );
}