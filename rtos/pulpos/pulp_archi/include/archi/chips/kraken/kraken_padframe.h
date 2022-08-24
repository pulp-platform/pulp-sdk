
#ifndef KRAKEN_PADFRAME_H
#define KRAKEN_PADFRAME_H
#include <stdint.h>

#define KRAKEN_PADFRAME_BASE_ADDRESS 0X1B000000

#ifndef KRAKEN_PADFRAME_BASE_ADDRESS
#error "KRAKEN_PADFRAME_BASE_ADDRESS is not defined. Set this token to the configuration base address of your padframe before you include this header file."
#endif



/**
 * Sets the chip2pad pad signal for the pad: pad_gpioa
 *
 * @param idx The instance index of the pad to configure (< 26)
 * @param value The value to program into the pad configuration register. A value smaller than 1.
 */
void kraken_padframe_aon_pad_gpioa_cfg_chip2pad_set(uint32_t idx, uint8_t value);

/**
 * Get the currently configured chip2pad value for the pad: pad_gpioa
 *
 * @param idx The instance index of the pad to configure (< 26)
 * @return The value of the chip2pad field
 */
uint8_t kraken_padframe_aon_pad_gpioa_cfg_chip2pad_get(uint32_t idx);

/**
 * Sets the rxe pad signal for the pad: pad_gpioa
 *
 * @param idx The instance index of the pad to configure (< 26)
 * @param value The value to program into the pad configuration register. A value smaller than 1.
 */
void kraken_padframe_aon_pad_gpioa_cfg_rxe_set(uint32_t idx, uint8_t value);

/**
 * Get the currently configured rxe value for the pad: pad_gpioa
 *
 * @param idx The instance index of the pad to configure (< 26)
 * @return The value of the rxe field
 */
uint8_t kraken_padframe_aon_pad_gpioa_cfg_rxe_get(uint32_t idx);

/**
 * Sets the trie pad signal for the pad: pad_gpioa
 *
 * @param idx The instance index of the pad to configure (< 26)
 * @param value The value to program into the pad configuration register. A value smaller than 1.
 */
void kraken_padframe_aon_pad_gpioa_cfg_trie_set(uint32_t idx, uint8_t value);

/**
 * Get the currently configured trie value for the pad: pad_gpioa
 *
 * @param idx The instance index of the pad to configure (< 26)
 * @return The value of the trie field
 */
uint8_t kraken_padframe_aon_pad_gpioa_cfg_trie_get(uint32_t idx);

/**
 * Sets the pue pad signal for the pad: pad_gpioa
 *
 * @param idx The instance index of the pad to configure (< 26)
 * @param value The value to program into the pad configuration register. A value smaller than 1.
 */
void kraken_padframe_aon_pad_gpioa_cfg_pue_set(uint32_t idx, uint8_t value);

/**
 * Get the currently configured pue value for the pad: pad_gpioa
 *
 * @param idx The instance index of the pad to configure (< 26)
 * @return The value of the pue field
 */
uint8_t kraken_padframe_aon_pad_gpioa_cfg_pue_get(uint32_t idx);

/**
 * Sets the pde pad signal for the pad: pad_gpioa
 *
 * @param idx The instance index of the pad to configure (< 26)
 * @param value The value to program into the pad configuration register. A value smaller than 1.
 */
void kraken_padframe_aon_pad_gpioa_cfg_pde_set(uint32_t idx, uint8_t value);

/**
 * Get the currently configured pde value for the pad: pad_gpioa
 *
 * @param idx The instance index of the pad to configure (< 26)
 * @return The value of the pde field
 */
uint8_t kraken_padframe_aon_pad_gpioa_cfg_pde_get(uint32_t idx);

/**
 * Sets the drv pad signal for the pad: pad_gpioa
 *
 * @param idx The instance index of the pad to configure (< 26)
 * @param value The value to program into the pad configuration register. A value smaller than 3.
 */
