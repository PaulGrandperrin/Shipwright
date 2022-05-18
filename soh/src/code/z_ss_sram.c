#include "ultra64.h"
#include "global.h"

#include <stdio.h>
#include <assert.h>
#include <string.h>

#if 0
typedef struct {
    /* 0x00 */ OSPiHandle piHandle;
    /* 0x74 */ OSIoMesg ioMesg;
    /* 0x8C */ OSMesgQueue mesgQ;
} SsSramContext; // size = 0xA4

SsSramContext sSsSramContext = { 0 };


    
void SsSram_Init(uintptr_t addr, u8 handleType, u8 handleDomain, u8 handleLatency, u8 handlePageSize, u8 handleRelDuration,
                 u8 handlePulse, u32 handleSpeed) {
    u32 prevInt;
    OSPiHandle* handle = &sSsSramContext.piHandle;

    if ((uintptr_t)OS_PHYSICAL_TO_K1(addr) != (*handle).baseAddress) {
        sSsSramContext.piHandle.type = handleType;
        (*handle).baseAddress = OS_PHYSICAL_TO_K1(addr);
        sSsSramContext.piHandle.latency = handleLatency;
        sSsSramContext.piHandle.pulse = handlePulse;
        sSsSramContext.piHandle.pageSize = handlePageSize;
        sSsSramContext.piHandle.relDuration = handleRelDuration;
        sSsSramContext.piHandle.domain = handleDomain;
        sSsSramContext.piHandle.speed = handleSpeed;

        bzero(&sSsSramContext.piHandle.transferInfo, sizeof(__OSTranxInfo));

        prevInt = __osDisableInt();
        //sSsSramContext.piHandle.next = __osPiTable;
        //__osPiTable = &sSsSramContext.piHandle;
        __osRestoreInt(prevInt);

        sSsSramContext.ioMesg.hdr.pri = OS_MESG_PRI_NORMAL;
        sSsSramContext.ioMesg.hdr.retQueue = &sSsSramContext.mesgQ;
        sSsSramContext.ioMesg.devAddr = addr;
    }
}

void SsSram_Dma(void* dramAddr, size_t size, s32 direction) {
    OSMesg mesg;

    osCreateMesgQueue(&sSsSramContext.mesgQ, &mesg, 1);
    sSsSramContext.ioMesg.dramAddr = dramAddr;
    sSsSramContext.ioMesg.size = size;
    osWritebackDCache(dramAddr, size);
    osEPiStartDma(&sSsSramContext.piHandle, &sSsSramContext.ioMesg, direction);
    osRecvMesg(&sSsSramContext.mesgQ, &mesg, OS_MESG_BLOCK);
    osInvalDCache(dramAddr, size);
}
#endif

void SsSram_ReadWrite(uintptr_t addr, void* dramAddr, size_t size, s32 direction) {
    osSyncPrintf("ssSRAMReadWrite:%08x %08x %08x %d\n", addr, (uintptr_t)dramAddr, size, direction);
    
    // find save path
    char save_path[255]; // who cares about overflows in C ;)
    char* xdg_data_home = getenv("XDG_DATA_HOME");
    char* home = getenv("HOME");
    if(xdg_data_home) {
        strcpy(save_path, xdg_data_home);
        strcpy(save_path + strlen(save_path), "/shipwright");
    } else if (home) {
        strcpy(save_path, home);
        strcpy(save_path + strlen(save_path), "/.local/share/shipwright");
    } else {
        strcpy(save_path, ".");
    }
    strcpy(save_path + strlen(save_path), "/oot_save.sav");
    
    //Check to see if the file exists
    FILE* saveFile;
    saveFile = fopen(save_path, "rb");

    if (saveFile == NULL) {

        saveFile = fopen(save_path, "wb");
        fseek(saveFile, 0, SEEK_SET);
        assert(saveFile != NULL); // OTRTODO LOG
        uint8_t zero = 0;

        for (uint32_t i = 0; i < SRAM_SIZE; i++) {
            fwrite(&zero, 1, 1, saveFile);
        }
        fclose(saveFile);
    } else {
        fclose(saveFile);
    }
    switch (direction) { 
        case OS_WRITE: {
            saveFile = fopen(save_path, "r+b");
            rewind(saveFile);
            fseek(saveFile, addr, SEEK_SET);
            fwrite(dramAddr, size, 1, saveFile);
            fclose(saveFile);
        } break;
        case OS_READ: {
            saveFile = fopen(save_path, "rb+");
            rewind(saveFile);
            fseek(saveFile, addr, SEEK_SET);
            fread(dramAddr, size, 1, saveFile);
            fclose(saveFile);
        } break;
    }
    //SsSram_Init(addr, DEVICE_TYPE_SRAM, PI_DOMAIN2, 5, 0xD, 2, 0xC, 0);
    //SsSram_Dma(dramAddr, size, direction);
}
