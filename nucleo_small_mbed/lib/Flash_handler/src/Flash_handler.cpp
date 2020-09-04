#include "Flash_handler.h"


Flash_handler::Flash_handler(){

    flash.init();

    flash_start = flash.get_flash_start();
    flash_size = flash.get_flash_size();
    flash_end = flash_start + flash_size;
    sector_size =flash.get_sector_size(flash_end - 1);
    flash_address = flash_end - sector_size;
    data_address = 0;
    data_size = 0;
}

void Flash_handler::Flash_write(void* data_addr, uint32_t data_byte){
    if(data_addr != 0){
        this->data_address = data_addr;
        this->data_size = data_byte;
        // write needs page_size
        this->flash.program(data_addr, this->flash_address, data_byte);
    }
}

void Flash_handler::Flash_erase(){
    flash.erase(flash_address, sector_size);    
}
void Flash_handler::Flash_read(void* data_addr,uint32_t data_byte){
    // read needs page_size
    flash.read(data_addr, this->flash_address, data_byte);
}
Flash_handler::~Flash_handler(){
    flash.deinit();
}