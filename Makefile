BOARD?=nrf52dk_nrf52832
VERSION?=0.0.0
SIGN_KEY?=$(HOME)/zephyrproject/bootloader/mcuboot/root-rsa-2048.pem


.PHONY: all clean flash menuconfig guiconfig init mcuboot flash_mcufoot sign flash_signed

all:
	west build -b $(BOARD)

clean:
	west build -t pristine

flash:
	west flash

menuconfig:
	west build -t menuconfig

guiconfig:
	west build -t guiconfig

init:
	@echo "source $(HOME)/zephyrproject/zephyr/zephyr-env.sh"

mcuboot:
	west build -b $(BOARD) -s $(HOME)/zephyrproject/bootloader/mcuboot/boot/zephyr/ -d build/mcuboot

flash_mcuboot:
	west flash -d build/mcuboot

sign:
	west sign -t imgtool -- --version $(VERSION) --key $(SIGN_KEY)

flash_signed:
	west flash --hex-file build/zephyr/zephyr.signed.hex
