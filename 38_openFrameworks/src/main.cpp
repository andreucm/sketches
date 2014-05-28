#include "ofMain.h"
#include "testApp.h"
#include "ofAppGlutWindow.h"

//========================================================================
int main( )
{
      ofAppGlutWindow window;
      
      //setup the GL context. Can be OF_WINDOW or OF_FULLSCREEN, pass in width and height too
      ofSetupOpenGL(&window, 1024,768, OF_FULLSCREEN); 
      
      //This kicks off the running of my app. 
      ofRunApp( new testApp());
}