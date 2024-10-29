// SPDX-License-Identifier: Apache-2.0
/*
 * template/exception_vector.S
 *
 * (C) 2019 Hidekazu Kato
 */
        .global         XXXX
        .section        ".text.excvec.XXXX", "ax", %progbits
        .type           XXXX, %function
        .balign         2048
XXXX:
        /*
         * current EL with SP0
         */

        /* Synchronous */
        .org            0x0000
        b               .

        /* IRQ */
        .org            0x0080
        b               .

        /* FIQ */
        .org            0x0100
        b               .

        /* SError */
        .org            0x0180
        b               .

        /*
         * current EL with SPx
         */

        /* Synchronous */
        .org            0x0200
        b               .

        /* IRQ */
        .org            0x0280
        b               .

        /* FIQ */
        .org            0x0300
        b               .

        /* SError */
        .org            0x0380
        b               .

        /*
         * lower EL using AArch64
         */

        /* Synchronous */
        .org            0x0400
        b               .

        /* IRQ */
        .org            0x0480
        b               .

        /* FIQ */
        .org            0x0500
        b               .

        /* SError */
        .org            0x0580
        b               .

        /*
         * lower EL using AArch32
         */

        /* Synchronous */
        .org            0x0600
        b               .

        /* IRQ */
        .org            0x0680
        b               .

        /* FIQ */
        .org            0x0700
        b               .

        /* SError */
        .org            0x0780
        b               .

        .end
 
