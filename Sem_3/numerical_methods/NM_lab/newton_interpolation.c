#include<stdio.h>
#define Max 30
#define PI 3.1415926

//===========================================================================================
float function(float x)
{
	return PI*x*x;
}
//-------------------------------------------------------------------------------------------
void input(float x[Max],float y[Max], int *n,float *h,float *start,float *p)
{
	
	printf("Enter the radius: ");
	scanf("%f",p);

	*n=10;
	*h=1;
	*start=1;
	for(int i=0;i<*n;i++)
	{
		x[i]=(*start)+i*(*h);
		y[i]=function(x[i]);
	}
	printf("\n");
	return;
}
//------------------------------------------------------------------------------
float fact(float n)
{
	if(n==0 || n==1)
		return 1;
	else 
		return n*fact(n-1);
}

//FORWARD Interpolation======================================================================
void printmf(float dy[Max][Max], int n)
{
	for(int i=0;i<n;i++)
	{
		for(int j=0;j<n-i;j++)
			printf("%f\t",dy[i][j]);
		printf("\n");
	}
}
float calUf(float u,float n)
{
	float x=1;
	for(int i=0;i<n;i++)
	{
		x*=u;
		u--;
	}
	return x;
}
void forwardDiff(float y[Max], float dy[Max][Max], int n)
{		
	for(int i=0;i<n;i++)
		dy[i][0]=y[i];
		
	for(int j=1;j<n;j++)
		for(int i=0;i<n-j;i++)
			dy[i][j]=dy[i+1][j-1]-dy[i][j-1];
	return;
}
//----------------------------------------------------------------------------
float forwardInterpolation(float x[Max],float y[Max], int n, float p)
{
	float dy[Max][Max];
	forwardDiff(y,dy,n);

	float sum=0;
	float u=(p-x[0])/(x[1]-x[0]);
	for(int i=0;i<n;i++)
		sum+=calUf(u,i)*dy[0][i]/fact(i);

	return sum;;
}

//BACKWARD Interpolation======================================================================
void printmb(float dy[Max][Max], int n)
{
	for(int i=0;i<n;i++)
	{
		for(int j=0;j<i;j++)
			printf("%f\t",dy[i][j]);
		printf("\n");
	}
}
float calUb(float u,float n)
{
	float x=1;
	for(int i=0;i<n;i++)
	{
		x*=u;
		u++;
	}
	return x;
}
void backwardDiff(float y[Max], float dy[Max][Max], int n)
{		
	for(int i=0;i<n;i++)
		dy[i][0]=y[i];
		
	for(int j=1;j<n;j++)
		for(int i=j;i<n;i++)
			dy[i][j]=dy[i][j-1]-dy[i-1][j-1];
	return;
}
//----------------------------------------------------------------------------
float backwardInterpolation(float x[Max],float y[Max], int n, float p)
{
	float dy[Max][Max];
	backwardDiff(y,dy,n);
	//printmb(dy,n);

	float sum=0;
	float u=(p-x[n-1])/(x[1]-x[0]);
	for(int i=0;i<n;i++)
		sum+=calUb(u,i)*dy[n-1][i]/fact(i);

	return sum;;
}

//===========================================================================================
int main()
{
	printf("Finding area of a cirle by using newton interpolation\n\n");
	int n;
	float x[Max],y[Max],h,start,p;
	input(x,y,&n,&h,&start,&p);
	
	float result1 = forwardInterpolation(x,y,n,p);
	printf("answer by forward interpolation = %f \nwith error = %f \n\n",result1,result1-PI*p*p);
	float result2 = backwardInterpolation(x,y,n,p);
	printf("answer by backward interpolation = %f \nwith error = %f \n",result2,result2-PI*p*p);	
	return 0;
}
