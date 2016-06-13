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
    arducam(smOV5642,CAM1_CS,CAM2_CS,CAM3_CS,CAM4_CS);
       
     // Check if the ArduCAM SPI1 bus is OK
    arducam_write_reg(ARDUCHIP_TEST1, 0x55, CAM1_CS);
    temp = arducam_read_reg(ARDUCHIP_TEST1, CAM1_CS);
    //printf("temp=%x\n",temp);
    if(temp != 0x55) {
        printf("SPI1 interface error!\n");
        exit(EXIT_FAILURE);
    }
     else{
    	   printf("SPI1 interface OK!\n");
    	}
    	
    	 // Check if the ArduCAM SPI2 bus is OK
    arducam_write_reg(ARDUCHIP_TEST1, 0x55, CAM2_CS);
    temp = arducam_read_reg(ARDUCHIP_TEST1, CAM2_CS);
    //printf("temp=%x\n",temp);
    if(temp != 0x55) {
        printf("SPI2 interface error!\n");
        exit(EXIT_FAILURE);
    }
     else{
    	   printf("SPI2 interface OK!\n");
    	}
    	
    	 // Check if the ArduCAM SPI3 bus is OK
    arducam_write_reg(ARDUCHIP_TEST1, 0x55, CAM3_CS);
    temp = arducam_read_reg(ARDUCHIP_TEST1, CAM3_CS);
    //printf("temp=%x\n",temp);
    if(temp != 0x55) {
        printf("SPI3 interface error!\n");
        exit(EXIT_FAILURE);
    }
     else{
    	   printf("SPI3 interface OK!\n");
    	}
    	
    		 // Check if the ArduCAM SPI4 bus is OK
    arducam_write_reg(ARDUCHIP_TEST1, 0x55, CAM4_CS);
    temp = arducam_read_reg(ARDUCHIP_TEST1, CAM4_CS);
    //printf("temp=%x\n",temp);
    if(temp != 0x55) {
        printf("SPI4 interface error!\n");
        exit(EXIT_FAILURE);
    }
     else{
    	   printf("SPI4 interface OK!\n");
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
    
     if (strcmp(argv[1], "-c") == 0 && argc == 7) {
        setup();
        arducam_set_format(fmtJPEG);
        arducam_init();
        // Change to JPEG capture mode and initialize the OV2640 module
        if (strcmp(argv[6], "320x240") == 0) arducam_OV5642_set_jpeg_size(OV5642_320x240);
        else if (strcmp(argv[6], "640x480") == 0) arducam_OV5642_set_jpeg_size(OV5642_640x480);
        //else if (strcmp(argv[6], "1280x720") == 0) arducam_OV5642_set_jpeg_size(OV5642_1280x720);
        else if (strcmp(argv[6], "1920x1080") == 0) arducam_OV5642_set_jpeg_size(OV5642_1920x1080);
        else if (strcmp(argv[6], "2048x1536") == 0) arducam_OV5642_set_jpeg_size(OV5642_2048x1536);
        else if (strcmp(argv[6], "2592x1944") == 0) arducam_OV5642_set_jpeg_size(OV5642_2592x1944);
      
        else {
            printf("Unknown resolution %s\n", argv[6]);
            exit(EXIT_FAILURE);
        }
        sleep(1); // Let auto exposure do it's thing after changing image settings
        printf("Changed resolution1 to %s\n", argv[6]);
        
        delay(1000);
         
        arducam_write_reg(ARDUCHIP_TIM, VSYNC_LEVEL_MASK,CAM1_CS);		//VSYNC is active HIGH  
        arducam_write_reg(ARDUCHIP_TIM, VSYNC_LEVEL_MASK,CAM2_CS);		//VSYNC is active HIGH 
        arducam_write_reg(ARDUCHIP_TIM, VSYNC_LEVEL_MASK,CAM3_CS);		//VSYNC is active HIGH 
        arducam_write_reg(ARDUCHIP_TIM, VSYNC_LEVEL_MASK,CAM4_CS);		//VSYNC is active HIGH  
     
        // Flush the FIFO
        arducam_flush_fifo(CAM1_CS);    
        // Clear the capture done flag
        arducam_clear_fifo_flag(CAM1_CS);
        // Start capture
        printf("Start capture\n");  
        arducam_start_capture(CAM1_CS); 
        while (!(arducam_read_reg(ARDUCHIP_TRIG,CAM1_CS) & CAP_DONE_MASK)) ;
        printf(" CAM1 Capture Done\n");
        
        // Flush the FIFO
        arducam_flush_fifo(CAM2_CS);    
        // Clear the capture done flag
        arducam_clear_fifo_flag(CAM2_CS); 
        arducam_start_capture(CAM2_CS); 
        while (!(arducam_read_reg(ARDUCHIP_TRIG,CAM2_CS) & CAP_DONE_MASK)) ;
        printf(" CAM2 Capture Done\n");
        
          // Flush the FIFO
        arducam_flush_fifo(CAM3_CS);    
        // Clear the capture done flag
        arducam_clear_fifo_flag(CAM3_CS);
        arducam_start_capture(CAM3_CS); 
        while (!(arducam_read_reg(ARDUCHIP_TRIG,CAM3_CS) & CAP_DONE_MASK)) ;
        printf(" CAM3 Capture Done\n");
        
         // Flush the FIFO
        arducam_flush_fifo(CAM4_CS);    
        // Clear the capture done flag
        arducam_clear_fifo_flag(CAM4_CS);
        arducam_start_capture(CAM4_CS); 
        while (!(arducam_read_reg(ARDUCHIP_TRIG,CAM4_CS) & CAP_DONE_MASK)) ;
        printf(" CAM4 Capture Done\n");
           
        // Open the new file
        FILE *fp1 = fopen(argv[2], "w+"); 
        FILE *fp2 = fopen(argv[3], "w+"); 
        FILE *fp3 = fopen(argv[4], "w+");
        FILE *fp4 = fopen(argv[5], "w+"); 
        
        
        if (!fp1) {
            printf("Error: could not open %s\n", argv[2]);
            exit(EXIT_FAILURE);
        }  
         if (!fp2) {
            printf("Error: could not open %s\n", argv[2]);
            exit(EXIT_FAILURE);
        }
         if (!fp3) {
            printf("Error: could not open %s\n", argv[2]);
            exit(EXIT_FAILURE);
        }
         if (!fp4) {
            printf("Error: could not open %s\n", argv[2]);
            exit(EXIT_FAILURE);
        }
           
        printf("Reading FIFO\n");
        size_t len1 = read_fifo_length(CAM1_CS);
         size_t len2 = read_fifo_length(CAM2_CS);
          size_t len3 = read_fifo_length(CAM3_CS);
           size_t len4 = read_fifo_length(CAM4_CS);
      if ((len1>=393216)||(len2>=393216)||(len3>=393216)||(len4>=393216)){
		   printf("Over size.");
		    exit(EXIT_FAILURE);
		  }
		  if (len1 == 0 ) printf("Size1 is 0."); 
		   if (len2 == 0 ) printf("Size2 is 0."); 
		  	if (len3 == 0 ) printf("Size3 is 0."); 
		  		if (len4 == 0 ) printf("Size4 is 0."); 
		  			
      int32_t i=0; 
      digitalWrite(CAM1_CS,LOW);  //Set CS1 low       
      set_fifo_burst(BURST_FIFO_READ);
      arducam_spi_transfers(buffer,1);//dummy read  
      while(len1>4096)
      {	 
      	arducam_transfers(&buffer[i],4096);
      	len1 -= 4096; i += 4096;
      }
      arducam_spi_transfers(&buffer[i],len1); 
      fwrite(buffer, len1+i, 1, fp1); i=0;
      digitalWrite(CAM1_CS,HIGH);  //Set CS1 HIGH
      
        
      digitalWrite(CAM2_CS,LOW);  //Set CS2 low       
      set_fifo_burst(BURST_FIFO_READ);
      arducam_spi_transfers(buffer,1);//dummy read  
      while(len2>4096)
      {	 
      	arducam_transfers(&buffer[i],4096);
      	len2 -= 4096; i += 4096;
      }
      arducam_spi_transfers(&buffer[i],len2); 
      fwrite(buffer, len2+i, 1, fp2); i=0;
      digitalWrite(CAM2_CS,HIGH);  //Set CS2 HIGH
        
      
      digitalWrite(CAM3_CS,LOW);  //Set CS3 low       
      set_fifo_burst(BURST_FIFO_READ);
      arducam_spi_transfers(buffer,1);//dummy read  
      while(len3>4096)
      {	 
      	arducam_transfers(&buffer[i],4096);
      	len3 -= 4096; i += 4096;
      }
      arducam_spi_transfers(&buffer[i],len3); 
      fwrite(buffer, len3+i, 1, fp3); i=0;
      digitalWrite(CAM3_CS,HIGH);  //Set CS3 HIGH
      
      
      digitalWrite(CAM4_CS,LOW);  //Set CS4 low       
      set_fifo_burst(BURST_FIFO_READ);
      arducam_spi_transfers(buffer,1);//dummy read  
      while(len4>4096)
      {	 
      	arducam_transfers(&buffer[i],4096);
      	len4 -= 4096; i += 4096;
      }
      arducam_spi_transfers(&buffer[i],len4); 
      fwrite(buffer, len4+i, 1, fp4); i=0;
      digitalWrite(CAM4_CS,HIGH);  //Set CS4 HIGH
      
      // Close the file
      fclose(fp1);
       fclose(fp2);
        fclose(fp3);
         fclose(fp4);
      // Clear the capture done flag
      arducam_clear_fifo_flag(CAM1_CS);
       arducam_clear_fifo_flag(CAM2_CS);
        arducam_clear_fifo_flag(CAM3_CS);
         arducam_clear_fifo_flag(CAM4_CS);
        
    } else {
        printf("Error: unknown or missing argument.\n");
        exit(EXIT_FAILURE);
    }
    exit(EXIT_SUCCESS);
}