void kraken_padframe_aon_pad_gpioa_cfg_drv_set(uint32_t idx, uint8_t value);

/**
 * Get the currently configured drv value for the pad: pad_gpioa
 *
 * @param idx The instance index of the pad to configure (< 26)
 * @return The value of the drv field
 */
uint8_t kraken_padframe_aon_pad_gpioa_cfg_drv_get(uint32_t idx);

/**
 * Sets the slw pad signal for the pad: pad_gpioa
 *
 * @param idx The instance index of the pad to configure (< 26)
 * @param value The value to program into the pad configuration register. A value smaller than 1.
 */
void kraken_padframe_aon_pad_gpioa_cfg_slw_set(uint32_t idx, uint8_t value);

/**
 * Get the currently configured slw value for the pad: pad_gpioa
 *
 * @param idx The instance index of the pad to configure (< 26)
 * @return The value of the slw field
 */
uint8_t kraken_padframe_aon_pad_gpioa_cfg_slw_get(uint32_t idx);

/**
 * Sets the smt pad signal for the pad: pad_gpioa
 *
 * @param idx The instance index of the pad to configure (< 26)
 * @param value The value to program into the pad configuration register. A value smaller than 1.
 */
void kraken_padframe_aon_pad_gpioa_cfg_smt_set(uint32_t idx, uint8_t value);

/**
 * Get the currently configured smt value for the pad: pad_gpioa
 *
 * @param idx The instance index of the pad to configure (< 26)
 * @return The value of the smt field
 */
uint8_t kraken_padframe_aon_pad_gpioa_cfg_smt_get(uint32_t idx);

