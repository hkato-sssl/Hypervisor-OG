/*
 * thread/thread_exit.S
 *
 * (C) 2020 Hidekazu Kato
 */
        .global         thread_exit

        .section        ".text.thread_exit", "ax", %progbits
        .type           thread_exit, %function
        .balign         4
thread_exit:
        svc             #0

        .end

