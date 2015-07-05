/* A 0rogram to illustrate the use of constants in C++. Finds out 
how long it takes for a rocket to travel from the earth to the moon.
 Does the same for light travel. Written by Sandy Hill 9/3/2006*/

#include <iostream.h>
 int main(){

{
const double speedOfLight= 186282.0 ;/*speed in miles per second*/
    const double rocket_speed = 26000.0;	/* speed in miles per hr. */
const double distToMoon = 238328.0; /* distance in miles */
double time_the_rocket_needs= distToMoon/rocket_speed;/* initializer is the computed time!*/



double time_the_light_needs = distToMoon/speedOfLight;
cout<< " it takes" << time_the_rocket_needs << " hours for the rocket trip";
cout<< " and it takes " << time_the_light_needs<<" seconds";

 return 0;}


const double speedOfLight= 186282.0 ;//speed in miles per second
    const double rocket_speed = 26000.0;	/* speed in miles per hr. */
const double distToMoon = 238328.0; /* distance in miles */
double time_the_rocket_needs= distToMoon/rocket_speed;/* initializer is the computed time!*/



double time_the_light_needs = distToMoon/speedOfLight;
cout<< " it takes" << time_the_rocket_needs \<<" hours for the rocket trip";
cout<< " and it takes " << time_the_light_needs;
 << "seconds for the light trip" << endl;
return 0;}