typedef enum {
  KRAKEN_PADFRAME_AON_PAD_GPIOA_REGISTER = 0,

KRAKEN_PADFRAME_AON_PAD_GPIOA_group_QSPI0_port_SPI_SDIO0 = 1,


KRAKEN_PADFRAME_AON_PAD_GPIOA_group_QSPI0_port_SPI_SDIO1 = 2,


KRAKEN_PADFRAME_AON_PAD_GPIOA_group_QSPI0_port_SPI_SDIO2 = 3,


KRAKEN_PADFRAME_AON_PAD_GPIOA_group_QSPI0_port_SPI_SDIO3 = 4,


KRAKEN_PADFRAME_AON_PAD_GPIOA_group_QSPI0_port_SPI_SCK = 5,


KRAKEN_PADFRAME_AON_PAD_GPIOA_group_QSPI0_port_SPI_CSN0 = 6,


KRAKEN_PADFRAME_AON_PAD_GPIOA_group_QSPI0_port_SPI_CSN1 = 7,


KRAKEN_PADFRAME_AON_PAD_GPIOA_group_QSPI0_port_SPI_CSN2 = 8,


KRAKEN_PADFRAME_AON_PAD_GPIOA_group_QSPI0_port_SPI_CSN3 = 9,


KRAKEN_PADFRAME_AON_PAD_GPIOA_group_QSPI1_port_SPI_SDIO0 = 10,


KRAKEN_PADFRAME_AON_PAD_GPIOA_group_QSPI1_port_SPI_SDIO1 = 11,


KRAKEN_PADFRAME_AON_PAD_GPIOA_group_QSPI1_port_SPI_SDIO2 = 12,


KRAKEN_PADFRAME_AON_PAD_GPIOA_group_QSPI1_port_SPI_SDIO3 = 13,


KRAKEN_PADFRAME_AON_PAD_GPIOA_group_QSPI1_port_SPI_SCK = 14,


KRAKEN_PADFRAME_AON_PAD_GPIOA_group_QSPI1_port_SPI_CSN0 = 15,


KRAKEN_PADFRAME_AON_PAD_GPIOA_group_QSPI1_port_SPI_CSN1 = 16,


KRAKEN_PADFRAME_AON_PAD_GPIOA_group_QSPI1_port_SPI_CSN2 = 17,


KRAKEN_PADFRAME_AON_PAD_GPIOA_group_QSPI1_port_SPI_CSN3 = 18,


KRAKEN_PADFRAME_AON_PAD_GPIOA_group_UART1_port_RX = 19,


KRAKEN_PADFRAME_AON_PAD_GPIOA_group_UART1_port_TX = 20,


KRAKEN_PADFRAME_AON_PAD_GPIOA_group_TIMER0_port_OUT0 = 21,


KRAKEN_PADFRAME_AON_PAD_GPIOA_group_TIMER0_port_OUT1 = 22,


KRAKEN_PADFRAME_AON_PAD_GPIOA_group_TIMER0_port_OUT2 = 23,


KRAKEN_PADFRAME_AON_PAD_GPIOA_group_TIMER0_port_OUT3 = 24,


KRAKEN_PADFRAME_AON_PAD_GPIOA_group_I2C0_port_I2C_SDA = 25,


KRAKEN_PADFRAME_AON_PAD_GPIOA_group_I2C0_port_I2C_SCL = 26,


KRAKEN_PADFRAME_AON_PAD_GPIOA_group_I2C1_port_I2C_SDA = 27,


KRAKEN_PADFRAME_AON_PAD_GPIOA_group_I2C1_port_I2C_SCL = 28,


KRAKEN_PADFRAME_AON_PAD_GPIOA_group_I2C2_port_I2C_SDA = 29,


KRAKEN_PADFRAME_AON_PAD_GPIOA_group_I2C2_port_I2C_SCL = 30,


KRAKEN_PADFRAME_AON_PAD_GPIOA_group_I2C3_port_I2C_SDA = 31,


KRAKEN_PADFRAME_AON_PAD_GPIOA_group_I2C3_port_I2C_SCL = 32,


KRAKEN_PADFRAME_AON_PAD_GPIOA_group_DVSI0_port_XYDATA0 = 33,


KRAKEN_PADFRAME_AON_PAD_GPIOA_group_DVSI0_port_XYDATA1 = 34,


KRAKEN_PADFRAME_AON_PAD_GPIOA_group_DVSI0_port_XYDATA2 = 35,


KRAKEN_PADFRAME_AON_PAD_GPIOA_group_DVSI0_port_XYDATA3 = 36,


KRAKEN_PADFRAME_AON_PAD_GPIOA_group_DVSI0_port_XYDATA4 = 37,


KRAKEN_PADFRAME_AON_PAD_GPIOA_group_DVSI0_port_XYDATA5 = 38,


KRAKEN_PADFRAME_AON_PAD_GPIOA_group_DVSI0_port_XYDATA6 = 39,


KRAKEN_PADFRAME_AON_PAD_GPIOA_group_DVSI0_port_XYDATA7 = 40,


KRAKEN_PADFRAME_AON_PAD_GPIOA_group_DVSI0_port_ON0 = 41,


KRAKEN_PADFRAME_AON_PAD_GPIOA_group_DVSI0_port_ON1 = 42,


KRAKEN_PADFRAME_AON_PAD_GPIOA_group_DVSI0_port_ON2 = 43,


KRAKEN_PADFRAME_AON_PAD_GPIOA_group_DVSI0_port_ON3 = 44,


KRAKEN_PADFRAME_AON_PAD_GPIOA_group_DVSI0_port_OFF0 = 45,


KRAKEN_PADFRAME_AON_PAD_GPIOA_group_DVSI0_port_OFF1 = 46,


KRAKEN_PADFRAME_AON_PAD_GPIOA_group_DVSI0_port_OFF2 = 47,


KRAKEN_PADFRAME_AON_PAD_GPIOA_group_DVSI0_port_OFF3 = 48,


KRAKEN_PADFRAME_AON_PAD_GPIOA_group_DVSI0_port_ASA = 49,


KRAKEN_PADFRAME_AON_PAD_GPIOA_group_DVSI0_port_ARE = 50,


KRAKEN_PADFRAME_AON_PAD_GPIOA_group_DVSI0_port_ASY = 51,


KRAKEN_PADFRAME_AON_PAD_GPIOA_group_DVSI0_port_YNRST = 52,


KRAKEN_PADFRAME_AON_PAD_GPIOA_group_DVSI0_port_YCLK = 53,


KRAKEN_PADFRAME_AON_PAD_GPIOA_group_DVSI0_port_SXY = 54,


KRAKEN_PADFRAME_AON_PAD_GPIOA_group_DVSI0_port_XCLK = 55,


KRAKEN_PADFRAME_AON_PAD_GPIOA_group_DVSI0_port_XNRST = 56,


KRAKEN_PADFRAME_AON_PAD_GPIOA_group_DVSI0_port_CFG0 = 57,


KRAKEN_PADFRAME_AON_PAD_GPIOA_group_DVSI0_port_CFG1 = 58,


KRAKEN_PADFRAME_AON_PAD_GPIOA_group_DVSI0_port_CFG2 = 59,


KRAKEN_PADFRAME_AON_PAD_GPIOA_group_DVSI0_port_CFG3 = 60,


KRAKEN_PADFRAME_AON_PAD_GPIOA_group_DVSI0_port_CFG4 = 61,


KRAKEN_PADFRAME_AON_PAD_GPIOA_group_DVSI0_port_CFG5 = 62,


KRAKEN_PADFRAME_AON_PAD_GPIOA_group_DVSI0_port_CFG6 = 63,


KRAKEN_PADFRAME_AON_PAD_GPIOA_group_DVSI0_port_CFG7 = 64,


KRAKEN_PADFRAME_AON_PAD_GPIOA_group_GPIO_A_port_GPIO0 = 65,


KRAKEN_PADFRAME_AON_PAD_GPIOA_group_GPIO_A_port_GPIO1 = 66,


KRAKEN_PADFRAME_AON_PAD_GPIOA_group_GPIO_A_port_GPIO2 = 67,


KRAKEN_PADFRAME_AON_PAD_GPIOA_group_GPIO_A_port_GPIO3 = 68,


KRAKEN_PADFRAME_AON_PAD_GPIOA_group_GPIO_A_port_GPIO4 = 69,


KRAKEN_PADFRAME_AON_PAD_GPIOA_group_GPIO_A_port_GPIO5 = 70,


KRAKEN_PADFRAME_AON_PAD_GPIOA_group_GPIO_A_port_GPIO6 = 71,


KRAKEN_PADFRAME_AON_PAD_GPIOA_group_GPIO_A_port_GPIO7 = 72,


KRAKEN_PADFRAME_AON_PAD_GPIOA_group_GPIO_A_port_GPIO8 = 73,


KRAKEN_PADFRAME_AON_PAD_GPIOA_group_GPIO_A_port_GPIO9 = 74,


KRAKEN_PADFRAME_AON_PAD_GPIOA_group_GPIO_A_port_GPIO10 = 75,


KRAKEN_PADFRAME_AON_PAD_GPIOA_group_GPIO_A_port_GPIO11 = 76,


KRAKEN_PADFRAME_AON_PAD_GPIOA_group_GPIO_A_port_GPIO12 = 77,


KRAKEN_PADFRAME_AON_PAD_GPIOA_group_GPIO_A_port_GPIO13 = 78,


KRAKEN_PADFRAME_AON_PAD_GPIOA_group_GPIO_A_port_GPIO14 = 79,


KRAKEN_PADFRAME_AON_PAD_GPIOA_group_GPIO_A_port_GPIO15 = 80,


KRAKEN_PADFRAME_AON_PAD_GPIOA_group_GPIO_A_port_GPIO16 = 81,


KRAKEN_PADFRAME_AON_PAD_GPIOA_group_GPIO_A_port_GPIO17 = 82,


KRAKEN_PADFRAME_AON_PAD_GPIOA_group_GPIO_A_port_GPIO18 = 83,


KRAKEN_PADFRAME_AON_PAD_GPIOA_group_GPIO_A_port_GPIO19 = 84,


KRAKEN_PADFRAME_AON_PAD_GPIOA_group_GPIO_A_port_GPIO20 = 85,


KRAKEN_PADFRAME_AON_PAD_GPIOA_group_GPIO_A_port_GPIO21 = 86,


KRAKEN_PADFRAME_AON_PAD_GPIOA_group_GPIO_A_port_GPIO22 = 87,


KRAKEN_PADFRAME_AON_PAD_GPIOA_group_GPIO_A_port_GPIO23 = 88,


KRAKEN_PADFRAME_AON_PAD_GPIOA_group_GPIO_A_port_GPIO24 = 89,


KRAKEN_PADFRAME_AON_PAD_GPIOA_group_GPIO_A_port_GPIO25 = 90,


KRAKEN_PADFRAME_AON_PAD_GPIOA_group_GPIO_C_port_GPIO0 = 91,


KRAKEN_PADFRAME_AON_PAD_GPIOA_group_GPIO_C_port_GPIO1 = 92,


KRAKEN_PADFRAME_AON_PAD_GPIOA_group_GPIO_C_port_GPIO2 = 93,


KRAKEN_PADFRAME_AON_PAD_GPIOA_group_GPIO_C_port_GPIO3 = 94,

} kraken_padframe_aon_pad_gpioa_mux_sel_t;

