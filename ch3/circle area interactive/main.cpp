//* A program that prompts the user for a Fahrenheit temperature, feeds back the value actually entered, and displays the corresponding Centigrade temperature. by Sandy Hill, 9/1/2006. hill@ecs.umass.edu */
#include<iostream.h>
int main()
{
double centigradeTemp;
double fahrenheitTemp;
cout<<" please input your choice of Fahrenheit temperature"<< endl;// prompt the user
cin>> fahrenheitTemp ;
cout<<" thanks, you gave me " <<  fahrenheitTemp<<endl;// feedback to the user
centigradeTemp = (fahrenheitTemp - 32) * (5.0/9.0);// the computed temp. in centigrade
cout<< " the corresponding centigradeTemp temperature is " <<\
centigradeTemp<<endl;
return 0;
}
