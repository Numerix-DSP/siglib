
/*
Modified from: https://raw.githubusercontent.com/raspberrypi/linux/rpi-3.10.y/Documentation/spi/spidev_test.c
 */

#include <stdint.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <linux/spi/spidev.h>

#define ARRAY_LENGTH(a) 	(sizeof(a) / sizeof((a)[0]))
#define PIXEL_BUFFER_SIZE	ARRAY_LENGTH(pixelBuffer)

uint8_t         pixelBuffer[] = {
  0xFF, 0xFF, 0xFF, 0xFF,
  0x00, 0x00, 0x00, 0x00,
  0xEF, 0x0F, 0x00, 0x00,
  0xEF, 0x00, 0x0F, 0x00,
  0xEF, 0x00, 0x00, 0x0F,
  0xFF, 0xFF, 0xFF, 0xFF,
};


static void pabort (
  const char *s)
{
  perror (s);
  abort ();
}

static const char *device = "/dev/spidev0.0";
static uint8_t  mode;
static uint8_t  bits = 8;
static uint32_t speed = 500000;
static uint16_t delay;

static void transfer (
  int fd)
{
  int             ret;
// Pixel frame
// Three 1's, Five brightness bits, 8 bits each : B : G : R
// Max brightness: 31, RGB: 255
// On
  uint8_t         rx[PIXEL_BUFFER_SIZE] = { 0, };
  struct spi_ioc_transfer tr = {
    .tx_buf = (unsigned long) pixelBuffer,
    .rx_buf = (unsigned long) rx,
    .len = PIXEL_BUFFER_SIZE,
    .delay_usecs = delay,
    .speed_hz = speed,
    .bits_per_word = bits,
  };

  ret = ioctl (fd, SPI_IOC_MESSAGE (1), &tr);
  if (ret < 1)
    pabort ("can't send spi message");

//  for (ret = 0; ret < PIXEL_BUFFER_SIZE; ret++) {
//    if (!(ret % 6))
//      puts("");
//    printf("%.2X ", rx[ret]);
//  }
//  puts("");
}


int led_driver (
  int ledNum,
  int R,
  int G,
  int B,
  int brightness)
{
  int             ret = 0;
  int             fd;

  fd = open (device, O_RDWR);
  if (fd < 0)
    pabort ("can't open device");

/*
 * spi mode
 */
  ret = ioctl (fd, SPI_IOC_WR_MODE, &mode);
  if (ret == -1)
    pabort ("can't set spi mode");

  ret = ioctl (fd, SPI_IOC_RD_MODE, &mode);
  if (ret == -1)
    pabort ("can't get spi mode");

/*
 * bits per word
 */
  ret = ioctl (fd, SPI_IOC_WR_BITS_PER_WORD, &bits);
  if (ret == -1)
    pabort ("can't set bits per word");

  ret = ioctl (fd, SPI_IOC_RD_BITS_PER_WORD, &bits);
  if (ret == -1)
    pabort ("can't get bits per word");

/*
 * max speed hz
 */
  ret = ioctl (fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed);
  if (ret == -1)
    pabort ("can't set max speed hz");

  ret = ioctl (fd, SPI_IOC_RD_MAX_SPEED_HZ, &speed);
  if (ret == -1)
    pabort ("can't get max speed hz");

//  printf("spi mode: %d\n", mode);
//  printf("bits per word: %d\n", bits);
//  printf("max speed: %d Hz (%d KHz)\n", speed, speed/1000);

  int             rPos = 8 + ((ledNum - 1) * 4) + 3;
  int             gPos = 8 + ((ledNum - 1) * 4) + 2;
  int             bPos = 8 + ((ledNum - 1) * 4) + 1;
  int             briPos = 8 + ((ledNum - 1) * 4) + 0;

  pixelBuffer[rPos] = R;
  pixelBuffer[gPos] = G;
  pixelBuffer[bPos] = B;
  pixelBuffer[briPos] = 0xE0 + (brightness & 0x1F);

  transfer (fd);

  close (fd);

  return ret;
}