/**
 * Choose the entity (a port or the dedicated configuration register) that controls pad_gpioa.
 *
 * @param idx The instance index of the pad to configure (< 26)
 * @param mux_sel Port or configuration register to connect to the pad.
 */
void kraken_padframe_aon_pad_gpioa_mux_set(uint32_t idx, kraken_padframe_aon_pad_gpioa_mux_sel_t mux_sel);

/**
 * Read the current multiplexer select value configured for pad_gpioa.
 *
 * @param idx The instance index of the pad to configure (< 26)
 * @return Port or configuration register currently connected to the pad.
 */
 kraken_padframe_aon_pad_gpioa_mux_sel_t kraken_padframe_aon_pad_gpioa_mux_get(uint32_t idx);



/**
 * Sets the chip2pad pad signal for the pad: pad_gpiob
 *
 * @param idx The instance index of the pad to configure (< 16)
 * @param value The value to program into the pad configuration register. A value smaller than 1.
 */
void kraken_padframe_aon_pad_gpiob_cfg_chip2pad_set(uint32_t idx, uint8_t value);

/**
 * Get the currently configured chip2pad value for the pad: pad_gpiob
 *
 * @param idx The instance index of the pad to configure (< 16)
 * @return The value of the chip2pad field
 */
uint8_t kraken_padframe_aon_pad_gpiob_cfg_chip2pad_get(uint32_t idx);

