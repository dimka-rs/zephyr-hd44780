# Zephyr HD44780

Simple project to play with common 16x2 LCD on **nRF52** using **Zephyr OS**.

![Photo](/img/photo.jpg "Photo")


# build and flash

    west build -t pristine
    west build -p auto -b nrf52_pca10040
    west flash
