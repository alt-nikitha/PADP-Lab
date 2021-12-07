#include<stdio.h>
#include<stdlib.h>
#include<gd.h>
#include<omp.h>


int main(int argc, char**argv){
    int color, red, green, blue, x,y,w,h,nt=4,tmp, tid;
    color=red=blue=green=tmp=x=y=w=h=0;
    FILE *fp = {0};
    gdImagePtr img;
    char *iname = NULL, *oname = NULL;
    iname = argv[1];
    oname = argv[2];
    fp = fopen(iname, "r");
    img = gdImageCreateFromPng(fp);
    fclose(fp);
    w = gdImageSX(img);
    h = gdImageSY(img);
    double t = omp_get_wtime();
    omp_set_num_threads(nt);
    #pragma omp parallel for private(y,color, red, green, blue) //schedule(dynamic,10)
    for(x=0;x<w;x++){
        for(y=0;y<h;y++){
            tid = omp_get_thread_num();
            color = gdImageGetPixel(img,x,y);
            red = gdImageRed(img,color);
            blue = gdImageBlue(img,color);
            green = gdImageGreen(img,color);
            tmp = (red+blue+green)/3;
            red = blue = green = tmp;
            if(tid ==0){
                color = gdImageColorAllocate(img, red,0,0);
                gdImageSetPixel(img, x,y, color);
            }
            if(tid == 1){

                color = gdImageColorAllocate(img, 0,green,0);
                gdImageSetPixel(img, x,y, color);

            }
            if(tid == 2){
                color = gdImageColorAllocate(img, 0,0,blue);
                gdImageSetPixel(img, x,y, color);
            }
            if(tid == 3){

                color = gdImageColorAllocate(img, 0,0,0);
                gdImageSetPixel(img, x,y, color);

            }
            
            

        }
    }

    t = omp_get_wtime() - t;
    printf("Time taken %lf\n",t);
    fp = fopen(oname, "w");
    gdImagePng(img, fp);
    fclose(fp);
    gdImageDestroy(img);
    return 0;


}