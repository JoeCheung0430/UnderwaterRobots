#include "send_ypr.h"
#include "math.h"
#include "stm32_iic.h" 
#include "usart.h"

float Pitch,Roll,Yaw,pitch_now,roll_now,yaw_now;
extern unsigned int Angle_change;
float temp;
float q0=1.0f,q1=0.0f,q2=0.0f,q3=0.0f;
volatile unsigned char rx_new;

signed char gyro_orientation[9] = {-1, 0, 0,
                                    0,-1, 0,
                                    0, 0, 1};

struct hal_s hal = {0};

//声明相关变量
unsigned long sensor_timestamp;
short gyro[3], accel[3], sensors;
unsigned char more;
long quat[4];
short gyro[3];
/****************/
short T_X,T_Y,T_Z;
int16_t Direction;
unsigned char BUF[10];       //接收数据缓存区

/*****************/
u8 Mpu_Init(u8 mode)
{
		u8 result = mpu_init();
	  //printf("result=%d\r\n",result);
		i2cWrite(0x68,0x37,0x02);//turn on Bypass Mode 
		delay_ms(10);
    if(!result)   //返回0代表初始化成功
    {   
			#if LOG
				printf("mpu initialization complete......\r\n");
      #endif
        
        //mpu_set_sensor
			result = mpu_set_sensors(INV_XYZ_GYRO | INV_XYZ_ACCEL);
        if(result) return 1;
        
        //mpu_configure_fifo
			result = mpu_configure_fifo(INV_XYZ_GYRO | INV_XYZ_ACCEL);
        if(result) return 2;
       
        //mpu_set_sample_rate
			result = mpu_set_sample_rate(DEFAULT_MPU_HZ);
        if(result) return 3;
       
        //dmp_load_motion_driver_firmvare
			result = dmp_load_motion_driver_firmware();
        if(result) return 4;
               
        //dmp_set_orientation
			result = dmp_set_orientation(inv_orientation_matrix_to_scalar(gyro_orientation));
        if(result) return 5;
        
        //dmp_enable_feature
			result = dmp_enable_feature(DMP_FEATURE_6X_LP_QUAT | DMP_FEATURE_TAP |
            DMP_FEATURE_ANDROID_ORIENT | DMP_FEATURE_SEND_RAW_ACCEL | DMP_FEATURE_SEND_CAL_GYRO |
            DMP_FEATURE_GYRO_CAL);
        if(result) return 6;
        
        //dmp_set_fifo_rate
			result = dmp_set_fifo_rate(DEFAULT_MPU_HZ);
        if(result) return 7;
                
        //不开自检，以水平作为零度
        //开启自检以当前位置作为零度
				if(mode==1)
				run_self_test();
        result = mpu_set_dmp_state(1);    
    }
}


void Get_ypr(void)
{
	dmp_read_fifo(gyro, accel, quat, &sensor_timestamp, &sensors, &more);
	if (sensors & INV_WXYZ_QUAT )
  {
    q0 = quat[0] / q30;
    q1 = quat[1] / q30;
    q2 = quat[2] / q30;
    q3 = quat[3] / q30;
    // 左倾pitch 为负值 ，前倾roll为负值
    Pitch  = asin(-2 * q1 * q3 + 2 * q0* q2)* 57.3 ; // pitch
    Roll = atan2(2 * q2 * q3 + 2 * q0 * q1, -2 * q1 * q1 - 2 * q2* q2 + 1)* 57.3; // roll
    Yaw = atan2(2*(q1*q2 + q0*q3),q0*q0+q1*q1-q2*q2-q3*q3) * 57.3;
	  // mpu_get_accel_reg(gyro,0);
    //printf("Pitch=%f\tRoll=%f\tYaw=%f\t\r\n",Pitch,Roll,Yaw);	           
  }
}

void Update_Magnetometer(void)
{
//	i2cWrite(GYRO_ADDRESS,PWR_MGMT_1, 0x00);	//解除休眠状态
//	i2cWrite(GYRO_ADDRESS,SMPLRT_DIV, 0x07);
//	i2cWrite(GYRO_ADDRESS,CONFIG, 0x06);
//	i2cWrite(GYRO_ADDRESS,GYRO_CONFIG, 0x18);
//	i2cWrite(GYRO_ADDRESS,ACCEL_CONFIG, 0x01);
   
    i2cWrite(MAG_ADDRESS,0x0A,0x01);
    delay_ms(10);	
    i2cRead (MAG_ADDRESS,MAG_XOUT_L,1,BUF);
    i2cRead (MAG_ADDRESS,MAG_XOUT_H,1,BUF+1);
    T_X=(BUF[1]<<8)|BUF[0];

    i2cRead(MAG_ADDRESS,MAG_YOUT_L,1,BUF+2);
    i2cRead(MAG_ADDRESS,MAG_YOUT_H,1,BUF+3);
    T_Y=	(BUF[3]<<8)|BUF[2];
                  //读取计算Y轴数据
    
    i2cread(MAG_ADDRESS,MAG_ZOUT_L,1,BUF+4);
    i2cread(MAG_ADDRESS,MAG_ZOUT_H,1,BUF+5);
    T_Z=	(BUF[5]<<8)|BUF[4];
                  //读取计算Z轴数据

    Direction=	atan2( 
                  (double) (   (int16_t)   (T_X +0)   ),
                  (double) (   (int16_t)   (T_Y -0)  )
                )*(180/3.14159265)+180;
    printf("Direction = %d \r\n",Direction);
}


