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
  struct fb_var_screeninfo var_info;

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

  // Get variable screen information
  if (ioctl(fbfd, FBIOGET_VSCREENINFO, &var_info)) {
    printf("Error reading variable screen info.\n");
  }

  printf("Display info for '%s': %dx%d, %d bpp\n", 
         devicename,
         var_info.xres, var_info.yres, 
         var_info.bits_per_pixel );

  // close file  
  close(fbfd);
  
  return 0;
}
