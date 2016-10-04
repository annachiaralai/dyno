#ifndef OPINION_H
#define OPINION_H
#include<vector>
#include<cmath>
#include<cstdlib>
#include<ctime>
#include<cstdio>
using namespace std;

template<int dim>
class Agent
{
 public:
 double x[dim];
 double theta[dim-1];
 Agent(){for(int k=0;k<dim;k++){x[k]=0;if(k<dim-1)theta[k]=0;}}
 float modulus();
 void update_spherical();
};

template <int dim> 
float Agent<dim>:: modulus()
{
float modulus=0;
for(int k=0;k<dim;k++)
modulus+=x[k]*x[k];
return(modulus);
}

template <int dim> 
void Agent<dim>:: update_spherical()
{
	if(dim==3)
	{
theta[0]=atan2(x[1],x[2]);
theta[1]=acos(x[2]/modulus());
    }
else theta[0]=atan2(x[1],x[2]);
}

template<int dim>
class Opinion
{
	public:
 double dt;
 int mtype;
 int msign;
 int **AdiacencyMatrix;
 vector< Agent<dim> > a_new;
 vector< Agent<dim> > a_tmp;
 vector< Agent<dim> > a_energy;
 vector< Agent<dim> > dynamics( vector< Agent<dim> > &p);
 //public:
 vector< Agent<dim> > a;
 Opinion(int num, int range, int msign, float ddt, int mtype);
 double energy();
 double centroid();
 double illai();
 vector<float> modulus_Agents();
 void print_illai();
 void print_Agents();
 void print_Agents_angles();
 void print_AdiacencyMatrix();
 void print_energy();
 void print_centroid();
 void ImplicitEulerStep();
 void EulerStep();
 void RKStep();
 };

template<int dim>
void Opinion<dim> :: EulerStep()
{
 // modified euler scheme

 // half step
 a_new=dynamics(a);
 for(int i=0; i<a.size(); i++)
 for(int k=0;k<dim;k++)
 a_new[i].x[k]=a[i].x[k]+dt/2*a_new[i].x[k];
 // step 
 a_new=dynamics(a_new);
 for(int i=0; i<a.size(); i++)
 {for(int k=0;k<dim;k++)
 a_new[i].x[k]=a[i].x[k]+dt*a_new[i].x[k];
 a_new[i].update_spherical();
 }
 // swap
 a=a_new;
   // update spherical coordinates
}

template<int dim>
void Opinion<dim> :: ImplicitEulerStep()
{
 int iteration=0;
 float errore=10, t;
  a_tmp=a;  
  while(errore > 1e-12 && iteration<100)
  {
  errore=0;
  
  //a_new=F(a_tmp) = a+dt*f(a_tmp);
  a_new=dynamics(a_tmp);
  for(int i=0; i<a.size();i++)
  {
  for(int k=0; k<dim;k++)
  {
  //printf("Iteration %d \n", iteration);
  //printf("anew prima %2.5f \n", a_new[i].x[k]);
  //printf("atmp prima %2.5f \n", a_tmp[i].x[k]);
  //printf("a prima %2.5f \n", a[i].x[k]);
  a_new[i].x[k]=a[i].x[k]+dt*a_new[i].x[k];
  errore+=(a_new[i].x[k]-a_tmp[i].x[k])*(a_new[i].x[k]-a_tmp[i].x[k]); //errore totale
  t=(a_new[i].x[k]-a_tmp[i].x[k])*(a_new[i].x[k]-a_tmp[i].x[k]);
  //printf("anew %2.5f \n", a_new[i].x[k]);
  //printf("atmp %2.5f \n", a_tmp[i].x[k]);
  //printf("dt %2.5f \n", dt);
  //printf("errore su componente %2.8f",t);
  //printf("\n");
  }
  //printf("local error %2.8f", errore);
  //printf("\n");
  }
  //printf("error %2.15f", errore);
  //printf("\n");
  //printf("iteration %d", iteration);
  //printf("\n");
  for(int i=0; i<a.size();i++)
  {
  for(int k=0; k<dim;k++)
  a_tmp[i].x[k]=a_new[i].x[k];
  a_new[i].update_spherical();
  }
  iteration++;
  }
  a=a_new;
}


