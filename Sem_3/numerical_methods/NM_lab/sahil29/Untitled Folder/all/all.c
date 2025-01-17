#include<stdio.h>
#include<math.h>
#include<stdlib.h>
#define prcn 0.000001


//problem functions============================================================
//f(x)-------------------------------------
double f(double x)
{
	return (x*x-x-6);
}
//f'(x)------------------------------------
double fd(double x)
{
	return (2*x-1);
}
//f''(x)-----------------------------------
double fdd(double x)
{
	return (2);
}
//==============================================================================



//standard functions used by methods============================================
//order of convergence------------------------
double conv(double e2,double e1)
{
	return log(e2)/log(e1);
}
//absolute error------------------------------
double error(double c1,double c2)
{
	return fabs(c1-c2);
}
//condition for convergence-------------------
int conv_cond(double x)
{
	double c=fabs(f(x)*fdd(x)/(fd(x)*fd(x)));
	if(c<1)
		return 1;
	else 
		return 0;
}
//==============================================================================




























//methods=======================================================================
//bisection method-----------------------------
void bisection(double a, double b)
{
	printf("\t\t\t\t\t\tBisection Method\n");
	printf("---------------------------------------------------------------------------------------------------------------------------\n");
	printf("iternation\tlowerbound\tupperbound\tmid-point\tf(c)\t\terror\t\torder of convergence\n");
	printf("---------------------------------------------------------------------------------------------------------------------------\n");
	
	int count=0;
	double mid;
	double e1,e2;
	double oconv;
	double new;
	FILE *fp = fopen("./data/bisection","w");
	
	while(fabs(a-b)>prcn)
	{
		e1=error(a,b);
		mid=(a+b)/2;
		if(f(a)*f(mid)<0)
		{	
			e2=error(a,mid);
			oconv=conv(e2,e1);
			count++;
			fprintf(fp,"%d,%lf\n",count,e1);
			printf("%d\t\t%lf\t%lf\t%lf\t%lf\t%lf\t%lf\n",count,a,b,mid,f(mid),e1,oconv);
			b=mid;
		}
		else
		{
			e2=error(mid,b);
			oconv=conv(e2,e1);
			count++;
			fprintf(fp,"%d,%lf\n",count,e1);
			printf("%d\t\t%lf\t%lf\t%lf\t%lf\t%lf\t%lf\n",count,a,b,mid,f(mid),e1,oconv);
			a=mid;
		}
	}
	printf("---------------------------------------------------------------------------------------------------------------------------\n");
	printf("Root : %lf\n\n\n\n",mid);
}
//
//
//
//
//
//
//
//
//
//
//
//
//regula falsie------------------------------------
void rf(double a,double b)
{
	printf("\t\t\t\t\t\tRegula Falsie\n");
	printf("---------------------------------------------------------------------------------------------------------------------------\n");
	printf("iternation\tlowerbound\tupperbound\tintersection-pont\tf(c)\t\terror\t\torder of convergence\n");
	printf("---------------------------------------------------------------------------------------------------------------------------\n");

	double cprev=a;
	double c=(a*f(b)-b*f(a))/(f(b)-f(a));
	double e1=error(c,cprev),e2;
	double oconv;
	int count=0;
	FILE *fp = fopen("./data/rf","w");
	
	while(fabs(e1)>prcn)
	{
		count++;
		c=(a*f(b)-b*f(a))/(f(b)-f(a));
		e1=error(c,cprev);
		cprev=c;
		
		if(f(a)*f(c)<0)
		{
			c=(a*f(c)-c*f(a))/(f(c)-f(a));
			e2=error(c,cprev);
			oconv=conv(e2,e1);
			fprintf(fp,"%d,%lf\n",count,e1);
			printf("%d\t\t%lf\t%lf\t%lf\t\t%lf\t%lf\t%lf\n",count,a,b,c,f(c),e2,oconv);
			b=c;
		}
		else
		{
			c=(c*f(b)-b*f(c))/(f(b)-f(c));
			e2=error(c,cprev);
			oconv=conv(e2,e1);
			fprintf(fp,"%d,%lf\n",count,e1);
			printf("%d\t\t%lf\t%lf\t%lf\t\t%lf\t%lf\t%lf\n",count,a,b,c,f(c),e2,oconv);
			a=c;
		}
	}
	printf("---------------------------------------------------------------------------------------------------------------------------\n");
	printf("Root : %lf\n\n\n\n",c);
}
//
//
//
//
//
//
//
//
//
//
//
//
//newton ralphson------------------------------------
void nr(double rold,double r)
{	
	//double r=PI;
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
		//printf("%f\t%f\n",eold,r);
		rold=r;
		r=rold-f(rold)/fd(rold);
		count++;
		e=error(r,rold);
		oconv=conv(e,eold);
		
		//printf("%f\t%f\t%f\n",eold,r,rold);
		fprintf(fp,"%d,%lf\n",count,e);
		printf("%d\t\t%f\t%f\t%f\n",count,r,e,oconv);
	}
	printf("---------------------------------------------------------------------------------------------------------------------------\n");
	printf("Root : %lf\n\n\n\n",r);
}
//
//
//
//
//
//
//
//
//
//
//
//
//secant method------------------------------------
void secant(double a,double b)
{
	printf("\t\t\t\t\t\tSecant method\n");
	printf("---------------------------------------------------------------------------------------------------------------------------\n");
	printf("iternation\tlowerbound\tupperbound\tintersection-pont\tf(c)\t\terror\t\torder of convergence\n");
	printf("---------------------------------------------------------------------------------------------------------------------------\n");

	double cprev=a;
	double c=(a*f(b)-b*f(a))/(f(b)-f(a));
	double e1=error(c,cprev),e2;
	double oconv;
	int count=0;
	FILE *fp = fopen("./data/secant","w");
	
	while(fabs(e1)>prcn)
	{
		count++;
		c=(a*f(b)-b*f(a))/(f(b)-f(a));
		e1=error(c,cprev);
		cprev=c;
		
		c=(a*f(c)-c*f(a))/(f(c)-f(a));
		e2=error(c,cprev);
		oconv=conv(e2,e1);
		fprintf(fp,"%d,%lf\n",count,e1);
		printf("%d\t\t%lf\t%lf\t%lf\t\t%lf\t%lf\t%lf\n",count,a,b,c,f(c),e2,oconv);
		b=c;
	}
	printf("---------------------------------------------------------------------------------------------------------------------------\n");
	printf("Root : %lf\n\n\n\n",c);
}
//
//
//
//
//
//
//
//
//
//
//
//
//fixed point iteration------------------------------------
double fp_f(double x)
{
	//return sqrt(x+6);
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
		//printf("%lf\t%lf\t%lf\n",x,x2,fabs(x2-x));
		//printf("%lf\n",x);
		double eprev=fabs(x2-x);
		x=x2;
		x2=fp_f(x2);
		count++;
		double e=fabs(x2-x);
		double oconv=conv(e,eprev);
		fprintf(fp,"%d,%lf\n",count,e);
		printf("%d\t\t%lf\t%lf\t%lf\n",count,x,e,oconv);
	}
	printf("---------------------------------------------------------------------------------------------------------------------------\n");
	printf("Root : %lf\n\n\n\n",x2);
}










































//================================================================================================

int main()
{
	double a,b;
	a=0;
	b=-3;
	bisection(a,b);
	rf(a,b);
	nr(a,b);
	secant(a,b);
	fixedpoint(b);
	return 0;
}
