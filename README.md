## Basic bootloader for stm32f103c8t6 board

A number of commands related to flash memory are supported such as locking, unlocking flash erasing pages...etc. the Most important commands are writing to flash and jumping to another location. User app is written at the beginning of page 32 (0x08008000) but any address in flash address space can be chosen.

The bootloader receives a packet from the host over uart (with dma) and start processing, then it start receiving the next packet. idle line detection interrupt is used to indicate that a command packet is fully received. a command packet consists of a command plus a number of bytes (dpending on the command). for example write command packet:
|0x03 | address | word1 | word2 | word3 | word4 |
|-----|---------|-------|-------|-------|-------|

flasher.py scirpt reads the hex image of the program to be written in flash and sends its content divided into 4-words chunks. If an ACK is sent by the bootloader, then the 4 words are written and the script reads the next 4 words, otherwise a NACK is sent and the script stops reading.

For testing, write the bootloader image (boot.bin) at the beginning of the flash (0x08000000) and run the script as follows:
```python
# replace com3 with your usb port
python flasher.py com3 test.hex
```