#include <SciLab/SprSciLab.h>
#include <Base/Affine.h>
using namespace Spr;
using namespace Scilab;

bool WriteMatrix(char* n, int* mA, int* nA, double* A){
	return cwritemat_(n, mA, nA, A, (long)strlen(n)) != 0;
}
double* GetMatrixptr(char* s, int* m, int* n, int* lp){
	cmatptr_(s, m, n, lp, (long)strlen(s));
	return stk(*lp);
}
bool ReadMatrix(char* s, int* m, int* n, double* buf){
	return creadmat_(s, m, n, buf, (long)strlen(s)) != 0;
}


int main(){
#ifdef WIN32
	if ( StartScilab(NULL,NULL,NULL) == false ) DSTR << "Error : StartScilab \n";
#else
    if ( StartScilab(SCI,NULL,NULL) == false ) DSTR << "Error : StartScilab \n";
#endif

	static double A[]={1,2,3,4};  int mA=2,nA=2;
	static double b[]={4,5};  int mb=2,nb=1;


	/* Create Scilab matrices A and b */
	WriteMatrix("A", &mA, &nA, A);
	WriteMatrix("b", &mb, &nb, b);

	SendScilabJob("disp(''A='');");
	SendScilabJob("disp(A);");
	SendScilabJob("disp(''b='');");
	SendScilabJob("disp(b);");
	SendScilabJob("disp(''x=A\\b'');");

	if ( SendScilabJob("A,b,x=A\\b;") != 0) 
	{
		fprintf(stdout,"Error occured during scilab execution (SendScilabJob)\n");
	}
	else 
	{
		double *cxtmp=NULL;
		int m,n,lp,i;

		/* Get m and n */
		GetMatrixptr("x", &m, &n, &lp);

		cxtmp=(double*)malloc((m*n)*sizeof(double));

		ReadMatrix("x", &m, &n, cxtmp);

		for(i=0;i<m*n;i++)
		{
			fprintf(stdout,"x[%d] = %5.2f\n",i,cxtmp[i]);
		}

		if (cxtmp) 
		{
			free(cxtmp);
			cxtmp=NULL;
		}
	}
	{
		SCMatrix A = ScilabMatrix("A");
		SCMatrix b = ScilabMatrix("b");
		SCMatrix x = ScilabMatrix("x");
		std::cout << A << x << " = " << b;

		SendScilabJob("y=A+A;");
		SCMatrix y = ScilabMatrix("y");
		std::cout << "y=" << y;
		A = y;
		SendScilabJob("disp(A);");
	}
}