/**
 * Sets the rxe pad signal for the pad: pad_gpiob
 *
 * @param idx The instance index of the pad to configure (< 16)
 * @param value The value to program into the pad configuration register. A value smaller than 1.
 */
void kraken_padframe_aon_pad_gpiob_cfg_rxe_set(uint32_t idx, uint8_t value);

/**
 * Get the currently configured rxe value for the pad: pad_gpiob
 *
 * @param idx The instance index of the pad to configure (< 16)
 * @return The value of the rxe field
 */
uint8_t kraken_padframe_aon_pad_gpiob_cfg_rxe_get(uint32_t idx);

/**
 * Sets the trie pad signal for the pad: pad_gpiob
 *
 * @param idx The instance index of the pad to configure (< 16)
 * @param value The value to program into the pad configuration register. A value smaller than 1.
 */
void kraken_padframe_aon_pad_gpiob_cfg_trie_set(uint32_t idx, uint8_t value);

/**
 * Get the currently configured trie value for the pad: pad_gpiob
 *
 * @param idx The instance index of the pad to configure (< 16)
 * @return The value of the trie field
 */
uint8_t kraken_padframe_aon_pad_gpiob_cfg_trie_get(uint32_t idx);

/**
 * Sets the pue pad signal for the pad: pad_gpiob
 *
 * @param idx The instance index of the pad to configure (< 16)
 * @param value The value to program into the pad configuration register. A value smaller than 1.
 */
