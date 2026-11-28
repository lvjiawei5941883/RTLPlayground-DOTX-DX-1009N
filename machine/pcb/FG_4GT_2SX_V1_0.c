/*
 * FG-4GT-2SX V1.0  —  广睿杰 GRJ-4GT-2SX
 * 主板丝印: FG-4GT-2SX V2.0  /  PCB 17452  /  RTL8372N  /  4MB flash  /  非网管原厂
 *
 * 基于上游 machine/pcb/FG_4GT_2SX_V2_0.c，LED 段（high_leds / led_sets / led_mux /
 * LED_GLB_IO_EN）原样保留 —— 该段在本板上实测灯色与功能均正常。
 *
 * 仅按本板实测修改 SFP 与 reset 引脚（上游 v2.0 批次用的是另一组 GPIO）：
 *   左笼 detect : GPIO38              -> GPIO30_ACL_BIT3_EN
 *   左笼 LOS    : GPIO_NA             -> GPIO37
 *   右笼 detect : GPIO37              -> GPIO50_I2C_SCL2_UART1_TX
 *   reset       : GPIO_NA             -> GPIO54_ACL_BIT2_EN
 *
 * I2C / sds / tx_disable 与上游一致，未改动（已由 RTL_4GT_2S__V1_04 交叉验证同板）。
 */

#define MACHINE_CHIPSET RTL8372
#include "machine_defs.h"

__code const struct machine machine = {
	MACHINE_COMMON_SETTING,
	.n_sfp = 2,
	.log_to_phys_port = {0, 0, 0, 6, 1, 2, 3, 4, 5},
	.phys_to_log_port = {4, 5, 6, 7, 8, 3, 0, 0, 0},
	.is_sfp = {0, 0, 0, 2, 0, 0, 0, 0, 1},

	// Left SFP port
	.sfp_port[0].pin_detect = GPIO30_ACL_BIT3_EN,
	.sfp_port[0].pin_los = GPIO37,
	.sfp_port[0].pin_tx_disable = GPIO_NA,
	.sfp_port[0].sds = 1,
	.sfp_port[0].i2c = I2CBUS( GPIO39_I2C_SDA4, GPIO40_I2C_SCL3_MDC1 ),

	// Right SFP port
	.sfp_port[1].pin_detect = GPIO50_I2C_SCL2_UART1_TX,
	.sfp_port[1].pin_los = GPIO_NA,
	.sfp_port[1].pin_tx_disable = GPIO_NA,
	.sfp_port[1].sds = 0,
	.sfp_port[1].i2c = I2CBUS( GPIO41_I2C_SDA3_MDIO1, GPIO40_I2C_SCL3_MDC1 ),

	.reset_pin = GPIO54_ACL_BIT2_EN,
	.high_leds = { .mux = LED_27 | LED_28_SYS | LED_29, .enable = LED_28_SYS | LED_29 },
	.port_led_set = { 0, 0, 0, 1, 0, 0, 0, 0, 1},
	/* Ports 1-4 RJ45 use set 0, port 5-6 SFP uses set 1
	 * Ports 1-4: Green: 2.5GBit, Amber: 10/100/1000MBit
	 * Ports 5-6: Green: 100MBit-10GBit
	 */
	.led_sets = {
			{
				LEDS_2G5 | LEDS_LINK | LEDS_ACT,
				LEDS_1G | LEDS_100M | LEDS_10M | LEDS_LINK | LEDS_ACT,
				0,
				LEDS_2G5 | LEDS_LINK | LEDS_ACT
			},
			{
				LEDS_10G | LEDS_5G | LEDS_2G5 | LEDS_1G | LEDS_100M | LEDS_10M | LEDS_LINK | LEDS_ACT,
				LEDS_10G | LEDS_LINK,
				0,
				LEDS_COL | LEDS_DUPLEX
			},
			{
				LEDS_1G | LEDS_100M | LEDS_10M | LEDS_LINK | LEDS_ACT,
				LEDS_2G5 | LEDS_1G | LEDS_LINK,
				LEDS_5G | LEDS_2G5 | LEDS_LINK | LEDS_ACT,
				LEDS_10G | LEDS_LINK | LEDS_ACT
			},
			{
				LEDS_TX,
				LEDS_RX,
				LEDS_10G | LEDS_TWO_PAIR_5G | LEDS_5G | LEDS_TWO_PAIR_2G5 |
					LEDS_2G5 | LEDS_TWO_PAIR_1G | LEDS_1G | LEDS_500M | LEDS_100M | LEDS_10M | LEDS_ACT,
				LEDS_10G | LEDS_TWO_PAIR_5G | LEDS_5G | LEDS_TWO_PAIR_2G5 |
					LEDS_2G5 | LEDS_TWO_PAIR_1G | LEDS_1G | LEDS_500M | LEDS_100M | LEDS_10M | LEDS_LINK
			},
	 },
	.led_mux_custom = 1,
	.led_mux = {
				0x0c, 0x0d, 0x0e, 0x10, 0x11, 0x12, 0x14, 0x3f, 0x15, 0x16,
				0x18, 0x0e, 0x19, 0x11, 0x12, 0x1a, 0x15, 0x16, 0x1c, 0x19,
				0x1a, 0x1d, 0x1d, 0x1e, 0x1e, 0x20, 0x21, 0x22
		},
	};

MACHINE_CUSTOM_INIT(
	REG_SET(RTL837X_REG_LED_GLB_IO_EN, 0x7624155b);
)
