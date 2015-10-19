//// Defines
#define MPU9150_ADDR			0x68
#define MPU9150_PWR_MGMT_1		0x6B   // R/W
#define MPU9150_USER_CTRL		0x6A   // R/W
#define MPU9150_WHO_AM_I		0x75   // R
#define MPU9150_SMPLRT_DIV		0x19   // R/W
#define MPU9150_CONFIG			0x1A   // R/W
#define MPU9150_GYRO_CONFIG		0x1B   // R/W
#define MPU9150_ACCEL_CONFIG	0x1C   // R/W
#define MPU9150_I2C_MST_CTRL	0x24   // R/W
#define MPU9150_INT_PIN_CFG		0x37   // R/W
#define MPU9150_INT_ENABLE		0x38   // R/W
#define MPU9150_INT_STATUS		0x3A   // R
#define MPU9150_ACCEL_XOUT_H	0x3B   // R
#define ACCEL_SENS0 			16384
#define ACCEL_SENS1				8192
#define ACCEL_SENS2 			4096
#define ACCEL_SENS3				2048
#define GYRO_SENS0				131
#define GYRO_SENS1				65.5
#define GYRO_SENS2				32.8
#define GYRO_SENS3				16.4
#define MAG_SENS				0.3

#define PERIOD					56250
#define SATURATION				56250
#define MAX_THRUST				14000
#define MAX_ROLL				.6
#define MAX_PITCH				.6
#define MAX_YAW					.25
#define ANGLE_RANGE				10
#define START_DUTYCYCLE 		3200
#define error_sum_MAX			.25

#define PI 						3.14156


//// end Defines
