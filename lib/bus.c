#include "../include/bus.h"
#include "cart.h"
#include "ram.h"
#include "cpu.h"


u8 bus_read(u16 address) {

    if (address >= 0x8000) {
        //ROM Data
        return cart_read(address);

    } else if (address < 0xA000) {
        //TODO
        printf("UNSUPPORTED BUS_READ(%04X)\n",address);

    } else if (address < 0xC000) {
        //cartridge RAM
        return cart_read(address);

    } else if (address < 0xE000) {
        //WRAM
        return wram_read(address);

    } else if (address < 0xFE00) {
        return 0;

    } else if (address < 0xFEA0) {
        //TODO
        printf("UNSUPPORTED BUS_READ(%04X)\n",address);

    } else if (address < 0xFF00) {
        //reserved unusable
        return 0;
    } else if (address < 0xFF80) {
        printf("UNSUPPORTED BUS_READ(%04X)\n",address);

    } else if (address == 0xFFFF) {
        //TODO
        return cpu_get_ie_register();
    }

    return hram_read(address);
}

void bus_write(u16 address, u8 value) {
    if (address < 0x8000) {
        //ROM DATA
        cart_write(address, value);

    } else if (address < 0xA000) {
        //TODO
        printf("UNSUPPORTED BUS_WRITE(%04X)\n",address);

    } else if (address < 0xC000) {
        //EXT-RAM
        cart_write(address, value);

    } else if (address < 0xE000) {
        //EXT-RAM
        wram_write(address, value);

    } else if (address < 0xFE00) {
        //reserved echo ram

    } else if (address < 0xFEA0) {
        //OAM

        //TODO
        printf("UNSUPPORTED BUS_WRITE(%04X)\n",address);

    } else if (address < 0xFF00) {
        //unusable reserved

    } else if (address < 0xFF80) {
        //IO REGISTERS
        //TODO
        printf("UNSUPPORTED BUS_WRITE(%04X)\n",address);

    } else if (address == 0xFFFF) {
        //CPU SET ENABLE REGISTER
    } else {
        hram_write(address, value);
    }


}

u16 bus_read16(u16 address) {
    u16 lo = bus_read(address);
    u16 hi = bus_read(address+1);

    return lo|(hi<<8);

}

void bus_write16(u16 address, u16 value) {
    bus_write(address+1,(value >> 8) &0xFF);
    bus_write(address, value & 0xFF);
}


