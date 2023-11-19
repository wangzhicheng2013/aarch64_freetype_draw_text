#include <sys/time.h>
#include "yuv_text_draw.hpp"
int test_draw_text_to_uyvy() {
    const int pic_w  = 1600;
    const int pic_h  = 1300;
    const int dest_pic_w  = pic_w;
    const int dest_pic_h  = pic_h;

    /* Set up test data's path and output data's path */
    const char *inputPathname  = "./UYVY_946684877_1_1600x1300.uyvy";
    const char *outputPathname = "./UYVY_946684877_2_1600x1300.uyvy";
    FILE *fin  = fopen(inputPathname , "rb+");
    if (!fin) {
        return -1;
    }
    FILE *fout = fopen(outputPathname, "wb+");
    if (!fout) {
        fclose(fin);
        return -1;
    }
    size_t size = pic_w * pic_h * 2;
    size_t dest_size = dest_pic_w * dest_pic_h * 2;
    /* Allocate memory for uyvy */
    unsigned char *src_img = (unsigned char *)malloc(size * sizeof(unsigned char));
    /* Read file data to buffer */
    fread(src_img, size, 1, fin);
    fclose(fin);
    struct timeval start_time, end_time;
    int loop = 10000;
    gettimeofday(&start_time, NULL);
    for (int i = 0;i < loop;i++) {
        YUV_TEXT_DRAW.add_text_to_uyvy(src_img, pic_w, pic_h, L"Smoke Alarm!", 100, 100, 500, 100);
    }
    gettimeofday(&end_time, NULL);
    double time_used = (end_time.tv_sec - start_time.tv_sec) + (end_time.tv_usec - start_time.tv_usec) / 1000000.0;
    printf("average time elapse:%lfs\n", time_used / loop);
	fwrite(src_img, dest_size, 1, fout);
    /* Write data of buf to fout */
   
    /* Close the file */
    fclose(fout);
    return 0;
}
int test_draw_text_to_yuyv() {
    const int pic_w  = 1600;
    const int pic_h  = 1300;
    const int dest_pic_w  = pic_w;
    const int dest_pic_h  = pic_h;

    /* Set up test data's path and output data's path */
    const char *inputPathname  = "./YUYV_946684877_1_1600x1300.yuyv";
    const char *outputPathname = "./YUYV_946684877_2_1600x1300.yuyv";
    FILE *fin  = fopen(inputPathname , "rb+");
    if (!fin) {
        return -1;
    }
    FILE *fout = fopen(outputPathname, "wb+");
    if (!fout) {
        fclose(fin);
        return -1;
    }
    size_t size = pic_w * pic_h * 2;
    size_t dest_size = dest_pic_w * dest_pic_h * 2;
    /* Allocate memory for uyvy */
    unsigned char *src_img = (unsigned char *)malloc(size * sizeof(unsigned char));
    /* Read file data to buffer */
    fread(src_img, size, 1, fin);
    fclose(fin);
    struct timeval start_time, end_time;
    int loop = 10000;
    gettimeofday(&start_time, NULL);
    for (int i = 0;i < loop;i++) {
        YUV_TEXT_DRAW.add_text_to_yuyv(src_img, pic_w, pic_h, L"Smoke Alarm!", 100, 100, 500, 100);
    }
    gettimeofday(&end_time, NULL);
    double time_used = (end_time.tv_sec - start_time.tv_sec) + (end_time.tv_usec - start_time.tv_usec) / 1000000.0;
    printf("average time elapse:%lfs\n", time_used / loop);
	fwrite(src_img, dest_size, 1, fout);
    /* Write data of buf to fout */
   
    /* Close the file */
    fclose(fout);
    return 0;
}
int main() {
    int error = YUV_TEXT_DRAW.init();
    if (error) {
        printf("yuv text draw init failed, error:%d\n", error);
        return -1;
    }
    test_draw_text_to_uyvy();
    test_draw_text_to_yuyv();
    
    return 0;
}