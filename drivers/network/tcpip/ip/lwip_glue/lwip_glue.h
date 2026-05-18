#ifndef _LWIP_GLUE_H_
#define _LWIP_GLUE_H_

#include <lwip/tcp.h>
#include <lwip/pbuf.h>
#include <lwip/ip_addr.h>
#include <tcpip.h>

#ifndef LWIP_TAG
    #define LWIP_TAG         'PIwl'
    #define LWIP_MESSAGE_TAG 'sMwl'
    #define LWIP_QUEUE_TAG   'uQwl'
#endif

typedef struct tcp_pcb* PTCP_PCB;

#define LWIP_IP6_TO_TDI(ip6, tdi)              \
    do {                                       \
        (tdi)->sin6_addr[0] = IP6_ADDR_BLOCK1(ip6); \
        (tdi)->sin6_addr[1] = IP6_ADDR_BLOCK2(ip6); \
        (tdi)->sin6_addr[2] = IP6_ADDR_BLOCK3(ip6); \
        (tdi)->sin6_addr[3] = IP6_ADDR_BLOCK4(ip6); \
        (tdi)->sin6_addr[4] = IP6_ADDR_BLOCK5(ip6); \
        (tdi)->sin6_addr[5] = IP6_ADDR_BLOCK6(ip6); \
        (tdi)->sin6_addr[6] = IP6_ADDR_BLOCK7(ip6); \
        (tdi)->sin6_addr[7] = IP6_ADDR_BLOCK8(ip6); \
    } while (0)

#define NTIPADDRESSTOIPADDR(x, y)                                     \
    do {                                                              \
        if ((x).Type == IP_ADDRESS_V4)                                \
        {                                                             \
            (y).type = IPADDR_TYPE_V4;                                \
            (y).u_addr.ip4.addr = (x).Address.IPv4Address;                   \
        }                                                             \
        else                                                          \
        {                                                             \
            (y).type = IPADDR_TYPE_V6;                                \
            IP6_ADDR(&(y).u_addr.ip6,                                 \
                ((x).Address.IPv6Address[0] << 16) | (x).Address.IPv6Address[1], \
                ((x).Address.IPv6Address[2] << 16) | (x).Address.IPv6Address[3], \
                ((x).Address.IPv6Address[4] << 16) | (x).Address.IPv6Address[5], \
                ((x).Address.IPv6Address[6] << 16) | (x).Address.IPv6Address[7]);\
        }                                                             \
    } while (0)

#define IPADDRTONTIPADDRESS(x, y)                                     \
    do {                                                              \
        if ((x).type == IPADDR_TYPE_V4)                               \
        {                                                             \
            (y).Type = IP_ADDRESS_V4;                                 \
            (y).Address.IPv4Address = (x).u_addr.ip4.addr;                   \
        }                                                             \
        else                                                          \
        {                                                             \
            (y).Type = IP_ADDRESS_V6;                                 \
            (y).Address.IPv6Address[0] = IP6_ADDR_BLOCK1(&(x).u_addr.ip6); \
            (y).Address.IPv6Address[1] = IP6_ADDR_BLOCK2(&(x).u_addr.ip6); \
            (y).Address.IPv6Address[2] = IP6_ADDR_BLOCK3(&(x).u_addr.ip6); \
            (y).Address.IPv6Address[3] = IP6_ADDR_BLOCK4(&(x).u_addr.ip6); \
            (y).Address.IPv6Address[4] = IP6_ADDR_BLOCK5(&(x).u_addr.ip6); \
            (y).Address.IPv6Address[5] = IP6_ADDR_BLOCK6(&(x).u_addr.ip6); \
            (y).Address.IPv6Address[6] = IP6_ADDR_BLOCK7(&(x).u_addr.ip6); \
            (y).Address.IPv6Address[7] = IP6_ADDR_BLOCK8(&(x).u_addr.ip6); \
        }                                                             \
    } while (0)

