/* a program to compute and display the average of three student grades that are input by the user. By Sandy Hill, 9/4/2006 . Reachable at hill@ecs.umass.edu */

#include<iostream.h>
double grade1, grade2,grade3, average;
int main()
{
cout<< " please input values for grade1, grade2, and grade3 in that order\n";
cin>>grade1>>grade2>>grade3;
cout<<" you gave me"<<grade1<< grade2 << grade3<<endl;
average = ( grade1 + grade2 + grade3) / 3.0;
cout<< " the average of those grades is:" << average;

return 0;}
