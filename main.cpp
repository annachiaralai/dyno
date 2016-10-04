#include "opinion.h"
#include "SDLGLwindow.h"
#include "camera.h"
#include <iostream>
#include <string>
#include <fstream>
using namespace std;

void usage(char *a)
{
 cout<<a<<" - Anna Chiara Lai 2012\n";
 cout<<"Usage:\n";
 cout<<"-a: number of agents\n";
 cout<<"-m: method 0-RK 1-E 2-Implicit Euler\n";
 cout<<"-t: matrix type 0-general 1-symmetric 2-antisymmetric 3-sign symmetric 4- general-symmetric_min \n";
 cout<<"-f: export data on filename\n";
}

// Main function
int main(int argc, char *argv[])
{
 const int dim=3; // 2 or 3 (on S1 or S2)
 int agents=40;   // number of agents
 int range=5;     // Adiacency Matrix random values in [-range,range] 
 float h=0.00001;    // Time step for Euler Scheme
 int method=0;    // 0 : Runge Kutta, 1 : Euler, 2 : ImplicitEuler
 int mtype=1;     // 0 : general, 1: symmetric, 2: antisymmetric, 3: sign symmetric; 4: symmetric sign-symmetric_min; 5: general-sign_symmetric;
					// 7: all -1
 int msign=2; // 0: general, 1: positive, 2: negative
 bool exp_coordinates=false, exp_energy=true, exp_log=false, exp_centroid=true, exp_all=false;  //Esporta un file di testo/ps contentente tempo/energia
 
 string  file, filename, file_energy, file_centroid, file_trajectories, ext1=".txt",ext2=".pdf";
 srand(time(0));  // random seed

 if(argc>1)
 {
  if(argc%2)
  {
   for(int i=1;i<argc-1;i+=2)
   {
         if ((string)argv[i]=="-a"){agents=atoi(argv[i+1]);}	  
    else if ((string)argv[i]=="-m"){method=atoi(argv[i+1]);}
    else if ((string)argv[i]=="-s"){msign=atoi(argv[i+1]);}
    else if ((string)argv[i]=="-t"){mtype=atoi(argv[i+1]);}	  
    else if ((string)argv[i]=="-f"){
	file=(string) argv[i+1];
	filename=(string)argv[i+1]+ext1;
	file_energy=(string)argv[i+1]+"enegy"+ext2;
	file_centroid=(string)argv[i+1]+"centroid"+ext2;
	file_trajectories=(string)argv[i+1]+"trajectories"+ext2;
	exp_all=true;
    }    	  
    else{cout<<"wrong parameters\n"; return 0;}
   }
  }
  else {cout<<"Error: wrong parameters\n"; usage(argv[0]); return 0;} 
 }
 else
 {
  usage(argv[0]); 
  cout<<"running with default parameters\n\n";
 }

 Opinion<dim> test(agents,range,msign,h,mtype);

 
 test.print_AdiacencyMatrix();
 
 float time_old, elapsed_time=0, time_new, dt, timer,timer_euler=0, euler_fps=0, fps = 1./60; // time variables
 bool done=false,Euler=true; 

 SDLGLwindow Gfx("Opinion v1.0",700,700,32,"font.ttf",40);
 Camera Dolly;
 SDL_Event event; 



fstream myfile;

//APERTURA FILE DATI
/*fstream myfile1;
fstream myfile2;
fstream myfilegeneral;
*/
/* if(exp_energy){
	 //fstream myfile;
     myfile.open (filename.c_str(), ios::out);
	 }
if(exp_energy&&exp_log){
	 //fstream myfile;
     myfile1.open (filename1.c_str(), ios::out);
	 }
if(exp_energy&&exp_centroid){
	 //fstream myfile;
     myfile2.open (filename2.c_str(), ios::out);
	 }
	 

 if(exp_illai){
	 //fstream myfile;
     myfile.open (illai.c_str(), ios::out);
	 }*/
	 
//if(exp_coordinates){
//	myfilegeneral.open (filename1.c_str(), ios::out);
//	}

if(exp_all)
{myfile.open (filename.c_str(), ios::out);}
	 

 while(!done)
 {
  time_old = time_new;  time_new = SDL_GetTicks();   dt = ( time_new - time_old )/1000;
  timer+=dt;
  elapsed_time+=dt;
  if(Euler)timer_euler+=dt;
 
  while (SDL_PollEvent(&event)) 
  {
   switch(event.type)
   {
    case SDL_KEYDOWN:
     switch(event.key.keysym.sym)
     {
      case SDLK_x      :  Dolly.set_speed_D(-1);  break;
      case SDLK_z      :  Dolly.set_speed_D(1);   break;
      case SDLK_RIGHT  :  Dolly.set_speed_XZ(-1); break;
      case SDLK_LEFT   :  Dolly.set_speed_XZ(1);  break;
      case SDLK_UP     :  Dolly.set_speed_Y(1);   break;
      case SDLK_DOWN   :  Dolly.set_speed_Y(-1);  break;
      case SDLK_SPACE  :  Euler=!Euler;           break;
      case SDLK_PLUS   :  euler_fps/=2.;          break;
      case SDLK_MINUS  :  euler_fps*=2.;          break;
      case SDLK_ESCAPE :  done=true;              break;
      default          :                          break;
     }
    break;

    case SDL_KEYUP:
     switch(event.key.keysym.sym)
     {
      case SDLK_RIGHT  : case SDLK_LEFT :  Dolly.set_speed_XZ(0);  break;
      case SDLK_UP     : case SDLK_DOWN :  Dolly.set_speed_Y(0);   break;
      case SDLK_x      : case SDLK_z    :  Dolly.set_speed_D(0);   break;
      case SDLK_ESCAPE :                   done=true;              break;
      default          :                                           break;
     }
    break;

    case SDL_VIDEORESIZE :  Gfx.resize(event.resize.w,event.resize.h);  break;

    case SDL_QUIT :  done = true;  break;
 
    default : break;
   }  
  }

  // Draw only 1/fps frames per second
  if(timer>fps)
  {
   timer=0;
/*
 
 double XX,YY,ZZ,X0,Y0,Z0,VX,VY,VZ,D;
 XX=test.a[0].x[0];YY=test.a[0].x[1]; ZZ=test.a[0].x[2];
 VX=test.dynamics(test.a)[0].x[0];
 VY=test.dynamics(test.a)[0].x[1];
 VZ=test.dynamics(test.a)[0].x[2];
 D=sqrt(XX*XX+YY*YY+ZZ*ZZ);
 X0=XX/D;Y0=YY/D;Z0=ZZ/D;
 D=sqrt(VX*VX+VY*VY+VZ*VZ);
 VX=VX/D;VY=VY/D;VZ=VZ/D;
*/

   // Clear screen and depth buffer
   glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);
   // Adjust camera
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
   gluLookAt(Dolly.X, Dolly.Y, Dolly.Z, 0., 0, 0, 0., 1., 0.); 
   //gluLookAt(XX-2*VX, YY-2*VY, ZZ-2*VZ, XX, YY, ZZ, X0, Y0, Z0); 

   glEnable(GL_LIGHTING);

   // Reference Sphere
   glDisable(GL_COLOR_MATERIAL);
    Gfx.WireSphere(0,0,0,1,32,255,255,255,127);
   glEnable(GL_COLOR_MATERIAL);

   // Agents
   for(int k=0;k<test.a.size();k++)
    if(dim==2)
     Gfx.SolidSphere(test.a[k].x[0],test.a[k].x[1],0,.05,32,255,255,0,255);
    else if(dim==3)
     Gfx.SolidSphere(test.a[k].x[0],test.a[k].x[1],test.a[k].x[2],.05,32,255,255,0,255);

   glDisable(GL_LIGHTING);
   
   // Axis
   Gfx.Axis(0,0,0,1.5);
   
   // Draw Text
   Gfx.BeginText();
   Gfx.Text("X", 10, 10,255,0,0);
   Gfx.Text("Y", 35, 10,0,255,0);
   Gfx.Text("Z", 60, 10,0,0,255);
   Gfx.EndText();

   // Swap buffers
   SDL_GL_SwapBuffers();
  }

  // Update camera
  Dolly.update(dt);

  // Update Euler Scheme
  
  if(Euler && timer_euler>euler_fps){timer_euler=0;
  switch (method)
  {	case 0 :  {test.RKStep();}
    case 1 :  test.EulerStep();
    case 2 :  test.ImplicitEulerStep();
   /*if(exp_energy){ myfile<<elapsed_time<<" "<< test.energy()<<"\n";}
   if(exp_energy&&exp_log){ myfile1<<elapsed_time<<" "<< log(test.energy())<<"\n";}
   if(exp_energy&&exp_centroid){ myfile2<<elapsed_time<<" "<< test.centroid()<<"\n";}
   if(exp_illai){ myfile<<elapsed_time<<" "<< test.illai()<<"\n";}*/
   if(exp_all){ 
	   myfile<< elapsed_time<<" "<<test.energy()<<" "<<test.centroid()<<" ";
	   for(int i=0; i<agents; i++)
	    {
		if(dim==3){myfile<< test.a[i].x[0]<<" "<<test.a[i].x[1]<<" "<<test.a[i].x[2]<<" "<<test.a[i].theta[0]<<" "<<test.a[i].theta[1]<<" ";}
		else{myfile<< test.a[i].x[0]<<" "<<test.a[i].x[1]<<" "<<test.a[i].theta[0]<<"\n";}
		} 
		myfile<<"\n";
    }
   
   //test.print_energy();
   //test.print_centroid();
   //test.print_Agents_angles();
   }
   
  // Prevent 100% cpu loading
  SDL_Delay(1);
 }
  for(int i=0;i < agents;i++)
  {  if(test.modulus_Agents()[i] < 0.95 || test.modulus_Agents()[i]>1.001)
   {cout<<"error in approximation\n"<<test.energy()<<"\n";
    done=true;}
}
}
 // Close SDL  
 SDL_Quit();
