#include<iostream.h>
const long speedOfLight= 1079252849 ;
 /*speed in km/second*/
    const int rocket_speed = 26000;
	/* speed in km/hr. */
const long distToMoon = 238328;
 // distance in miles
/* ##########################3 MAIN MAIN MAIN############### */
int main(){

/* Now introduce these quantities as variables in the correct units */
double speedOfLightMPh = speedOfLight * .62137;
/* speed of light in mh*/

/* note: this cannot be a constant, so change its name a little*/
 long distToMoon;

double rocketSpeed=rocket_speed * 0.62137;
/* speed in mph */
float time_for_rocket_trip = distToMoon/rocket_speed;
float time_for_light_trip = distToMoon/speedOfLight;
cout<< " it takes" <<time_for_rocket_trip<< " hours for the rocket trip"<< endl;
cout<< " and it takes " << time_for_light_trip<< "seconds for the light trip" << endl;
return 0;}
