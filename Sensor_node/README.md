## Running this code

This project was based on Contiki's cc26xx example.
To run this code, you need to:

1. Setup Contiki environment
   https://github.com/contiki-os/contiki
2. Change to cc26xx folder
   ```sh
   cd /example/cc26xx
   ```
3. Replace the existing cc26xx-demo.c file with the one provided in this project
4. Compile the code
   ```sh
   make BOARD=sensortag/cc2650 cc26xx-demo
   ```
5. Download and install Flash Programmer 2
   https://www.ti.com/tool/FLASH-PROGRAMMER
6. Connect your SensorTag to the host PC
7. Flash your device using the generated binary file
