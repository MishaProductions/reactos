#pragma once

#define TIPASTE(x,y) x ## y

#define IF_LIST_ITER(n) \
    PLIST_ENTRY TIPASTE(n,Entry); \
    PIP_INTERFACE n;

#define ForEachInterface(n) \
    TIPASTE(n,Entry) = InterfaceListHead.Flink; \
    while (TIPASTE(n,Entry) != &InterfaceListHead) { \
              TI_DbgPrint \
                  (MAX_TRACE,( # n ": %x\n", \
                               TIPASTE(n,Entry))); \
              ASSERT(TIPASTE(n,Entry)); \
	      n = CONTAINING_RECORD(TIPASTE(n,Entry), IP_INTERFACE, \
				    ListEntry); \
	      ASSERT(n);

#define ADDR_LIST_ITER(n) \
    PLIST_ENTRY TIPASTE(n,Entry); \
    PIP_INTERFACE_ADDRESS n;

#define ForEachAddress(x, n) \
    TIPASTE(n,Entry) = x.Flink; \
    while (TIPASTE(n,Entry) != &x) { \
              TI_DbgPrint \
                  (MAX_TRACE,( # n ": %x\n", \
                               TIPASTE(n,Entry))); \
              ASSERT(TIPASTE(n,Entry)); \
	      n = CONTAINING_RECORD(TIPASTE(n,Entry), IP_INTERFACE_ADDRESS, \
				    ListEntry); \
	      ASSERT(n);

#define EndFor(n) \
     TI_DbgPrint(MAX_TRACE,("Next " # n " %x\n",  \
			    TIPASTE(n,Entry->Flink))); \
     TIPASTE(n,Entry) = TIPASTE(n,Entry)->Flink; \
}

extern int IPAddressTableIndex;

inline void InsertAddress(PIP_INTERFACE Interface, IP_ADDRESS Address, UINT Mask, IP_INTERFACE_ADDRESS_TYPE AddressType)
{
    PIP_INTERFACE_ADDRESS Entry = (PIP_INTERFACE_ADDRESS)ExAllocatePool(NonPagedPool, sizeof(IP_INTERFACE_ADDRESS));
    Entry->Address.Type = Address.Type;
    if (Address.Type == IP_ADDRESS_V4)
    {
        Entry->Address.Address.IPv4Address = Address.Address.IPv4Address;
    }
    else
    {
        memcpy(&Entry->Address.Address.IPv6Address, Address.Address.IPv6Address, sizeof(IPv6_RAW_ADDRESS));
    }

    Entry->Type = AddressType;
    Entry->MaskBits = Mask;
    Entry->Index = IPAddressTableIndex++;
    InitializeListHead(&Entry->ListEntry);
    ExInterlockedInsertTailList(&Interface->Addresses, &Entry->ListEntry, &InterfaceListLock);
}

inline PIP_ADDRESS FindAddressByAddressType(PIP_INTERFACE Interface, UCHAR Type)
{
    ADDR_LIST_ITER(Address);
    ForEachAddress(Interface->Addresses, Address) {
        if (Address->Address.Type == Type)
        {
            return &Address->Address;
        }
    } EndFor(Address);

    return NULL;
}
