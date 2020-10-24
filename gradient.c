#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <sys/ioctl.h>

/*
 * Show device info for the given framebuffer device.
 */
int main(int argc, char* argv[])
{
  char *devicename;
  int fbfd = 0;
  struct fb_var_screeninfo vinfo;
  struct fb_fix_screeninfo finfo;
  long int screensize = 0;
  char *fbp = 0;

  if (argc != 2) {
    printf("Usage: %s <devicename>\n", argv[0]);
    return 1;
  }

  devicename = argv[1];
  fbfd = open(devicename, O_RDWR);
  if (fbfd == -1) {
    printf("Error: cannot open framebuffer device '%s'.\n", devicename);
    return(1);
  }

  if (ioctl(fbfd, FBIOGET_VSCREENINFO, &vinfo)) {
    printf("Error reading variable screen info.\n");
  }
  if (ioctl(fbfd, FBIOGET_FSCREENINFO, &finfo)) {
    printf("Error reading fixed screen info.\n");
  }

  screensize = finfo.smem_len;

  fbp = (char*)mmap(0, 
                    screensize, 
                    PROT_READ | PROT_WRITE, 
                    MAP_SHARED, 
                    fbfd, 0);

  close(fbfd);

  if ((int)fbp == -1) {
    printf("Failed to mmap.\n");
  } else {
    short *pixels = (short *) fbp;
    for (int x=0 ; x<vinfo.xres ; x++) {
      for (int y=0 ; y<vinfo.yres ; y++) {
        int r = x * 32 / vinfo.xres;
        int g = y * 64 / vinfo.yres;
        int b = 31 - r;
        int col = (r << 11) + (g << 5) + b;
	// printf("%d %d %d\n", r, g, b);
	pixels[y * vinfo.xres + x] = 0;
      }
    }
  }

  munmap(fbp, screensize);
  return 0;
}
