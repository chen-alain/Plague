#pragma once
#include <string>

enum TYPE{ BACTERIUM, FUNGUS, OTHER };

//Lower key means easier to spread between humans.
enum TRANSMISSION_MODES{ AIRBORNE = 0, TOUCH, SALIVA, CREATURE };

class Disease
{
public:
	Disease( std::string, int, int, bool, bool, bool, bool );
	Disease::Disease( std::string, int, int, bool[] );
	Disease(void);
	~Disease(void);

	bool setName( std::string );
	std::string getName();
	bool setType( int );
	int getType();
	bool setRR( int );
	int getRR();
	int getTransRating();
	void setTransmissionMode( bool, bool, bool, bool );
	bool getTransmissionMode( int );


private:
	std::string name;
	int type;
	int reproductionRating;//On a scale from 0-100;
	int transmissionRating;//On a scale from 0-100;
	bool transmissionMode[4];
};