void kraken_padframe_aon_pad_gpiob_cfg_pue_set(uint32_t idx, uint8_t value);

/**
 * Get the currently configured pue value for the pad: pad_gpiob
 *
 * @param idx The instance index of the pad to configure (< 16)
 * @return The value of the pue field
 */
uint8_t kraken_padframe_aon_pad_gpiob_cfg_pue_get(uint32_t idx);

/**
 * Sets the pde pad signal for the pad: pad_gpiob
 *
 * @param idx The instance index of the pad to configure (< 16)
 * @param value The value to program into the pad configuration register. A value smaller than 1.
 */
void kraken_padframe_aon_pad_gpiob_cfg_pde_set(uint32_t idx, uint8_t value);

/**
 * Get the currently configured pde value for the pad: pad_gpiob
 *
 * @param idx The instance index of the pad to configure (< 16)
 * @return The value of the pde field
 */
uint8_t kraken_padframe_aon_pad_gpiob_cfg_pde_get(uint32_t idx);

/**
 * Sets the drv pad signal for the pad: pad_gpiob
 *
 * @param idx The instance index of the pad to configure (< 16)
 * @param value The value to program into the pad configuration register. A value smaller than 3.
 */
void kraken_padframe_aon_pad_gpiob_cfg_drv_set(uint32_t idx, uint8_t value);

/**
 * Get the currently configured drv value for the pad: pad_gpiob
 *
 * @param idx The instance index of the pad to configure (< 16)
 * @return The value of the drv field
 */
uint8_t kraken_padframe_aon_pad_gpiob_cfg_drv_get(uint32_t idx);

/**
 * Sets the slw pad signal for the pad: pad_gpiob
 *
 * @param idx The instance index of the pad to configure (< 16)
 * @param value The value to program into the pad configuration register. A value smaller than 1.
 */
void kraken_padframe_aon_pad_gpiob_cfg_slw_set(uint32_t idx, uint8_t value);

/**
 * Get the currently configured slw value for the pad: pad_gpiob
 *
 * @param idx The instance index of the pad to configure (< 16)
 * @return The value of the slw field
 */
uint8_t kraken_padframe_aon_pad_gpiob_cfg_slw_get(uint32_t idx);

/**
 * Sets the smt pad signal for the pad: pad_gpiob
 *
 * @param idx The instance index of the pad to configure (< 16)
 * @param value The value to program into the pad configuration register. A value smaller than 1.
 */
void kraken_padframe_aon_pad_gpiob_cfg_smt_set(uint32_t idx, uint8_t value);

/**
 * Get the currently configured smt value for the pad: pad_gpiob
 *
 * @param idx The instance index of the pad to configure (< 16)
 * @return The value of the smt field
 */
uint8_t kraken_padframe_aon_pad_gpiob_cfg_smt_get(uint32_t idx);

