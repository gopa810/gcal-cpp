#pragma once

class TLocation  
{
public:
	static TLocation gloc[];
	static int GetLocationCount();

	const char * name;
	float latitude;
	float longitude;
	unsigned short country_code;
	unsigned short timezone_id;

	static const char * ParseDegreesFromString(const char * str, bool bNorth, double &Longitude);
};