// Close energy file, if necessary
/*if(exp_energy)
{
 myfile.close();
 myfile.open("commands.txt",ios::out);
 myfile<<"plot '"<<filename<<"' w l linecolor rgb 'black' \npause -1 \n"<<"set term post \n"<<"set output '"<<fileoutput<<"' \n"<<"replot";
 myfile.close();
 string mkd="mkdir "+file;
 string cpp="cd ..";
 string mvd="mv "+filename + " "+file+"/"+filename;
 string go="cd "+file;
 system(mkd.c_str());
 system("gnuplot commands.txt");
 system(mvd.c_str());
 mvd="mv "+fileoutput + " "+file+"/"+fileoutput;
 system(mvd.c_str());
 system(cpp.c_str());
}

if(exp_energy&&exp_log)
{
 myfile1.close();
 myfile1.open("commands1.txt",ios::out);
 myfile1<<"plot '"<<filename1<<"' w l linecolor rgb 'black' \n"<<"set term post \n"<<"set output '"<<fileoutput1<<"' \n"<<"replot";
  myfile1.close();
 string mvd="mv "+filename1 + " "+file+"/"+filename1;
 string cpp="cd ..";
 string go="cd "+file;
  system("gnuplot commands1.txt");
 system(mvd.c_str());
 mvd="mv "+fileoutput1 + " "+file+"/"+fileoutput1;
 system(mvd.c_str());
 system(cpp.c_str());
}


if(exp_energy&&exp_centroid)
{
 myfile2.close();
 myfile2.open("commands2.txt",ios::out);
 myfile2<<"plot '"<<filename2<<"' w l linecolor rgb 'black' \n"<<"set term post \n"<<"set output '"<<fileoutput2<<"' \n"<<"replot";
  myfile2.close();
 string mvd="mv "+filename2 + " "+file+"/"+filename2;
 string go="cd "+file;
 string cpp="cd ..";
 system("gnuplot commands2.txt");
 system(mvd.c_str());
 mvd="mv "+fileoutput2 + " "+file+"/"+fileoutput2;
 system(mvd.c_str());
 system(cpp.c_str());
}


if(exp_illai)
{
 myfile.close();
 myfile.open("commands.txt",ios::out);
 myfile<<"plot '"<<filename<<"' w l linecolor rgb 'black'\npause -1 \n"<<"set term post \n"<<"set output '"<<fileoutput2<<"' \n"<<"replot";
 myfile.close();
 string mkd="mkdir "+file;
 string mvd="mv "+filename + " "+file+"/"+filename;
 string go="cd "+file;
 system(mkd.c_str());
 system("gnuplot commands.txt");
 system(mvd.c_str());
 mvd="mv "+fileoutput + " "+file+"/"+fileoutput;
 system(mvd.c_str());
}
*/

