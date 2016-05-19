#ifndef _MEMORYSAVER_
#define _MEMORYSAVER_

//Uncomment the following definition when you use them

#define OV2640_CAM
//#define OV5642_CAM

#if defined OV5642_CAM	
	#include "ov5642_regs.h"
#endif

#if defined OV2640_CAM
	#include "ov2640_regs.h"
#endif

#endif	//_MEMORYSAVER_
