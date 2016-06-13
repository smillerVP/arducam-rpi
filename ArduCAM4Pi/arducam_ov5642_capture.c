/*-----------------------------------------

//Update History:
//2016/06/13 	V1.1	by Lee	add support for burst mode
--------------------------------------*/
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <wiringPiSPI.h>
#include <unistd.h>
#include "arducam.h"
#define OV5642_CHIPID_HIGH 0x300a
#define OV5642_CHIPID_LOW 0x300b

#define BUF_SIZE (384*1024)
uint8_t buffer[BUF_SIZE] = {0xFF};

void setup()
{
    uint8_t vid,pid;
    uint8_t temp;
    wiring_init();
    arducam(smOV5642,CAM1_CS,-1,-1,-1);
     // Check if the ArduCAM SPI bus is OK
    arducam_write_reg(ARDUCHIP_TEST1, 0x55, CAM1_CS);
    temp = arducam_read_reg(ARDUCHIP_TEST1, CAM1_CS);
    //printf("temp=%x\n",temp);  //  debug
    if(temp != 0x55) {
        printf("SPI interface error!\n");
        //exit(EXIT_FAILURE);
    }
     else{
    	   printf("SPI interface OK!\n");
    	}
    	
    // Change MCU mode
    arducam_write_reg(ARDUCHIP_MODE, 0x00, CAM1_CS);
    
    // Check if the camera module type is OV5642
    arducam_i2c_word_read(OV5642_CHIPID_HIGH, &vid);
    arducam_i2c_word_read(OV5642_CHIPID_LOW, &pid);
    if((vid != 0x56) || (pid != 0x42)) {
        printf("Can't find OV5642 module!\n");
        exit(EXIT_FAILURE);
    } else {
        printf("OV5642 detected\n");
    }
}

int main(int argc, char *argv[])
{
    if (argc == 1) {
        printf("Usage: %s [-s <resolution>] | [-c <filename]", argv[0]);
        printf(" -s <resolution> Set resolution, valid resolutions are:\n");      
        printf("                   320x240\n");
        printf("                   640x480\n");
        printf("                   800x600\n");
        //printf("                   1280x720\n");
        printf("                   1920x1080\n");
        printf("                   2048x1563\n");
        printf("                   2592x1944\n");
        printf(" -c <filename>   Capture image\n");
        exit(EXIT_SUCCESS);
    }
    
    if (strcmp(argv[1], "-c") == 0 && argc == 4) {
        setup();
        arducam_set_format(fmtJPEG);
        arducam_init();
        
        // Change to JPEG capture mode and initialize the OV2640 module   
        if (strcmp(argv[3], "320x240") == 0) arducam_OV5642_set_jpeg_size(OV5642_320x240);
        else if (strcmp(argv[3], "640x480") == 0) arducam_OV5642_set_jpeg_size(OV5642_640x480);
        //else if (strcmp(argv[2], "1280x720") == 0) arducam_OV5642_set_jpeg_size(OV5642_1280x720);
        else if (strcmp(argv[3], "1920x1080") == 0) arducam_OV5642_set_jpeg_size(OV5642_1920x1080);
        else if (strcmp(argv[3], "2048x1536") == 0) arducam_OV5642_set_jpeg_size(OV5642_2048x1536);
        else if (strcmp(argv[3], "2592x1944") == 0) arducam_OV5642_set_jpeg_size(OV5642_2592x1944);
      
        else {
            printf("Unknown resolution %s\n", argv[3]);
            exit(EXIT_FAILURE);
        }
        sleep(1); // Let auto exposure do it's thing after changing image settings
        printf("Changed resolution1 to %s\n", argv[3]);
        
        delay(1000);
        
        arducam_write_reg(ARDUCHIP_TIM, VSYNC_LEVEL_MASK,CAM1_CS);		//VSYNC is active HIGH   
			  
        // Flush the FIFO
        arducam_flush_fifo(CAM1_CS);    
        // Clear the capture done flag
        arducam_clear_fifo_flag(CAM1_CS);
        // Start capture
        printf("Start capture\n");  
        arducam_start_capture(CAM1_CS);
        
       while (!(arducam_read_reg(ARDUCHIP_TRIG,CAM1_CS) & CAP_DONE_MASK)) ;
        
        printf(" CAM1 Capture Done\n");
                 
        // Open the new file
        FILE *fp1 = fopen(argv[2], "w+");
        
        if (!fp1) {
            printf("Error: could not open %s\n", argv[2]);
            exit(EXIT_FAILURE);
        }
          
        printf("Reading FIFO\n");
        
        size_t len = read_fifo_length(CAM1_CS);
	      if (len >= 393216){
			   printf("Over size.");
			    exit(EXIT_FAILURE);
			  }else if (len == 0 ){
			    printf("Size is 0.");
			    exit(EXIT_FAILURE);
			  } 
			  digitalWrite(CAM1_CS,LOW);  //Set CS low       
	      set_fifo_burst(BURST_FIFO_READ);
	      arducam_spi_transfers(buffer,1);//dummy read  
	      int32_t i=0;
	      while(len>4096)
	      {	 
	      	arducam_transfers(&buffer[i],4096);
	      	len -= 4096;
	      	i += 4096;
	      }
	      arducam_spi_transfers(&buffer[i],len); 
	
	      fwrite(buffer, len+i, 1, fp1);
	      digitalWrite(CAM1_CS,HIGH);  //Set CS HIGH
	       //Close the file
	      delay(100);
	      fclose(fp1);  
	      // Clear the capture done flag
	      arducam_clear_fifo_flag(CAM1_CS);
    
    } else {
        printf("Error: unknown or missing argument.\n");
        exit(EXIT_FAILURE);
    }
    exit(EXIT_SUCCESS);
}
