/*
 * system/system_alert.S
 *
 * (C) 2019 Hidekazu Kato
 */
        .global         system_alert

        .section        ".text.system_alert", "ax", %progbits
        .type           system_alert, %function
        .balign         4
        /*
         * void system_alert(const char *func, uint32_t line, const char *message)
         */
system_alert:
        svc             #0xfffe
        ret

        .end