template<int dim>
void Opinion<dim> :: RKStep()
{
//	printf("entering in RK \n");
 // runge kutta scheme
 vector< Agent<dim> > k1, k2, k3, k4;
 
 // coefficient

 k1=dynamics(a);
 
 for(int i=0; i<a.size(); i++)
 for(int k=0;k<dim;k++)
   {
    a_new[i].x[k]=a[i].x[k]+0.5*dt*k1[i].x[k];
	}
 k2=dynamics(a_new);
 
 for(int i=0; i<a.size(); i++)
 for(int k=0;k<dim;k++)
   {
	   a_new[i].x[k]=a[i].x[k]+0.5*dt*k2[i].x[k];
	}
 k3=dynamics(a_new);
 
 for(int i=0; i<a.size(); i++)
 for(int k=0;k<dim;k++)
   {
   a_new[i].x[k]=a_new[i].x[k]+dt*k3[i].x[k];
	}
 k4=dynamics(a_new); 
  
  //step
 for(int i=0; i<a.size(); i++)
 for(int k=0;k<dim;k++)
   {
   a_new[i].x[k]=a[i].x[k]+dt/6*k1[i].x[k]+dt/3*k2[i].x[k]+dt/3*k3[i].x[k]+dt/6*k4[i].x[k];
   double modulus=0;
   for(int l=0;l<dim;l++)
  {
    modulus+=a_new[i].x[l]*a_new[i].x[l];
  }
  // a_new[i].x[k]=a_new[i].x[k];// /modulus;
  a_new[i].update_spherical();
  }
  // swap
 a=a_new;
}


template<int dim>
void Opinion<dim> :: print_Agents()
{
 printf("Listing Agents\n");
 for(int i=0;i<a.size();i++)
 {
  printf("Agent %d ( ",i+1);
    printf(") mod=%2.3f ",a[i].modulus());
  printf("\n");
 }
}

template<int dim>
void Opinion<dim> :: print_Agents_angles()
{
 printf("Listing Agents spherical coordinates\n");
 for(int i=0;i<a.size();i++)
 {
  printf("Agent %d ( ",i+1);
  if(dim==3){
  printf(") theta=%2.3f ",a[i].theta[0]);
  printf("    phi=%2.3f ",a[i].theta[1]);
    printf("\n");
 }
 else printf(") theta=%2.3f ",a[i].theta[0]);
}
}


template <int dim>
vector <float> Opinion<dim> :: modulus_Agents()
{
vector <float> modulus (a.size());
 for(int i=0;i<a.size();i++)
 {
  modulus[i]=a[i].modulus();
 }
return(modulus);
}


template<int dim>
void Opinion<dim> :: print_AdiacencyMatrix()
{
 printf("Adiacency Matrix\n");
 for(int i=0;i<a.size();i++)
 {
  for(int j=0;j<a.size();j++)
  {
   printf("%d ",AdiacencyMatrix[i][j]);
  }
  printf("\n"); 
 }
 printf("\n"); 
}

template<int dim>
Opinion<dim> :: Opinion(int num, int range, int msign, float ddt , int mtype)
{
 int theta,phi;
 a.resize(num);
 a_new.resize(num);
 dt=ddt;
AdiacencyMatrix = new int*[num];
//printf("entering in opinion()");
int **m = new int*[num]; //allocata per il caso mtype =4
for(int i=0;i<num;i++)
	{
	AdiacencyMatrix[i] = new int[num];
	if(mtype==4||mtype==5) m[i]= new int[num];
    }
    
if(mtype==4)
	{
	int t;
	  for (int i=0; i <num; i++)
	    {
		 for (int j=i; j<num; j++)
	         { 
    		   m[i][j] = -range + rand()%(2*range+1);
				t =rand()%(range);
					if(m[i][j]<0)
					{m[j][i]=-t;}
					else if (m[i][j]>0) 
						{m[j][i]=t;} 
				if (m[i][j]==0) m[j][i]=0;
			  }
		  }	
	}

if(mtype==5)
	{
  for (int i=0; i <num; i++)
	    {
		 for (int j=0; j<num; j++)
	         {
			   if(i==j) m[i][j]=0;
    		   else
    		   m[i][j] = -range + rand()%(2*range+1);
    		   }
		  }	
	}
	

for(int i=0;i<num;i++)
	{
	switch (mtype)
		{case 0:
				for(int j=0;j<num;j++)
					{
				    if(i==j) AdiacencyMatrix[i][j]=0;
    		       else
					if(msign==0) AdiacencyMatrix[i][j] = -range + rand()%(2*range+1);
					    else if (msign==1) AdiacencyMatrix[i][j] = rand()%(range);
					          else if (msign==2) AdiacencyMatrix[i][j] = -rand()%(range);
					};
				break;
		 case 1: 
				for(int j=i;j<num;j++)
					{
					if(i==j) AdiacencyMatrix[i][j]=0;
    		       else	
					if(msign==0) AdiacencyMatrix[i][j] = -range + rand()%(2*range+1);
					    else if (msign==1) AdiacencyMatrix[i][j] = rand()%(range);
					          else if (msign==2) AdiacencyMatrix[i][j] = -rand()%(range);
					//AdiacencyMatrix[i][j] = -range + rand()%(2*range+1);
					AdiacencyMatrix[j][i] = AdiacencyMatrix[i][j];
					};
				break;
		 case 2: 
				for(int j=i;j<num;j++)
					{
					if(i==j) AdiacencyMatrix[i][j]=0;
    		        else	
					AdiacencyMatrix[i][j] = -range + rand()%(2*range+1);
					AdiacencyMatrix[j][i] = - AdiacencyMatrix[i][j];
					};
				break;
		 case 3:
		        int t;
		        for (int j=i; j<num; j++)
		        {
				if(i==j) AdiacencyMatrix[i][j]=0;
    		       else	
		         {AdiacencyMatrix[i][j] = -range + rand()%(2*range+1);
				  t =rand()%(range);
					if(AdiacencyMatrix[i][j]<0)
						{AdiacencyMatrix[j][i]=-t;}
					else if (AdiacencyMatrix[i][j]>0) 
						{AdiacencyMatrix[j][i]=t;} 
				if (AdiacencyMatrix[i][j]==0) AdiacencyMatrix[j][i]=0;
			     }
			}
				break;
		case 4:
		        {
		         for (int j=i; j<num; j++)
		         {
		        AdiacencyMatrix[i][j] = max(0,m[i][j]-m[j][i]);
		        AdiacencyMatrix[j][i] = max(0,m[j][i]-m[i][j]);
				 }
			    }
				break;		
		case 5:
		        {
		         for (int j=i; j<num; j++)
		         {
		        AdiacencyMatrix[i][j] = max(0,m[i][j]-m[j][i]);
		        AdiacencyMatrix[j][i] = max(0,m[j][i]-m[i][j]);
				 }
			    }
				break;						
		case 6:
		        {
		         for (int j=0; j<num; j++)
		         {
		        AdiacencyMatrix[i][j] = rand()%(range)+1;
		        }
			    }
			    
		case 7:
		        {
		         for (int j=0; j<num; j++)
		         {
		        AdiacencyMatrix[i][j] = -10;
		        }
			    }
				
				
         }
  switch(dim)
  {
   case 2:
    theta=rand()%360;
    a[i].x[0]=cos(theta*M_PI/180.);
    a[i].x[1]=sin(theta*M_PI/180.);
   break;
   case 3:
    theta=rand()%181;
    phi=rand()%360;
    a[i].x[0]=cos(phi*M_PI/180.)*sin(theta*M_PI/180.);
    a[i].x[1]=sin(phi*M_PI/180.)*sin(theta*M_PI/180.);
    a[i].x[2]=cos(theta*M_PI/180.);
   break;
  }
 }
}

