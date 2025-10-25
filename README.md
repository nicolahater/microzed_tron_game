# Tron Light Cycles on MicroZed (Embedded C)

Real-time **Tron/Light Cycles** game for the **Xilinx Zynq-7000 (Z-7010) MicroZed** evaluation kit.  
Runs on the classroom **MZ_APO** setup with LCD, RGB LEDs, and three rotary encoders (red/green/blue).  
Focus: low-level device access (SPI/GPIO/phys-mem mapping), real-time updates, and simple menu/UI.

## 🚀 Features
- **Two game modes**
  - **CLASSIC:** walls at screen edges (collision with border ends the round)
  - **INFINITY:** wrap-around screen (exit one edge, appear on the opposite)
- **Play modes:** Player vs AI or Player vs Player
- **Controls:** three rotary encoders (green = menu select/confirm; red/blue = turn + press for boost)

## Build (Cross-compile)
This project is set up to cross-compile for ARM using the toolchain specified in the `Makefile`.

**Requirements**
- ARM cross toolchain (e.g. `arm-linux-gnueabihf-gcc`, `arm-linux-gnueabihf-g++`)
- SSH access to the MicroZed target (see *Run on target* below)

**Build locally**
```bash
make clean all
```
This produces a static binary named **`main`**.

## ⚙️ Build & Run Instructions

### 1. Cross-compile for ARM
Make sure the ARM cross-compiler toolchain is installed (`arm-linux-gnueabihf-gcc`). Then run:
```bash
make clean all
```

### 2. Deploy and Run on MicroZed
Connect to the target board via SSH and specify its IP address:
```bash
make TARGET_IP=192.168.202.xx run
```

To enable SSH key-based connection:
```bash
ssh-add /opt/apo/zynq/ssh-connect/mzapo-root-key
```

---


## 🎮 Controls & Gameplay
For detailed gameplay instructions, menu navigation, and hardware control overview, see the [**User Manual (PDF)**](docs/User_Manual.pdf).


## 🧾 Acknowledgment
This project was developed as part of the **B35APO – Architektura počítačů (Computer Architectures)** course at **Czech Technical University in Prague (CTU FEL)**.

The following files were provided as part of the official course template and serve as the foundation for hardware interaction and build configuration:
- `Makefile` – build and remote execution rules  
- `font_prop14x16.c`, `font_rom8x16.c`, `font_types.h` – font rendering utilities  
- `mzapo_parlcd.c/h` – low-level LCD display access  
- `mzapo_phys.c/h` – physical memory mapping utilities  
- `mzapo_regs.h` – base address and register definitions for the MZ_APO board  