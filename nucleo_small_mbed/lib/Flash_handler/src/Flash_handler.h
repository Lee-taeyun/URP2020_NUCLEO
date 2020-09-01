#ifndef FLASH_HANDLER_H
#define FLASH_HANDLER_H
#include <mbed.h>

class Flash_handler{
    private:
        FlashIAP flash;
        uint32_t flash_start;
        uint32_t flash_size;
        uint32_t flash_end;
        uint32_t sector_size;
        uint32_t flash_address;
        void* data_address;
        uint32_t data_size;
    public:
        Flash_handler();
        void Flash_write( void* data_addr, uint32_t data_byte);
        void Flash_erase();
        void Flash_read(void* data_addr,uint32_t data_byte);
        ~Flash_handler();
};


#endif