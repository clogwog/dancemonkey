#include "led-matrix.h"
#include "graphics.h"

#include <unistd.h>
#include <math.h>
#include <stdio.h>
#include <functional>
#include <cstdlib>
#include <iostream>
#include <cstring>
#include <ctime>
#include <signal.h>
#include <syslog.h>
#include <sys/time.h>
#include <fstream>

#include "brent.h"

using namespace std;

using rgb_matrix::GPIO;
using rgb_matrix::RGBMatrix;
using rgb_matrix::Canvas;


void usage()
{
	printf("Usage: weather icon currentTemp[int] minTemp[int] maxTempi[int] \n");
	exit (1);
}

void DrawIcon(Canvas *canvas, const uint8_t animation_icons[][bmsize] , int index, float brightness )
{
     int l =0;
     for( int y=0; y <=31; y++ )
     {
         for ( int x = 0; x < 32; x++ )
         {
	     bool draw = true;
	     if (index>0 && animation_icons[index][l] == animation_icons[index-1][l]  && animation_icons[index][l+1] == animation_icons[index-1][l+1] && animation_icons[index][l+2] == animation_icons[index-1][l+2] )
		draw = false;

	     if( draw)
             	canvas->SetPixel(x, y, brightness * animation_icons[index][l++] , brightness * animation_icons[index][l++], brightness * animation_icons[index][l++]);
	     else
		l = l + 3;
         }
     }
}



int main(int argc, char *argv[]) 
{
  setlogmask( LOG_UPTO (LOG_NOTICE));
  GPIO io;
  if (!io.Init())
    return 1;


  std::string font_type = "./pongnumberfont.bdf";
  rgb_matrix::Font font;
  if (!font.LoadFont(font_type.c_str())) 
  {
	cout <<  "Couldn't load font " << font_type << std::endl;
        return 1;
  }

  Canvas *canvas = new RGBMatrix(&io, 32, 1);

  float brightnessCorrection = 0.2;
  time_t t = time(0);
  struct tm* now = localtime(&t);
  if (now->tm_hour < 7 || now->tm_hour > 22 )
    brightnessCorrection = 0.1;

  float brightness = 0.2 * brightnessCorrection;

  rgb_matrix::Color text_color(100 * brightnessCorrection, 100 * brightnessCorrection, 100 * brightnessCorrection);

  canvas->Clear();

  rgb_matrix::Color timeColor(brightness * 200, brightness * 200 , brightness * 200);
  canvas->Clear();
  for( int repeat = 0; repeat <=1  ; repeat++ )
  {

    now = localtime(&t);
    std::string strHour = std::to_string(now->tm_hour);
    if( strHour.length() == 1) strHour.insert(0, 1, '0');
    std::string strMinute = std::to_string(now->tm_min);
    if( strMinute.length() == 1) strMinute.insert(0,1,'0');

    for( int index = 0; index < animation_frames; index++)
    {

	DrawIcon(canvas, animation_icons, index, brightnessCorrection);
        rgb_matrix::DrawText(canvas, font, 0,8 , text_color, strHour.c_str());
        rgb_matrix::DrawText(canvas, font, 21, 8, text_color, strMinute.c_str());
							
	usleep(100000);
    }
  }

  canvas->Clear();
  delete canvas;
  return 0;
}

