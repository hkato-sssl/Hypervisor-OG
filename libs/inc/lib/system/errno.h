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

#if defined(ASSEMBLY) || (! defined(HYPERVISOR))

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

#endif

#ifndef ASSEMBLY

#ifdef __cplusplus
extern "C" {
#endif

/* includes */

#include <stdint.h>

/* defines */

#ifdef HYPERVISOR

#define E2BIG           system_errno(10, __FILE__, __LINE__)
#define EACCES          system_errno(11, __FILE__, __LINE__)
#define EADDRINUSE      system_errno(12, __FILE__, __LINE__)
#define EADDRNOTAVAIL   system_errno(13, __FILE__, __LINE__)
#define EAFNOSUPPORT    system_errno(14, __FILE__, __LINE__)
#define EAGAIN          system_errno(15, __FILE__, __LINE__)
#define EALREADY        system_errno(16, __FILE__, __LINE__)
#define EBADF           system_errno(17, __FILE__, __LINE__)
#define EBADMSG         system_errno(18, __FILE__, __LINE__)
#define EBUSY           system_errno(19, __FILE__, __LINE__)
#define ECANCELED       system_errno(20, __FILE__, __LINE__)
#define ECHILD          system_errno(21, __FILE__, __LINE__)
#define ECONNABORTED    system_errno(22, __FILE__, __LINE__)
#define ECONNREFUSED    system_errno(23, __FILE__, __LINE__)
#define ECONNRESET      system_errno(24, __FILE__, __LINE__)
#define EDEADLK         system_errno(25, __FILE__, __LINE__)
#define EDESTADDRREQ    system_errno(26, __FILE__, __LINE__)
#define EDOM            system_errno(27, __FILE__, __LINE__)
#define EDQUOT          system_errno(28, __FILE__, __LINE__)
#define EEXIST          system_errno(29, __FILE__, __LINE__)
#define EFAULT          system_errno(30, __FILE__, __LINE__)
#define EFBIG           system_errno(31, __FILE__, __LINE__)
#define EHOSTUNREACH    system_errno(32, __FILE__, __LINE__)
#define EIDRM           system_errno(33, __FILE__, __LINE__)
#define EILSEQ          system_errno(34, __FILE__, __LINE__)
#define EINPROGRESS     system_errno(35, __FILE__, __LINE__)
#define EINTR           system_errno(36, __FILE__, __LINE__)
#define EINVAL          system_errno(37, __FILE__, __LINE__)
#define EIO             system_errno(38, __FILE__, __LINE__)
#define EISCONN         system_errno(39, __FILE__, __LINE__)
#define EISDIR          system_errno(40, __FILE__, __LINE__)
#define ELOOP           system_errno(41, __FILE__, __LINE__)
#define EMFILE          system_errno(42, __FILE__, __LINE__)
#define EMLINK          system_errno(43, __FILE__, __LINE__)
#define EMSGSIZE        system_errno(44, __FILE__, __LINE__)
#define EMULTIHOP       system_errno(45, __FILE__, __LINE__)
#define ENAMETOOLONG    system_errno(46, __FILE__, __LINE__)
#define ENETDOWN        system_errno(47, __FILE__, __LINE__)
#define ENETRESET       system_errno(48, __FILE__, __LINE__)
#define ENETUNREACH     system_errno(49, __FILE__, __LINE__)
#define ENFILE          system_errno(50, __FILE__, __LINE__)
#define ENOBUFS         system_errno(51, __FILE__, __LINE__)
#define ENODATA         system_errno(52, __FILE__, __LINE__)
#define ENODEV          system_errno(53, __FILE__, __LINE__)
#define ENOENT          system_errno(54, __FILE__, __LINE__)
#define ENOEXEC         system_errno(55, __FILE__, __LINE__)
#define ENOLCK          system_errno(56, __FILE__, __LINE__)
#define ENOLINK         system_errno(57, __FILE__, __LINE__)
#define ENOMEM          system_errno(58, __FILE__, __LINE__)
#define ENOMSG          system_errno(59, __FILE__, __LINE__)
#define ENOPROTOOPT     system_errno(60, __FILE__, __LINE__)
#define ENOSPC          system_errno(61, __FILE__, __LINE__)
#define ENOSR           system_errno(62, __FILE__, __LINE__)
#define ENOSTR          system_errno(63, __FILE__, __LINE__)
#define ENOSYS          system_errno(64, __FILE__, __LINE__)
#define ENOTCONN        system_errno(65, __FILE__, __LINE__)
#define ENOTDIR         system_errno(66, __FILE__, __LINE__)
#define ENOTEMPTY       system_errno(67, __FILE__, __LINE__)
#define ENOTRECOVERABLE system_errno(68, __FILE__, __LINE__)
#define ENOTSOCK        system_errno(69, __FILE__, __LINE__)
#define ENOTSUP         system_errno(70, __FILE__, __LINE__)
#define ENOTTY          system_errno(71, __FILE__, __LINE__)
#define ENXIO           system_errno(72, __FILE__, __LINE__)
#define EOPNOTSUPP      system_errno(70, __FILE__, __LINE__)
#define EOVERFLOW       system_errno(73, __FILE__, __LINE__)
#define EOWNERDEAD      system_errno(74, __FILE__, __LINE__)
#define EPERM           system_errno(75, __FILE__, __LINE__)
#define EPIPE           system_errno(76, __FILE__, __LINE__)
#define EPROTO          system_errno(77, __FILE__, __LINE__)
#define EPROTONOSUPPORT system_errno(78, __FILE__, __LINE__)
#define EPROTOTYPE      system_errno(79, __FILE__, __LINE__)
#define ERANGE          system_errno(80, __FILE__, __LINE__)
#define EROFS           system_errno(81, __FILE__, __LINE__)
#define ESPIPE          system_errno(82, __FILE__, __LINE__)
#define ESRCH           system_errno(83, __FILE__, __LINE__)
#define ESTALE          system_errno(84, __FILE__, __LINE__)
#define ETIME           system_errno(85, __FILE__, __LINE__)
#define ETIMEDOUT       system_errno(86, __FILE__, __LINE__)
#define ETXTBSY         system_errno(87, __FILE__, __LINE__)
#define EWOULDBLOCK     system_errno(15, __FILE__, __LINE__)
#define EXDEV           system_errno(88, __FILE__, __LINE__)

#endif

/* types */

typedef int32_t errno_t;

/* variables */

/* functions */

errno_t system_errno(errno_t code, const char *file, unsigned long line);

#ifdef __cplusplus
}
#endif

#endif /* ASSEMBLY */

#endif /* LIB_SYSTEM_ERRNO_H */

