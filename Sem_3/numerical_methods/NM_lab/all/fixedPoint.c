#include<stdio.h>
#include<math.h>
#include<stdlib.h>
#define prcn 0.000001
//problem functions
//f(x)
double f(double x)
{
	return (x*x-x-6);
}
//standard functions used by methods
//order of convergence
double conv(double e2,double e1)
{
	return log(e2)/log(e1);
}
//absolute error
double error(double c1,double c2)
{
	return fabs(c1-c2);
}
//fixed point iteration
double fp_f(double x)
{
	return -sqrt(x+6);
}
void fixedpoint(double x)
{
	printf("\t\t\t\t\t\tFixed point iteration method\n");
	printf("---------------------------------------------------------------------------------------------------------------------------\n");
	printf("iternation\tpoint\t\terror\t\torder of convergence\n");
	printf("---------------------------------------------------------------------------------------------------------------------------\n");
	int count=0;
	double x2=fp_f(x);
	FILE *fp = fopen("./data/fixedpoint","w");
	while(fabs(x2-x)>prcn)
	{
		double eprev=fabs(x2-x);
		x=x2;
		x2=fp_f(x2);
		count++;
		double e=fabs(x2-x);
		double oconv=conv(e,eprev);
		fprintf(fp,"%lf,%lf\n",oconv,e);
		printf("%d\t\t%lf\t%lf\t%lf\n",count,x,e,oconv);
	}
	printf("---------------------------------------------------------------------------------------------------------------------------\n");
	printf("Root : %lf\n\n\n\n",x2);
}
int main()
{
	double a,b;
	a=0;
	b=-3;
	fixedpoint(b);
	return 0;
}
