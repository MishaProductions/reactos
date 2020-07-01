/*
 * PROJECT:     NEC PC-98 series HAL
 * LICENSE:     GPL-2.0-or-later (https://spdx.org/licenses/GPL-2.0-or-later)
 * PURPOSE:     IRQL mapping
 * COPYRIGHT:   Copyright 2020 Dmitry Borisov (di.sean@protonmail.com)
 */

/* INCLUDES *******************************************************************/

#include <hal.h>

/* GLOBALS ********************************************************************/

/* This table contains the static x86 PIC mapping between IRQLs and IRQs */
ULONG KiI8259MaskTable[32] =
{
    /*
     * See comments of the PC/AT version.
     */
    0b00000000000000000000000000000000, /* IRQL 0 */
    0b00000000000000000000000000000000, /* IRQL 1 */
    0b00000000000000000000000000000000, /* IRQL 2 */
    0b00000000000000000000000000000000, /* IRQL 3 */
    0b11111111100000000000000000000000, /* IRQL 4 */
    0b11111111110000000000000000000000, /* IRQL 5 */
    0b11111111111000000000000000000000, /* IRQL 6 */
    0b11111111111100000000000000000000, /* IRQL 7 */
    0b11111111111110000000000000000000, /* IRQL 8 */
    0b11111111111111000000000000000000, /* IRQL 9 */
    0b11111111111111100000000000000000, /* IRQL 10 */
    0b11111111111111110000000000000000, /* IRQL 11 */

    /*
     * Okay, now we're finally starting to mask off IRQs on the slave PIC, from
     * IRQ15 to IRQ8. Note that the output of INT of RTC is connected to the IR7.
     * We need to keep this IRQ unmasked, so we can ensure profiling the whole system.
     */
    0b11111111111111110000000000000000, /* IRQL 12 */
    0b11111111111111110100000000000000, /* IRQL 13 */
    0b11111111111111110110000000000000, /* IRQL 14 */
    0b11111111111111110111000000000000, /* IRQL 15 */
    0b11111111111111110111100000000000, /* IRQL 16 */
    0b11111111111111110111110000000000, /* IRQL 17 */
    0b11111111111111110111111000000000, /* IRQL 18 */
    0b11111111111111110111111000000000, /* IRQL 19 */

    /*
     * Now we mask off the IRQs on the master. Since IRQL 19 we have 0 "droplet".
     * It's the cascade IRQ that we use to bridge the slave PIC with the master PIC.
     */
    0b11111111111111110111111100000000, /* IRQL 20 */
    0b11111111111111110111111101000000, /* IRQL 21 */
    0b11111111111111110111111101100000, /* IRQL 22 */
    0b11111111111111110111111101110000, /* IRQL 23 */
    0b11111111111111110111111101111000, /* IRQL 24 */
    0b11111111111111110111111101111100, /* IRQL 25 */
    0b11111111111111110111111101111110, /* IRQL 26 */
    0b11111111111111111111111101111110, /* IRQL 27 */

    /*
     * See comments of the PC/AT version.
     */
    0b11111111111111111111111101111111, /* IRQL 28 */
    0b11111111111111111111111101111111, /* IRQL 29 */
    0b11111111111111111111111101111111, /* IRQL 30 */
    0b11111111111111111111111101111111  /* IRQL 31 */
};

/* This table indicates which IRQs, if pending, can preempt a given IRQL level */
ULONG FindHigherIrqlMask[32] =
{
    /*
     * See comments of the PC/AT version.
     */
    0b11111111111111111111111111111110, /* IRQL 0 */
    0b11111111111111111111111111111100, /* IRQL 1 */
    0b11111111111111111111111111111000, /* IRQL 2 */
    0b11111111111111111111111111110000, /* IRQL 3 */
    0b00000111111111111111111111110000, /* IRQL 4 */
    0b00000011111111111111111111110000, /* IRQL 5 */
    0b00000001111111111111111111110000, /* IRQL 6 */
    0b00000000111111111111111111110000, /* IRQL 7 */
    0b00000000011111111111111111110000, /* IRQL 8 */
    0b00000000001111111111111111110000, /* IRQL 9 */
    0b00000000000111111111111111110000, /* IRQL 10 */

    /*
     * Now we start progressivly limiting which slave PIC interrupts have the
     * right to preempt us at each level. The RTC timer used for profiling,
     * so it will always preempt until we reach PROFILE_LEVEL.
     */
    0b00000000000011111111111111110000, /* IRQL 11 */
    0b00000000000011111111111111110000, /* IRQL 12 */
    0b00000000000010111111111111110000, /* IRQL 13 */
    0b00000000000010011111111111110000, /* IRQL 14 */
    0b00000000000010001111111111110000, /* IRQL 15 */
    0b00000000000010000111111111110000, /* IRQL 16 */
    0b00000000000010000011111111110000, /* IRQL 17 */
    0b00000000000010000001111111110000, /* IRQL 18 */
    0b00000000000010000001111111110000, /* IRQL 19 */

    /*
     * Now with IRQs on the master PIC.
     */
    0b00000000000010000000011111110000, /* IRQL 20 */
    0b00000000000010000000001111110000, /* IRQL 21 */
    0b00000000000010000000000111110000, /* IRQL 22 */
    0b00000000000010000000000011110000, /* IRQL 23 */
    0b00000000000010000000000001110000, /* IRQL 24 */
    0b00000000000010000000000000110000, /* IRQL 25 */
    0b00000000000010000000000000010000, /* IRQL 26 */

    /*
     * See comments of the PC/AT version.
     */
    0b00000000000000000000000000010000, /* IRQL 27 */
    0b00000000000000000000000000000000, /* IRQL 28 */
    0b00000000000000000000000000000000, /* IRQL 29 */
    0b00000000000000000000000000000000, /* IRQL 30 */
    0b00000000000000000000000000000000  /* IRQL 31 */
};