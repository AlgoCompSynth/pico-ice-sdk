#include "pico/stdlib.h"
#include "pico/stdio.h"
#include "boards/pico_ice.h"
#include "ice/usb.h"

#include "hardware/dma.h"
#include "hardware/gpio.h"
#include "hardware/spi.h"
#include "ice/init.h"
#include "ice/fpga.h"
#include "ice/fpga_comm.h"



#define DATA_LEN 8
#define START_ADDR 0xdead

int main() {
  stdio_init_all();
  ice_init();

  // Dont let the FPGA on the bus as it isnt programmed yet. Remove this eventually...
  //ice_fpga_halt();
  ice_fpga_reset();
  init_fpga_comm_spi_dma_interface();

  static uint8_t write_data[DATA_LEN];
  static uint8_t read_data[DATA_LEN];
  for (uint16_t i = 0; i < DATA_LEN; i++)
        write_data[i] = i;

  for (;;) {
    ice_write_fpga_spi_dma(START_ADDR, write_data, sizeof(write_data));
    ice_read_fpga_spi_dma(read_data, START_ADDR, sizeof(read_data));
    //for (size_t i = 0; i < DATA_LEN; i++)
    //    if(read_data[i] != i)
    //      printf("Error at 0x%x\n\r", i);

    ice_usb_task();
  }
}