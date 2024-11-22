#ifndef ICM20600_H
#define ICM20600_H

#ifdef __cplusplus
extern "C" {
#endif

#include <driver/i2c_master.h>
#include <stdio.h>
#include <stdlib.h>


#define ICM20600_XG_OFFS_TC_H_REG 0x04
#define ICM20600_XG_OFFS_TC_L_REG 0x05
#define ICM20600_YG_OFFS_TC_H_REG 0x07
#define ICM20600_YG_OFFS_TC_L_REG 0x08
#define ICM20600_ZG_OFFS_TC_H_REG 0x0a
#define ICM20600_ZG_OFFS_TC_L_REG 0x0b
#define ICM20600_SELF_TEST_X_ACCEL_REG 0x0d
#define ICM20600_SELF_TEST_Y_ACCEL_REG 0x0e
#define ICM20600_SELF_TEST_Z_ACCEL_REG 0x0f
#define ICM20600_XG_OFFS_USRH_REG 0x13
#define ICM20600_XG_OFFS_USRL_REG 0x14
#define ICM20600_YG_OFFS_USRH_REG 0x15
#define ICM20600_YG_OFFS_USRL_REG 0x16
#define ICM20600_ZG_OFFS_USRH_REG 0x17
#define ICM20600_ZG_OFFS_USRL_REG 0x18
#define ICM20600_SMPLRT_DIV_REG 0x19

#define ICM20600_CONFIG_REG 0x1a

#define ICM20600_GYRO_CONFIG_REG 0x1b
typedef enum gyro_scale {
  FS_250_dps = 0x00,
  FS_500_dps = 0x08,
  FS_1000_dps = 0x10,
  FS_2000_dps = 0x18
} gyro_full_scale;

#define ICM20600_GYRO_FS_MASK 0x18
#define ICMP20600_GYRO_SENSITIVITY_250_dps (1.0f / 131.0f)
#define ICMP20600_GYRO_SENSITIVITY_500_dps (1.0f / 65.5f)
#define ICMP20600_GYRO_SENSITIVITY_1000_dps (1.0f / 32.8f)
#define ICMP20600_GYRO_SENSITIVITY_2000_dps (1.0f / 16.4f)

#define ICM20600_ACCEL_CONFIG_REG 0x1c
typedef enum accel_scale {
  FS_2g = 0x00,
  FS_4g = 0x08,
  FS_8g = 0x10,
  FS_16g = 0x18,
} accel_full_scale;

#define ICM20600_ACC_FS_MASK 0x18
#define ICMP20600_ACC_SENSITIVITY_2g (1.0f / 16384.0f)
#define ICMP20600_ACC_SENSITIVITY_4g (1.0f / 8192.0f)
#define ICMP20600_ACC_SENSITIVITY_8g (1.0f / 4096.0f)
#define ICMP20600_ACC_SENSITIVITY_16g (1.0f / 2048.0f)

#define ICM20600_ACCEL_CONFIG2_REG 0x1d
typedef enum accel_lownoise_odr {
  // ACC_RATE_4K_BW_1046 = 0,
  ACC_RATE_1K_BW_420 = 0x00,
  ACC_RATE_1K_BW_218,
  ACC_RATE_1K_BW_99,
  ACC_RATE_1K_BW_44,
  ACC_RATE_1K_BW_21,
  ACC_RATE_1K_BW_10,
  ACC_RATE_1K_BW_5,
} acc_lownoise_odr;

typedef enum acc_average_lp {
  ACC_AVERAGE_4 = 0x00,
  ACC_AVERAGE_8 = 0x10,
  ACC_AVERAGE_16 = 0x20,
  ACC_AVERAGE_32 = 0x30,
} acc_averaging_filter_lp_mode;

#define ICM20600_GYRO_LP_MODE_CFG_REG 0x1e
typedef enum gyro_average_lp {
  GYRO_AVERAGE_1 = 0x00,
  GYRO_AVERAGE_2,
  GYRO_AVERAGE_4,
  GYRO_AVERAGE_8,
  GYRO_AVERAGE_16,
  GYRO_AVERAGE_32,
  GYRO_AVERAGE_64,
  GYRO_AVERAGE_128,
} gyro_averaging_filter_lp_mode;

#define ICM20600_ACCEL_WOM_X_THR_REG 0x20
#define ICM20600_ACCEL_WOM_Y_THR_REG 0x21
#define ICM20600_ACCEL_WOM_Z_THR_REG 0x22
#define ICM20600_FIFO_EN_REG 0x23
#define ICM20600_FSYNC_INT_REG 0x36
#define ICM20600_INT_PIN_CFG_REG 0x37
#define ICM20600_INT_ENABLE_REG 0x38
#define ICM20600_FIFO_WM_INT_STATUS_REG 0x39
#define ICM20600_INT_STATUS_REG 0x3a
#define ICM20600_ACCEL_XOUT_H_REG 0x3b
#define ICM20600_ACCEL_XOUT_L_REG 0x3c
#define ICM20600_ACCEL_YOUT_H_REG 0x3d
#define ICM20600_ACCEL_YOUT_L_REG 0x3e
#define ICM20600_ACCEL_ZOUT_H_REG 0x3f
#define ICM20600_ACCEL_ZOUT_L_REG 0x40
#define ICM20600_TEMP_OUT_H_REG 0x41
#define ICM20600_TEMP_OUT_L_REG 0x42
#define ICM20600_GYRO_XOUT_H_REG 0x43
#define ICM20600_GYRO_XOUT_L_REG 0x44
#define ICM20600_GYRO_YOUT_H_REG 0x45
#define ICM20600_GYRO_YOUT_L_REG 0x46
#define ICM20600_GYRO_ZOUT_H_REG 0x47
#define ICM20600_GYRO_ZOUT_L_REG 0x48
#define ICM20600_SELF_TEST_X_GYRO_REG 0x50
#define ICM20600_SELF_TEST_Y_GYRO_REG 0x51
#define ICM20600_SELF_TEST_Z_GYRO_REG 0x52
#define ICM20600_FIFO_WM_TH1_REG 0x60
#define ICM20600_FIFO_WM_TH2_REG 0x61
#define ICM20600_SIGNAL_PATH_RESET_REG 0x68
#define ICM20600_ACCEL_INTEL_CTRL_REG 0x69
#define ICM20600_USER_CTRL_REG 0x6A

#define ICM20600_PWR_MGMT_1_REG 0x6b
#define ICM20600_RESET 0x80
#define ICM20600_SLEEP_MODE 0x40
#define ICM20600_CYCLE 0x20
#define ICM20600_GYRO_STANDBY 0x10
#define ICM20600_TEMP_DISABLE 0x08
#define ICM20600_CLOCK_SOURCE_INT_OSC 0x00
#define ICM20600_CLOCK_SOURCE_AUTO 0x01

#define ICM20600_PWR_MGMT_2_REG 0x6c
#define ICM20600_X_ACC_ON 0x00
#define ICM20600_X_ACC_DIS 0x20
#define ICM20600_Y_ACC_ON 0x00
#define ICM20600_Y_ACC_DIS 0x10
#define ICM20600_Z_ACC_ON 0x00
#define ICM20600_Z_ACC_DIS 0x08
#define ICM20600_X_GYRO_ON 0x00
#define ICM20600_X_GYRO_DIS 0x04
#define ICM20600_Y_GYRO_ON 0x00
#define ICM20600_Y_GYRO_DIS 0x02
#define ICM20600_Z_GYRO_ON 0x00
#define ICM20600_Z_GYRO_DIS 0x01
#define ICM20600_GYRO_ONLY 0x38
#define ICM20600_ACC_ONLY 0x07
#define ICM20600_ALL_DEVICES_ON 0x00
#define ICM20600_ALL_DEVICES_DIS 0x3F

#define ICM20600_I2C_IF_REG 0x70
#define ICM20600_FIFO_COUNTH_REG 0x72
#define ICM20600_FIFO_COUNTL_REG 0x73
#define ICM20600_FIFO_R_W_REG 0x74

#define ICM20600_WHO_AM_I_REG 0x75
#define I_AM_ICM20600 0x11

#define ICM20600_XA_OFFSET_H_REG 0x77
#define ICM20600_XA_OFFSET_L_REG 0x78
#define ICM20600_YA_OFFSET_H_REG 0x7a
#define ICM20600_YA_OFFSET_L_REG 0x7b
#define ICM20600_ZA_OFFSET_H_REG 0x7d
#define ICM20600_ZA_OFFSET_L_REG 0x7e

#define ICM_20600_I2C_ADDR 0x69

typedef enum icm20600_power {
  SLEEP_MODE = 0,
  STANDYBY_MODE,
  ACC_LOW_POWER,
  ACC_LOW_NOISE,
  GYRO_LOW_POWER,
  GYRO_LOW_NOISE,
  _6AXIS_LOW_POWER,
  _6AXIS_LOW_NOISE,
} icm20600_power_mode;

typedef enum { CM206000_OK = 0, CM206000_ERROR = -1 } icm206000_status;

typedef struct icm20600_accel_axes {
  float accel_x;
  float accel_y;
  float accel_z;
} icm20600_accel_axes_t;

typedef struct icm20600_gyro_axes {
  float gyro_x;
  float gyro_y;
  float gyro_z;
} icm20600_gyro_axes_t;

typedef struct icm20600_init {
  struct {
    gyro_full_scale gyro_fs;
    gyro_averaging_filter_lp_mode gyro_aver_filter;
  } gyro_init;

  struct {
    accel_full_scale accel_fs;
    acc_averaging_filter_lp_mode accel_aver_filter;

  } accel_init;
} icm20600_init_t;

typedef struct icm20600_dev {
  i2c_master_dev_handle_t icm20600_device_handle;
  i2c_master_bus_handle_t icm20600_master_handle;
  icm20600_init_t init;
  icm20600_power_mode power_mode;
  icm20600_gyro_axes_t axes_gyro;
  icm20600_accel_axes_t axes_accel;
} icm20600_dev_t;

void icm20600_init(icm20600_dev_t *dev);
void icm20600_read(icm20600_dev_t *dev, uint8_t reg, uint8_t *data,
                   size_t data_size);
void icm20600_write(icm20600_dev_t *dev, uint8_t reg, uint8_t data);
uint8_t icm20600_get_id(icm20600_dev_t *dev);
void icm20600_read_acc_gyro(icm20600_dev_t *dev);

#ifdef __cplusplus
}
#endif

#endif