template<int dim>
vector< Agent<dim> > Opinion<dim> :: dynamics( vector< Agent<dim> > &p)
{
 vector< Agent<dim> > ris;
 ris.resize(p.size());
 double dot;
 // for each agent i
 for(int i=0;i<p.size();i++)
 {
  Agent<dim> tmp;
  // for each agent j
  for(int j=0;j<p.size();j++)
  {
   if(j!=i)
   {
    // compute <p_i,p_j>
    dot=0;
    for(int k=0;k<dim;k++)
    {
     dot+=p[i].x[k]*p[j].x[k];
    }  
    // compute sum_j A(i,j)*(p_j - <p_i,p_j> p_i)
    for(int k=0;k<dim;k++)
    {
     tmp.x[k]+=AdiacencyMatrix[i][j]*(p[j].x[k]-dot*p[i].x[k]);
    } 
   }    
  }
  ris[i]=tmp;
  //printf("dyn %d  %lf \n",i,ris[i]);
  
 }
 
 return ris;
}

template<int dim>
void Opinion<dim> :: print_energy()
{
 printf(" energy=%2.3f ",energy());
  printf("\n");
}


template<int dim>
void Opinion<dim> :: print_centroid()
{
 printf(" centroid=%2.3f ",centroid());
  printf("\n");
}


template<int dim>
double Opinion<dim> :: centroid()
{
 a_energy=dynamics(a); 	
 double centroid=0;
 for(int k=0;k<dim;k++)
 {
  double modulus=0;
 for(int i=0;i<a.size();i++)
 {
    modulus+=a[i].x[k];
  }
   centroid+=modulus*modulus;
 }
 return(centroid);
 }
 
 template<int dim>
double Opinion<dim> :: illai()
{
  double illai=0;
  for(int k=0;k<dim;k++)
 {
   illai+=a[1].x[k]*a[2].x[k];
 }
 return(1-illai);
}

template<int dim>
void Opinion<dim> :: print_illai()
{
 printf(" illai=%2.3f ",illai());
  printf("\n");
}





template<int dim>
double Opinion<dim> :: energy()
{
 a_energy=dynamics(a); 	
 double energy=0;
 for(int i=0;i<a.size();i++)
 {
   double modulus=0;
   for(int k=0;k<dim;k++)
  {
    modulus+=a_energy[i].x[k]*a_energy[i].x[k];
  }
  energy+=modulus;
 }
 return(energy);
 }


#endif