typedef enum {
  KRAKEN_PADFRAME_AON_PAD_GPIOB_REGISTER = 0,

KRAKEN_PADFRAME_AON_PAD_GPIOB_group_QSPI2_port_SPI_SDIO0 = 1,


KRAKEN_PADFRAME_AON_PAD_GPIOB_group_QSPI2_port_SPI_SDIO1 = 2,


KRAKEN_PADFRAME_AON_PAD_GPIOB_group_QSPI2_port_SPI_SDIO2 = 3,


KRAKEN_PADFRAME_AON_PAD_GPIOB_group_QSPI2_port_SPI_SDIO3 = 4,


KRAKEN_PADFRAME_AON_PAD_GPIOB_group_QSPI2_port_SPI_SCK = 5,


KRAKEN_PADFRAME_AON_PAD_GPIOB_group_QSPI2_port_SPI_CSN0 = 6,


KRAKEN_PADFRAME_AON_PAD_GPIOB_group_QSPI2_port_SPI_CSN1 = 7,


KRAKEN_PADFRAME_AON_PAD_GPIOB_group_QSPI2_port_SPI_CSN2 = 8,


KRAKEN_PADFRAME_AON_PAD_GPIOB_group_QSPI2_port_SPI_CSN3 = 9,


KRAKEN_PADFRAME_AON_PAD_GPIOB_group_QSPI3_port_SPI_SDIO0 = 10,


KRAKEN_PADFRAME_AON_PAD_GPIOB_group_QSPI3_port_SPI_SDIO1 = 11,


KRAKEN_PADFRAME_AON_PAD_GPIOB_group_QSPI3_port_SPI_SDIO2 = 12,


KRAKEN_PADFRAME_AON_PAD_GPIOB_group_QSPI3_port_SPI_SDIO3 = 13,


KRAKEN_PADFRAME_AON_PAD_GPIOB_group_QSPI3_port_SPI_SCK = 14,


KRAKEN_PADFRAME_AON_PAD_GPIOB_group_QSPI3_port_SPI_CSN0 = 15,


KRAKEN_PADFRAME_AON_PAD_GPIOB_group_QSPI3_port_SPI_CSN1 = 16,


KRAKEN_PADFRAME_AON_PAD_GPIOB_group_QSPI3_port_SPI_CSN2 = 17,


KRAKEN_PADFRAME_AON_PAD_GPIOB_group_QSPI3_port_SPI_CSN3 = 18,


KRAKEN_PADFRAME_AON_PAD_GPIOB_group_UART0_port_RX = 19,


KRAKEN_PADFRAME_AON_PAD_GPIOB_group_UART0_port_TX = 20,


KRAKEN_PADFRAME_AON_PAD_GPIOB_group_TIMER1_port_OUT0 = 21,


KRAKEN_PADFRAME_AON_PAD_GPIOB_group_TIMER1_port_OUT1 = 22,


KRAKEN_PADFRAME_AON_PAD_GPIOB_group_TIMER1_port_OUT2 = 23,


KRAKEN_PADFRAME_AON_PAD_GPIOB_group_TIMER1_port_OUT3 = 24,


KRAKEN_PADFRAME_AON_PAD_GPIOB_group_TIMER2_port_OUT0 = 25,


KRAKEN_PADFRAME_AON_PAD_GPIOB_group_TIMER2_port_OUT1 = 26,


KRAKEN_PADFRAME_AON_PAD_GPIOB_group_TIMER2_port_OUT2 = 27,


KRAKEN_PADFRAME_AON_PAD_GPIOB_group_TIMER2_port_OUT3 = 28,


KRAKEN_PADFRAME_AON_PAD_GPIOB_group_TIMER3_port_OUT0 = 29,


KRAKEN_PADFRAME_AON_PAD_GPIOB_group_TIMER3_port_OUT1 = 30,


KRAKEN_PADFRAME_AON_PAD_GPIOB_group_TIMER3_port_OUT2 = 31,


KRAKEN_PADFRAME_AON_PAD_GPIOB_group_TIMER3_port_OUT3 = 32,


KRAKEN_PADFRAME_AON_PAD_GPIOB_group_CPI0_port_PCLK = 33,


KRAKEN_PADFRAME_AON_PAD_GPIOB_group_CPI0_port_HSYNC = 34,


KRAKEN_PADFRAME_AON_PAD_GPIOB_group_CPI0_port_VSYNC = 35,


KRAKEN_PADFRAME_AON_PAD_GPIOB_group_CPI0_port_DATA0 = 36,


KRAKEN_PADFRAME_AON_PAD_GPIOB_group_CPI0_port_DATA1 = 37,


KRAKEN_PADFRAME_AON_PAD_GPIOB_group_CPI0_port_DATA2 = 38,


KRAKEN_PADFRAME_AON_PAD_GPIOB_group_CPI0_port_DATA3 = 39,


KRAKEN_PADFRAME_AON_PAD_GPIOB_group_CPI0_port_DATA4 = 40,


KRAKEN_PADFRAME_AON_PAD_GPIOB_group_CPI0_port_DATA5 = 41,


KRAKEN_PADFRAME_AON_PAD_GPIOB_group_CPI0_port_DATA6 = 42,


KRAKEN_PADFRAME_AON_PAD_GPIOB_group_CPI0_port_DATA7 = 43,


KRAKEN_PADFRAME_AON_PAD_GPIOB_group_GPIO_B_port_GPIO0 = 44,


KRAKEN_PADFRAME_AON_PAD_GPIOB_group_GPIO_B_port_GPIO1 = 45,


KRAKEN_PADFRAME_AON_PAD_GPIOB_group_GPIO_B_port_GPIO2 = 46,


KRAKEN_PADFRAME_AON_PAD_GPIOB_group_GPIO_B_port_GPIO3 = 47,


KRAKEN_PADFRAME_AON_PAD_GPIOB_group_GPIO_B_port_GPIO4 = 48,


KRAKEN_PADFRAME_AON_PAD_GPIOB_group_GPIO_B_port_GPIO5 = 49,


KRAKEN_PADFRAME_AON_PAD_GPIOB_group_GPIO_B_port_GPIO6 = 50,


KRAKEN_PADFRAME_AON_PAD_GPIOB_group_GPIO_B_port_GPIO7 = 51,


KRAKEN_PADFRAME_AON_PAD_GPIOB_group_GPIO_B_port_GPIO8 = 52,


KRAKEN_PADFRAME_AON_PAD_GPIOB_group_GPIO_B_port_GPIO9 = 53,


KRAKEN_PADFRAME_AON_PAD_GPIOB_group_GPIO_B_port_GPIO10 = 54,


KRAKEN_PADFRAME_AON_PAD_GPIOB_group_GPIO_B_port_GPIO11 = 55,


KRAKEN_PADFRAME_AON_PAD_GPIOB_group_GPIO_B_port_GPIO12 = 56,


KRAKEN_PADFRAME_AON_PAD_GPIOB_group_GPIO_B_port_GPIO13 = 57,


KRAKEN_PADFRAME_AON_PAD_GPIOB_group_GPIO_B_port_GPIO14 = 58,


KRAKEN_PADFRAME_AON_PAD_GPIOB_group_GPIO_B_port_GPIO15 = 59,

} kraken_padframe_aon_pad_gpiob_mux_sel_t;

/**
 * Choose the entity (a port or the dedicated configuration register) that controls pad_gpiob.
 *
 * @param idx The instance index of the pad to configure (< 16)
 * @param mux_sel Port or configuration register to connect to the pad.
 */
void kraken_padframe_aon_pad_gpiob_mux_set(uint32_t idx, kraken_padframe_aon_pad_gpiob_mux_sel_t mux_sel);

/**
 * Read the current multiplexer select value configured for pad_gpiob.
 *
 * @param idx The instance index of the pad to configure (< 16)
 * @return Port or configuration register currently connected to the pad.
 */
 kraken_padframe_aon_pad_gpiob_mux_sel_t kraken_padframe_aon_pad_gpiob_mux_get(uint32_t idx);




#endif /*  KRAKEN_PADFRAME_H */
