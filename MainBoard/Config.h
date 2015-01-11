#ifndef __CONFIG_H__
#define __CONFIG_H__

/* Pin define */
#define SERVER_PIN		2
#define SD_SDCS_PIN             4
#define SD_CS_PIN              10

/* Constant define */
#define CHILDREN_COUNT      9
#define FPS                25
#define FRAME_US (1000 / FPS)

/**
 * ExtEggStatus
 * +-------------+-------------+
 * |Manual/Detect|  Open/Close |
 * +-------------+-------------+
 * Detect: 0
 * Manual: 1
 * Close : 0
 * Open  : 1
 */
#define STATUS 0x00000001 /* Open/Close */
#define MODE   0x00000002 /* Manual/Detect */

#endif // __CONFIG_H__
