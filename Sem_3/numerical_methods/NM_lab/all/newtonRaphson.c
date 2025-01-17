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
//f'(x)
double fd(double x)
{
	return (2*x-1);
}
//f''(x)
double fdd(double x)
{
	return (2);
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
//condition for convergence
int conv_cond(double x)
{
	double c=fabs(f(x)*fdd(x)/(fd(x)*fd(x)));
	if(c<1)
		return 1;
	else 
		return 0;
}
//newton ralphson
void nr(double rold,double r)
{	
	int count=0;
	double oconv;
	FILE *fp = fopen("./data/nr","w");
	printf("\t\t\t\t\t\tNewton Ralphson\n");
	printf("---------------------------------------------------------------------------------------------------------------------------\n");
	double e=error(r,rold),eold;
	printf("iteration\troot\t\terror\t\torder of convergence\n");
	printf("---------------------------------------------------------------------------------------------------------------------------\n");
	while(e>prcn)
	{
		if(!conv_cond(r))
		{
			printf("condition for convergence not statisfied");
			break;
		}
		eold=error(r,rold);
		rold=r;
		r=rold-f(rold)/fd(rold);
		count++;
		e=error(r,rold);
		oconv=conv(e,eold);
		fprintf(fp,"%lf,%lf\n",oconv,e);
		printf("%d\t\t%f\t%f\t%f\n",count,r,e,oconv);
	}
	printf("---------------------------------------------------------------------------------------------------------------------------\n");
	printf("Root : %lf\n\n\n\n",r);
}
int main()
{
	double a,b;
	a=0;
	b=-3;
	nr(a,b);
	return 0;
}
