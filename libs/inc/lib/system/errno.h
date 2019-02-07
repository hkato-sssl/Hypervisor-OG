/*
 * lib/system/errno.h
 *
 * (C) 2019 Hidekazu Kato
 */

#ifndef LIB_SYSTEM_ERRNO_H
#define LIB_SYSTEM_ERRNO_H

/* includes */

/* defines */

#define SUCCESS         0

#define E2BIG           10      /* Argument list too long. */
#define EACCES          11      /* Permission denied. */
#define EADDRINUSE      12      /* Address in use. */
#define EADDRNOTAVAIL   13      /* Address not available. */
#define EAFNOSUPPORT    14      /* Address family not supported. */
#define EAGAIN          15      /* Resource unavailable, try again (may be the same value as [EWOULDBLOCK]). */
#define EALREADY        16      /* Connection already in progress. */
#define EBADF           17      /* Bad file descriptor. */
#define EBADMSG         18      /* Bad message. */
#define EBUSY           19      /* Device or resource busy. */
#define ECANCELED       20      /* Operation canceled. */
#define ECHILD          21      /* No child processes. */
#define ECONNABORTED    22      /* Connection aborted. */
#define ECONNREFUSED    23      /* Connection refused. */
#define ECONNRESET      24      /* Connection reset. */
#define EDEADLK         25      /* Resource deadlock would occur. */
#define EDESTADDRREQ    26      /* Destination address required. */
#define EDOM            27      /* Mathematics argument out of domain of function. */
#define EDQUOT          28      /* Reserved. */
#define EEXIST          29      /* File exists. */
#define EFAULT          30      /* Bad address. */
#define EFBIG           31      /* File too large. */
#define EHOSTUNREACH    32      /* Host is unreachable. */
#define EIDRM           33      /* Identifier removed. */
#define EILSEQ          34      /* Illegal byte sequence. */
#define EINPROGRESS     35      /* Operation in progress. */
#define EINTR           36      /* Interrupted function. */
#define EINVAL          37      /* Invalid argument. */
#define EIO             38      /* I/O error. */
#define EISCONN         39      /* Socket is connected. */
#define EISDIR          40      /* Is a directory. */
#define ELOOP           41      /* Too many levels of symbolic links. */
#define EMFILE          42      /* File descriptor value too large. */
#define EMLINK          43      /* Too many links. */
#define EMSGSIZE        44      /* Message too large. */
#define EMULTIHOP       45      /* Reserved. */
#define ENAMETOOLONG    46      /* Filename too long. */
#define ENETDOWN        47      /* Network is down. */
#define ENETRESET       48      /* Connection aborted by network. */
#define ENETUNREACH     49      /* Network unreachable. */
#define ENFILE          50      /* Too many files open in system. */
#define ENOBUFS         51      /* No buffer space available. */
#define ENODATA         52      /* No message is available on the STREAM head read queue. */
#define ENODEV          53      /* No such device. */
#define ENOENT          54      /* No such file or directory. */
#define ENOEXEC         55      /* Executable file format error. */
#define ENOLCK          56      /* No locks available. */
#define ENOLINK         57      /* Reserved. */
#define ENOMEM          58      /* Not enough space. */
#define ENOMSG          59      /* No message of the desired type. */
#define ENOPROTOOPT     60      /* Protocol not available. */
#define ENOSPC          61      /* No space left on device. */
#define ENOSR           62      /* No STREAM resources. */
#define ENOSTR          63      /* Not a STREAM. */
#define ENOSYS          64      /* Functionality not supported. */
#define ENOTCONN        65      /* The socket is not connected. */
#define ENOTDIR         66      /* Not a directory or a symbolic link to a directory. */
#define ENOTEMPTY       67      /* Directory not empty. */
#define ENOTRECOVERABLE 68      /* State not recoverable. */
#define ENOTSOCK        69      /* Not a socket. */
#define ENOTSUP         70      /* Not supported (may be the same value as [EOPNOTSUPP]). */
#define ENOTTY          71      /* Inappropriate I/O control operation. */
#define ENXIO           72      /* No such device or address. */
#define EOPNOTSUPP      ENOTSUP /* Operation not supported on socket (may be the same value as [ENOTSUP]). */
#define EOVERFLOW       73      /* Value too large to be stored in data type. */
#define EOWNERDEAD      74      /* Previous owner died. */
#define EPERM           75      /* Operation not permitted. */
#define EPIPE           76      /* Broken pipe. */
#define EPROTO          77      /* Protocol error. */
#define EPROTONOSUPPORT 78      /* Protocol not supported. */
#define EPROTOTYPE      79      /* Protocol wrong type for socket. */
#define ERANGE          80      /* Result too large. */
#define EROFS           81      /* Read-only file system. */
#define ESPIPE          82      /* Invalid seek. */
#define ESRCH           83      /* No such process. */
#define ESTALE          84      /* Reserved. */
#define ETIME           85      /* Stream ioctl() timeout. */
#define ETIMEDOUT       86      /* Connection timed out. */
#define ETXTBSY         87      /* Text file busy. */
#define EWOULDBLOCK     EAGAIN  /* Operation would block (may be the same value as [EAGAIN]). */
#define EXDEV           88      /* Cross-device link. */

#ifndef ASSEMBLY

#ifdef __cplusplus
extern "C" {
#endif

/* includes */

#include <stdint.h>

/* defines */

/* types */

typedef int32_t errno_t;

/* variables */

/* functions */

#ifdef __cplusplus
}
#endif

#endif /* ASSEMBLY */

#endif /* LIB_SYSTEM_ERRNO_H */

