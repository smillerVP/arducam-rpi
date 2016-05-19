all :  ov2640_capture ov2640_4cams_capture ov5642_capture ov5642_4cams_capture

objects = arducam.o arducam_arch_raspberrypi.o


ov2640_capture : $(objects) arducam_ov2640_capture.o 
	gcc -o ov2640_capture $(objects) arducam_ov2640_capture.o -lwiringPi -Wall	
ov2640_4cams_capture : $(objects) arducam_ov2640_4cams_capture.o 
	gcc -o ov2640_4cams_capture $(objects) arducam_ov2640_4cams_capture.o -lwiringPi -Wall	
ov5642_capture : $(objects) arducam_ov5642_capture.o 
	gcc -o ov5642_capture $(objects) arducam_ov5642_capture.o -lwiringPi -Wall
ov5642_4cams_capture : $(objects) arducam_ov5642_4cams_capture.o 
	gcc -o ov5642_4cams_capture $(objects) arducam_ov5642_4cams_capture.o -lwiringPi -Wall
	
arducam.o : arducam.c
	gcc -c arducam.c -lwiringPi -Wall
arducam_arch_raspberrypi.o : arducam_arch_raspberrypi.c
	gcc -c arducam_arch_raspberrypi.c -lwiringPi -Wall


arducam_ov2640_capture.o : arducam_ov2640_capture.c
	gcc -c arducam_ov2640_capture.c -lwiringPi -Wall
arducam_ov2640_4cams_capture.o : arducam_ov2640_4cams_capture.c
	gcc -c arducam_ov2640_4cams_capture.c -lwiringPi -Wall	
arducam_ov5642_capture.o : arducam_ov5642_capture.c
	gcc -c arducam_ov5642_capture.c -lwiringPi -Wall
arducam_ov5642_4cams_capture.o : arducam_ov5642_4cams_capture.c
	gcc -c arducam_ov5642_4cams_capture.c -lwiringPi -Wall	

clean : 
	rm -f  ov2640_capture ov2640_4cams_capture ov5642_capture ov5642_4cams_capture $(objects) *.o