#define TDIIPADDRESSTOIPADDR(x, y)                                      \
    do {                                                               \
        if ((x)->TAAddressCount > 0)                                    \
        {                                                              \
            if ((x)->Address[0].AddressType == TDI_ADDRESS_TYPE_IP)     \
            {                                                          \
                (y)->type = IPADDR_TYPE_V4;                             \
                (y)->u_addr.ip4.addr =                                  \
                    (x)->Address[0].Address[0].in_addr;                 \
            }                                                          \
            else if ((x)->Address[0].AddressType == TDI_ADDRESS_TYPE_IP6)\
            {                                                          \
                (y).type = IPADDR_TYPE_V6;                             \
                IP6_ADDR(&(y).u_addr.ip6,                              \
                    ((x).Address[0].Address[0].sin6_addr[0] << 16) |   \
                     (x).Address[0].Address[0].sin6_addr[1],           \
                    ((x).Address[0].Address[0].sin6_addr[2] << 16) |   \
                     (x).Address[0].Address[0].sin6_addr[3],           \
                    ((x).Address[0].Address[0].sin6_addr[4] << 16) |   \
                     (x).Address[0].Address[0].sin6_addr[5],           \
                    ((x).Address[0].Address[0].sin6_addr[6] << 16) |   \
                     (x).Address[0].Address[0].sin6_addr[7]);          \
            }                                                          \
        }                                                              \
    } while (0)

#define IPADDRTOTDIIPADDRESS(x, y)                                      \
    do {                                                               \
        (y)->TAAddressCount = 1;                                        \
                                                                         \
        if ((x).type == IPADDR_TYPE_V4)                                \
        {                                                              \
            (y)->Address[0].AddressLength = sizeof(TDI_ADDRESS_IP);     \
            (y)->Address[0].AddressType = TDI_ADDRESS_TYPE_IP;          \
            (y)->Address[0].Address[0].sin_port = 0;                    \
            (y)->Address[0].Address[0].in_addr =                        \
                (x).u_addr.ip4.addr;                                   \
            memset((y)->Address[0].Address[0].sin_zero, 0, 8);          \
        }                                                              \
        else                                                           \
        {                                                              \
            PTA_IP6_ADDRESS AddressIP6 = (PTA_IP6_ADDRESS)y;\
            (AddressIP6)->Address[0].AddressLength = sizeof(TDI_ADDRESS_IP6);    \
            (AddressIP6)->Address[0].AddressType = TDI_ADDRESS_TYPE_IP6;         \
            (AddressIP6)->Address[0].Address[0].sin6_port = 0;                   \
            (AddressIP6)->Address[0].Address[0].sin6_flowinfo = 0;               \
            (AddressIP6)->Address[0].Address[0].sin6_scope_id = 0;               \
                                                                         \
            (AddressIP6)->Address[0].Address[0].sin6_addr[0] =                   \
                IP6_ADDR_BLOCK1(&(x).u_addr.ip6);                      \
            (AddressIP6)->Address[0].Address[0].sin6_addr[1] =                   \
                IP6_ADDR_BLOCK2(&(x).u_addr.ip6);                      \
            (AddressIP6)->Address[0].Address[0].sin6_addr[2] =                   \
                IP6_ADDR_BLOCK3(&(x).u_addr.ip6);                      \
            (AddressIP6)->Address[0].Address[0].sin6_addr[3] =                   \
                IP6_ADDR_BLOCK4(&(x).u_addr.ip6);                      \
            (AddressIP6)->Address[0].Address[0].sin6_addr[4] =                   \
                IP6_ADDR_BLOCK5(&(x).u_addr.ip6);                      \
            (AddressIP6)->Address[0].Address[0].sin6_addr[5] =                   \
                IP6_ADDR_BLOCK6(&(x).u_addr.ip6);                      \
            (AddressIP6)->Address[0].Address[0].sin6_addr[6] =                   \
                IP6_ADDR_BLOCK7(&(x).u_addr.ip6);                      \
            (AddressIP6)->Address[0].Address[0].sin6_addr[7] =                   \
                IP6_ADDR_BLOCK8(&(x).u_addr.ip6);                      \
        }                                                              \
    } while (0)

