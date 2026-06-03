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

static inline void NtIpAddressToLWIPIpAddress(IP_ADDRESS* x, ip_addr_t* y)
{
    if (x->Type == IP_ADDRESS_V4)
    {                                                             
        y->type = IPADDR_TYPE_V4;
        y->u_addr.ip4.addr = x->Address.IPv4Address;
    }                                                             
    else                                                          
    {                                                             
        y->type = IPADDR_TYPE_V6;
        memcpy(y->u_addr.ip6.addr, x->Address.IPv6Address, sizeof(x->Address.IPv6Address));
    }
}

static inline void LWIPIpAddressToNTIpAddress(ip_addr_t* x, PIP_ADDRESS y)
{
    if (x->type == IPADDR_TYPE_V4)
    {
        y->Type = IP_ADDRESS_V4;
        y->Address.IPv4Address = x->u_addr.ip4.addr;
    }
    else if (x->type == IPADDR_TYPE_V6)
    {
        y->Type = IP_ADDRESS_V6;
         memcpy(y->Address.IPv6Address, x->u_addr.ip6.addr, sizeof(x->u_addr.ip6.addr));
    }
}

static inline void LWIPIpAddressToTDIIpAddress(ip_addr_t* x, PTA_IP_ADDRESS y)
{
    y->TAAddressCount = 1;

    if (x->type == IPADDR_TYPE_V6)
    {
        y->Address[0].AddressLength = TDI_ADDRESS_LENGTH_IP;
        y->Address[0].AddressType = TDI_ADDRESS_TYPE_IP;
        y->Address[0].Address->in_addr = x->u_addr.ip4.addr;
        RtlZeroMemory(&y->Address[0].Address[0].sin_zero,
                  sizeof(y->Address[0].Address[0].sin_zero));
    }
    else if (x->type == IPADDR_TYPE_V6)
    {
        PTA_IP6_ADDRESS AddressIP6 = (PTA_IP6_ADDRESS)y;
        AddressIP6->Address[0].AddressLength = TDI_ADDRESS_LENGTH_IP;
        AddressIP6->Address[0].AddressType = TDI_ADDRESS_TYPE_IP;
        memcpy(&AddressIP6->Address[0].Address->sin6_addr, x->u_addr.ip6.addr, sizeof(x->u_addr.ip6.addr));
    }
}

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