#include <unistd.h>
if(exp_all)
{
 myfile.close();
 myfile.open("commands.txt",ios::out);
// myfile<<"set term post \n"; 
  myfile <<"set term wxt \n set xrange [*:*] \n set yrange [*:*] \n unset zrange \n";
 if(exp_energy)
 {myfile<<"plot '"<<filename<<"' u 1:2 w l lc rgb \"black\" title \"energy\"\n"<<"set term pdf \n set output '"<<file_energy<<"' \n"<<"replot \n set term wxt \n";}
 if(exp_centroid)
 {myfile<<"plot '"<<filename<<"' u 1:3 w l lc rgb \"black\" title \"centroid\" \n"<<"set term pdf \n set output '"<<file_centroid<<"' \n"<<"replot \n set term wxt \n";}
 if(exp_coordinates)
     {myfile<<"set term wxt\n ";
     		 myfile<<"set xrange [-1:1]\n set yrange [-1:1]\n set zrange [-1:1]\n set key outside \n set view equal xyz\n set ticslevel 0\n splot ";
     		for(int i=0; i<agents; i++)
 {myfile<<"'"<<filename<<"' u "<<4+5*i<<":"<<5+5*i<<":"<<6+5*i<<" w l title \"a("<<i<<")\""<<((i==agents-1) ? "\n" : "," );}
 
 myfile<<"set term pdf \n set output '"<<file_trajectories<<"' \n replot \n pause -1 \n set term wxt\n";}
 myfile.close();
 string mkd="mkdir "+file;
 string mvd="mv "+filename + " "+file+"/"+filename;
 system(mkd.c_str());
 system(mvd.c_str());
 mvd="mv commands.txt " +file+"/commands.txt";
 system(mvd.c_str());
 system("pwd");
 string go1="cd "+file;
 printf("%s \n", go1.c_str());
 chdir(file.c_str());
 system("pwd");
  string go="gnuplot commands.txt";
 system(go.c_str());
 }



 return 0;
}
