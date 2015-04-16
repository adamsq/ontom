#ifndef PGN_1_
#define PGN_1_

typedef enum {
    PGN_CRM = 0x000100,
    PGN_BRM = 0x000200,
    PGN_BCP = 0x000600,
    PGN_CTS = 0x000700,
    PGN_CML = 0x000800,
    PGN_BRO = 0x000900,
    PGN_CRO = 0x000A00,
    PGN_BCL = 0x001000,
    PGN_BCS = 0x001100,
    PGN_CCS = 0x001200,
    PGN_BSM = 0x001300,
    PGN_BMV = 0x001500,
    PGN_BMT = 0x001600,
    PGN_BSP = 0x001700,
    PGN_BST = 0x001900,
    PGN_CST = 0x001A00,
    PGN_BSD = 0x001C00,
    PGN_CSD = 0x001D00,
    PGN_BEM = 0x001E00,
    PGN_CEM = 0x001F00
}CAN_PGN;

typedef enum {
    I_CRM = 0,
    I_BRM = 1,
    I_BCP = 2,
    I_CTS = 3,
    I_CML = 4,
    I_BRO = 5,
    I_CRO = 6,
    I_BCL = 7,
    I_BCS = 8,
    I_CCS = 9,
    I_BSM = 10,
    I_BMV = 11,
    I_BMT = 12,
    I_BSP = 13,
    I_BST = 14,
    I_CST = 15,
    I_BSD = 16,
    I_CSD = 17,
    I_BEM = 18,
    I_CEM = 19
}CAN_PGN_STATISTICS;

#endif
