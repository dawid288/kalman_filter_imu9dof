#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <stdio.h>
#include <stdlib.h>
#include <driver/i2c_master.h>
#include <driver/uart.h>
#include <string.h>
#include "icm20600.h"
#include "ak09918.h"
#include "imu_9dof.h"

const uart_port_t uart_num = UART_NUM_0;

void imu_9dof_i2c_init(icm20600_dev_t* icm_dev, ak09918_dev_t* ak_dev)
{
    i2c_master_bus_config_t i2c_mst_config = { .clk_source = I2C_CLK_SRC_DEFAULT,
        .i2c_port = ICM_20600_I2C_PORT,
        .scl_io_num = ICM_20600_I2C_SDL,
        .sda_io_num = ICM_20600_I2C_SDA,
        .glitch_ignore_cnt = 7,
        .flags.enable_internal_pullup = true };

    i2c_master_bus_handle_t bus_handle;
    ESP_ERROR_CHECK(i2c_new_master_bus(&i2c_mst_config, &bus_handle));

    icm_dev->icm20600_master_handle = ak_dev->ak09918_master_handle = bus_handle;

    uart_config_t uart_config = {
        .baud_rate = 115200,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
    };
    // Configure UART parameters
    ESP_ERROR_CHECK(uart_param_config(uart_num, &uart_config));

    ESP_ERROR_CHECK(uart_set_pin(UART_NUM_0, 1, 3, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE));

    uart_driver_install(uart_num, 1024, 0, 0, NULL, 0);
}

void imu_9dof_read(icm20600_dev_t* icm_dev, ak09918_dev_t* ak_dev)
{
    icm20600_read_acc_gyro(icm_dev);
    ak09918_read_mag(ak_dev);

    /*char test_str[50];
    sprintf(test_str, "\nACCEL: X:%.3f g, Y:%.3f g, Z:%.3f g\r\n", icm_dev->axes_accel.accel_x,
    icm_dev->axes_accel.accel_y, icm_dev->axes_accel.accel_z);
    sprintf(test_str, "%.3f,%.3f,%.3f", icm_dev->axes_accel.accel_x, icm_dev->axes_accel.accel_y,
        icm_dev->axes_accel.accel_z);
    uart_write_bytes(uart_num, (const char*) test_str, strlen(test_str));
    sprintf(test_str, "GYRO: X:%.3f dps, Y:%.3f dps, Z:%.3f dps\r\n", icm_dev->axes_gyro.gyro_x,
    icm_dev->axes_gyro.gyro_y, icm_dev->axes_gyro.gyro_z);
    sprintf(test_str, "%.3f,%.3f,%.3f", icm_dev->axes_gyro.gyro_x, icm_dev->axes_gyro.gyro_y,
    icm_dev->axes_gyro.gyro_z);
    uart_write_bytes(uart_num, (const char*) test_str, strlen(test_str));*/

    printf("\nACCEL: X:%.3f g, Y:%.3f g, Z:%.3f g\r\n", icm_dev->axes_accel.accel_x, icm_dev->axes_accel.accel_y,
        icm_dev->axes_accel.accel_z);
    printf("GYRO: X:%.3f dps, Y:%.3f dps, Z:%.3f dps\r\n", icm_dev->axes_gyro.gyro_x, icm_dev->axes_gyro.gyro_y,
        icm_dev->axes_gyro.gyro_z);
    printf("COMP: X:%.3f uT, Y:%.3f uT, Z:%.3f uT\r\n", ak_dev->comp_axes.comp_x, ak_dev->comp_axes.comp_y,
        ak_dev->comp_axes.comp_z);
}