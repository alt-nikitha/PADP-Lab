#include<stdio.h>
#include<gd.h>
#include<omp.h>
#include<stdlib.h>

int main(int argc, char** argv){
    int nt=16, color,tmp, red, green, blue, x,y,w,h;
    red=green=blue=color=w=h=x=y=0;
    gdImagePtr img;
    char *iname,*oname;
    iname = NULL;
    oname = NULL;
    iname = argv[1];
    oname = argv[2];
    FILE *fp = {0};
    fp = fopen(iname, "r");
    // printf("%p",fp);
    // if(fp==NULL){
    //     printf("HAHA\n");
    //     exit(0);
    // }
    img = gdImageCreateFromPng(fp);
    fclose(fp);
    w = gdImageSX(img);
    h = gdImageSY(img);
    double t = omp_get_wtime();
    omp_set_num_threads(nt);
    #pragma omp parallel for private(y,color,red,blue,green) schedule(guided, 100)
    for(x=0;x<w;x++){
        for(y=0;y<h;y++){
            color = gdImageGetPixel(img,x,y);
            red = gdImageRed(img,color);
            green = gdImageGreen(img, color);
            blue = gdImageBlue(img, color);
            tmp = (red+blue+green)/3;
            red = green = blue = tmp;
            color = gdImageColorAllocate(img, red, blue, green);
            gdImageSetPixel(img, x, y, color);
            
        }
    }
    t = omp_get_wtime() - t;
    printf("\n time taken: %lf\n",t);
    fp = fopen(oname, "w");
    gdImagePng(img,fp);
    fclose(fp);
    gdImageDestroy(img);


    return 0;
}