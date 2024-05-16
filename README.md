#Basic bootloader for stm32f103c8t6 board
A command is sent from the host over UART interface and is handled by the target according to its number. The dma is used for more efficient communication.
The end of the message sent by the host is detected by an idle line interrupt. When the cpu detects an idle line it stops the dma and start processing the command. After
processing, the dma is re-enabled to start receiving the next command


Supported commands include locking, unlocking flash, and erasing flash memory pages. Most important commands are writing to flash and jumping to user application
The application image is written at the beginning of page 32 (address 0x08008000), this address has to be specified in the linker script file for the application code.
Flashing is done by help of the flasher.py script. To run the flasher, specify the usb port and path to the application hex image, for example:
python flasher.py com3 Debug/blinky.hex
