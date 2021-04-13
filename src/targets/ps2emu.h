#ifndef SRC_TARGETS_PS2EMU_H
#define SRC_TARGETS_PS2EMU_H

#include "target.h"
#include <Windows.h>
#include <stdint.h>

#define INVALID_ADDRESS -1

typedef struct PS2EmuTargetInitParams {
	bool useIPC; //Should I use IPC, or raw memory access ?
	uint32_t port; //Port where IPC socket is open.
} PS2EmuTargetInitParams;

class PS2EmuTarget : public Target {
public:
	size_t readTargetMemory(uint32_t addr, uint8_t* buf, size_t readSize);
	int init(const void* initParams = nullptr);
	void cleanup(void);

private:
	uint32_t translateAddressToPS2AddressSpace(uint32_t addr) {
		uint32_t ret = (addr & 0x3FFFFFF); //Keep only the low bytes, because max address is 0x2000000 -- This *might* not hold true for Scratchpad
		if (ret > 0x2000000) return INVALID_ADDRESS;
		else return (ret | this->EEMemBase);
	}
	bool useIPC = false;
	bool initialized = false;
	//PCSX2Ipc* IpcHandle = nullptr;
	HANDLE PS2EmuHandle = nullptr;
	uint32_t EEMemBase = 0; //PCSX2 doesn't map PS2's EE Memory to the same address all the time since 1.7, so we need to offset all accesses ourselves (in non-IPC mode).
};

extern PS2EmuTarget PS2_emu_target;
#endif