typedef struct _QUEUE_ENTRY
{
    struct pbuf *p;
    ULONG Offset;
    LIST_ENTRY ListEntry;
} QUEUE_ENTRY, *PQUEUE_ENTRY;

struct lwip_callback_msg
{
    /* Synchronization */
    KEVENT Event;

    /* Input */
    union {
        struct {
            PVOID Arg;
        } Socket;
        struct {
            struct tcp_pcb* pcb;
        } FreeSocket;
        struct {
            PCONNECTION_ENDPOINT Connection;
            ip_addr_t *IpAddress;
            u16_t Port;
        } Bind;
        struct {
            PCONNECTION_ENDPOINT Connection;
            u8_t Backlog;
        } Listen;
        struct {
            PCONNECTION_ENDPOINT Connection;
            void *Data;
            u16_t DataLength;
        } Send;
        struct {
            PCONNECTION_ENDPOINT Connection;
            ip_addr_t *IpAddress;
            u16_t Port;
        } Connect;
        struct {
            PCONNECTION_ENDPOINT Connection;
            int shut_rx;
            int shut_tx;
        } Shutdown;
        struct {
            PCONNECTION_ENDPOINT Connection;
            int Callback;
        } Close;
    } Input;

    /* Output */
    union {
        struct {
            struct tcp_pcb *NewPcb;
        } Socket;
        struct {
            err_t Error;
        } Bind;
        struct {
            struct tcp_pcb *NewPcb;
        } Listen;
        struct {
            err_t Error;
            u32_t Information;
        } Send;
        struct {
            err_t Error;
        } Connect;
        struct {
            err_t Error;
        } Shutdown;
        struct {
            err_t Error;
        } Close;
    } Output;
};

NTSTATUS    LibTCPGetDataFromConnectionQueue(PCONNECTION_ENDPOINT Connection, PUCHAR RecvBuffer, UINT RecvLen, UINT *Received);

/* External TCP event handlers */
extern void TCPConnectEventHandler(void *arg, const err_t err);
extern void TCPAcceptEventHandler(void *arg, PTCP_PCB newpcb);
extern void TCPSendEventHandler(void *arg, const u16_t space);
extern void TCPFinEventHandler(void *arg, const err_t err);
extern void TCPRecvEventHandler(void *arg);

/* TCP functions */
PTCP_PCB    LibTCPSocket(void *arg);
VOID        LibTCPFreeSocket(PTCP_PCB pcb);
err_t       LibTCPBind(PCONNECTION_ENDPOINT Connection, ip_addr_t *const ipaddr, const u16_t port);
PTCP_PCB    LibTCPListen(PCONNECTION_ENDPOINT Connection, const u8_t backlog);
err_t       LibTCPSend(PCONNECTION_ENDPOINT Connection, void *const dataptr, const u16_t len, ULONG *sent, const int safe);
err_t       LibTCPConnect(PCONNECTION_ENDPOINT Connection, ip_addr_t *const ipaddr, const u16_t port);
err_t       LibTCPShutdown(PCONNECTION_ENDPOINT Connection, const int shut_rx, const int shut_tx);
err_t       LibTCPClose(PCONNECTION_ENDPOINT Connection, const int safe, const int callback);

err_t       LibTCPGetPeerName(PTCP_PCB pcb, ip_addr_t *const ipaddr, u16_t *const port);
err_t       LibTCPGetHostName(PTCP_PCB pcb, ip_addr_t *const ipaddr, u16_t *const port);
void        LibTCPAccept(PTCP_PCB pcb, struct tcp_pcb *listen_pcb, void *arg);
void        LibTCPSetNoDelay(PTCP_PCB pcb, BOOLEAN Set);
void        LibTCPGetSocketStatus(PTCP_PCB pcb, PULONG State);

/* IP functions */
void LibIPInsertPacket(void *ifarg, const void *const data, const u32_t size);
void LibIPInitialize(void);
void LibIPShutdown(void);

#endif
