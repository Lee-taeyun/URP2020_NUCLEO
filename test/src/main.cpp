#include <mbed.h>

FlashIAP flash;

int main() {
    printf("Starting...\r\n");

    flash.init();
  
    const uint32_t  flash_start = flash.get_flash_start();
    const uint32_t  flash_size = flash.get_flash_size();
    const uint32_t  flash_end = flash_start + flash_size;
    const uint32_t  page_size = flash.get_page_size();                  // in bytes
    uint32_t        sector_size = flash.get_sector_size(flash_end - 1); // in bytes
    uint32_t        addr = flash_end - sector_size;                     // address of first byte in the last sector

    printf("flash_start = 0x%.8x\r\n", flash_start);
    printf("flash_size = 0x%.8x\r\n", flash_size);
    printf("flash_end = 0x%.8x\r\n", flash_end);
    printf("page_size = 0x%.8x\r\n", page_size);
    printf("sector_size = 0x%.8x\r\n", sector_size);
    printf("addr = 0x%.8x\r\n", addr);
    printf("----------------------\r\n");
    printf("sizeof double=0x%.8x\r\n",sizeof(double));
    // Create an array in SRAM (later on to be stored in the first page of last sector in FLASH)
    uint32_t        data_len = 15;      // calculate available space
    uint32_t*       data = new uint32_t[15] {1, 2, 3};    // create an array in SRAM and initialize the first three elements
    printf("data_len = %d\r\n", data_len);
    data[11] = 572; // set element 11 to 572
    
    // Print the first 15 elements
    for (int i = 0; i < 15; i++) {
        printf("data[%d] = %d\r\n", i, data[i]);
    }

    // Store the array 'data' in FLASH
    flash.erase(addr, sector_size);         // erase needs sector_size (not page size)
    flash.program(data, addr, data_len*sizeof(uint32_t));   // write needs page_size


    printf("----------------------\r\n");
    
    // Clear the first 15 elements in SRAM (to see whether they are going to be updated with values stored in FLASH)
    for (int i = 0; i < 15; i++) {
        data[i] = 0;
        printf("data[%d] = %d\r\n", i, data[i]);
    }

    printf("----------------------\r\n");
    
    // Update the SRAM array with values stored in FLASH
    flash.read(data, addr, data_len*sizeof(uint32_t));      // read needs page_size

    // Print the first 15 elements
    for (int i = 0; i < 15; i++) {
        printf("data[%d] = %d\r\n", i, data[i]);
    }

    delete[] data;
    flash.deinit();
  
    printf("----------------------\r\n");
    printf("Done.\r\n");
}