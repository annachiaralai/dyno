#ifndef CAMERA_H
#define CAMERA_H
#include <cmath>

class Camera
{
 float default_speed_XZ,
       default_speed_Y,
       default_speed_D,
       distance_min,
       distance_max,
       angle_XZ,
       angle_Y,
       speed_XZ,
       speed_Y,
       speed_D,
       distance;

 public:

 float X,
       Y,
       Z;

 Camera();

 void update(float dt);
 void set_speed_XZ(int direction){ speed_XZ= direction * default_speed_XZ;}
 void set_speed_Y (int direction){ speed_Y = direction * default_speed_Y; }
 void set_speed_D (int direction){ speed_D = direction * default_speed_D; }
};


Camera :: Camera()
{    
 default_speed_XZ=50; // degrees per second
 default_speed_Y=50;  // degrees per second
 default_speed_D=10;  // units per second
 distance_min=3;
 distance_max=10;
 angle_XZ=0;
 angle_Y=0;
 speed_XZ=0;
 speed_Y=0;
 speed_D=0;
 distance=5;
 X=distance*sin(angle_XZ*M_PI/180.)*cos(angle_Y*M_PI/180.);
 Y=distance*sin(angle_Y*M_PI/180.);
 Z=distance*cos(angle_XZ*M_PI/180.)*cos(angle_Y*M_PI/180.);
}

void Camera :: update(float dt)
{
 if(speed_XZ!=0 || speed_D!=0 || speed_Y!=0)
 {  
  angle_XZ+=speed_XZ*dt; 
  if(angle_XZ>360 || angle_XZ<-360) angle_XZ=0;

  angle_Y+=speed_Y*dt; 
  if(angle_Y>90) angle_Y=90;
  if(angle_Y<-90) angle_Y=-90;

  distance+=speed_D*dt; 
  if(distance>distance_max) distance=distance_max;
  if(distance<distance_min) distance=distance_min;

  X=distance*sin(angle_XZ*M_PI/180.)*cos(angle_Y*M_PI/180.);
  Y=distance*sin(angle_Y*M_PI/180.);
  Z=distance*cos(angle_XZ*M_PI/180.)*cos(angle_Y*M_PI/180.);
 }
}

#endif

