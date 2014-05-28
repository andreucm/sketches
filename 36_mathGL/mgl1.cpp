#include <mgl2/qt.h>
int sample(mglGraph *gr)
{
      mglData dat(30,40);   // data to for plotting
      for(long i=0;i<30;i++)   
            for(long j=0;j<40;j++)
                  dat.a[i+30*j] = 1/(1+(i-15)*(i-15)/225.+(j-20)*(j-20)/400.);

      gr->Rotate(60,40);
      gr->Box();
      gr->Surf(dat);
      return 0;
}
//-----------------------------------------------------
int main(int argc,char **argv)
{
      mglQT gr(sample,"MathGL examples");
      return gr.Run();
}

