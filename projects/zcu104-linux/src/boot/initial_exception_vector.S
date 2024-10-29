// SPDX-License-Identifier: Apache-2.0
/*
 * boot/initial_exception_vector.S
 *
 * (C) 2019 Hidekazu Kato
 */
        .global         initial_exception_vector
        .section        ".text.excvec", "ax", %progbits
        .type           initial_exception_vector, %function
        .balign         2048
initial_exception_vector:
        // current EL with SP0

        /*
         * Synchronous
         */
        .org            0x0000
        b               .

        /*
         * IRQ
         */
        .org            0x0080
        b               .

        /*
         * FIQ
         */
        .org            0x0100
        b               .

        /*
         * SError
         */
        .org            0x0180
        b               .

        //
        // current EL with SPx
        //
        .org            0x0200
        b               .

        .org            0x0280
        b               .

        .org            0x0300
        b               .

        .org            0x0380
        b               .

        // lower EL using AArch64

        .org            0x0400
        b               .

        .org            0x0480
        b               .

        .org            0x0500
        b               .

        .org            0x0580
        b               .

        // lower EL using AArch32

        .org            0x0600
        b               .

        .org            0x0680
        b               .

        .org            0x0700
        b               .

        .org            0x0780
        b               .

        .end
        
