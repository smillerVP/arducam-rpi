

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

#define BUF_SIZE (128*1024)

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
    uint8_t buf[BUF_SIZE];
    int i = 0;
    uint8_t temp, temp_last;
      
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
        
        
       //Save the CAM1_CS picture 
       
        i = 0;
        temp = arducam_read_fifo(CAM1_CS);
        // Write first image data to buffer
        buf[i++] = temp;
        // Read JPEG data from FIFO
        while((temp != 0xD9) | (temp_last != 0xFF)) {
            temp_last = temp;
            temp = arducam_read_fifo(CAM1_CS);
            // Write image data to buffer if not full
            if(i < BUF_SIZE) {
                buf[i++] = temp;
            } else {
                // Write BUF_SIZE uint8_ts image data to file
                fwrite(buf, BUF_SIZE, 1, fp1);
                i = 0;
                buf[i++] = temp;
            }
        }
        // Write the remain uint8_ts in the buffer
        if(i > 0) {
            fwrite(buf, i, 1, fp1);
        }
        // Close the file
        fclose(fp1);
        // Clear the capture done flag
        arducam_clear_fifo_flag(CAM1_CS);
        
        
        //Save the CAM2_CS picture 
        i = 0;
        temp = arducam_read_fifo(CAM2_CS);
        // Write first image data to buffer
        buf[i++] = temp;
        // Read JPEG data from FIFO
        while((temp != 0xD9) | (temp_last != 0xFF)) {
            temp_last = temp;
            temp = arducam_read_fifo(CAM2_CS);
            // Write image data to buffer if not full
            if(i < BUF_SIZE) {
                buf[i++] = temp;
            } else {
                // Write BUF_SIZE uint8_ts image data to file
                fwrite(buf, BUF_SIZE, 1, fp2);
                i = 0;
                buf[i++] = temp;
            }
        }
        // Write the remain uint8_ts in the buffer
        if(i > 0) {
            fwrite(buf, i, 1, fp2);
        }
        // Close the file
        fclose(fp2);
        // Clear the capture done flag
        arducam_clear_fifo_flag(CAM2_CS);
        
        
        //Save the CAM3_CS picture 
        i = 0;
        temp = arducam_read_fifo(CAM3_CS);
        // Write first image data to buffer
        buf[i++] = temp;
        // Read JPEG data from FIFO
        while((temp != 0xD9) | (temp_last != 0xFF)) {
            temp_last = temp;
            temp = arducam_read_fifo(CAM3_CS);
            // Write image data to buffer if not full
            if(i < BUF_SIZE) {
                buf[i++] = temp;
            } else {
                // Write BUF_SIZE uint8_ts image data to file
                fwrite(buf, BUF_SIZE, 1, fp3);
                i = 0;
                buf[i++] = temp;
            }
        }
        // Write the remain uint8_ts in the buffer
        if(i > 0) {
            fwrite(buf, i, 1, fp3);
        }
        // Close the file
        fclose(fp3);
        // Clear the capture done flag
        arducam_clear_fifo_flag(CAM3_CS);
        
         //Save the CAM4_CS picture
        i = 0;
        temp = arducam_read_fifo(CAM4_CS);
        // Write first image data to buffer
        buf[i++] = temp;
        // Read JPEG data from FIFO
        while((temp != 0xD9) | (temp_last != 0xFF)) {
            temp_last = temp;
            temp = arducam_read_fifo(CAM4_CS);
            // Write image data to buffer if not full
            if(i < BUF_SIZE) {
                buf[i++] = temp;
            } else {
                // Write BUF_SIZE uint8_ts image data to file
                fwrite(buf, BUF_SIZE, 1, fp4);
                i = 0;
                buf[i++] = temp;
            }
        }
        // Write the remain uint8_ts in the buffer
        if(i > 0) {
            fwrite(buf, i, 1, fp4);
        }
        // Close the file
        fclose(fp4);
        // Clear the capture done flag
        arducam_clear_fifo_flag(CAM4_CS);

    } else {
        printf("Error: unknown or missing argument.\n");
        exit(EXIT_FAILURE);
    }
    exit(EXIT_SUCCESS);
}
