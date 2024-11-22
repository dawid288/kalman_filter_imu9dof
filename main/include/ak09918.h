#ifndef AK09918_H
#define AK09918_H

#ifdef __cplusplus
extern "C" {
#endif

#include <driver/i2c_master.h>
#include <stdio.h>
#include <stdlib.h>


#define AK09918_WIA1_REG 0x00 // Company ID

#define AK09918_WIA2_REG 0x01 // Device ID
#define I_AM_AK09918 0x0C

#define AK09918_RSV1_REG 0x02 // Reserved 1
#define AK09918_RSV2_REG 0x03 // Reserved 2
#define AK09918_ST1_REG 0x10  // DataStatus 1
#define AK09918_HXL_REG 0x11  // X-axis data
#define AK09918_HXH_REG 0x12
#define AK09918_HYL_REG 0x13 // Y-axis data
#define AK09918_HYH_REG 0x14
#define AK09918_HZL_REG 0x15 // Z-axis data
#define AK09918_HZH_REG 0x16
#define AK09918_TMPS_REG 0x17  // Dummy
#define AK09918_ST2_REG 0x18   // Datastatus 2
#define AK09918_CNTL1_REG 0x30 // Dummy

#define AK09918_CNTL2_REG 0x31 // Control settings
#define AK09918_MODE_MASK 0x1F

#define AK09918_CNTL3_REG 0x32 // Control settings

#define AK09918_SOFT_RESET_BIT 0x01 // Soft Reset
#define AK09918_NORMAL_MODE_BIT 0x00

#define AK09918_HOFL_BIT 0x08 // Sensor Over Flow
#define AK09918_DOR_BIT 0x02  // Data Over Run
#define AK09918_DRDY_BIT 0x01 // Data Ready

#define AK09918_I2C_ADDR 0x0c // I2C address

#define AK09918_SENSIVITITY 0.15f

typedef enum AK09918_mode {
  AK09918_POWER_DOWN = 0x00,
  AK09918_NORMAL = 0x01,
  AK09918_CONTINUOUS_10HZ = 0x02,
  AK09918_CONTINUOUS_20HZ = 0x04,
  AK09918_CONTINUOUS_50HZ = 0x06,
  AK09918_CONTINUOUS_100HZ = 0x08,
  AK09918_SELF_TEST = 0x10,
} AK09918_mode_type_t;

typedef enum AK09918_state {
  AK09918_ERR_OK = 0,       // ok
  AK09918_ERR_DOR = 1,      // data skipped
  AK09918_ERR_NOT_RDY = 2,  // not ready
  AK09918_ERR_OVERFLOW = 5, // sensor overflow, means |x|+|y|+|z| >= 4912uT
} AK09918_state_t;

typedef struct ak09918_comp_axes {
  float comp_x;
  float comp_y;
  float comp_z;
} ak09918_comp_axes_t;

typedef struct ak09918_dev {
  i2c_master_dev_handle_t ak09918_device_handle;
  i2c_master_bus_handle_t ak09918_master_handle;
  ak09918_comp_axes_t comp_axes;
  AK09918_mode_type_t comp_mode;
} ak09918_dev_t;

void ak09918_read(ak09918_dev_t *dev, uint8_t reg, uint8_t *data,
                  size_t data_size);
void ak09918_write(ak09918_dev_t *dev, uint8_t reg, uint8_t data);
uint8_t ak09918_get_id(ak09918_dev_t *dev);
void ak09918_init(ak09918_dev_t *dev);
void ak09918_set_mode(ak09918_dev_t *dev, AK09918_mode_type_t mode);
void ak09918_read_mag(ak09918_dev_t *dev);

#ifdef __cplusplus
}
#endif

#endif