/*
 * KNSoft.SMBIOS (https://github.com/KNSoft/KNSoft.SMBIOS)
 *
 * Definitions for SMBIOS Reference Specification (DMTF DSP0134, https://www.dmtf.org/standards/smbios).
 * See the repository for more information with usage and sample.
 * Updated to SMBIOS 3.8.0.
 *
 * Licensed under the MIT license.
 * Copyright (c) KNSoft.org (https://github.com/KNSoft). All rights reserved.
 */

#pragma once

#pragma region Preparations

#ifndef SMBIOS_VERSION
#define SMBIOS_VERSION 0x03080000 // 0x(Major)(Minor)(Revision)(WIP)
#endif
static_assert(SMBIOS_VERSION >= 0x02000000);

typedef unsigned char       BYTE;
typedef unsigned short      WORD;
typedef unsigned long       DWORD;
typedef unsigned __int64    QWORD;

typedef BYTE DMI_STRING;

#ifdef _MSC_VER

#include <sal.h>
#pragma warning(disable: 4200) // MS-Spec: nonstandard extension used: zero-sized array in struct/union

#else

#pragma push_macro("_Field_size_")
#pragma push_macro("_Field_size_bytes_")
#ifndef _Field_size_
#define _Field_size_(x)
#endif
#ifndef _Field_size_bytes_
#define _Field_size_bytes_(x)
#endif

#endif

#pragma endregion Basic types, SAL and Version Control

#pragma region SMBIOS Specification

#pragma pack(push, 1)

/* aka RawSMBIOSData: https://learn.microsoft.com/en-us/windows/win32/api/sysinfoapi/nf-sysinfoapi-getsystemfirmwaretable */
typedef struct _SMBIOS_RAW_DATA
{
    BYTE Used20CallingMethod;
    BYTE SMBIOSMajorVersion;
    BYTE SMBIOSMinorVersion;
    BYTE DmiRevision;
    DWORD Length;
    _Field_size_bytes_(Length) BYTE SMBIOSTableData[]; // SMBIOS_TABLE_HEADER
} SMBIOS_RAW_DATA, *PSMBIOS_RAW_DATA;

typedef struct _SMBIOS_TABLE_HEADER
{
    BYTE Type;      // SMBIOS_INFORMATION_TYPE_*
    BYTE Length;
    WORD Handle;
} SMBIOS_TABLE_HEADER, *PSMBIOS_TABLE_HEADER;
static_assert(sizeof(SMBIOS_TABLE_HEADER) == 4);

#pragma region Platform Firmware Information (Type 0)

#define SMBIOS_PLATFORM_FIRMWARE_INFORMATION_TYPE ((BYTE)0)

#define SMBIOS_PLATFORM_FIRMWARE_EXTENDEDROMSIZE_UNIT_MB ((WORD)00b) // Unit is MB
#define SMBIOS_PLATFORM_FIRMWARE_EXTENDEDROMSIZE_UNIT_GB ((WORD)01b) // Unit is GB

typedef struct _SMBIOS_PLATFORM_FIRMWARE_INFORMATION
{
    DMI_STRING Vendor;
    DMI_STRING Version;
    WORD BIOSStartingAddressSegment;
    DMI_STRING ReleaseDate;
    BYTE ROMSize;
    union
    {
        QWORD Characteristics;
        struct
        {
            QWORD Reserved0 : 1;                    // 00 Reserved
            QWORD Reserved1 : 1;                    // 01 Reserved
            QWORD Unknown0 : 1;                     // 02 Unknown
            QWORD CharacteristicsNotSupported : 1;  // 03 Firmware Characteristics are not supported
            QWORD ISA : 1;                          // 04 ISA is supported
            QWORD MCA : 1;                          // 05 MCA is supported
            QWORD EISA : 1;                         // 06 EISA is supported
            QWORD PCI : 1;                          // 07 PCI is supported
            QWORD PCCard : 1;                       // 08 PC card (PCMCIA) is supported
            QWORD PNP : 1;                          // 09 Plug and Play is supported
            QWORD APM : 1;                          // 10 APM is supported
            QWORD Upgradeable : 1;                  // 11 Firmware is upgradeable (Flash)
            QWORD ShadowingIsAllowed : 1;           // 12 Firmware shadowing is allowed
            QWORD VL_VESA : 1;                      // 13 VL-VESA is supported
            QWORD ESCD : 1;                         // 14 ESCD support is available
            QWORD BootFromCD : 1;                   // 15 Boot from CD is supported
            QWORD SelectableBoot : 1;               // 16 Selectable boot is supported
            QWORD ROMIsSocketed : 1;                // 17 Firmware ROM is socketed (e.g., PLCC or SOP socket)
            QWORD BootFromPCCard : 1;               // 18 Boot from PC card (PCMCIA) is supported
            QWORD EDDSpecification : 1;             // 19 EDD specification is supported
            QWORD FloppyForNEC9800_1Dot2MB : 1;     // 20 Int 13h — Japanese floppy for NEC 9800 1.2 MB (3.5”, 1K bytes/sector, 360 RPM) is supported
            QWORD FloppyForToshiba_1Dot2MB : 1;     // 21 Int 13h — Japanese floppy for Toshiba 1.2 MB (3.5”, 360 RPM) is supported
            QWORD Floppy_5Dot25Inch_360KB : 1;      // 22 Int 13h — 5.25” / 360 KB floppy services are supported
            QWORD Floppy_5Dot25Inch_1Dot2MB : 1;    // 23 Int 13h — 5.25” / 1.2 MB floppy services are supported
            QWORD Floppy_3Dot5Inch_720KB : 1;       // 24 Int 13h — 3.5” / 720 KB floppy services are supported
            QWORD Floppy_3Dot5Inch_2Dot88MB : 1;    // 25 Int 13h — 3.5” / 2.88 MB floppy services are supported
            QWORD PrintScreenService : 1;           // 26 Int 5h, print screen service is supported
            QWORD _8042KeyboardServices : 1;        // 27 Int 9h, 8042 keyboard services are supported
            QWORD SerialServices : 1;               // 28 Int 14h, serial services are supported
            QWORD PrinterServices : 1;              // 29 Int 17h, printer services are supported
            QWORD CGA_Mono_VideoServices : 1;       // 30 Int 10h, CGA/Mono Video Services are supported
            QWORD NEC_PC98 : 1;                     // 31 NEC PC-98
            QWORD ReservedForBIOSVendor : 16;       // 32:47 Reserved for platform firmware vendor
            QWORD ReservedForSystemVendor : 16;     // 48:63 Reserved for system vendor
        };
    };
#if SMBIOS_VERSION >= 0x02010000
    union
    {
        BYTE CharacteristicsExtensionByte1;
        struct
        {
            BYTE ACPI : 1;                  // 00 ACPI is supported
            BYTE USBLegacy : 1;             // 01 USB Legacy is supported
            BYTE AGP : 1;                   // 02 AGP is supported
            BYTE I2OBoot : 1;               // 03 I2O boot is supported
            BYTE LS120_SuperDiskBoot : 1;   // 04 LS-120 SuperDisk boot is supported
            BYTE ATAPI_ZIP_DriveBoot : 1;   // 05 ATAPI ZIP drive boot is supported
            BYTE _1394Boot : 1;             // 06 1394 boot is supported
            BYTE SmartBattery : 1;          // 07 Smart battery is supported
        };
    };
#if SMBIOS_VERSION >= 0x02030000
    union
    {
        BYTE CharacteristicsExtensionByte2;
        struct
        {
            BYTE BIOSBootSpecification : 1;                     // 00 BIOS Boot Specification is supported
            BYTE FunctionKeyInitiatedNetworkServiceBoot : 1;    // 01 Function key-initiated network service boot is supported
            BYTE EnableTargetedContentDistribution : 1;         // 02 Enable targeted content distribution
            BYTE UEFISpecification : 1;                         // 03 UEFI Specification is supported
            BYTE VirtualMachine : 1;                            // 04 SMBIOS table describes a virtual machine
            BYTE ManufacturingMode : 1;                         // 05 Manufacturing mode is supported
            BYTE ManufacturingModeEnabled : 1;                  // 06 Manufacturing mode is enabled
            BYTE Reserved : 1;                                  // 07 Reserved
        };
    };
#if SMBIOS_VERSION >= 0x02040000
    BYTE MajorRelease;
    BYTE MinorRelease;
    BYTE ECFirmwareMajorRelease;
    BYTE ECFirmwareMinorRelease;
#if SMBIOS_VERSION >= 0x03010000
    union
    {
        WORD ExtendedROMSize;
        struct
        {
            WORD Size : 14; // 00:13 Size
            WORD Unit : 2;  // 14:15 Unit, SMBIOS_PLATFORM_FIRMWARE_EXTENDEDROMSIZE_UNIT_*
        };
    };
#endif // SMBIOS_VERSION >= 0x03010000
#endif // SMBIOS_VERSION >= 0x02040000
#endif // SMBIOS_VERSION >= 0x02030000
#endif // SMBIOS_VERSION >= 0x02010000
} SMBIOS_PLATFORM_FIRMWARE_INFORMATION, *PSMBIOS_PLATFORM_FIRMWARE_INFORMATION;

#pragma endregion

#pragma region System Information (Type 1)

#define SMBIOS_SYSTEM_INFORMATION_TYPE ((BYTE)1)

#define SMBIOS_SYSTEM_WAKEUPTYPE_RESERVED           ((BYTE)00h) // Reserved
#define SMBIOS_SYSTEM_WAKEUPTYPE_OTHER              ((BYTE)01h) // Other
#define SMBIOS_SYSTEM_WAKEUPTYPE_UNKNOW             ((BYTE)02h) // Unknown
#define SMBIOS_SYSTEM_WAKEUPTYPE_APM_TIMER          ((BYTE)03h) // APM Timer
#define SMBIOS_SYSTEM_WAKEUPTYPE_MODEM_RING         ((BYTE)04h) // Modem Ring
#define SMBIOS_SYSTEM_WAKEUPTYPE_LAN_REMOTE         ((BYTE)05h) // LAN Remote
#define SMBIOS_SYSTEM_WAKEUPTYPE_POWER_SWITCH       ((BYTE)06h) // Power Switch
#define SMBIOS_SYSTEM_WAKEUPTYPE_PCI_PME            ((BYTE)07h) // PCI PME#
#define SMBIOS_SYSTEM_WAKEUPTYPE_AC_POWER_RESTORED  ((BYTE)08h) // AC Power Restored

typedef struct _SMBIOS_SYSTEM_INFORMATION
{
    DMI_STRING Manufacturer;
    DMI_STRING ProductName;
    DMI_STRING Version;
    DMI_STRING SerialNumber;
#if SMBIOS_VERSION >= 0x02010000
    BYTE UUID[16];
    BYTE WakeUpType; // SMBIOS_SYSTEM_WAKEUPTYPE_*
#if SMBIOS_VERSION >= 0x02040000
    DMI_STRING SKUNumber;
    DMI_STRING Famliy;
#endif // SMBIOS_VERSION >= 0x02040000
#endif // SMBIOS_VERSION >= 0x02010000
} SMBIOS_SYSTEM_INFORMATION, *PSMBIOS_SYSTEM_INFORMATION;

#pragma endregion

#pragma region Baseboard Information (Type 2)

#define SMBIOS_BASEBOARD_INFORMATION_TYPE ((BYTE)2)

#define SMBIOS_BASEBOARD_TYPE_UNKNOWN                   ((BYTE)01h) // Unknown
#define SMBIOS_BASEBOARD_TYPE_OTHER                     ((BYTE)02h) // Other
#define SMBIOS_BASEBOARD_TYPE_SERVER_BLADE              ((BYTE)03h) // Server Blade
#define SMBIOS_BASEBOARD_TYPE_CONNECTIVITY_SWITCH       ((BYTE)04h) // Connectivity Switch
#define SMBIOS_BASEBOARD_TYPE_SYSTEM_MANAGEMENT_MODULE  ((BYTE)05h) // System Management Module
#define SMBIOS_BASEBOARD_TYPE_PROCESSOR_MODULE          ((BYTE)06h) // Processor Module
#define SMBIOS_BASEBOARD_TYPE_IO_MODULE                 ((BYTE)07h) // I/O Module
#define SMBIOS_BASEBOARD_TYPE_MEMORY_MODULE             ((BYTE)08h) // Memory Module
#define SMBIOS_BASEBOARD_TYPE_DAUGHTER_BOARD            ((BYTE)09h) // Daughter board
#define SMBIOS_BASEBOARD_TYPE_MOTHERBOARD               ((BYTE)0Ah) // Motherboard (includes processor, memory, and I/O)
#define SMBIOS_BASEBOARD_TYPE_PROCESSOR_MEMORY_MODULE   ((BYTE)0Bh) // Processor/Memory Module
#define SMBIOS_BASEBOARD_TYPE_PROCESSOR_IO_MODULE       ((BYTE)0Ch) // Processor/IO Module
#define SMBIOS_BASEBOARD_TYPE_INTERCONNECT_BOARD        ((BYTE)0Dh) // Interconnect board

typedef struct _SMBIOS_BASEBOARD_INFORMATION
{
    DMI_STRING Manufacturer;
    DMI_STRING Product;
    DMI_STRING Version;
    DMI_STRING SerialNumber;
    DMI_STRING AssetTag;
    union
    {
        BYTE FeatureFlags;
        struct
        {
            BYTE HostingBoard : 1;      // 00 The board is a hosting board (for example, a motherboard)
            BYTE RequiresAuxiliary : 1; // 01 The board requires at least one daughter board or auxiliary card to function properly
            BYTE Removable : 1;         // 02 The board is removable
            BYTE Replaceable : 1;       // 03 The board is replaceable
            BYTE HotSwappable : 1;      // 04 The board is s hot swappable
            BYTE Reserved : 3;          // 05:07 Reserved for future definition by this specification
        };
    };
    DMI_STRING LocationInChassis;
    WORD ChassisHandle;
    BYTE BoardType; // SMBIOS_BASEBOARD_TYPE_*
    BYTE NumberOfContainedObjectHandles;
    _Field_size_(NumberOfContainedObjectHandles) WORD ContainedObjectHandles[];
} SMBIOS_BASEBOARD_INFORMATION, *PSMBIOS_BASEBOARD_INFORMATION;

#pragma endregion

#pragma region System Enclosure or Chassis (Type 3)

#define SMBIOS_SYSTEM_ENCLOSURE_OR_CHASSIS_TYPE ((BYTE)3)

#define SMBIOS_SYSTEM_ENCLOSURE_OR_CHASSIS_TYPE_OTHER                   ((BYTE)01h) // Other
#define SMBIOS_SYSTEM_ENCLOSURE_OR_CHASSIS_TYPE_UNKNOWN                 ((BYTE)02h) // Unknown
#define SMBIOS_SYSTEM_ENCLOSURE_OR_CHASSIS_TYPE_DESKTOP                 ((BYTE)03h) // Desktop
#define SMBIOS_SYSTEM_ENCLOSURE_OR_CHASSIS_TYPE_LOW_PROFILE_DESKTOP     ((BYTE)04h) // Low Profile Desktop
#define SMBIOS_SYSTEM_ENCLOSURE_OR_CHASSIS_TYPE_PIZZA_BOX               ((BYTE)05h) // Pizza Box
#define SMBIOS_SYSTEM_ENCLOSURE_OR_CHASSIS_TYPE_MINI_TOWER              ((BYTE)06h) // Mini Tower
#define SMBIOS_SYSTEM_ENCLOSURE_OR_CHASSIS_TYPE_TOWER                   ((BYTE)07h) // Tower
#define SMBIOS_SYSTEM_ENCLOSURE_OR_CHASSIS_TYPE_PORTABLE                ((BYTE)08h) // Portable
#define SMBIOS_SYSTEM_ENCLOSURE_OR_CHASSIS_TYPE_LAPTOP                  ((BYTE)09h) // Laptop
#define SMBIOS_SYSTEM_ENCLOSURE_OR_CHASSIS_TYPE_NOTEBOOK                ((BYTE)0Ah) // Notebook
#define SMBIOS_SYSTEM_ENCLOSURE_OR_CHASSIS_TYPE_HAND_HELD               ((BYTE)0Bh) // Hand Held
#define SMBIOS_SYSTEM_ENCLOSURE_OR_CHASSIS_TYPE_DOCKING_STATION         ((BYTE)0Ch) // Docking Station
#define SMBIOS_SYSTEM_ENCLOSURE_OR_CHASSIS_TYPE_ALL_IN_ONE              ((BYTE)0Dh) // All in One
#define SMBIOS_SYSTEM_ENCLOSURE_OR_CHASSIS_TYPE_SUB_NOTEBOOK            ((BYTE)0Eh) // Sub Notebook
#define SMBIOS_SYSTEM_ENCLOSURE_OR_CHASSIS_TYPE_SPACE_SAVING            ((BYTE)0Fh) // Space-saving
#define SMBIOS_SYSTEM_ENCLOSURE_OR_CHASSIS_TYPE_LUNCH_BOX               ((BYTE)10h) // Lunch Box
#define SMBIOS_SYSTEM_ENCLOSURE_OR_CHASSIS_TYPE_MAIN_SERVER_CHASSIS     ((BYTE)11h) // Main Server Chassis
#define SMBIOS_SYSTEM_ENCLOSURE_OR_CHASSIS_TYPE_EXPANSION_CHASSIS       ((BYTE)12h) // Expansion Chassis
#define SMBIOS_SYSTEM_ENCLOSURE_OR_CHASSIS_TYPE_SUB_CHASSIS             ((BYTE)13h) // SubChassis
#define SMBIOS_SYSTEM_ENCLOSURE_OR_CHASSIS_TYPE_BUS_EXPANSION_CHASSIS   ((BYTE)14h) // Bus Expansion Chassis
#define SMBIOS_SYSTEM_ENCLOSURE_OR_CHASSIS_TYPE_PERIPHERAL_CHASSIS      ((BYTE)15h) // Peripheral Chassis
#define SMBIOS_SYSTEM_ENCLOSURE_OR_CHASSIS_TYPE_RAID_CHASSIS            ((BYTE)16h) // RAID Chassis
#define SMBIOS_SYSTEM_ENCLOSURE_OR_CHASSIS_TYPE_RACK_MOUNT_CHASSIS      ((BYTE)17h) // Rack Mount Chassis
#define SMBIOS_SYSTEM_ENCLOSURE_OR_CHASSIS_TYPE_SEALED_CASE_PC          ((BYTE)18h) // Sealed-case PC
#define SMBIOS_SYSTEM_ENCLOSURE_OR_CHASSIS_TYPE_MULTI_SYSTEM_CHASSIS    ((BYTE)19h) // Multi-system chassis
#define SMBIOS_SYSTEM_ENCLOSURE_OR_CHASSIS_TYPE_COMPACT_PCI             ((BYTE)1Ah) // Compact PCI
#define SMBIOS_SYSTEM_ENCLOSURE_OR_CHASSIS_TYPE_ADVANCED_TCA            ((BYTE)1Bh) // Advanced TCA
#define SMBIOS_SYSTEM_ENCLOSURE_OR_CHASSIS_TYPE_BLADE                   ((BYTE)1Ch) // Blade
#define SMBIOS_SYSTEM_ENCLOSURE_OR_CHASSIS_TYPE_BLADE_ENCLOSURE         ((BYTE)1Dh) // Blade Enclosure
#define SMBIOS_SYSTEM_ENCLOSURE_OR_CHASSIS_TYPE_TABLET                  ((BYTE)1Eh) // Tablet
#define SMBIOS_SYSTEM_ENCLOSURE_OR_CHASSIS_TYPE_CONVERTIBLE             ((BYTE)1Fh) // Convertible
#define SMBIOS_SYSTEM_ENCLOSURE_OR_CHASSIS_TYPE_DETACHABLE              ((BYTE)20h) // Detachable
#define SMBIOS_SYSTEM_ENCLOSURE_OR_CHASSIS_TYPE_IOT_GATEWAY             ((BYTE)21h) // IoT Gateway
#define SMBIOS_SYSTEM_ENCLOSURE_OR_CHASSIS_TYPE_EMBEDDED_PC             ((BYTE)22h) // Embedded PC
#define SMBIOS_SYSTEM_ENCLOSURE_OR_CHASSIS_TYPE_MINI_PC                 ((BYTE)23h) // Mini PC
#define SMBIOS_SYSTEM_ENCLOSURE_OR_CHASSIS_TYPE_STICK_PC                ((BYTE)24h) // Stick PC

#define SMBIOS_SYSTEM_ENCLOSURE_OR_CHASSIS_STATE_OTHER              ((BYTE)01h) // Other
#define SMBIOS_SYSTEM_ENCLOSURE_OR_CHASSIS_STATE_UNKNOWN            ((BYTE)02h) // Unknown
#define SMBIOS_SYSTEM_ENCLOSURE_OR_CHASSIS_STATE_SAFE               ((BYTE)03h) // Safe
#define SMBIOS_SYSTEM_ENCLOSURE_OR_CHASSIS_STATE_WARNING            ((BYTE)04h) // Warning
#define SMBIOS_SYSTEM_ENCLOSURE_OR_CHASSIS_STATE_CRITICAL           ((BYTE)05h) // Critical
#define SMBIOS_SYSTEM_ENCLOSURE_OR_CHASSIS_STATE_NON_RECOVERABLE    ((BYTE)06h) // Non-recoverable

#define SMBIOS_SYSTEM_ENCLOSURE_OR_CHASSIS_SECURITY_STATE_OTHER                         ((BYTE)01h) // Other
#define SMBIOS_SYSTEM_ENCLOSURE_OR_CHASSIS_SECURITY_STATE_UNKNOWN                       ((BYTE)02h) // Unknown
#define SMBIOS_SYSTEM_ENCLOSURE_OR_CHASSIS_SECURITY_STATE_NONE                          ((BYTE)03h) // None
#define SMBIOS_SYSTEM_ENCLOSURE_OR_CHASSIS_SECURITY_STATE_EXTERNAL_INTERFACE_LOCKED_OUT ((BYTE)04h) // External interface locked out
#define SMBIOS_SYSTEM_ENCLOSURE_OR_CHASSIS_SECURITY_STATE_EXTERNAL_INTERFACE_ENABLED    ((BYTE)05h) // External interface enabled 

typedef struct _SMBIOS_SYSTEM_ENCLOSURE_OR_CHASSIS
{
    DMI_STRING Manufacturer;
    union
    {
        BYTE Type;
        struct
        {
            BYTE Value : 7;         // 00:06: SMBIOS_SYSTEM_ENCLOSURE_OR_CHASSIS_TYPE_*
            BYTE ChassisLock : 1;   // 07 Chassis lock is present
        };
    };
    DMI_STRING Version;
    DMI_STRING SerialNumber;
    DMI_STRING AssetTagNumber;
#if SMBIOS_VERSION >= 0x02010000
    BYTE BootUpState;       // SMBIOS_SYSTEM_ENCLOSURE_OR_CHASSIS_STATE_*
    BYTE PowerSupplyState;  // SMBIOS_SYSTEM_ENCLOSURE_OR_CHASSIS_STATE_*
    BYTE ThermalState;      // SMBIOS_SYSTEM_ENCLOSURE_OR_CHASSIS_STATE_*
    BYTE SecurityStatus;    // SMBIOS_SYSTEM_ENCLOSURE_OR_CHASSIS_SECURITY_STATE_*
#if SMBIOS_VERSION >= 0x02030000
    DWORD OEMDefined;
    BYTE Height;
    BYTE NumberOfPowerCords;
    BYTE ContainedElementCount;
    BYTE ContainedElementRecordLength;
    _Field_size_bytes_(ContainedElementCount * ContainedElementRecordLength) BYTE ContainedElements[];
/*
#if SMBIOS_VERSION >= 0x02070000
    DMI_STRING SKUNumber;
#endif // SMBIOS_VERSION >= 0x02070000
*/
#endif // SMBIOS_VERSION >= 0x02030000
#endif // SMBIOS_VERSION >= 0x02010000
} SMBIOS_SYSTEM_ENCLOSURE_OR_CHASSIS, *PSMBIOS_SYSTEM_ENCLOSURE_OR_CHASSIS;

#pragma endregion

#pragma region Processor Information (Type 4)

#define SMBIOS_PROCESSOR_INFORMATION_TYPE ((BYTE)4)

#define SMBIOS_PROCESSOR_TYPE_OTHER             ((BYTE)001h) // Other
#define SMBIOS_PROCESSOR_TYPE_UNKNOWN           ((BYTE)002h) // Unknown
#define SMBIOS_PROCESSOR_TYPE_CENTRAL_PROCESSOR ((BYTE)003h) // Central Processor
#define SMBIOS_PROCESSOR_TYPE_MATH_PROCESSOR    ((BYTE)004h) // Math Processor
#define SMBIOS_PROCESSOR_TYPE_DSP_PROCESSOR     ((BYTE)005h) // DSP Processor
#define SMBIOS_PROCESSOR_TYPE_VIDEO_PROCESSOR   ((BYTE)006h) // Video Processor

#define SMBIOS_PROCESSOR_FAMILY_OTHER                                                   ((BYTE)001h)    // Other
#define SMBIOS_PROCESSOR_FAMILY_UNKNOWN                                                 ((BYTE)002h)    // Unknown
#define SMBIOS_PROCESSOR_FAMILY_8086                                                    ((BYTE)003h)    // 8086
#define SMBIOS_PROCESSOR_FAMILY_80286                                                   ((BYTE)004h)    // 80286
#define SMBIOS_PROCESSOR_FAMILY_INTEL386_PROCESSOR                                      ((BYTE)005h)    // Intel386™ processor
#define SMBIOS_PROCESSOR_FAMILY_INTEL486_PROCESSOR                                      ((BYTE)006h)    // Intel486™ processor
#define SMBIOS_PROCESSOR_FAMILY_8087                                                    ((BYTE)007h)    // 8087
#define SMBIOS_PROCESSOR_FAMILY_80287                                                   ((BYTE)008h)    // 80287
#define SMBIOS_PROCESSOR_FAMILY_80387                                                   ((BYTE)009h)    // 80387
#define SMBIOS_PROCESSOR_FAMILY_80487                                                   ((BYTE)00Ah)    // 80487
#define SMBIOS_PROCESSOR_FAMILY_INTEL_PENTIUM_PROCESSOR                                 ((BYTE)00Bh)    // Intel® Pentium® processor
#define SMBIOS_PROCESSOR_FAMILY_PENTIUM_PRO_PROCESSOR                                   ((BYTE)00Ch)    // Pentium® Pro processor
#define SMBIOS_PROCESSOR_FAMILY_PENTIUM_II_PROCESSOR                                    ((BYTE)00Dh)    // Pentium® II processor
#define SMBIOS_PROCESSOR_FAMILY_PENTIUM_PROCESSOR_WITH_MMX_TECHNOLOGY                   ((BYTE)00Eh)    // Pentium® processor with MMX™ technology
#define SMBIOS_PROCESSOR_FAMILY_INTEL_CELERON_PROCESSOR                                 ((BYTE)00Fh)    // Intel® Celeron® processor
#define SMBIOS_PROCESSOR_FAMILY_PENTIUM_II_XEON_PROCESSOR                               ((BYTE)010h)    // Pentium® II Xeon® processor
#define SMBIOS_PROCESSOR_FAMILY_PENTIUM_III_PROCESSOR                                   ((BYTE)011h)    // Pentium® III processor
#define SMBIOS_PROCESSOR_FAMILY_M1_FAMILY                                               ((BYTE)012h)    // M1 Family
#define SMBIOS_PROCESSOR_FAMILY_M2_FAMILY                                               ((BYTE)013h)    // M2 Family
#define SMBIOS_PROCESSOR_FAMILY_INTEL_CELERON_M_PROCESSOR                               ((BYTE)014h)    // Intel® Celeron® M processor
#define SMBIOS_PROCESSOR_FAMILY_INTEL_PENTIUM_4_HT_PROCESSOR                            ((BYTE)015h)    // Intel® Pentium® 4 HT processor
#define SMBIOS_PROCESSOR_FAMILY_INTEL_PROCESSOR                                         ((BYTE)016h)    // Intel® Processor
#define SMBIOS_PROCESSOR_FAMILY_AMD_DURON_PROCESSOR_FAMILY_1                            ((BYTE)018h)    // AMD Duron™ Processor Family [1]
#define SMBIOS_PROCESSOR_FAMILY_K5_FAMILY_1                                             ((BYTE)019h)    // K5 Family [1]
#define SMBIOS_PROCESSOR_FAMILY_K6_FAMILY_1                                             ((BYTE)01Ah)    // K6 Family [1]
#define SMBIOS_PROCESSOR_FAMILY_K6_2_1                                                  ((BYTE)01Bh)    // K6-2 [1]
#define SMBIOS_PROCESSOR_FAMILY_K6_3_1                                                  ((BYTE)01Ch)    // K6-3 [1]
#define SMBIOS_PROCESSOR_FAMILY_AMD_ATHLON_PROCESSOR_FAMILY_1                           ((BYTE)01Dh)    // AMD Athlon™ Processor Family [1]
#define SMBIOS_PROCESSOR_FAMILY_AMD29000_FAMILY                                         ((BYTE)01Eh)    // AMD29000 Family
#define SMBIOS_PROCESSOR_FAMILY_K6_2_PLUS                                               ((BYTE)01Fh)    // K6-2+
#define SMBIOS_PROCESSOR_FAMILY_POWER_PC_FAMILY                                         ((BYTE)020h)    // Power PC Family
#define SMBIOS_PROCESSOR_FAMILY_POWER_PC_601                                            ((BYTE)021h)    // Power PC 601
#define SMBIOS_PROCESSOR_FAMILY_POWER_PC_603                                            ((BYTE)022h)    // Power PC 603
#define SMBIOS_PROCESSOR_FAMILY_POWER_PC_603_PLUS                                       ((BYTE)023h)    // Power PC 603+
#define SMBIOS_PROCESSOR_FAMILY_POWER_PC_604                                            ((BYTE)024h)    // Power PC 604
#define SMBIOS_PROCESSOR_FAMILY_POWER_PC_620                                            ((BYTE)025h)    // Power PC 620
#define SMBIOS_PROCESSOR_FAMILY_POWER_PC_X704                                           ((BYTE)026h)    // Power PC x704
#define SMBIOS_PROCESSOR_FAMILY_POWER_PC_750                                            ((BYTE)027h)    // Power PC 750
#define SMBIOS_PROCESSOR_FAMILY_INTEL_CORE_DUO_PROCESSOR                                ((BYTE)028h)    // Intel® Core™ Duo processor
#define SMBIOS_PROCESSOR_FAMILY_INTEL_CORE_DUO_MOBILE_PROCESSOR                         ((BYTE)029h)    // Intel® Core™ Duo mobile processor
#define SMBIOS_PROCESSOR_FAMILY_INTEL_CORE_SOLO_MOBILE_PROCESSOR                        ((BYTE)02Ah)    // Intel® Core™ Solo mobile processor
#define SMBIOS_PROCESSOR_FAMILY_INTEL_ATOM_PROCESSOR                                    ((BYTE)02Bh)    // Intel® Atom™ processor
#define SMBIOS_PROCESSOR_FAMILY_INTEL_CORE_M_PROCESSOR                                  ((BYTE)02Ch)    // Intel® Core™ M processor
#define SMBIOS_PROCESSOR_FAMILY_INTEL_CORE_M3_PROCESSOR                                 ((BYTE)02Dh)    // Intel® Core™ m3 processor
#define SMBIOS_PROCESSOR_FAMILY_INTEL_CORE_M5_PROCESSOR                                 ((BYTE)02Eh)    // Intel® Core™ m5 processor
#define SMBIOS_PROCESSOR_FAMILY_INTEL_CORE_M7_PROCESSOR                                 ((BYTE)02Fh)    // Intel® Core™ m7 processor
#define SMBIOS_PROCESSOR_FAMILY_ALPHA_FAMILY_2                                          ((BYTE)030h)    // Alpha Family [2]
#define SMBIOS_PROCESSOR_FAMILY_ALPHA_21064                                             ((BYTE)031h)    // Alpha 21064
#define SMBIOS_PROCESSOR_FAMILY_ALPHA_21066                                             ((BYTE)032h)    // Alpha 21066
#define SMBIOS_PROCESSOR_FAMILY_ALPHA_21164                                             ((BYTE)033h)    // Alpha 21164
#define SMBIOS_PROCESSOR_FAMILY_ALPHA_21164PC                                           ((BYTE)034h)    // Alpha 21164PC
#define SMBIOS_PROCESSOR_FAMILY_ALPHA_21164A                                            ((BYTE)035h)    // Alpha 21164a
#define SMBIOS_PROCESSOR_FAMILY_ALPHA_21264                                             ((BYTE)036h)    // Alpha 21264
#define SMBIOS_PROCESSOR_FAMILY_ALPHA_21364                                             ((BYTE)037h)    // Alpha 21364
#define SMBIOS_PROCESSOR_FAMILY_AMD_TURION_II_ULTRA_DUAL_CORE_MOBILE_M_PROCESSOR_FAMILY ((BYTE)038h)    // AMD Turion™ II Ultra Dual-Core Mobile M Processor Family
#define SMBIOS_PROCESSOR_FAMILY_AMD_TURION_II_DUAL_CORE_MOBILE_M_PROCESSOR_FAMILY       ((BYTE)039h)    // AMD Turion™ II Dual-Core Mobile M Processor Family
#define SMBIOS_PROCESSOR_FAMILY_AMD_ATHLON_II_DUAL_CORE_M_PROCESSOR_FAMILY              ((BYTE)03Ah)    // AMD Athlon™ II Dual-Core M Processor Family
#define SMBIOS_PROCESSOR_FAMILY_AMD_OPTERON_6100_SERIES_PROCESSOR                       ((BYTE)03Bh)    // AMD Opteron™ 6100 Series Processor
#define SMBIOS_PROCESSOR_FAMILY_AMD_OPTERON_4100_SERIES_PROCESSOR                       ((BYTE)03Ch)    // AMD Opteron™ 4100 Series Processor
#define SMBIOS_PROCESSOR_FAMILY_AMD_OPTERON_6200_SERIES_PROCESSOR                       ((BYTE)03Dh)    // AMD Opteron™ 6200 Series Processor
#define SMBIOS_PROCESSOR_FAMILY_AMD_OPTERON_4200_SERIES_PROCESSOR                       ((BYTE)03Eh)    // AMD Opteron™ 4200 Series Processor
#define SMBIOS_PROCESSOR_FAMILY_AMD_FX_SERIES_PROCESSOR                                 ((BYTE)03Fh)    // AMD FX™ Series Processor
#define SMBIOS_PROCESSOR_FAMILY_MIPS_FAMILY                                             ((BYTE)040h)    // MIPS Family
#define SMBIOS_PROCESSOR_FAMILY_MIPS_R4000                                              ((BYTE)041h)    // MIPS R4000
#define SMBIOS_PROCESSOR_FAMILY_MIPS_R4200                                              ((BYTE)042h)    // MIPS R4200
#define SMBIOS_PROCESSOR_FAMILY_MIPS_R4400                                              ((BYTE)043h)    // MIPS R4400
#define SMBIOS_PROCESSOR_FAMILY_MIPS_R4600                                              ((BYTE)044h)    // MIPS R4600
#define SMBIOS_PROCESSOR_FAMILY_MIPS_R10000                                             ((BYTE)045h)    // MIPS R10000
#define SMBIOS_PROCESSOR_FAMILY_AMD_C_SERIES_PROCESSOR                                  ((BYTE)046h)    // AMD C-Series Processor
#define SMBIOS_PROCESSOR_FAMILY_AMD_E_SERIES_PROCESSOR                                  ((BYTE)047h)    // AMD E-Series Processor
#define SMBIOS_PROCESSOR_FAMILY_AMD_A_SERIES_PROCESSOR                                  ((BYTE)048h)    // AMD A-Series Processor
#define SMBIOS_PROCESSOR_FAMILY_AMD_G_SERIES_PROCESSOR                                  ((BYTE)049h)    // AMD G-Series Processor
#define SMBIOS_PROCESSOR_FAMILY_AMD_Z_SERIES_PROCESSOR                                  ((BYTE)04Ah)    // AMD Z-Series Processor
#define SMBIOS_PROCESSOR_FAMILY_AMD_R_SERIES_PROCESSOR                                  ((BYTE)04Bh)    // AMD R-Series Processor
#define SMBIOS_PROCESSOR_FAMILY_AMD_OPTERON_4300_SERIES_PROCESSOR                       ((BYTE)04Ch)    // AMD Opteron™ 4300 Series Processor
#define SMBIOS_PROCESSOR_FAMILY_AMD_OPTERON_6300_SERIES_PROCESSOR                       ((BYTE)04Dh)    // AMD Opteron™ 6300 Series Processor
#define SMBIOS_PROCESSOR_FAMILY_AMD_OPTERON_3300_SERIES_PROCESSOR                       ((BYTE)04Eh)    // AMD Opteron™ 3300 Series Processor
#define SMBIOS_PROCESSOR_FAMILY_AMD_FIREPRO_SERIES_PROCESSOR                            ((BYTE)04Fh)    // AMD FirePro™ Series Processor
#define SMBIOS_PROCESSOR_FAMILY_SPARC_FAMILY                                            ((BYTE)050h)    // SPARC Family
#define SMBIOS_PROCESSOR_FAMILY_SUPERSPARC                                              ((BYTE)051h)    // SuperSPARC
#define SMBIOS_PROCESSOR_FAMILY_MICROSPARC_II                                           ((BYTE)052h)    // microSPARC II
#define SMBIOS_PROCESSOR_FAMILY_MICROSPARC_IIEP                                         ((BYTE)053h)    // microSPARC IIep
#define SMBIOS_PROCESSOR_FAMILY_ULTRASPARC                                              ((BYTE)054h)    // UltraSPARC
#define SMBIOS_PROCESSOR_FAMILY_ULTRASPARC_II                                           ((BYTE)055h)    // UltraSPARC II
#define SMBIOS_PROCESSOR_FAMILY_ULTRASPARC_Iii                                          ((BYTE)056h)    // UltraSPARC Iii
#define SMBIOS_PROCESSOR_FAMILY_ULTRASPARC_III                                          ((BYTE)057h)    // UltraSPARC III
#define SMBIOS_PROCESSOR_FAMILY_ULTRASPARC_IIIi                                         ((BYTE)058h)    // UltraSPARC IIIi
#define SMBIOS_PROCESSOR_FAMILY_68040_FAMILY                                            ((BYTE)060h)    // 68040 Family
#define SMBIOS_PROCESSOR_FAMILY_68XXX                                                   ((BYTE)061h)    // 68xxx
#define SMBIOS_PROCESSOR_FAMILY_68000                                                   ((BYTE)062h)    // 68000
#define SMBIOS_PROCESSOR_FAMILY_68010                                                   ((BYTE)063h)    // 68010
#define SMBIOS_PROCESSOR_FAMILY_68020                                                   ((BYTE)064h)    // 68020
#define SMBIOS_PROCESSOR_FAMILY_68030                                                   ((BYTE)065h)    // 68030
#define SMBIOS_PROCESSOR_FAMILY_AMD_ATHLON_X4_QUAD_CORE_PROCESSOR_FAMILY                ((BYTE)066h)    // AMD Athlon(TM) X4 Quad-Core Processor Family
#define SMBIOS_PROCESSOR_FAMILY_AMD_OPTERON_X1000_SERIES_PROCESSOR                      ((BYTE)067h)    // AMD Opteron(TM) X1000 Series Processor
#define SMBIOS_PROCESSOR_FAMILY_AMD_OPTERON_X2000_SERIES_APU                            ((BYTE)068h)    // AMD Opteron(TM) X2000 Series APU
#define SMBIOS_PROCESSOR_FAMILY_AMD_OPTERON_A_SERIES_PROCESSOR                          ((BYTE)069h)    // AMD Opteron(TM) A-Series Processor
#define SMBIOS_PROCESSOR_FAMILY_AMD_OPTERON_X3000_SERIES_APU                            ((BYTE)06Ah)    // AMD Opteron(TM) X3000 Series APU
#define SMBIOS_PROCESSOR_FAMILY_AMD_ZEN_PROCESSOR_FAMILY                                ((BYTE)06Bh)    // AMD Zen Processor Family
#define SMBIOS_PROCESSOR_FAMILY_HOBBIT_FAMILY                                           ((BYTE)070h)    // Hobbit Family
#define SMBIOS_PROCESSOR_FAMILY_CRUSOE_TM5000_FAMILY                                    ((BYTE)078h)    // Crusoe™ TM5000 Family
#define SMBIOS_PROCESSOR_FAMILY_CRUSOE_TM3000_FAMILY                                    ((BYTE)079h)    // Crusoe™ TM3000 Family
#define SMBIOS_PROCESSOR_FAMILY_EFFICEON_TM8000_FAMILY                                  ((BYTE)07Ah)    // Efficeon™ TM8000 Family
#define SMBIOS_PROCESSOR_FAMILY_WEITEK                                                  ((BYTE)080h)    // Weitek
#define SMBIOS_PROCESSOR_FAMILY_ITANIUM_PROCESSOR                                       ((BYTE)082h)    // Itanium™ processor
#define SMBIOS_PROCESSOR_FAMILY_AMD_ATHLON_64_PROCESSOR_FAMILY                          ((BYTE)083h)    // AMD Athlon™ 64 Processor Family
#define SMBIOS_PROCESSOR_FAMILY_AMD_OPTERON_PROCESSOR_FAMILY                            ((BYTE)084h)    // AMD Opteron™ Processor Family
#define SMBIOS_PROCESSOR_FAMILY_AMD_SEMPRON_PROCESSOR_FAMILY                            ((BYTE)085h)    // AMD Sempron™ Processor Family
#define SMBIOS_PROCESSOR_FAMILY_AMD_TURION_64_MOBILE_TECHNOLOGY                         ((BYTE)086h)    // AMD Turion™ 64 Mobile Technology
#define SMBIOS_PROCESSOR_FAMILY_DUAL_CORE_AMD_OPTERON_PROCESSOR_FAMILY                  ((BYTE)087h)    // Dual-Core AMD Opteron™ Processor Family
#define SMBIOS_PROCESSOR_FAMILY_AMD_ATHLON_64_X2_DUAL_CORE_PROCESSOR_FAMILY             ((BYTE)088h)    // AMD Athlon™ 64 X2 Dual-Core Processor Family
#define SMBIOS_PROCESSOR_FAMILY_AMD_TURION_64_X2_MOBILE_TECHNOLOGY                      ((BYTE)089h)    // AMD Turion™ 64 X2 Mobile Technology
#define SMBIOS_PROCESSOR_FAMILY_QUAD_CORE_AMD_OPTERON_PROCESSOR_FAMILY                  ((BYTE)08Ah)    // Quad-Core AMD Opteron™ Processor Family
#define SMBIOS_PROCESSOR_FAMILY_THIRD_GENERATION_AMD_OPTERON_PROCESSOR_FAMILY           ((BYTE)08Bh)    // Third-Generation AMD Opteron™ Processor Family
#define SMBIOS_PROCESSOR_FAMILY_AMD_PHENOM_FX_QUAD_CORE_PROCESSOR_FAMILY                ((BYTE)08Ch)    // AMD Phenom™ FX Quad-Core Processor Family
#define SMBIOS_PROCESSOR_FAMILY_AMD_PHENOM_X4_QUAD_CORE_PROCESSOR_FAMILY                ((BYTE)08Dh)    // AMD Phenom™ X4 Quad-Core Processor Family
#define SMBIOS_PROCESSOR_FAMILY_AMD_PHENOM_X2_DUAL_CORE_PROCESSOR_FAMILY                ((BYTE)08Eh)    // AMD Phenom™ X2 Dual-Core Processor Family
#define SMBIOS_PROCESSOR_FAMILY_AMD_ATHLON_X2_DUAL_CORE_PROCESSOR_FAMILY                ((BYTE)08Fh)    // AMD Athlon™ X2 Dual-Core Processor Family
#define SMBIOS_PROCESSOR_FAMILY_PA_RISC_FAMILY                                          ((BYTE)090h)    // PA-RISC Family
#define SMBIOS_PROCESSOR_FAMILY_PA_RISC_8500                                            ((BYTE)091h)    // PA-RISC 8500
#define SMBIOS_PROCESSOR_FAMILY_PA_RISC_8000                                            ((BYTE)092h)    // PA-RISC 8000
#define SMBIOS_PROCESSOR_FAMILY_PA_RISC_7300LC                                          ((BYTE)093h)    // PA-RISC 7300LC
#define SMBIOS_PROCESSOR_FAMILY_PA_RISC_7200                                            ((BYTE)094h)    // PA-RISC 7200
#define SMBIOS_PROCESSOR_FAMILY_PA_RISC_7100LC                                          ((BYTE)095h)    // PA-RISC 7100LC
#define SMBIOS_PROCESSOR_FAMILY_PA_RISC_7100                                            ((BYTE)096h)    // PA-RISC 7100
#define SMBIOS_PROCESSOR_FAMILY_V30_FAMILY                                              ((BYTE)0A0h)    // V30 Family
#define SMBIOS_PROCESSOR_FAMILY_QUAD_CORE_INTEL_XEON_PROCESSOR_3200_SERIES              ((BYTE)0A1h)    // Quad-Core Intel® Xeon® processor 3200 Series
#define SMBIOS_PROCESSOR_FAMILY_DUAL_CORE_INTEL_XEON_PROCESSOR_3000_SERIES              ((BYTE)0A2h)    // Dual-Core Intel® Xeon® processor 3000 Series
#define SMBIOS_PROCESSOR_FAMILY_QUAD_CORE_INTEL_XEON_PROCESSOR_5300_SERIES              ((BYTE)0A3h)    // Quad-Core Intel® Xeon® processor 5300 Series
#define SMBIOS_PROCESSOR_FAMILY_DUAL_CORE_INTEL_XEON_PROCESSOR_5100_SERIES              ((BYTE)0A4h)    // Dual-Core Intel® Xeon® processor 5100 Series
#define SMBIOS_PROCESSOR_FAMILY_DUAL_CORE_INTEL_XEON_PROCESSOR_5000_SERIES              ((BYTE)0A5h)    // Dual-Core Intel® Xeon® processor 5000 Series
#define SMBIOS_PROCESSOR_FAMILY_DUAL_CORE_INTEL_XEON_PROCESSOR_LV                       ((BYTE)0A6h)    // Dual-Core Intel® Xeon® processor LV
#define SMBIOS_PROCESSOR_FAMILY_DUAL_CORE_INTEL_XEON_PROCESSOR_ULV                      ((BYTE)0A7h)    // Dual-Core Intel® Xeon® processor ULV
#define SMBIOS_PROCESSOR_FAMILY_DUAL_CORE_INTEL_XEON_PROCESSOR_7100_SERIES              ((BYTE)0A8h)    // Dual-Core Intel® Xeon® processor 7100 Series
#define SMBIOS_PROCESSOR_FAMILY_QUAD_CORE_INTEL_XEON_PROCESSOR_5400_SERIES              ((BYTE)0A9h)    // Quad-Core Intel® Xeon® processor 5400 Series
#define SMBIOS_PROCESSOR_FAMILY_QUAD_CORE_INTEL_XEON_PROCESSOR                          ((BYTE)0AAh)    // Quad-Core Intel® Xeon® processor
#define SMBIOS_PROCESSOR_FAMILY_DUAL_CORE_INTEL_XEON_PROCESSOR_5200_SERIES              ((BYTE)0ABh)    // Dual-Core Intel® Xeon® processor 5200 Series
#define SMBIOS_PROCESSOR_FAMILY_DUAL_CORE_INTEL_XEON_PROCESSOR_7200_SERIES              ((BYTE)0ACh)    // Dual-Core Intel® Xeon® processor 7200 Series
#define SMBIOS_PROCESSOR_FAMILY_QUAD_CORE_INTEL_XEON_PROCESSOR_7300_SERIES              ((BYTE)0ADh)    // Quad-Core Intel® Xeon® processor 7300 Series
#define SMBIOS_PROCESSOR_FAMILY_QUAD_CORE_INTEL_XEON_PROCESSOR_7400_SERIES              ((BYTE)0AEh)    // Quad-Core Intel® Xeon® processor 7400 Series
#define SMBIOS_PROCESSOR_FAMILY_MULTI_CORE_INTEL_XEON_PROCESSOR_7400_SERIES             ((BYTE)0AFh)    // Multi-Core Intel® Xeon® processor 7400 Series
#define SMBIOS_PROCESSOR_FAMILY_PENTIUM_III_XEON_PROCESSOR                              ((BYTE)0B0h)    // Pentium® III Xeon® processor
#define SMBIOS_PROCESSOR_FAMILY_PENTIUM_III_PROCESSOR_WITH_INTEL_SPEEDSTEP_TECHNOLOGY   ((BYTE)0B1h)    // Pentium® III Processor with Intel® SpeedStep™ Technology
#define SMBIOS_PROCESSOR_FAMILY_PENTIUM_4_PROCESSOR                                     ((BYTE)0B2h)    // Pentium® 4 Processor
#define SMBIOS_PROCESSOR_FAMILY_INTEL_XEON_PROCESSOR                                    ((BYTE)0B3h)    // Intel® Xeon® processor
#define SMBIOS_PROCESSOR_FAMILY_AS400_FAMILY                                            ((BYTE)0B4h)    // AS400 Family
#define SMBIOS_PROCESSOR_FAMILY_INTEL_XEON_PROCESSOR_MP                                 ((BYTE)0B5h)    // Intel® Xeon® processor MP
#define SMBIOS_PROCESSOR_FAMILY_AMD_ATHLON_XP_PROCESSOR_FAMILY                          ((BYTE)0B6h)    // AMD Athlon™ XP Processor Family
#define SMBIOS_PROCESSOR_FAMILY_AMD_ATHLON_MP_PROCESSOR_FAMILY                          ((BYTE)0B7h)    // AMD Athlon™ MP Processor Family
#define SMBIOS_PROCESSOR_FAMILY_INTEL_ITANIUM_2_PROCESSOR                               ((BYTE)0B8h)    // Intel® Itanium® 2 processor
#define SMBIOS_PROCESSOR_FAMILY_INTEL_PENTIUM_M_PROCESSOR                               ((BYTE)0B9h)    // Intel® Pentium® M processor
#define SMBIOS_PROCESSOR_FAMILY_INTEL_CELERON_D_PROCESSOR                               ((BYTE)0BAh)    // Intel® Celeron® D processor
#define SMBIOS_PROCESSOR_FAMILY_INTEL_PENTIUM_D_PROCESSOR                               ((BYTE)0BBh)    // Intel® Pentium® D processor
#define SMBIOS_PROCESSOR_FAMILY_INTEL_PENTIUM_PROCESSOR_EXTREME_EDITION                 ((BYTE)0BCh)    // Intel® Pentium® Processor Extreme Edition
#define SMBIOS_PROCESSOR_FAMILY_INTEL_CORE_SOLO_PROCESSOR                               ((BYTE)0BDh)    // Intel® Core™ Solo Processor
#define SMBIOS_PROCESSOR_FAMILY_INTEL_CORE_2_DUO_PROCESSOR                              ((BYTE)0BFh)    // Intel® Core™ 2 Duo Processor
#define SMBIOS_PROCESSOR_FAMILY_INTEL_CORE_2_SOLO_PROCESSOR                             ((BYTE)0C0h)    // Intel® Core™ 2 Solo processor
#define SMBIOS_PROCESSOR_FAMILY_INTEL_CORE_2_EXTREME_PROCESSOR                          ((BYTE)0C1h)    // Intel® Core™ 2 Extreme processor
#define SMBIOS_PROCESSOR_FAMILY_INTEL_CORE_2_QUAD_PROCESSOR                             ((BYTE)0C2h)    // Intel® Core™ 2 Quad processor
#define SMBIOS_PROCESSOR_FAMILY_INTEL_CORE_2_EXTREME_MOBILE_PROCESSOR                   ((BYTE)0C3h)    // Intel® Core™ 2 Extreme mobile processor
#define SMBIOS_PROCESSOR_FAMILY_INTEL_CORE_2_DUO_MOBILE_PROCESSOR                       ((BYTE)0C4h)    // Intel® Core™ 2 Duo mobile processor
#define SMBIOS_PROCESSOR_FAMILY_INTEL_CORE_2_SOLO_MOBILE_PROCESSOR                      ((BYTE)0C5h)    // Intel® Core™ 2 Solo mobile processor
#define SMBIOS_PROCESSOR_FAMILY_INTEL_CORE_I7_PROCESSOR                                 ((BYTE)0C6h)    // Intel® Core™ i7 processor
#define SMBIOS_PROCESSOR_FAMILY_DUAL_CORE_INTEL_CELERON_PROCESSOR                       ((BYTE)0C7h)    // Dual-Core Intel® Celeron® processor
#define SMBIOS_PROCESSOR_FAMILY_IBM390_FAMILY                                           ((BYTE)0C8h)    // IBM390 Family
#define SMBIOS_PROCESSOR_FAMILY_G4                                                      ((BYTE)0C9h)    // G4
#define SMBIOS_PROCESSOR_FAMILY_G5                                                      ((BYTE)0CAh)    // G5
#define SMBIOS_PROCESSOR_FAMILY_ESA_390_G6                                              ((BYTE)0CBh)    // ESA/390 G6
#define SMBIOS_PROCESSOR_FAMILY_Z_ARCHITECTURE_BASE                                     ((BYTE)0CCh)    // z/Architecture base
#define SMBIOS_PROCESSOR_FAMILY_INTEL_CORE_I5_PROCESSOR                                 ((BYTE)0CDh)    // Intel® Core™ i5 processor
#define SMBIOS_PROCESSOR_FAMILY_INTEL_CORE_I3_PROCESSOR                                 ((BYTE)0CEh)    // Intel® Core™ i3 processor
#define SMBIOS_PROCESSOR_FAMILY_INTEL_CORE_I9_PROCESSOR                                 ((BYTE)0CFh)    // Intel® Core™ i9 processor
#define SMBIOS_PROCESSOR_FAMILY_INTEL_XEON_D_PROCESSOR_FAMILY                           ((BYTE)0D0h)    // Intel® Xeon® D Processor family
#define SMBIOS_PROCESSOR_FAMILY_VIA_C7_M_PROCESSOR_FAMILY                               ((BYTE)0D2h)    // VIA C7™-M Processor Family
#define SMBIOS_PROCESSOR_FAMILY_VIA_C7_D_PROCESSOR_FAMILY                               ((BYTE)0D3h)    // VIA C7™-D Processor Family
#define SMBIOS_PROCESSOR_FAMILY_VIA_C7_PROCESSOR_FAMILY                                 ((BYTE)0D4h)    // VIA C7™ Processor Family
#define SMBIOS_PROCESSOR_FAMILY_VIA_EDEN_PROCESSOR_FAMILY                               ((BYTE)0D5h)    // VIA Eden™ Processor Family
#define SMBIOS_PROCESSOR_FAMILY_MULTI_CORE_INTEL_XEON_PROCESSOR                         ((BYTE)0D6h)    // Multi-Core Intel® Xeon® processor
#define SMBIOS_PROCESSOR_FAMILY_DUAL_CORE_INTEL_XEON_PROCESSOR_3XXX_SERIES              ((BYTE)0D7h)    // Dual-Core Intel® Xeon® processor 3xxx Series
#define SMBIOS_PROCESSOR_FAMILY_QUAD_CORE_INTEL_XEON_PROCESSOR_3XXX_SERIES              ((BYTE)0D8h)    // Quad-Core Intel® Xeon® processor 3xxx Series
#define SMBIOS_PROCESSOR_FAMILY_VIA_NANO_PROCESSOR_FAMILY                               ((BYTE)0D9h)    // VIA Nano™ Processor Family
#define SMBIOS_PROCESSOR_FAMILY_DUAL_CORE_INTEL_XEON_PROCESSOR_5XXX_SERIES              ((BYTE)0DAh)    // Dual-Core Intel® Xeon® processor 5xxx Series
#define SMBIOS_PROCESSOR_FAMILY_QUAD_CORE_INTEL_XEON_PROCESSOR_5XXX_SERIES              ((BYTE)0DBh)    // Quad-Core Intel® Xeon® processor 5xxx Series
#define SMBIOS_PROCESSOR_FAMILY_DUAL_CORE_INTEL_XEON_PROCESSOR_7XXX_SERIES              ((BYTE)0DDh)    // Dual-Core Intel® Xeon® processor 7xxx Series
#define SMBIOS_PROCESSOR_FAMILY_QUAD_CORE_INTEL_XEON_PROCESSOR_7XXX_SERIES              ((BYTE)0DEh)    // Quad-Core Intel® Xeon® processor 7xxx Series
#define SMBIOS_PROCESSOR_FAMILY_MULTI_CORE_INTEL_XEON_PROCESSOR_7XXX_SERIES             ((BYTE)0DFh)    // Multi-Core Intel® Xeon® processor 7xxx Series
#define SMBIOS_PROCESSOR_FAMILY_MULTI_CORE_INTEL_XEON_PROCESSOR_3400_SERIES             ((BYTE)0E0h)    // Multi-Core Intel® Xeon® processor 3400 Series
#define SMBIOS_PROCESSOR_FAMILY_AMD_OPTERON_3000_SERIES_PROCESSOR                       ((BYTE)0E4h)    // AMD Opteron™ 3000 Series Processor
#define SMBIOS_PROCESSOR_FAMILY_AMD_SEMPRON_II_PROCESSOR                                ((BYTE)0E5h)    // AMD Sempron™ II Processor
#define SMBIOS_PROCESSOR_FAMILY_EMBEDDED_AMD_OPTERON_QUAD_CORE_PROCESSOR_FAMILY         ((BYTE)0E6h)    // Embedded AMD Opteron™ Quad-Core Processor Family
#define SMBIOS_PROCESSOR_FAMILY_AMD_PHENOM_TRIPLE_CORE_PROCESSOR_FAMILY                 ((BYTE)0E7h)    // AMD Phenom™ Triple-Core Processor Family
#define SMBIOS_PROCESSOR_FAMILY_AMD_TURION_ULTRA_DUAL_CORE_MOBILE_PROCESSOR_FAMILY      ((BYTE)0E8h)    // AMD Turion™ Ultra Dual-Core Mobile Processor Family
#define SMBIOS_PROCESSOR_FAMILY_AMD_TURION_DUAL_CORE_MOBILE_PROCESSOR_FAMILY            ((BYTE)0E9h)    // AMD Turion™ Dual-Core Mobile Processor Family
#define SMBIOS_PROCESSOR_FAMILY_AMD_ATHLON_DUAL_CORE_PROCESSOR_FAMILY                   ((BYTE)0EAh)    // AMD Athlon™ Dual-Core Processor Family
#define SMBIOS_PROCESSOR_FAMILY_AMD_SEMPRON_SI_PROCESSOR_FAMILY                         ((BYTE)0EBh)    // AMD Sempron™ SI Processor Family
#define SMBIOS_PROCESSOR_FAMILY_AMD_PHENOM_II_PROCESSOR_FAMILY                          ((BYTE)0ECh)    // AMD Phenom™ II Processor Family
#define SMBIOS_PROCESSOR_FAMILY_AMD_ATHLON_II_PROCESSOR_FAMILY                          ((BYTE)0EDh)    // AMD Athlon™ II Processor Family
#define SMBIOS_PROCESSOR_FAMILY_SIX_CORE_AMD_OPTERON_PROCESSOR_FAMILY                   ((BYTE)0EEh)    // Six-Core AMD Opteron™ Processor Family
#define SMBIOS_PROCESSOR_FAMILY_AMD_SEMPRON_M_PROCESSOR_FAMILY                          ((BYTE)0EFh)    // AMD Sempron™ M Processor Family
#define SMBIOS_PROCESSOR_FAMILY_I860                                                    ((BYTE)0FAh)    // i860
#define SMBIOS_PROCESSOR_FAMILY_I960                                                    ((BYTE)0FBh)    // i960
#define SMBIOS_PROCESSOR_FAMILY_ARMV7                                                   ((BYTE)0100h)   // ARMv7
#define SMBIOS_PROCESSOR_FAMILY_ARMV8                                                   ((BYTE)0101h)   // ARMv8
#define SMBIOS_PROCESSOR_FAMILY_ARMV9                                                   ((BYTE)0102h)   // ARMv9
#define SMBIOS_PROCESSOR_FAMILY_SH_3                                                    ((BYTE)0104h)   // SH-3
#define SMBIOS_PROCESSOR_FAMILY_SH_4                                                    ((BYTE)0105h)   // SH-4
#define SMBIOS_PROCESSOR_FAMILY_ARM                                                     ((BYTE)0118h)   // ARM
#define SMBIOS_PROCESSOR_FAMILY_STRONGARM                                               ((BYTE)0119h)   // StrongARM
#define SMBIOS_PROCESSOR_FAMILY_6X86                                                    ((BYTE)012Ch)   // 6x86
#define SMBIOS_PROCESSOR_FAMILY_MEDIAGX                                                 ((BYTE)012Dh)   // MediaGX
#define SMBIOS_PROCESSOR_FAMILY_MII                                                     ((BYTE)012Eh)   // MII
#define SMBIOS_PROCESSOR_FAMILY_WINCHIP                                                 ((BYTE)0140h)   // WinChip
#define SMBIOS_PROCESSOR_FAMILY_DSP                                                     ((BYTE)015Eh)   // DSP
#define SMBIOS_PROCESSOR_FAMILY_VIDEO_PROCESSOR                                         ((BYTE)01F4h)   // Video Processor
#define SMBIOS_PROCESSOR_FAMILY_RISC_V_RV32                                             ((BYTE)0200h)   // RISC-V RV32
#define SMBIOS_PROCESSOR_FAMILY_RISC_V_RV64                                             ((BYTE)0201h)   // RISC-V RV64
#define SMBIOS_PROCESSOR_FAMILY_RISC_V_RV128                                            ((BYTE)0202h)   // RISC-V RV128
#define SMBIOS_PROCESSOR_FAMILY_LOONGARCH                                               ((BYTE)0258h)   // LoongArch
#define SMBIOS_PROCESSOR_FAMILY_LOONGSON_1_PROCESSOR_FAMILY                             ((BYTE)0259h)   // Loongson™ 1 Processor Family
#define SMBIOS_PROCESSOR_FAMILY_LOONGSON_2_PROCESSOR_FAMILY                             ((BYTE)025Ah)   // Loongson™ 2 Processor Family
#define SMBIOS_PROCESSOR_FAMILY_LOONGSON_3_PROCESSOR_FAMILY                             ((BYTE)025Bh)   // Loongson™ 3 Processor Family
#define SMBIOS_PROCESSOR_FAMILY_LOONGSON_2K_PROCESSOR_FAMILY                            ((BYTE)025Ch)   // Loongson™ 2K Processor Family
#define SMBIOS_PROCESSOR_FAMILY_LOONGSON_3A_PROCESSOR_FAMILY                            ((BYTE)025Dh)   // Loongson™ 3A Processor Family
#define SMBIOS_PROCESSOR_FAMILY_LOONGSON_3B_PROCESSOR_FAMILY                            ((BYTE)025Eh)   // Loongson™ 3B Processor Family
#define SMBIOS_PROCESSOR_FAMILY_LOONGSON_3C_PROCESSOR_FAMILY                            ((BYTE)025Fh)   // Loongson™ 3C Processor Family
#define SMBIOS_PROCESSOR_FAMILY_LOONGSON_3D_PROCESSOR_FAMILY                            ((BYTE)0260h)   // Loongson™ 3D Processor Family
#define SMBIOS_PROCESSOR_FAMILY_LOONGSON_3E_PROCESSOR_FAMILY                            ((BYTE)0261h)   // Loongson™ 3E Processor Family
#define SMBIOS_PROCESSOR_FAMILY_DUAL_CORE_LOONGSON_2K_PROCESSOR_2XXX_SERIES             ((BYTE)0262h)   // Dual-Core Loongson™ 2K Processor 2xxx Series
#define SMBIOS_PROCESSOR_FAMILY_QUAD_CORE_LOONGSON_3A_PROCESSOR_5XXX_SERIES             ((BYTE)026Ch)   // Quad-Core Loongson™ 3A Processor 5xxx Series
#define SMBIOS_PROCESSOR_FAMILY_MULTI_CORE_LOONGSON_3A_PROCESSOR_5XXX_SERIES            ((BYTE)026Dh)   // Multi-Core Loongson™ 3A Processor 5xxx Series
#define SMBIOS_PROCESSOR_FAMILY_QUAD_CORE_LOONGSON_3B_PROCESSOR_5XXX_SERIES             ((BYTE)026Eh)   // Quad-Core Loongson™ 3B Processor 5xxx Series
#define SMBIOS_PROCESSOR_FAMILY_MULTI_CORE_LOONGSON_3B_PROCESSOR_5XXX_SERIES            ((BYTE)026Fh)   // Multi-Core Loongson™ 3B Processor 5xxx Series
#define SMBIOS_PROCESSOR_FAMILY_MULTI_CORE_LOONGSON_3C_PROCESSOR_5XXX_SERIES            ((BYTE)0270h)   // Multi-Core Loongson™ 3C Processor 5xxx Series
#define SMBIOS_PROCESSOR_FAMILY_MULTI_CORE_LOONGSON_3D_PROCESSOR_5XXX_SERIES            ((BYTE)0271h)   // Multi-Core Loongson™ 3D Processor 5xxx Series
#define SMBIOS_PROCESSOR_FAMILY_INTEL_CORE_3                                            ((BYTE)0300h)   // Intel® Core™ 3
#define SMBIOS_PROCESSOR_FAMILY_INTEL_CORE_5                                            ((BYTE)0301h)   // Intel® Core™ 5
#define SMBIOS_PROCESSOR_FAMILY_INTEL_CORE_7                                            ((BYTE)0302h)   // Intel® Core™ 7
#define SMBIOS_PROCESSOR_FAMILY_INTEL_CORE_9                                            ((BYTE)0303h)   // Intel® Core™ 9
#define SMBIOS_PROCESSOR_FAMILY_INTEL_CORE_ULTRA_3                                      ((BYTE)0304h)   // Intel® Core™ Ultra 3
#define SMBIOS_PROCESSOR_FAMILY_INTEL_CORE_ULTRA_5                                      ((BYTE)0305h)   // Intel® Core™ Ultra 5
#define SMBIOS_PROCESSOR_FAMILY_INTEL_CORE_ULTRA_7                                      ((BYTE)0306h)   // Intel® Core™ Ultra 7
#define SMBIOS_PROCESSOR_FAMILY_INTEL_CORE_ULTRA_9                                      ((BYTE)0307h)   // Intel® Core™ Ultra 9

#define SMBIOS_PROCESSOR_CPU_STATUS_UNKNOWN             ((BYTE)0h) // Unknown
#define SMBIOS_PROCESSOR_CPU_STATUS_ENABLED             ((BYTE)1h) // CPU Enabled
#define SMBIOS_PROCESSOR_CPU_STATUS_DISABLED_BY_USER    ((BYTE)2h) // CPU Disabled by User through Firmware Setup
#define SMBIOS_PROCESSOR_CPU_STATUS_DISABLED_BY_BIOS    ((BYTE)3h) // CPU Disabled By firmware (POST Error)
#define SMBIOS_PROCESSOR_CPU_STATUS_IDLE                ((BYTE)4h) // CPU is Idle, waiting to be enabled
#define SMBIOS_PROCESSOR_CPU_STATUS_OTHER               ((BYTE)7h) // Other

#define SMBIOS_PROCESSOR_UPGRADE_OTHER                  ((BYTE)01h) // Other
#define SMBIOS_PROCESSOR_UPGRADE_UNKNOWN                ((BYTE)02h) // Unknown
#define SMBIOS_PROCESSOR_UPGRADE_DAUGHTER_BOARD         ((BYTE)03h) // Daughter Board
#define SMBIOS_PROCESSOR_UPGRADE_ZIF_SOCKET             ((BYTE)04h) // ZIF Socket
#define SMBIOS_PROCESSOR_UPGRADE_REPLACEABLE_PIGGY_BACK ((BYTE)05h) // Replaceable Piggy Back
#define SMBIOS_PROCESSOR_UPGRADE_NONE                   ((BYTE)06h) // None
#define SMBIOS_PROCESSOR_UPGRADE_LIF_SOCKET             ((BYTE)07h) // LIF Socket
#define SMBIOS_PROCESSOR_UPGRADE_SLOT_1                 ((BYTE)08h) // Slot 1
#define SMBIOS_PROCESSOR_UPGRADE_SLOT_2                 ((BYTE)09h) // Slot 2
#define SMBIOS_PROCESSOR_UPGRADE_370_PIN_SOCKET         ((BYTE)0Ah) // 370-pin socket
#define SMBIOS_PROCESSOR_UPGRADE_SLOT_A                 ((BYTE)0Bh) // Slot A
#define SMBIOS_PROCESSOR_UPGRADE_SLOT_M                 ((BYTE)0Ch) // Slot M
#define SMBIOS_PROCESSOR_UPGRADE_SOCKET_423             ((BYTE)0Dh) // Socket 423
#define SMBIOS_PROCESSOR_UPGRADE_SOCKET_A               ((BYTE)0Eh) // Socket A (Socket 462)
#define SMBIOS_PROCESSOR_UPGRADE_SOCKET_478             ((BYTE)0Fh) // Socket 478
#define SMBIOS_PROCESSOR_UPGRADE_SOCKET_754             ((BYTE)10h) // Socket 754
#define SMBIOS_PROCESSOR_UPGRADE_SOCKET_940             ((BYTE)11h) // Socket 940
#define SMBIOS_PROCESSOR_UPGRADE_SOCKET_939             ((BYTE)12h) // Socket 939
#define SMBIOS_PROCESSOR_UPGRADE_SOCKET_mPGA604         ((BYTE)13h) // Socket mPGA604
#define SMBIOS_PROCESSOR_UPGRADE_SOCKET_LGA771          ((BYTE)14h) // Socket LGA771
#define SMBIOS_PROCESSOR_UPGRADE_SOCKET_LGA775          ((BYTE)15h) // Socket LGA775
#define SMBIOS_PROCESSOR_UPGRADE_SOCKET_S1              ((BYTE)16h) // Socket S1
#define SMBIOS_PROCESSOR_UPGRADE_SOCKET_AM2             ((BYTE)17h) // Socket AM2
#define SMBIOS_PROCESSOR_UPGRADE_SOCKET_F               ((BYTE)18h) // Socket F (1207)
#define SMBIOS_PROCESSOR_UPGRADE_SOCKET_LGA1366         ((BYTE)19h) // Socket LGA1366
#define SMBIOS_PROCESSOR_UPGRADE_SOCKET_G34             ((BYTE)1Ah) // Socket G34
#define SMBIOS_PROCESSOR_UPGRADE_SOCKET_AM3             ((BYTE)1Bh) // Socket AM3
#define SMBIOS_PROCESSOR_UPGRADE_SOCKET_C32             ((BYTE)1Ch) // Socket C32
#define SMBIOS_PROCESSOR_UPGRADE_SOCKET_LGA1156         ((BYTE)1Dh) // Socket LGA1156
#define SMBIOS_PROCESSOR_UPGRADE_SOCKET_LGA1567         ((BYTE)1Eh) // Socket LGA1567
#define SMBIOS_PROCESSOR_UPGRADE_SOCKET_PGA988A         ((BYTE)1Fh) // Socket PGA988A
#define SMBIOS_PROCESSOR_UPGRADE_SOCKET_BGA1288         ((BYTE)20h) // Socket BGA1288
#define SMBIOS_PROCESSOR_UPGRADE_SOCKET_rPGA988B        ((BYTE)21h) // Socket rPGA988B
#define SMBIOS_PROCESSOR_UPGRADE_SOCKET_BGA1023         ((BYTE)22h) // Socket BGA1023
#define SMBIOS_PROCESSOR_UPGRADE_SOCKET_BGA1224         ((BYTE)23h) // Socket BGA1224
#define SMBIOS_PROCESSOR_UPGRADE_SOCKET_LGA1155         ((BYTE)24h) // Socket LGA1155
#define SMBIOS_PROCESSOR_UPGRADE_SOCKET_LGA1356         ((BYTE)25h) // Socket LGA1356
#define SMBIOS_PROCESSOR_UPGRADE_SOCKET_LGA2011         ((BYTE)26h) // Socket LGA2011
#define SMBIOS_PROCESSOR_UPGRADE_SOCKET_FS1             ((BYTE)27h) // Socket FS1
#define SMBIOS_PROCESSOR_UPGRADE_SOCKET_FS2             ((BYTE)28h) // Socket FS2
#define SMBIOS_PROCESSOR_UPGRADE_SOCKET_FM1             ((BYTE)29h) // Socket FM1
#define SMBIOS_PROCESSOR_UPGRADE_SOCKET_FM2             ((BYTE)2Ah) // Socket FM2
#define SMBIOS_PROCESSOR_UPGRADE_SOCKET_LGA2011_3       ((BYTE)2Bh) // Socket LGA2011-3
#define SMBIOS_PROCESSOR_UPGRADE_SOCKET_LGA1356_3       ((BYTE)2Ch) // Socket LGA2011-3
#define SMBIOS_PROCESSOR_UPGRADE_SOCKET_LGA1150         ((BYTE)2Dh) // Socket LGA1150
#define SMBIOS_PROCESSOR_UPGRADE_SOCKET_BGA1168         ((BYTE)2Eh) // Socket BGA1168
#define SMBIOS_PROCESSOR_UPGRADE_SOCKET_BGA1234         ((BYTE)2Fh) // Socket BGA1234
#define SMBIOS_PROCESSOR_UPGRADE_SOCKET_BGA1364         ((BYTE)30h) // Socket BGA1364
#define SMBIOS_PROCESSOR_UPGRADE_SOCKET_AM4             ((BYTE)31h) // Socket AM4
#define SMBIOS_PROCESSOR_UPGRADE_SOCKET_LGA1151         ((BYTE)32h) // Socket LGA1151
#define SMBIOS_PROCESSOR_UPGRADE_SOCKET_BGA1356         ((BYTE)33h) // Socket BGA1356
#define SMBIOS_PROCESSOR_UPGRADE_SOCKET_BGA1440         ((BYTE)34h) // Socket BGA1440
#define SMBIOS_PROCESSOR_UPGRADE_SOCKET_BGA1515         ((BYTE)35h) // Socket BGA1515
#define SMBIOS_PROCESSOR_UPGRADE_SOCKET_LGA3647_1       ((BYTE)36h) // Socket LGA3647-1
#define SMBIOS_PROCESSOR_UPGRADE_SOCKET_SP3             ((BYTE)37h) // Socket SP3
#define SMBIOS_PROCESSOR_UPGRADE_SOCKET_SP3r2           ((BYTE)38h) // Socket SP3r2
#define SMBIOS_PROCESSOR_UPGRADE_SOCKET_LGA2066         ((BYTE)39h) // Socket LGA2066
#define SMBIOS_PROCESSOR_UPGRADE_SOCKET_BGA1392         ((BYTE)3Ah) // Socket BGA1392
#define SMBIOS_PROCESSOR_UPGRADE_SOCKET_BGA1510         ((BYTE)3Bh) // Socket BGA1510
#define SMBIOS_PROCESSOR_UPGRADE_SOCKET_BGA1528         ((BYTE)3Ch) // Socket BGA1528
#define SMBIOS_PROCESSOR_UPGRADE_SOCKET_LGA4189         ((BYTE)3Dh) // Socket LGA4189
#define SMBIOS_PROCESSOR_UPGRADE_SOCKET_LGA1200         ((BYTE)3Eh) // Socket LGA1200
#define SMBIOS_PROCESSOR_UPGRADE_SOCKET_LGA4677         ((BYTE)3Fh) // Socket LGA4677
#define SMBIOS_PROCESSOR_UPGRADE_SOCKET_LGA1700         ((BYTE)40h) // Socket LGA1700
#define SMBIOS_PROCESSOR_UPGRADE_SOCKET_BGA1744         ((BYTE)41h) // Socket BGA1744
#define SMBIOS_PROCESSOR_UPGRADE_SOCKET_BGA1781         ((BYTE)42h) // Socket BGA1781
#define SMBIOS_PROCESSOR_UPGRADE_SOCKET_BGA1211         ((BYTE)43h) // Socket BGA1211
#define SMBIOS_PROCESSOR_UPGRADE_SOCKET_BGA2422         ((BYTE)44h) // Socket BGA2422
#define SMBIOS_PROCESSOR_UPGRADE_SOCKET_LGA1211         ((BYTE)45h) // Socket LGA1211
#define SMBIOS_PROCESSOR_UPGRADE_SOCKET_LGA2422         ((BYTE)46h) // Socket LGA2422
#define SMBIOS_PROCESSOR_UPGRADE_SOCKET_LGA5773         ((BYTE)47h) // Socket LGA5773
#define SMBIOS_PROCESSOR_UPGRADE_SOCKET_BGA5773         ((BYTE)48h) // Socket BGA5773
#define SMBIOS_PROCESSOR_UPGRADE_SOCKET_AM5             ((BYTE)49h) // Socket AM5
#define SMBIOS_PROCESSOR_UPGRADE_SOCKET_SP5             ((BYTE)4Ah) // Socket SP5
#define SMBIOS_PROCESSOR_UPGRADE_SOCKET_SP6             ((BYTE)4Bh) // Socket SP6
#define SMBIOS_PROCESSOR_UPGRADE_SOCKET_BGA883          ((BYTE)4Ch) // Socket BGA883
#define SMBIOS_PROCESSOR_UPGRADE_SOCKET_BGA1190         ((BYTE)4Dh) // Socket BGA1190
#define SMBIOS_PROCESSOR_UPGRADE_SOCKET_BGA4129         ((BYTE)4Eh) // Socket BGA4129
#define SMBIOS_PROCESSOR_UPGRADE_SOCKET_LGA4710         ((BYTE)4Fh) // Socket LGA4710
#define SMBIOS_PROCESSOR_UPGRADE_SOCKET_LGA7529         ((BYTE)50h) // Socket LGA7529
#define SMBIOS_PROCESSOR_UPGRADE_SOCKET_BGA1964         ((BYTE)51h) // Socket BGA1964
#define SMBIOS_PROCESSOR_UPGRADE_SOCKET_BGA1792         ((BYTE)52h) // Socket BGA1792
#define SMBIOS_PROCESSOR_UPGRADE_SOCKET_BGA2049         ((BYTE)53h) // Socket BGA2049
#define SMBIOS_PROCESSOR_UPGRADE_SOCKET_BGA2551         ((BYTE)54h) // Socket BGA2551
#define SMBIOS_PROCESSOR_UPGRADE_SOCKET_LGA1851         ((BYTE)55h) // Socket LGA1851
#define SMBIOS_PROCESSOR_UPGRADE_SOCKET_BGA2114         ((BYTE)56h) // Socket BGA2114
#define SMBIOS_PROCESSOR_UPGRADE_SOCKET_BGA2833         ((BYTE)57h) // Socket BGA2833
#define SMBIOS_PROCESSOR_UPGRADE_INVALID                ((BYTE)FFh) // Invalid

typedef struct _SMBIOS_PROCESSOR_INFORMATION
{
    DMI_STRING SocketDesignation;
    BYTE Type;  // SMBIOS_PROCESSOR_TYPE_*
    BYTE Family; // SMBIOS_PROCESSOR_FAMILY_*
    DMI_STRING Manufacturer;
    QWORD ID;
    DMI_STRING Version;
    union
    {
        BYTE Voltage;
        struct
        {
            BYTE Voltage5V : 1;     // 00 5V
            BYTE Voltage3Dot3V : 1; // 01 3.3V
            BYTE Voltage2Dot9V : 1; // 02 2.9V
            BYTE Reserved0 : 1;     // 03 Reserved, must be zero
            BYTE Reserved1 : 3;     // 04:06 Reserved, must be zero
            BYTE LegacyMode : 1;    // Should be 0
        };
        struct
        {
            BYTE VoltageTimes10 : 7;        // Current voltage times 10
            BYTE NotLegacyModeVoltage : 1;  // Should be 1
        };
    }; // Deprecated from version 3.8.0
    WORD ExternalClock;
    WORD MaxSpeed;
    WORD CurrentSpeed;
    union
    {
        BYTE Status;
        struct
        {
            BYTE CPUStatus : 3;             // 00:02 SMBIOS_PROCESSOR_CPU_STATUS_*
            BYTE Reserved2 : 3;             // 03:05 Reserved, must be zero
            BYTE CPUSocketPopulated : 1;    // 06 CPU Socket Populated
            BYTE Reserved3 : 1;             // 07 Reserved, must be zero
        };
    };
    BYTE Upgrade; // SMBIOS_PROCESSOR_UPGRADE_*
#if SMBIOS_VERSION >= 0x02010000
    WORD L1CacheHandle;
    WORD L2CacheHandle;
    WORD L3CacheHandle;
#if SMBIOS_VERSION >= 0x02030000
    DMI_STRING SerialNumber;
    DMI_STRING AssetTag;
    DMI_STRING PartNumber;
#if SMBIOS_VERSION >= 0x02050000
    BYTE CoreCount;
    BYTE CoreEnabled;
    BYTE ThreadCount;
    union
    {
        WORD Characteristics;
        struct
        {
            WORD Reserved4 : 1;                 // 00 Reserved
            WORD Unknown : 1;                   // 01 Unknown
            WORD _64BitCapable : 1;             // 02 64-bit Capable
            WORD MultiCore : 1;                 // 03 Multi-Core
            WORD HardwareThread : 1;            // 04 Hardware Thread
            WORD ExecuteProtection : 1;         // 05 Execute Protection
            WORD EnhancedVirtualization : 1;    // 06 Enhanced Virtualization
            WORD PowerPerformanceControl : 1;   // 07 Power/Performance Control
            WORD _128BitCapable : 1;            // 08 128-bit Capable
            WORD Arm64SoCId : 1;                // 09 Arm64 SoC ID
            WORD Reserved5 : 6;                 // 10:15 Reserved
        };
    };
#if SMBIOS_VERSION >= 0x02060000
    WORD Famliy2;
#if SMBIOS_VERSION >= 0x03000000
    WORD CoreCount2;
    WORD CoreEnabled2;
    WORD ThreadCount2;
#if SMBIOS_VERSION >= 0x03060000
    WORD ThreadEnabled;
#if SMBIOS_VERSION >= 0x03080000
    DMI_STRING SocketType;
#endif // SMBIOS_VERSION >= 0x03080000
#endif // SMBIOS_VERSION >= 0x03060000
#endif // SMBIOS_VERSION >= 0x03000000
#endif // SMBIOS_VERSION >= 0x02060000
#endif // SMBIOS_VERSION >= 0x02050000
#endif // SMBIOS_VERSION >= 0x02030000
#endif // SMBIOS_VERSION >= 0x02010000
} SMBIOS_PROCESSOR_INFORMATION, *PSMBIOS_PROCESSOR_INFORMATION;

#pragma endregion

typedef enum _SMBIOS_INFORMATION_TYPE
{
    SMBIOSMemoryControllerInformation = 5, /* Obsolete */
    SMBIOSMemoryModuleInformation = 6, /* Obsolete */
    SMBIOSCacheInformation = 7,
    SMBIOSPortConnectorInformation = 8,
    SMBIOSSystemSlots = 9,
    SMBIOSOnBoardDevicesInformation = 10, /* Obsolete */
    SMBIOSOEMStrings = 11,
    SMBIOSSystemConfigurationOptions = 12,
    SMBIOSBIOSLanguageInformation = 13,
    SMBIOSGroupAssociations = 14,
    SMBIOSSystemEventLog = 15,
    SMBIOSPhysicalMemoryArray = 16,
    SMBIOSMemoryDevice = 17,
    SMBIOS32BitMemoryErrorInformation = 18,
    SMBIOSMemoryArrayMappedAddress = 19,
    SMBIOSMemoryDeviceMappedAddress = 20,
    SMBIOSBuiltinPointingDevice = 21,
    SMBIOSPortableBattery = 22,
    SMBIOSSystemReset = 23,
    SMBIOSHardwareSecurity = 24,
    SMBIOSSystemPowerControls = 25,
    SMBIOSVoltageProbe = 26,
    SMBIOSCoolingDevice = 27,
    SMBIOSTemperatureProbe = 28,
    SMBIOSElectricalCurrentProbe = 29,
    SMBIOSOutOfBandRemoteAccess = 30,
    SMBIOSBootIntegrityServicesEntryPoint = 31,
    SMBIOSSystemBootInformation = 32,
    SMBIOS64BitMemoryErrorInformation = 33,
    SMBIOSManagementDevice = 34,
    SMBIOSManagementDeviceComponent = 35,
    SMBIOSManagementDeviceThresholdData = 36,
    SMBIOSMemoryChannel = 37,
    SMBIOSIPMIDeviceInformation = 38,
    SMBIOSSystemPowerSupply = 39,
    SMBIOSAdditionalInformation = 40,
    SMBIOSOnboardDevicesExtendedInformation = 41,
    SMBIOSManagementControllerHostInterface = 42,
    SMBIOSTPMDevice = 43,
    SMBIOSProcessorAdditionalInformation = 44,
    SMBIOSFirmwareInventoryInformation = 45,
    SMBIOSStringProperty = 46,
    SMBIOSInactive = 126,
    SMBIOSEndOfTable = 127
} SMBIOS_INFORMATION_TYPE, *PSMBIOS_INFORMATION_TYPE;

typedef struct _SMBIOS_CACHE_SRAM_TYPE
{
    WORD Other : 1;
    WORD Unknown : 1;
    WORD NonBurst : 1;
    WORD Burst : 1;
    WORD PipelineBurst : 1;
    WORD Synchronous : 1;
    WORD Asynchronous : 1;
    WORD Reserved : 9;
} SMBIOS_CACHE_SRAM_TYPE, *PSMBIOS_CACHE_SRAM_TYPE;
static_assert(sizeof(SMBIOS_CACHE_SRAM_TYPE) == sizeof(WORD));

typedef struct _SMBIOS_TABLE
{
    SMBIOS_TABLE_HEADER Header;
    union
    {
        SMBIOS_PLATFORM_FIRMWARE_INFORMATION PlatformFirmwareInformation; // Type 0
        SMBIOS_SYSTEM_INFORMATION SystemInformation; // Type 1
        SMBIOS_BASEBOARD_INFORMATION BaseboardInformation; // Type 2
        SMBIOS_SYSTEM_ENCLOSURE_OR_CHASSIS  SystemEnclosureOrChassis; // Type 3

        struct
        {
            BYTE SocketDesignation;
            union
            {
                WORD CacheConfiguration;
                struct
                {
                    WORD CacheLevel : 3;
                    WORD CacheSocketed : 1;
                    WORD Reserved0 : 1;
#define SMBIOS_CACHE_LOCATION_INTERNAL    00b
#define SMBIOS_CACHE_LOCATION_EXTERNAL    01b
#define SMBIOS_CACHE_LOCATION_RESERVED    10b
#define SMBIOS_CACHE_LOCATION_UNKNOWN     11b
                    WORD Location : 2;          /* SMBIOS_CACHE_LOCATION_* */
                    WORD Enabled : 1;
#define SMBIOS_CACHE_OPERATIONAL_MODE_WRITE_THROUGH                 00b
#define SMBIOS_CACHE_OPERATIONAL_MODE_WRITE_BACK                    01b
#define SMBIOS_CACHE_OPERATIONAL_MODE_VARIES_WITH_MEMORY_ADDRESS    10b
#define SMBIOS_CACHE_OPERATIONAL_MODE_UNKNOWN                       11b
                    WORD OperationalMode : 2;   /* SMBIOS_CACHE_OPERATIONAL_MODE_* */
                    WORD Reserved1 : 6;
                };
            };
            union
            {
                WORD MaximumCacheSize;
                struct
                {
                    WORD MaxSizeInGranularity : 15;
                    WORD MaxSizeGranularity : 1;
                };
            };
            union
            {
                WORD InstalledSize;
                struct
                {
                    WORD InstalledSizeInGranularity : 15;
                    WORD InstalledSizeGranularity : 1;
                };
            };
            SMBIOS_CACHE_SRAM_TYPE SupportedSRAMType;
            SMBIOS_CACHE_SRAM_TYPE CurrentSRAMType;
            BYTE CacheSpeed;
#define SMBIOS_CACHE_ERROR_CORRECTION_TYPE_OTHER            ((BYTE)01h)
#define SMBIOS_CACHE_ERROR_CORRECTION_TYPE_UNKNOWN          ((BYTE)02h)
#define SMBIOS_CACHE_ERROR_CORRECTION_TYPE_NONE             ((BYTE)03h)
#define SMBIOS_CACHE_ERROR_CORRECTION_TYPE_PARITY           ((BYTE)04h)
#define SMBIOS_CACHE_ERROR_CORRECTION_TYPE_SINGLE_BIT_ECC   ((BYTE)05h)
#define SMBIOS_CACHE_ERROR_CORRECTION_TYPE_MULTI_BIT_ECC    ((BYTE)06h)
            BYTE ErrorCorrectionType;   /* SMBIOS_CACHE_ERROR_CORRECTION_TYPE_* */
#define SMBIOS_CACHE_SYSTEM_CACHE_TYPE_OTHER        ((BYTE)01h)
#define SMBIOS_CACHE_SYSTEM_CACHE_TYPE_UNKNOWN      ((BYTE)02h)
#define SMBIOS_CACHE_SYSTEM_CACHE_TYPE_INSTRUCTION  ((BYTE)03h)
#define SMBIOS_CACHE_SYSTEM_CACHE_TYPE_DATA         ((BYTE)04h)
#define SMBIOS_CACHE_SYSTEM_CACHE_TYPE_UNIFIED      ((BYTE)05h)
            BYTE SystemCacheType;       /* SMBIOS_CACHE_SYSTEM_CACHE_TYPE_* */
#define SMBIOS_CACHE_ASSOCIATIVITY_OTHER                    ((BYTE)01h)
#define SMBIOS_CACHE_ASSOCIATIVITY_UNKNOWN                  ((BYTE)02h)
#define SMBIOS_CACHE_ASSOCIATIVITY_DIRECT_MAPPED            ((BYTE)03h)
#define SMBIOS_CACHE_ASSOCIATIVITY_2_WAY_SET_ASSOCIATIVE    ((BYTE)04h)
#define SMBIOS_CACHE_ASSOCIATIVITY_4_WAY_SET_ASSOCIATIVE    ((BYTE)05h)
#define SMBIOS_CACHE_ASSOCIATIVITY_FULL_ASSOCIATIVE         ((BYTE)06h)
#define SMBIOS_CACHE_ASSOCIATIVITY_8_WAY_SET_ASSOCIATIVE    ((BYTE)07h)
#define SMBIOS_CACHE_ASSOCIATIVITY_16_WAY_SET_ASSOCIATIVE   ((BYTE)08h)
#define SMBIOS_CACHE_ASSOCIATIVITY_12_WAY_SET_ASSOCIATIVE   ((BYTE)09h)
#define SMBIOS_CACHE_ASSOCIATIVITY_24_WAY_SET_ASSOCIATIVE   ((BYTE)0Ah)
#define SMBIOS_CACHE_ASSOCIATIVITY_32_WAY_SET_ASSOCIATIVE   ((BYTE)0Bh)
#define SMBIOS_CACHE_ASSOCIATIVITY_48_WAY_SET_ASSOCIATIVE   ((BYTE)0Ch)
#define SMBIOS_CACHE_ASSOCIATIVITY_64_WAY_SET_ASSOCIATIVE   ((BYTE)0Dh)
#define SMBIOS_CACHE_ASSOCIATIVITY_20_WAY_SET_ASSOCIATIVE   ((BYTE)0Eh)
            BYTE Associativity;         /* SMBIOS_CACHE_ASSOCIATIVITY_* */
            union
            {
                DWORD MaximumCacheSize2;
                struct
                {
                    DWORD MaxSizeInGranularity2 : 31;
                    DWORD MaxSizeGranularity2 : 1;
                };
            };
            union
            {
                DWORD InstalledCacheSize2;
                struct
                {
                    DWORD InstalledCacheSizeInGranularity2 : 31;
                    DWORD InstalledCacheSizeGranularity2 : 1;
                };
            };
        } TYPE_7_CACHE_INFO;
        struct
        {
            BYTE InternalReferenceDesignator;
#define SMBIOS_PORT_CONNECTOR_TYPE_NONE                             ((BYTE)00h)
#define SMBIOS_PORT_CONNECTOR_TYPE_CENTRONICS                       ((BYTE)01h)
#define SMBIOS_PORT_CONNECTOR_TYPE_MINI_CENTRONICS                  ((BYTE)02h)
#define SMBIOS_PORT_CONNECTOR_TYPE_MINI_PROPRIETARY                 ((BYTE)03h)
#define SMBIOS_PORT_CONNECTOR_TYPE_DB_25_PIN_MALE                   ((BYTE)04h)
#define SMBIOS_PORT_CONNECTOR_TYPE_DB_25_PIN_FEMALE                 ((BYTE)05h)
#define SMBIOS_PORT_CONNECTOR_TYPE_DB_15_PIN_MALE                   ((BYTE)06h)
#define SMBIOS_PORT_CONNECTOR_TYPE_DB_15_PIN_FEMALE                 ((BYTE)07h)
#define SMBIOS_PORT_CONNECTOR_TYPE_DB_9_PIN_MALE                    ((BYTE)08h)
#define SMBIOS_PORT_CONNECTOR_TYPE_DB_9_PIN_FEMALE                  ((BYTE)09h)
#define SMBIOS_PORT_CONNECTOR_TYPE_RJ_11                            ((BYTE)0Ah)
#define SMBIOS_PORT_CONNECTOR_TYPE_RJ_45                            ((BYTE)0Bh)
#define SMBIOS_PORT_CONNECTOR_TYPE_50_PIN_MINISCSI                  ((BYTE)0Ch)
#define SMBIOS_PORT_CONNECTOR_TYPE_MINI_DIN                         ((BYTE)0Dh)
#define SMBIOS_PORT_CONNECTOR_TYPE_MICRO_DIN                        ((BYTE)0Eh)
#define SMBIOS_PORT_CONNECTOR_TYPE_PS_2                             ((BYTE)0Fh)
#define SMBIOS_PORT_CONNECTOR_TYPE_INFRARED                         ((BYTE)10h)
#define SMBIOS_PORT_CONNECTOR_TYPE_HP_HIL                           ((BYTE)11h)
#define SMBIOS_PORT_CONNECTOR_TYPE_ACCESS_BUS                       ((BYTE)12h)
#define SMBIOS_PORT_CONNECTOR_TYPE_SSA_SCSI                         ((BYTE)13h)
#define SMBIOS_PORT_CONNECTOR_TYPE_CIRCULAR_DIN_8_MALE              ((BYTE)14h)
#define SMBIOS_PORT_CONNECTOR_TYPE_CIRCULAR_DIN_8_FEMALE            ((BYTE)15h)
#define SMBIOS_PORT_CONNECTOR_TYPE_ON_BOARD_IDE                     ((BYTE)16h)
#define SMBIOS_PORT_CONNECTOR_TYPE_ON_BOARD_FLOPPY                  ((BYTE)17h)
#define SMBIOS_PORT_CONNECTOR_TYPE_9_PIN_DUAL_INLINE                ((BYTE)18h)
#define SMBIOS_PORT_CONNECTOR_TYPE_25_PIN_DUAL_INLINE               ((BYTE)19h)
#define SMBIOS_PORT_CONNECTOR_TYPE_50_PIN_DUAL_INLINE               ((BYTE)1Ah)
#define SMBIOS_PORT_CONNECTOR_TYPE_68_PIN_DUAL_INLINE               ((BYTE)1Bh)
#define SMBIOS_PORT_CONNECTOR_TYPE_ON_BOARD_SOUND_INPUT_FROM_CD_ROM ((BYTE)1Ch)
#define SMBIOS_PORT_CONNECTOR_TYPE_MINI_CENTRONICS_TYPE_14          ((BYTE)1Dh)
#define SMBIOS_PORT_CONNECTOR_TYPE_MINI_CENTRONICS_TYPE_26          ((BYTE)1Eh)
#define SMBIOS_PORT_CONNECTOR_TYPE_MINI_JACK                        ((BYTE)1Fh)
#define SMBIOS_PORT_CONNECTOR_TYPE_BNC                              ((BYTE)20h)
#define SMBIOS_PORT_CONNECTOR_TYPE_1394                             ((BYTE)21h)
#define SMBIOS_PORT_CONNECTOR_TYPE_SAS_SATA_PLUG_RECEPTACLE         ((BYTE)22h)
#define SMBIOS_PORT_CONNECTOR_TYPE_USB_TYPE_C_RECEPTACLE            ((BYTE)23h)
#define SMBIOS_PORT_CONNECTOR_TYPE_PC_98                            ((BYTE)A0h)
#define SMBIOS_PORT_CONNECTOR_TYPE_PC_98HIRESO                      ((BYTE)A1h)
#define SMBIOS_PORT_CONNECTOR_TYPE_PC_H98                           ((BYTE)A2h)
#define SMBIOS_PORT_CONNECTOR_TYPE_PC_98NOTE                        ((BYTE)A3h)
#define SMBIOS_PORT_CONNECTOR_TYPE_PC_98FULL                        ((BYTE)A4h)
#define SMBIOS_PORT_CONNECTOR_TYPE_OTHER                            ((BYTE)FFh)
            BYTE InternalConnectorType; /* SMBIOS_PORT_CONNECTOR_TYPE_* */
            BYTE ExternalReferenceDesignator;
            BYTE ExternalConnectorType; /* SMBIOS_PORT_CONNECTOR_TYPE_* */
#define SMBIOS_PORT_TYPE_NONE                           ((BYTE)00h)
#define SMBIOS_PORT_TYPE_PARALLEL_PORT_XT_AT_COMPATIBLE ((BYTE)01h)
#define SMBIOS_PORT_TYPE_PARALLEL_PORT_PS_2             ((BYTE)02h)
#define SMBIOS_PORT_TYPE_PARALLEL_PORT_ECP              ((BYTE)03h)
#define SMBIOS_PORT_TYPE_PARALLEL_PORT_EPP              ((BYTE)04h)
#define SMBIOS_PORT_TYPE_PARALLEL_PORT_ECP_EPP          ((BYTE)05h)
#define SMBIOS_PORT_TYPE_SERIAL_PORT_XT_AT_COMPATIBLE   ((BYTE)06h)
#define SMBIOS_PORT_TYPE_SERIAL_PORT_16450_COMPATIBLE   ((BYTE)07h)
#define SMBIOS_PORT_TYPE_SERIAL_PORT_16550_COMPATIBLE   ((BYTE)08h)
#define SMBIOS_PORT_TYPE_SERIAL_PORT_16550A_COMPATIBLE  ((BYTE)09h)
#define SMBIOS_PORT_TYPE_SCSI_PORT                      ((BYTE)0Ah)
#define SMBIOS_PORT_TYPE_MIDI_PORT                      ((BYTE)0Bh)
#define SMBIOS_PORT_TYPE_JOY_STICK_PORT                 ((BYTE)0Ch)
#define SMBIOS_PORT_TYPE_KEYBOARD_PORT                  ((BYTE)0Dh)
#define SMBIOS_PORT_TYPE_MOUSE_PORT                     ((BYTE)0Eh)
#define SMBIOS_PORT_TYPE_SSA_SCSI                       ((BYTE)0Fh)
#define SMBIOS_PORT_TYPE_USB                            ((BYTE)10h)
#define SMBIOS_PORT_TYPE_FIRE_WIRE                      ((BYTE)11h)
#define SMBIOS_PORT_TYPE_PCMCIA_TYPE_1                  ((BYTE)12h)
#define SMBIOS_PORT_TYPE_PCMCIA_TYPE_2                  ((BYTE)13h)
#define SMBIOS_PORT_TYPE_PCMCIA_TYPE_3                  ((BYTE)14h)
#define SMBIOS_PORT_TYPE_CARD_BUS                       ((BYTE)15h)
#define SMBIOS_PORT_TYPE_ACCESS_BUS_PORT                ((BYTE)16h)
#define SMBIOS_PORT_TYPE_SCSI_2                         ((BYTE)17h)
#define SMBIOS_PORT_TYPE_SCSI_WIDE                      ((BYTE)18h)
#define SMBIOS_PORT_TYPE_PC_98                          ((BYTE)19h)
#define SMBIOS_PORT_TYPE_PC_98_Hireso                   ((BYTE)1Ah)
#define SMBIOS_PORT_TYPE_PC_H98                         ((BYTE)1Bh)
#define SMBIOS_PORT_TYPE_VIDEO_PORT                     ((BYTE)1Ch)
#define SMBIOS_PORT_TYPE_AUDIO_PORT                     ((BYTE)1Dh)
#define SMBIOS_PORT_TYPE_MODEM_PORT                     ((BYTE)1Eh)
#define SMBIOS_PORT_TYPE_NETWORK_PORT                   ((BYTE)1Fh)
#define SMBIOS_PORT_TYPE_SATA                           ((BYTE)20h)
#define SMBIOS_PORT_TYPE_SAS                            ((BYTE)21h)
#define SMBIOS_PORT_TYPE_MFDP                           ((BYTE)22h)
#define SMBIOS_PORT_TYPE_THUNDERBOLT                    ((BYTE)23h)
#define SMBIOS_PORT_TYPE_8251_COMPATIBLE                ((BYTE)A0h)
#define SMBIOS_PORT_TYPE_8251_FIFO_COMPATIBLE           ((BYTE)A1h)
#define SMBIOS_PORT_TYPE_OTHER                          ((BYTE)FFh)
            BYTE PortType; /* SMBIOS_PORT_TYPE_* */
        } TYPE_8_PORT_CONNECTOR_INFO;
        struct
        {
            BYTE Designation;
#define SMBIOS_SYSTEM_SLOTS_TYPE_OTHER                                  ((BYTE)01h)
#define SMBIOS_SYSTEM_SLOTS_TYPE_UNKNOWN                                ((BYTE)02h)
#define SMBIOS_SYSTEM_SLOTS_TYPE_ISA                                    ((BYTE)03h)
#define SMBIOS_SYSTEM_SLOTS_TYPE_MCA                                    ((BYTE)04h)
#define SMBIOS_SYSTEM_SLOTS_TYPE_EISA                                   ((BYTE)05h)
#define SMBIOS_SYSTEM_SLOTS_TYPE_PCI                                    ((BYTE)06h)
#define SMBIOS_SYSTEM_SLOTS_TYPE_PC_CARD                                ((BYTE)07h)
#define SMBIOS_SYSTEM_SLOTS_TYPE_VL_VESA                                ((BYTE)08h)
#define SMBIOS_SYSTEM_SLOTS_TYPE_PROPRIETARY                            ((BYTE)09h)
#define SMBIOS_SYSTEM_SLOTS_TYPE_PROCESSOR_CARD_SLOT                    ((BYTE)0Ah)
#define SMBIOS_SYSTEM_SLOTS_TYPE_PROPRIETARY_MEMORY_CARD_SLOT           ((BYTE)0Bh)
#define SMBIOS_SYSTEM_SLOTS_TYPE_IO_RISER_CARD_SLOT                     ((BYTE)0Ch)
#define SMBIOS_SYSTEM_SLOTS_TYPE_NUBUS                                  ((BYTE)0Dh)
#define SMBIOS_SYSTEM_SLOTS_TYPE_PCI_66MHZ_CAPABLE                      ((BYTE)0Eh)
#define SMBIOS_SYSTEM_SLOTS_TYPE_AGP                                    ((BYTE)0Fh)
#define SMBIOS_SYSTEM_SLOTS_TYPE_AGP_2X                                 ((BYTE)10h)
#define SMBIOS_SYSTEM_SLOTS_TYPE_AGP_4X                                 ((BYTE)11h)
#define SMBIOS_SYSTEM_SLOTS_TYPE_PCI_X                                  ((BYTE)12h)
#define SMBIOS_SYSTEM_SLOTS_TYPE_AGP_8X                                 ((BYTE)13h)
#define SMBIOS_SYSTEM_SLOTS_TYPE_M2_SOCKET_1_DP                         ((BYTE)14h)
#define SMBIOS_SYSTEM_SLOTS_TYPE_M2_SOCKET_1_SD                         ((BYTE)15h)
#define SMBIOS_SYSTEM_SLOTS_TYPE_M2_SOCKET_2                            ((BYTE)16h)
#define SMBIOS_SYSTEM_SLOTS_TYPE_M2_SOCKET_3                            ((BYTE)17h)
#define SMBIOS_SYSTEM_SLOTS_TYPE_M2_SOCKET_MXM_TYPE_I                   ((BYTE)18h)
#define SMBIOS_SYSTEM_SLOTS_TYPE_M2_SOCKET_MXM_TYPE_II                  ((BYTE)19h)
#define SMBIOS_SYSTEM_SLOTS_TYPE_M2_SOCKET_MXM_TYPE_III_STANDARD        ((BYTE)1Ah)
#define SMBIOS_SYSTEM_SLOTS_TYPE_M2_SOCKET_MXM_TYPE_III_HE              ((BYTE)1Bh)
#define SMBIOS_SYSTEM_SLOTS_TYPE_M2_SOCKET_MXM_TYPE_IV                  ((BYTE)1Ch)
#define SMBIOS_SYSTEM_SLOTS_TYPE_M2_SOCKET_MXM_3_TYPE_A                 ((BYTE)1Dh)
#define SMBIOS_SYSTEM_SLOTS_TYPE_M2_SOCKET_MXM_3_TYPE_B                 ((BYTE)1Eh)
#define SMBIOS_SYSTEM_SLOTS_TYPE_PCI_EXPRESS_GEN_2_SFF_8639             ((BYTE)1Fh)
#define SMBIOS_SYSTEM_SLOTS_TYPE_PCI_EXPRESS_GEN_3_SFF_8639             ((BYTE)20h)
#define SMBIOS_SYSTEM_SLOTS_TYPE_PCI_EXPRESS_MINI_52_PIN_WITH_BOTTOM_SIDE_KEEP_OUTS     ((BYTE)21h)
#define SMBIOS_SYSTEM_SLOTS_TYPE_PCI_EXPRESS_MINI_52_PIN_WITHOUT_BOTTOM_SIDE_KEEP_OUTS  ((BYTE)22h)
#define SMBIOS_SYSTEM_SLOTS_TYPE_PCI_EXPRESS_MINI_76_PIN                ((BYTE)23h)
#define SMBIOS_SYSTEM_SLOTS_TYPE_PCI_EXPRESS_GEN_4_SFF_8639             ((BYTE)24h)
#define SMBIOS_SYSTEM_SLOTS_TYPE_PCI_EXPRESS_GEN_5_SFF_8639             ((BYTE)25h)
#define SMBIOS_SYSTEM_SLOTS_TYPE_OCP_NIC_3_SFF                          ((BYTE)26h)
#define SMBIOS_SYSTEM_SLOTS_TYPE_OCP_NIC_3_LFF                          ((BYTE)27h)
#define SMBIOS_SYSTEM_SLOTS_TYPE_OCP_NIC_PRIOR_TO_3                     ((BYTE)28h)
#define SMBIOS_SYSTEM_SLOTS_TYPE_CXL_FLEXBUS_1                          ((BYTE)30h)
#define SMBIOS_SYSTEM_SLOTS_TYPE_PC_98_C20                              ((BYTE)A0h)
#define SMBIOS_SYSTEM_SLOTS_TYPE_PC_98_C24                              ((BYTE)A1h)
#define SMBIOS_SYSTEM_SLOTS_TYPE_PC_98_E                                ((BYTE)A2h)
#define SMBIOS_SYSTEM_SLOTS_TYPE_PC_98_LOCAL_BUS                        ((BYTE)A3h)
#define SMBIOS_SYSTEM_SLOTS_TYPE_PC_98_CARD                             ((BYTE)A4h)
#define SMBIOS_SYSTEM_SLOTS_TYPE_PCI_EXPRESS                            ((BYTE)A5h)
#define SMBIOS_SYSTEM_SLOTS_TYPE_PCI_EXPRESS_X1                         ((BYTE)A6h)
#define SMBIOS_SYSTEM_SLOTS_TYPE_PCI_EXPRESS_X2                         ((BYTE)A7h)
#define SMBIOS_SYSTEM_SLOTS_TYPE_PCI_EXPRESS_X4                         ((BYTE)A8h)
#define SMBIOS_SYSTEM_SLOTS_TYPE_PCI_EXPRESS_X8                         ((BYTE)A9h)
#define SMBIOS_SYSTEM_SLOTS_TYPE_PCI_EXPRESS_X16                        ((BYTE)AAh)
#define SMBIOS_SYSTEM_SLOTS_TYPE_PCI_EXPRESS_GEN_2                      ((BYTE)ABh)
#define SMBIOS_SYSTEM_SLOTS_TYPE_PCI_EXPRESS_GEN_2_X1                   ((BYTE)ACh)
#define SMBIOS_SYSTEM_SLOTS_TYPE_PCI_EXPRESS_GEN_2_X2                   ((BYTE)ADh)
#define SMBIOS_SYSTEM_SLOTS_TYPE_PCI_EXPRESS_GEN_2_X4                   ((BYTE)AEh)
#define SMBIOS_SYSTEM_SLOTS_TYPE_PCI_EXPRESS_GEN_2_X8                   ((BYTE)AFh)
#define SMBIOS_SYSTEM_SLOTS_TYPE_PCI_EXPRESS_GEN_2_X16                  ((BYTE)B0h)
#define SMBIOS_SYSTEM_SLOTS_TYPE_PCI_EXPRESS_GEN_3                      ((BYTE)B1h)
#define SMBIOS_SYSTEM_SLOTS_TYPE_PCI_EXPRESS_GEN_3_X1                   ((BYTE)B2h)
#define SMBIOS_SYSTEM_SLOTS_TYPE_PCI_EXPRESS_GEN_3_X2                   ((BYTE)B3h)
#define SMBIOS_SYSTEM_SLOTS_TYPE_PCI_EXPRESS_GEN_3_X4                   ((BYTE)B4h)
#define SMBIOS_SYSTEM_SLOTS_TYPE_PCI_EXPRESS_GEN_3_X8                   ((BYTE)B5h)
#define SMBIOS_SYSTEM_SLOTS_TYPE_PCI_EXPRESS_GEN_3_X16                  ((BYTE)B6h)
#define SMBIOS_SYSTEM_SLOTS_TYPE_PCI_EXPRESS_GEN_4                      ((BYTE)B8h)
#define SMBIOS_SYSTEM_SLOTS_TYPE_PCI_EXPRESS_GEN_4_X1                   ((BYTE)B9h)
#define SMBIOS_SYSTEM_SLOTS_TYPE_PCI_EXPRESS_GEN_4_X2                   ((BYTE)BAh)
#define SMBIOS_SYSTEM_SLOTS_TYPE_PCI_EXPRESS_GEN_4_X4                   ((BYTE)BBh)
#define SMBIOS_SYSTEM_SLOTS_TYPE_PCI_EXPRESS_GEN_4_X8                   ((BYTE)BCh)
#define SMBIOS_SYSTEM_SLOTS_TYPE_PCI_EXPRESS_GEN_4_X16                  ((BYTE)BDh)
#define SMBIOS_SYSTEM_SLOTS_TYPE_PCI_EXPRESS_GEN_5                      ((BYTE)BEh)
#define SMBIOS_SYSTEM_SLOTS_TYPE_PCI_EXPRESS_GEN_5_X1                   ((BYTE)BFh)
#define SMBIOS_SYSTEM_SLOTS_TYPE_PCI_EXPRESS_GEN_5_X2                   ((BYTE)C0h)
#define SMBIOS_SYSTEM_SLOTS_TYPE_PCI_EXPRESS_GEN_5_X4                   ((BYTE)C1h)
#define SMBIOS_SYSTEM_SLOTS_TYPE_PCI_EXPRESS_GEN_5_X8                   ((BYTE)C2h)
#define SMBIOS_SYSTEM_SLOTS_TYPE_PCI_EXPRESS_GEN_5_X16                  ((BYTE)C3h)
#define SMBIOS_SYSTEM_SLOTS_TYPE_PCI_EXPRESS_GEN_6_AND_BEYOND           ((BYTE)C4h)
#define SMBIOS_SYSTEM_SLOTS_TYPE_EDSFF_E1S_E1L                          ((BYTE)C5h)
#define SMBIOS_SYSTEM_SLOTS_TYPE_EDSFF_E3S_E3L                          ((BYTE)C6h)
            BYTE Type;              /* SMBIOS_SYSTEM_SLOTS_TYPE_* */
#define SMBIOS_SYSTEM_SLOTS_DATA_BUS_WIDTH_OTHER        ((BYTE)01h)
#define SMBIOS_SYSTEM_SLOTS_DATA_BUS_WIDTH_UNKNOWN      ((BYTE)02h)
#define SMBIOS_SYSTEM_SLOTS_DATA_BUS_WIDTH_8_BIT        ((BYTE)03h)
#define SMBIOS_SYSTEM_SLOTS_DATA_BUS_WIDTH_16_BIT       ((BYTE)04h)
#define SMBIOS_SYSTEM_SLOTS_DATA_BUS_WIDTH_32_BIT       ((BYTE)05h)
#define SMBIOS_SYSTEM_SLOTS_DATA_BUS_WIDTH_64_BIT       ((BYTE)06h)
#define SMBIOS_SYSTEM_SLOTS_DATA_BUS_WIDTH_128_BIT      ((BYTE)07h)
#define SMBIOS_SYSTEM_SLOTS_DATA_BUS_WIDTH_1X           ((BYTE)08h)
#define SMBIOS_SYSTEM_SLOTS_DATA_BUS_WIDTH_2X           ((BYTE)09h)
#define SMBIOS_SYSTEM_SLOTS_DATA_BUS_WIDTH_4X           ((BYTE)0Ah)
#define SMBIOS_SYSTEM_SLOTS_DATA_BUS_WIDTH_8X           ((BYTE)0Bh)
#define SMBIOS_SYSTEM_SLOTS_DATA_BUS_WIDTH_12X          ((BYTE)0Ch)
#define SMBIOS_SYSTEM_SLOTS_DATA_BUS_WIDTH_16X          ((BYTE)0Dh)
#define SMBIOS_SYSTEM_SLOTS_DATA_BUS_WIDTH_32X          ((BYTE)0Eh)
            BYTE SlotDataBusWidth;  /* SMBIOS_SYSTEM_SLOTS_DATA_BUS_WIDTH_* */
#define SMBIOS_SYSTEM_SLOTS_CURRENT_USAGE_OTHER         ((BYTE)01h)
#define SMBIOS_SYSTEM_SLOTS_CURRENT_USAGE_UNKNOWN       ((BYTE)02h)
#define SMBIOS_SYSTEM_SLOTS_CURRENT_USAGE_AVAILABLE     ((BYTE)03h)
#define SMBIOS_SYSTEM_SLOTS_CURRENT_USAGE_IN_USE        ((BYTE)04h)
#define SMBIOS_SYSTEM_SLOTS_CURRENT_USAGE_UNAVAILABLE   ((BYTE)05h)
            BYTE CurrentUsage;      /* SMBIOS_SYSTEM_SLOTS_CURRENT_USAGE_* */
#define SMBIOS_SYSTEM_SLOTS_LENGTH_OTHER                        ((BYTE)01h)
#define SMBIOS_SYSTEM_SLOTS_LENGTH_UNKNOWN                      ((BYTE)02h)
#define SMBIOS_SYSTEM_SLOTS_LENGTH_SHORT                        ((BYTE)03h)
#define SMBIOS_SYSTEM_SLOTS_LENGTH_LONG                         ((BYTE)04h)
#define SMBIOS_SYSTEM_SLOTS_LENGTH_2DOT5_INCH_DRIVE_FORM_FACTOR ((BYTE)05h)
#define SMBIOS_SYSTEM_SLOTS_LENGTH_3DOT5_INCH_DRIVE_FORM_FACTOR ((BYTE)06h)
            BYTE Length;            /* SMBIOS_SYSTEM_SLOTS_LENGTH_* */
            WORD ID;
            union
            {
                BYTE Characteristics1;
                struct
                {
                    BYTE CharacteristicsUnknown : 1;
                    BYTE Provides5Dot0Volts : 1;
                    BYTE Provides3Dot3Volts : 1;
                    BYTE OpeningSharedWithAnother : 1;
                    BYTE PCCardSupportsPCCard16 : 1;
                    BYTE PCCardSupportsCardBus : 1;
                    BYTE PCCardSupportsZoomVideo : 1;
                    BYTE PCCardSupportsModemRingResume : 1;
                };
            };
            union
            {
                BYTE Characteristics2;
                struct
                {
                    BYTE PCISupportsPMESignal : 1;
                    BYTE SupportsHotPlugDevices : 1;
                    BYTE PCISupportsSMBusSignal : 1;
                    BYTE PCIeSupportsBifurcation : 1;
                    BYTE SupportsAsyncOrSurpriseRemoval : 1;
                    BYTE FlexbusCXL1Capable : 1;
                    BYTE FlexbusCXL2Capable : 1;
                    BYTE FlexbusCXL3Capable : 1;
                };
            };
            WORD SegmentGroupNumber;
            BYTE BusNumber;
            union
            {
                BYTE DeviceFunctionNumber;
                struct
                {
                    BYTE FunctionNumber : 3;
                    BYTE DeviceNumber : 5;
                };
            };
            BYTE DataBusWidth;
            BYTE PeerGroupingCount;
            _Field_size_bytes_(5 * PeerGroupingCount) BYTE PeerGroups[];
            /* BYTE Information; */
            /* BYTE PhysicalWidth; */
            /* WORD Pitch; */
            /* BYTE Height; */
        } TYPE_9_SYSTEM_SLOTS;
        struct
        {
#define SMBIOS_ONBOARD_DEVICES_TYPE_OTHER           ((BYTE)01h)
#define SMBIOS_ONBOARD_DEVICES_TYPE_UNKNOWN         ((BYTE)02h)
#define SMBIOS_ONBOARD_DEVICES_TYPE_VIDEO           ((BYTE)03h)
#define SMBIOS_ONBOARD_DEVICES_TYPE_SCSI_CONTROLLER ((BYTE)04h)
#define SMBIOS_ONBOARD_DEVICES_TYPE_ETHERNET        ((BYTE)05h)
#define SMBIOS_ONBOARD_DEVICES_TYPE_TOKEN_RING      ((BYTE)06h)
#define SMBIOS_ONBOARD_DEVICES_TYPE_SOUND           ((BYTE)07h)
#define SMBIOS_ONBOARD_DEVICES_TYPE_PATA_CONTROLLER ((BYTE)08h)
#define SMBIOS_ONBOARD_DEVICES_TYPE_SATA_CONTROLLER ((BYTE)09h)
#define SMBIOS_ONBOARD_DEVICES_TYPE_SAS_CONTROLLER  ((BYTE)0Ah)
            _Field_size_((Header.Length - sizeof(Header)) / 2) BYTE Type[]; /* SMBIOS_ONBOARD_DEVICES_TYPE_* */
            /* _Field_size_((Header.Length - sizeof(Header)) / 2) BYTE DescriptionString[]; */
        } TYPE_10_OBSOLETE_ON_BOARD_DEVICES_INFO;
        struct
        {
            BYTE Count;
        } TYPE_11_OEM_STRINGS;
        struct
        {
            BYTE Count;
        } TYPE_12_SYSTEM_CONFIGURATION_OPTIONS;
        struct
        {
            BYTE InstallableLanguages;
            union
            {
                BYTE Flags;
                struct
                {
                    BYTE AbbreviatedFormat : 1;
                    BYTE Reserved0 : 7;
                };
            };
            BYTE Reserved1[15];
            BYTE CurrentLanguage;
        } TYPE_13_BIOS_LANGUAGE_INFO;
        struct
        {
            BYTE GroupName;
            BYTE ItemType1;
            WORD ItemHandle1;
            /* ItemType2, ItemHandle2, ..., ItemTypeN, ItemHandleN */
        } TYPE_14_GROUP_ASSOCIATIONS;
        struct
        {
#define SMBIOS_PHYSICAL_MEMORY_ARRAY_LOCATION_OTHER                         ((BYTE)01h)
#define SMBIOS_PHYSICAL_MEMORY_ARRAY_LOCATION_UNKNOWN                       ((BYTE)02h)
#define SMBIOS_PHYSICAL_MEMORY_ARRAY_LOCATION_SYSTEM_BOARD_OR_MOTHERBOARD   ((BYTE)03h)
#define SMBIOS_PHYSICAL_MEMORY_ARRAY_LOCATION_ISA_ADD_ON_CARD               ((BYTE)04h)
#define SMBIOS_PHYSICAL_MEMORY_ARRAY_LOCATION_EISA_ADD_ON_CARD              ((BYTE)05h)
#define SMBIOS_PHYSICAL_MEMORY_ARRAY_LOCATION_PCI_ADD_ON_CARD               ((BYTE)06h)
#define SMBIOS_PHYSICAL_MEMORY_ARRAY_LOCATION_MCA_ADD_ON_CARD               ((BYTE)07h)
#define SMBIOS_PHYSICAL_MEMORY_ARRAY_LOCATION_PCMCIA_ADD_ON_CARD            ((BYTE)08h)
#define SMBIOS_PHYSICAL_MEMORY_ARRAY_LOCATION_PROPRIETARY_ADD_ON_CARD       ((BYTE)09h)
#define SMBIOS_PHYSICAL_MEMORY_ARRAY_LOCATION_NUBUS                         ((BYTE)0Ah)
#define SMBIOS_PHYSICAL_MEMORY_ARRAY_LOCATION_PC_98_C20_ADD_ON_CARD         ((BYTE)A0h)
#define SMBIOS_PHYSICAL_MEMORY_ARRAY_LOCATION_PC_98_C24_ADD_ON_CARD         ((BYTE)A1h)
#define SMBIOS_PHYSICAL_MEMORY_ARRAY_LOCATION_PC_98_E_ADD_ON_CARD           ((BYTE)A2h)
#define SMBIOS_PHYSICAL_MEMORY_ARRAY_LOCATION_PC_98_LOCAL_BUS_ADD_ON_CARD   ((BYTE)A3h)
#define SMBIOS_PHYSICAL_MEMORY_ARRAY_LOCATION_CXL_ADD_ON_CARD               ((BYTE)A4h)
            BYTE Location;          /* SMBIOS_PHYSICAL_MEMORY_ARRAY_LOCATION_* */
#define SMBIOS_PHYSICAL_MEMORY_ARRAY_USE_OTHER              ((BYTE)01h)
#define SMBIOS_PHYSICAL_MEMORY_ARRAY_USE_UNKNOWN            ((BYTE)02h)
#define SMBIOS_PHYSICAL_MEMORY_ARRAY_USE_SYSTEM_MEMORY      ((BYTE)03h)
#define SMBIOS_PHYSICAL_MEMORY_ARRAY_USE_VIDEO_MEMORY       ((BYTE)04h)
#define SMBIOS_PHYSICAL_MEMORY_ARRAY_USE_FLASH_MEMORY       ((BYTE)05h)
#define SMBIOS_PHYSICAL_MEMORY_ARRAY_USE_NON_VOLATILE_RAM   ((BYTE)06h)
#define SMBIOS_PHYSICAL_MEMORY_ARRAY_USE_CACHE_MEMORY       ((BYTE)07h)
            BYTE Use;               /* SMBIOS_PHYSICAL_MEMORY_ARRAY_USE_* */
#define SMBIOS_PHYSICAL_MEMORY_ARRAY_ERROR_CORRECTION_TYPE_OTHER            ((BYTE)01h)
#define SMBIOS_PHYSICAL_MEMORY_ARRAY_ERROR_CORRECTION_TYPE_UNKNOWN          ((BYTE)02h)
#define SMBIOS_PHYSICAL_MEMORY_ARRAY_ERROR_CORRECTION_TYPE_NONE             ((BYTE)03h)
#define SMBIOS_PHYSICAL_MEMORY_ARRAY_ERROR_CORRECTION_TYPE_PARITY           ((BYTE)04h)
#define SMBIOS_PHYSICAL_MEMORY_ARRAY_ERROR_CORRECTION_TYPE_SINGLE_BIT_ECC   ((BYTE)05h)
#define SMBIOS_PHYSICAL_MEMORY_ARRAY_ERROR_CORRECTION_TYPE_MULTI_BIT_ECC    ((BYTE)06h)
#define SMBIOS_PHYSICAL_MEMORY_ARRAY_ERROR_CORRECTION_TYPE_CRC              ((BYTE)07h)
            BYTE ErrorCorrection;   /* SMBIOS_PHYSICAL_MEMORY_ARRAY_ERROR_CORRECTION_TYPE_* */
            DWORD MaximumCapacity;
            WORD ErrorInformationHandle;
            WORD NumberOfMemoryDevices;
            QWORD ExtendedMaximumCapacity;
        } TYPE_16_PHYSICAL_MEMORY_ARRAY;
        struct
        {
            WORD PhysicalMemoryArrayHandle;
            WORD ErrorInformationHandle;
            WORD TotalWidth;
            WORD DataWidth;
            WORD Size;
#define SMBIOS_MEMORY_DEVICE_FORM_FACTOR_OTHER              ((BYTE)01h)
#define SMBIOS_MEMORY_DEVICE_FORM_FACTOR_UNKNOWN            ((BYTE)02h)
#define SMBIOS_MEMORY_DEVICE_FORM_FACTOR_SIMM               ((BYTE)03h)
#define SMBIOS_MEMORY_DEVICE_FORM_FACTOR_SIP                ((BYTE)04h)
#define SMBIOS_MEMORY_DEVICE_FORM_FACTOR_CHIP               ((BYTE)05h)
#define SMBIOS_MEMORY_DEVICE_FORM_FACTOR_DIP                ((BYTE)06h)
#define SMBIOS_MEMORY_DEVICE_FORM_FACTOR_ZIP                ((BYTE)07h)
#define SMBIOS_MEMORY_DEVICE_FORM_FACTOR_PROPRIETARY_CARD   ((BYTE)08h)
#define SMBIOS_MEMORY_DEVICE_FORM_FACTOR_DIMM               ((BYTE)09h)
#define SMBIOS_MEMORY_DEVICE_FORM_FACTOR_TSOP               ((BYTE)0Ah)
#define SMBIOS_MEMORY_DEVICE_FORM_FACTOR_ROW_OF_CHIPS       ((BYTE)0Bh)
#define SMBIOS_MEMORY_DEVICE_FORM_FACTOR_RIMM               ((BYTE)0Ch)
#define SMBIOS_MEMORY_DEVICE_FORM_FACTOR_SODIMM             ((BYTE)0Dh)
#define SMBIOS_MEMORY_DEVICE_FORM_FACTOR_SRIMM              ((BYTE)0Eh)
#define SMBIOS_MEMORY_DEVICE_FORM_FACTOR_FB_DIMM            ((BYTE)0Fh)
#define SMBIOS_MEMORY_DEVICE_FORM_FACTOR_DIE                ((BYTE)10h)
            BYTE FormFactor;    /* SMBIOS_MEMORY_DEVICE_FORM_FACTOR_* */
            BYTE DeviceSet;
            BYTE DeviceLocator;
            BYTE BankLocator;
#define SMBIOS_MEMORY_DEVICE_TYPE_OTHER                         ((BYTE)01h)
#define SMBIOS_MEMORY_DEVICE_TYPE_UNKNOWN                       ((BYTE)02h)
#define SMBIOS_MEMORY_DEVICE_TYPE_DRAM                          ((BYTE)03h)
#define SMBIOS_MEMORY_DEVICE_TYPE_EDRAM                         ((BYTE)04h)
#define SMBIOS_MEMORY_DEVICE_TYPE_VRAM                          ((BYTE)05h)
#define SMBIOS_MEMORY_DEVICE_TYPE_SRAM                          ((BYTE)06h)
#define SMBIOS_MEMORY_DEVICE_TYPE_RAM                           ((BYTE)07h)
#define SMBIOS_MEMORY_DEVICE_TYPE_ROM                           ((BYTE)08h)
#define SMBIOS_MEMORY_DEVICE_TYPE_FLASH                         ((BYTE)09h)
#define SMBIOS_MEMORY_DEVICE_TYPE_EEPROM                        ((BYTE)0Ah)
#define SMBIOS_MEMORY_DEVICE_TYPE_FEPROM                        ((BYTE)0Bh)
#define SMBIOS_MEMORY_DEVICE_TYPE_EPROM                         ((BYTE)0Ch)
#define SMBIOS_MEMORY_DEVICE_TYPE_CDRAM                         ((BYTE)0Dh)
#define SMBIOS_MEMORY_DEVICE_TYPE_3DRAM                         ((BYTE)0Eh)
#define SMBIOS_MEMORY_DEVICE_TYPE_SDRAM                         ((BYTE)0Fh)
#define SMBIOS_MEMORY_DEVICE_TYPE_SGRAM                         ((BYTE)10h)
#define SMBIOS_MEMORY_DEVICE_TYPE_RDRAM                         ((BYTE)11h)
#define SMBIOS_MEMORY_DEVICE_TYPE_DDR                           ((BYTE)12h)
#define SMBIOS_MEMORY_DEVICE_TYPE_DDR2                          ((BYTE)13h)
#define SMBIOS_MEMORY_DEVICE_TYPE_DDR2_FB_DIMM                  ((BYTE)14h)
#define SMBIOS_MEMORY_DEVICE_TYPE_DDR3                          ((BYTE)18h)
#define SMBIOS_MEMORY_DEVICE_TYPE_FBD2                          ((BYTE)19h)
#define SMBIOS_MEMORY_DEVICE_TYPE_DDR4                          ((BYTE)1Ah)
#define SMBIOS_MEMORY_DEVICE_TYPE_LPDDR                         ((BYTE)1Bh)
#define SMBIOS_MEMORY_DEVICE_TYPE_LPDDR2                        ((BYTE)1Ch)
#define SMBIOS_MEMORY_DEVICE_TYPE_LPDDR3                        ((BYTE)1Dh)
#define SMBIOS_MEMORY_DEVICE_TYPE_LPDDR4                        ((BYTE)1Eh)
#define SMBIOS_MEMORY_DEVICE_TYPE_LOGICAL_NON_VOLATILE_DEVICE   ((BYTE)1Fh)
#define SMBIOS_MEMORY_DEVICE_TYPE_HBM                           ((BYTE)20h)
#define SMBIOS_MEMORY_DEVICE_TYPE_HBM2                          ((BYTE)21h)
#define SMBIOS_MEMORY_DEVICE_TYPE_DDR5                          ((BYTE)22h)
#define SMBIOS_MEMORY_DEVICE_TYPE_LPDDR5                        ((BYTE)23h)
#define SMBIOS_MEMORY_DEVICE_TYPE_HBM3                          ((BYTE)24h)
            BYTE Type;          /* SMBIOS_MEMORY_DEVICE_TYPE_* */
            union
            {
                WORD TypeDetail;
                struct
                {
                    WORD Reserved0 : 1;
                    WORD TypeOther : 1;
                    WORD TypeUnknow : 1;
                    WORD FastPaged : 1;
                    WORD StaticColumn : 1;
                    WORD PseudoStatic : 1;
                    WORD RAMBUS : 1;
                    WORD Synchronous : 1;
                    WORD CMOS : 1;
                    WORD EDO : 1;
                    WORD WindowDRAM : 1;
                    WORD CacheDRAM : 1;
                    WORD NonVolatile : 1;
                    WORD Registered : 1;
                    WORD Unbuffered : 1;
                    WORD LRDIMM : 1;
                };
            };
            WORD Speed;
            BYTE Manufacturer;
            BYTE SerialNumber;
            BYTE AssetTag;
            BYTE PartNumber;
            union
            {
                BYTE Attributes;
                struct
                {
                    BYTE Rank : 4;
                    BYTE Reserved1 : 4;
                };
            };
            DWORD ExtendedSize;
            WORD ConfiguredMemorySpeed;
            WORD MinimumVoltage;
            WORD MaximumVoltage;
            WORD ConfiguredVoltage;
#define SMBIOS_MEMORY_DEVICE_TECHNOLOGY_OTHER                           ((BYTE)01h)
#define SMBIOS_MEMORY_DEVICE_TECHNOLOGY_UNKNOWN                         ((BYTE)02h)
#define SMBIOS_MEMORY_DEVICE_TECHNOLOGY_DRAM                            ((BYTE)03h)
#define SMBIOS_MEMORY_DEVICE_TECHNOLOGY_NVDIMM_N                        ((BYTE)04h)
#define SMBIOS_MEMORY_DEVICE_TECHNOLOGY_NVDIMM_F                        ((BYTE)05h)
#define SMBIOS_MEMORY_DEVICE_TECHNOLOGY_NVDIMM_P                        ((BYTE)06h)
#define SMBIOS_MEMORY_DEVICE_TECHNOLOGY_INTEL_OPTANE_PERSISTENT_MEMORY  ((BYTE)07h)
            BYTE Technology;    /* SMBIOS_MEMORY_DEVICE_TECHNOLOGY_* */
            union
            {
                WORD OperatingModeCapability;
                struct
                {
                    WORD Reserved2 : 1;
                    WORD OperatingModeCapabilityOther : 1;
                    WORD OperatingModeCapabilityUnknown : 1;
                    WORD VolatileMemory : 1;
                    WORD ByteAccessiblePersistentMemory : 1;
                    WORD BlockAccessiblePersistentMemory : 1;
                    WORD Reserved3 : 10;
                };
            };
            BYTE FirmwareVersion;
            WORD ModuleManufacturerID;
            WORD ModuleProductID;
            WORD SubsystemControllerManufacturerID;
            QWORD NonVolatileSize;
            QWORD VolatileSize;
            QWORD CacheSize;
            QWORD LogicalSize;
            DWORD ExtendedSpeed;
            DWORD ExtendedConfiguredSpeed;
            WORD PMIC0ManufacturerID;
            WORD PMIC0RevisionNumber;
            WORD RCDManufacturerID;
            WORD RCDRevisionNumber;
        } TYPE_17_MEMORY_DEVICE;
        struct
        {
            DWORD StartingAddress;
            DWORD EndingAddress;
            WORD Handle;
            BYTE PartitionWidth;
            QWORD ExtendedStartingAddress;
            QWORD ExtendedEndingAddress;
        } TYPE_19_MEMORY_ARRAY_MAPPED_ADDRESS;
        struct
        {
            DWORD StartingAddress;
            DWORD EndingAddress;
            WORD Handle;
            WORD MemoryArrayMappedAddressHandle;
            BYTE PartitionRowPosition;
            BYTE InterleavePosition;
            BYTE InterleavedDataDepth;
            QWORD ExtendedStartingAddress;
            QWORD ExtendedEndingAddress;
        } TYPE_20_MEMORY_DEVICE_MAPPED_ADDRESS;
        struct
        {
            union
            {
                BYTE Capabilities;
                struct
                {
                    BYTE Status : 1;
#define SMBIOS_SYSTEM_RESET_BOOT_OPTION_RESERVED            00b
#define SMBIOS_SYSTEM_RESET_BOOT_OPTION_OPERATING_SYSTEM    01b
#define SMBIOS_SYSTEM_RESET_BOOT_OPTION_SYSTEM_UTILITIES    10b
#define SMBIOS_SYSTEM_RESET_BOOT_OPTION_DO_NOT_REBOOT       11b
                    BYTE BootOption : 2;        /* SMBIOS_SYSTEM_RESET_BOOT_OPTION_* */
                    BYTE BootOptionOnLimit : 2; /* SMBIOS_SYSTEM_RESET_BOOT_OPTION_* */
                    BYTE SystemContainsAWatchdogTimer : 1;
                    BYTE Reserved : 2;
                };
            };
            WORD ResetCount;
            WORD ResetLimit;
            WORD TimerInterval;
            WORD Timeout;
        } TYPE_23_SYSTEM_RESET;
        struct
        {
            union
            {
                BYTE Settings;
#define SMBIOS_HARDWARE_SECURITY_SETTINGS_STATUS_DISABLED           00b
#define SMBIOS_HARDWARE_SECURITY_SETTINGS_STATUS_ENABLED            01b
#define SMBIOS_HARDWARE_SECURITY_SETTINGS_STATUS_NOT_IMPLEMENTED    10b
#define SMBIOS_HARDWARE_SECURITY_SETTINGS_STATUS_UNKNOWN            11b
                struct
                {
                    /* SMBIOS_HARDWARE_SECURITY_SETTINGS_STATUS_* */
                    BYTE FrontPanelResetStatus : 2;
                    BYTE AdministratorPasswordStatus : 2;
                    BYTE KeyboardPasswordStatus : 2;
                    BYTE PowerOnPasswordStatus : 2;
                };
            };
        } TYPE_24_HARDWARE_SECURITY;
        struct
        {
            BYTE NextScheduledPowerOnMonth;
            BYTE NextScheduledPowerOnDayOfMonth;
            BYTE NextScheduledPowerOnHour;
            BYTE NextScheduledPowerOnMinute;
            BYTE NextScheduledPowerOnSecond;
        } TYPE_25_SYSTEM_POWER_CONTROLS;
        struct
        {
            BYTE Description;
            union
            {
                BYTE LocationAndStatus;
                struct
                {
#define SMBIOS_VOLTAGE_PROBE_LOCATION_OTHER                     00001b
#define SMBIOS_VOLTAGE_PROBE_LOCATION_UNKNOWN                   00010b
#define SMBIOS_VOLTAGE_PROBE_LOCATION_PROCESSOR                 00011b
#define SMBIOS_VOLTAGE_PROBE_LOCATION_DISK                      00100b
#define SMBIOS_VOLTAGE_PROBE_LOCATION_PERIPHERAL_BAY            00101b
#define SMBIOS_VOLTAGE_PROBE_LOCATION_SYSTEM_MANAGEMENT_MODULE  00110b
#define SMBIOS_VOLTAGE_PROBE_LOCATION_MOTHERBOARD               00111b
#define SMBIOS_VOLTAGE_PROBE_LOCATION_MEMORY_MODULE             01000b
#define SMBIOS_VOLTAGE_PROBE_LOCATION_PROCESSOR_MODULE          01001b
#define SMBIOS_VOLTAGE_PROBE_LOCATION_POWER_UNIT                01010b
#define SMBIOS_VOLTAGE_PROBE_LOCATION_ADD_IN_CARD               01011b
                    BYTE Location : 5;  /* SMBIOS_VOLTAGE_PROBE_LOCATION_* */
#define SMBIOS_VOLTAGE_PROBE_STATUS_OTHER           001b
#define SMBIOS_VOLTAGE_PROBE_STATUS_UNKNOWN         010b
#define SMBIOS_VOLTAGE_PROBE_STATUS_OK              011b
#define SMBIOS_VOLTAGE_PROBE_STATUS_NON_CRITICAL    100b
#define SMBIOS_VOLTAGE_PROBE_STATUS_CRITICAL        101b
#define SMBIOS_VOLTAGE_PROBE_STATUS_NON_RECOVERABLE 110b
                    BYTE Status : 3;    /* SMBIOS_VOLTAGE_PROBE_STATUS_* */
                };
            };
        } TYPE_26_VOLTAGE_PROBE;
        struct
        {
            WORD TemperatureProbeHandle;
            union
            {
                BYTE DeviceTypeAndStatus;
                struct
                {
#define SMBIOS_COOLING_DEVICE_TYPE_FAN                      00011b
#define SMBIOS_COOLING_DEVICE_TYPE_CENTRIFUGAL_BLOWER       00100b
#define SMBIOS_COOLING_DEVICE_TYPE_CHIP_FAN                 00101b
#define SMBIOS_COOLING_DEVICE_TYPE_CABINET_FAN              00110b
#define SMBIOS_COOLING_DEVICE_TYPE_POWER_SUPPLY_FAN         00111b
#define SMBIOS_COOLING_DEVICE_TYPE_HEAT_PIPE                01000b
#define SMBIOS_COOLING_DEVICE_TYPE_INTEGRATED_REFRIGERATION 01001b
#define SMBIOS_COOLING_DEVICE_TYPE_ACTIVE_COOLING           10000b
#define SMBIOS_COOLING_DEVICE_TYPE_PASSIVE_COOLING          10001b
                    BYTE Type : 5;      /* SMBIOS_COOLING_DEVICE_TYPE_* */
#define SMBIOS_COOLING_DEVICE_STATUS_OTHER              001b
#define SMBIOS_COOLING_DEVICE_STATUS_UNKNOWN            010b
#define SMBIOS_COOLING_DEVICE_STATUS_OK                 011b
#define SMBIOS_COOLING_DEVICE_STATUS_NON_CRITICAL       100b
#define SMBIOS_COOLING_DEVICE_STATUS_CRITICAL           101b
#define SMBIOS_COOLING_DEVICE_STATUS_NON_RECOVERABLE    110b
                    BYTE Status : 3;    /* SMBIOS_COOLING_DEVICE_STATUS_* */
                };
            };
            BYTE CoolingUnitGroup;
            DWORD OEMDefined;
            WORD NominalSpeed;
            BYTE Description;
        } TYPE_27_COOLING_DEVICE;
        struct
        {
            BYTE Description;
            union
            {
                BYTE LocationAndStatus;
                struct
                {
#define SMBIOS_TEMPERATURE_PROBE_LOCATION_OTHER                     00001b
#define SMBIOS_TEMPERATURE_PROBE_LOCATION_UNKNOWN                   00010b
#define SMBIOS_TEMPERATURE_PROBE_LOCATION_PROCESSOR                 00011b
#define SMBIOS_TEMPERATURE_PROBE_LOCATION_DISK                      00100b
#define SMBIOS_TEMPERATURE_PROBE_LOCATION_PERIPHERAL_BAY            00101b
#define SMBIOS_TEMPERATURE_PROBE_LOCATION_SYSTEM_MANAGEMENT_MODULE  00110b
#define SMBIOS_TEMPERATURE_PROBE_LOCATION_MOTHERBOARD               00111b
#define SMBIOS_TEMPERATURE_PROBE_LOCATION_MEMORY_MODULE             01000b
#define SMBIOS_TEMPERATURE_PROBE_LOCATION_PROCESSOR_MODULE          01001b
#define SMBIOS_TEMPERATURE_PROBE_LOCATION_POWER_UNIT                01010b
#define SMBIOS_TEMPERATURE_PROBE_LOCATION_ADD_IN_CARD               01011b
#define SMBIOS_TEMPERATURE_PROBE_LOCATION_FRONT_PANEL_BOARD         01100b
#define SMBIOS_TEMPERATURE_PROBE_LOCATION_BACK_PANEL_BOARD          01101b
#define SMBIOS_TEMPERATURE_PROBE_LOCATION_POWER_SYSTEM_BOARD        01110b
#define SMBIOS_TEMPERATURE_PROBE_LOCATION_DRIVE_BACK_PLANE          01111b
                    BYTE Location : 5;  /* SMBIOS_TEMPERATURE_PROBE_LOCATION_* */
#define SMBIOS_TEMPERATURE_PROBE_STATUS_OTHER           001b
#define SMBIOS_TEMPERATURE_PROBE_STATUS_UNKNOWN         010b
#define SMBIOS_TEMPERATURE_PROBE_STATUS_OK              011b
#define SMBIOS_TEMPERATURE_PROBE_STATUS_NON_CRITICAL    100b
#define SMBIOS_TEMPERATURE_PROBE_STATUS_CRITICAL        101b
#define SMBIOS_TEMPERATURE_PROBE_STATUS_NON_RECOVERABLE 110b
                    BYTE Status : 3;    /* SMBIOS_TEMPERATURE_PROBE_STATUS_* */
                };
            };
            WORD MaximumValue;
            WORD MinimumValue;
            WORD Resolution;
            WORD Tolerance;
            WORD Accuracy;
            DWORD OEMDefined;
            WORD NominalValue;
        } TYPE_28_TEMPERATURE_PROBE;
        struct
        {
            BYTE Description;
            union
            {
                BYTE LocationAndStatus;
                struct
                {
#define SMBIOS_ELECTRICAL_CURRENT_PROBE_LOCATION_OTHER                      00001b
#define SMBIOS_ELECTRICAL_CURRENT_PROBE_LOCATION_UNKNOWN                    00010b
#define SMBIOS_ELECTRICAL_CURRENT_PROBE_LOCATION_PROCESSOR                  00011b
#define SMBIOS_ELECTRICAL_CURRENT_PROBE_LOCATION_DISK                       00100b
#define SMBIOS_ELECTRICAL_CURRENT_PROBE_LOCATION_PERIPHERAL_BAY             00101b
#define SMBIOS_ELECTRICAL_CURRENT_PROBE_LOCATION_SYSTEM_MANAGEMENT_MODULE   00110b
#define SMBIOS_ELECTRICAL_CURRENT_PROBE_LOCATION_MOTHERBOARD                00111b
#define SMBIOS_ELECTRICAL_CURRENT_PROBE_LOCATION_MEMORY_MODULE              01000b
#define SMBIOS_ELECTRICAL_CURRENT_PROBE_LOCATION_PROCESSOR_MODULE           01001b
#define SMBIOS_ELECTRICAL_CURRENT_PROBE_LOCATION_POWER_UNIT                 01010b
#define SMBIOS_ELECTRICAL_CURRENT_PROBE_LOCATION_ADD_IN_CARD                01011b
                    BYTE Location : 5;  /* SMBIOS_ELECTRICAL_CURRENT_PROBE_LOCATION_* */
#define SMBIOS_ELECTRICAL_CURRENT_PROBE_STATUS_OTHER            001b
#define SMBIOS_ELECTRICAL_CURRENT_PROBE_STATUS_UNKNOWN          010b
#define SMBIOS_ELECTRICAL_CURRENT_PROBE_STATUS_OK               011b
#define SMBIOS_ELECTRICAL_CURRENT_PROBE_STATUS_NON_CRITICAL     100b
#define SMBIOS_ELECTRICAL_CURRENT_PROBE_STATUS_CRITICAL         101b
#define SMBIOS_ELECTRICAL_CURRENT_PROBE_STATUS_NON_RECOVERABLE  110b
                    BYTE Status : 3;    /* SMBIOS_ELECTRICAL_CURRENT_PROBE_STATUS_* */
                };
            };
            WORD MaximumValue;
            WORD MinimumValue;
            WORD Resolution;
            WORD Tolerance;
            WORD Accuracy;
            DWORD OEMDefined;
            WORD NominalValue;
        } TYPE_29_ELECTRICAL_CURRENT_PROBE;
        struct
        {
            BYTE ManufacturerName;
            union
            {
                BYTE Connections;
                struct
                {
                    BYTE InboundConnectionEnabled : 1;
                    BYTE OutboundConnectionEnabled : 1;
                    BYTE Reserved : 6;
                };
            };
        } TYPE_30_OUT_OF_BAND_REMOTE_ACCESS;
        struct
        {
            BYTE Reserved[6];
            BYTE BootStatus[10];
        } TYPE_32_SYSTEM_BOOT_INFORMATION;
        struct
        {
            BYTE Description;
#define SMBIOS_MANAGEMENT_DEVICE_TYPE_OTHER                         ((BYTE)01h)
#define SMBIOS_MANAGEMENT_DEVICE_TYPE_UNKNOWN                       ((BYTE)02h)
#define SMBIOS_MANAGEMENT_DEVICE_TYPE_NATIONAL_SEMICONDUCTOR_LM75   ((BYTE)03h)
#define SMBIOS_MANAGEMENT_DEVICE_TYPE_NATIONAL_SEMICONDUCTOR_LM78   ((BYTE)04h)
#define SMBIOS_MANAGEMENT_DEVICE_TYPE_NATIONAL_SEMICONDUCTOR_LM79   ((BYTE)05h)
#define SMBIOS_MANAGEMENT_DEVICE_TYPE_NATIONAL_SEMICONDUCTOR_LM80   ((BYTE)06h)
#define SMBIOS_MANAGEMENT_DEVICE_TYPE_NATIONAL_SEMICONDUCTOR_LM81   ((BYTE)07h)
#define SMBIOS_MANAGEMENT_DEVICE_TYPE_ANALOG_DEVICES_ADM9240        ((BYTE)08h)
#define SMBIOS_MANAGEMENT_DEVICE_TYPE_DALLAS_SEMICONDUCTOR_DS1780   ((BYTE)09h)
#define SMBIOS_MANAGEMENT_DEVICE_TYPE_MAXIM_1617                    ((BYTE)0Ah)
#define SMBIOS_MANAGEMENT_DEVICE_TYPE_GENESYS_GL518SM               ((BYTE)0Bh)
#define SMBIOS_MANAGEMENT_DEVICE_TYPE_WINBOND_W83781D               ((BYTE)0Ch)
#define SMBIOS_MANAGEMENT_DEVICE_TYPE_HOLTEK_HT82H791               ((BYTE)0Dh)
            BYTE Type;          /* SMBIOS_MANAGEMENT_DEVICE_TYPE_* */
            DWORD Address;
#define SMBIOS_MANAGEMENT_DEVICE_ADDRESS_TYPE_OTHER     ((BYTE)01h)
#define SMBIOS_MANAGEMENT_DEVICE_ADDRESS_TYPE_UNKNOWN   ((BYTE)02h)
#define SMBIOS_MANAGEMENT_DEVICE_ADDRESS_TYPE_IO_PORT   ((BYTE)03h)
#define SMBIOS_MANAGEMENT_DEVICE_ADDRESS_TYPE_MEMORY    ((BYTE)04h)
#define SMBIOS_MANAGEMENT_DEVICE_ADDRESS_TYPE_SMBUS     ((BYTE)05h)
            BYTE AddressType;   /* SMBIOS_MANAGEMENT_DEVICE_ADDRESS_TYPE_* */
        } TYPE_34_MANAGEMENT_DEVICE;
        struct
        {
            BYTE Description;
            WORD ManagementDeviceHandle;
            WORD ComponentHandle;
            WORD ThresholdHandle;
        } TYPE_35_MANAGEMENT_DEVICE_COMPONENT;
        struct
        {
            BYTE PowerUnitGroup;
            BYTE Location;
            BYTE DeviceName;
            BYTE Manufacturer;
            BYTE SerialNumber;
            BYTE AssetTagNumber;
            BYTE ModelPartNumber;
            BYTE RevisionLevel;
            WORD MaxPowerCapacity;
            union
            {
                WORD Characteristics;
                struct
                {
                    WORD HotReplaceable : 1;
                    WORD Present : 1;
                    WORD UnpluggedFromTheWall : 1;
#define SMBIOS_SYSTEM_POWER_SUPPLY_INPUT_VOLTAGE_RANGE_SWITCHING_OTHER          0001b
#define SMBIOS_SYSTEM_POWER_SUPPLY_INPUT_VOLTAGE_RANGE_SWITCHING_UNKNOWN        0010b
#define SMBIOS_SYSTEM_POWER_SUPPLY_INPUT_VOLTAGE_RANGE_SWITCHING_MANUAL         0011b
#define SMBIOS_SYSTEM_POWER_SUPPLY_INPUT_VOLTAGE_RANGE_SWITCHING_AUTO_SWITCH    0100b
#define SMBIOS_SYSTEM_POWER_SUPPLY_INPUT_VOLTAGE_RANGE_SWITCHING_WIDE_RANGE     0101b
#define SMBIOS_SYSTEM_POWER_SUPPLY_INPUT_VOLTAGE_RANGE_SWITCHING_NOT_APPLICABLE 0110b
                    WORD DMTFInputVoltageRangeSwitching : 4;    /* SMBIOS_SYSTEM_POWER_SUPPLY_INPUT_VOLTAGE_RANGE_SWITCHING_* */
#define SMBIOS_SYSTEM_POWER_SUPPLY_STATUS_OTHER         001b
#define SMBIOS_SYSTEM_POWER_SUPPLY_STATUS_UNKNOWN       010b
#define SMBIOS_SYSTEM_POWER_SUPPLY_STATUS_OK            011b
#define SMBIOS_SYSTEM_POWER_SUPPLY_STATUS_NON_CRITICAL  100b
#define SMBIOS_SYSTEM_POWER_SUPPLY_STATUS_CRITICAL      101b
                    WORD Status : 3;                            /* SMBIOS_SYSTEM_POWER_SUPPLY_STATUS_* */
#define SMBIOS_SYSTEM_POWER_SUPPLY_TYPE_OTHER       0001b  
#define SMBIOS_SYSTEM_POWER_SUPPLY_TYPE_UNKNOWN     0010b  
#define SMBIOS_SYSTEM_POWER_SUPPLY_TYPE_LINEAR      0011b  
#define SMBIOS_SYSTEM_POWER_SUPPLY_TYPE_SWITCHING   0100b  
#define SMBIOS_SYSTEM_POWER_SUPPLY_TYPE_BATTERY     0101b  
#define SMBIOS_SYSTEM_POWER_SUPPLY_TYPE_UPS         0110b  
#define SMBIOS_SYSTEM_POWER_SUPPLY_TYPE_CONVERTER   0111b  
#define SMBIOS_SYSTEM_POWER_SUPPLY_TYPE_REGULATOR   1000b  
                    WORD DMTFPowerSupplyType : 4;               /* SMBIOS_SYSTEM_POWER_SUPPLY_TYPE_* */
                    WORD Reserved : 2;
                };
            };
        } TYPE_39_SYSTEM_POWER_SUPPLY;
        struct
        {
            BYTE ReferenceDesignation;
            union
            {
                BYTE Type;
                struct
                {
#define SMBIOS_ONBOARD_DEVICES_EXTENDED_TYPE_OTHER              ((BYTE)01h)
#define SMBIOS_ONBOARD_DEVICES_EXTENDED_TYPE_UNKNOWN            ((BYTE)02h)
#define SMBIOS_ONBOARD_DEVICES_EXTENDED_TYPE_VIDEO              ((BYTE)03h)
#define SMBIOS_ONBOARD_DEVICES_EXTENDED_TYPE_SCSI_CONTROLLER    ((BYTE)04h)
#define SMBIOS_ONBOARD_DEVICES_EXTENDED_TYPE_ETHERNET           ((BYTE)05h)
#define SMBIOS_ONBOARD_DEVICES_EXTENDED_TYPE_TOKEN_RING         ((BYTE)06h)
#define SMBIOS_ONBOARD_DEVICES_EXTENDED_TYPE_SOUND              ((BYTE)07h)
#define SMBIOS_ONBOARD_DEVICES_EXTENDED_TYPE_PATA_CONTROLLER    ((BYTE)08h)
#define SMBIOS_ONBOARD_DEVICES_EXTENDED_TYPE_SATA_CONTROLLER    ((BYTE)09h)
#define SMBIOS_ONBOARD_DEVICES_EXTENDED_TYPE_SAS_CONTROLLER     ((BYTE)0Ah)
#define SMBIOS_ONBOARD_DEVICES_EXTENDED_TYPE_WIRELESS_LAN       ((BYTE)0Bh)
#define SMBIOS_ONBOARD_DEVICES_EXTENDED_TYPE_BLUETOOTH          ((BYTE)0Ch)
#define SMBIOS_ONBOARD_DEVICES_EXTENDED_TYPE_WWAN               ((BYTE)0Dh)
#define SMBIOS_ONBOARD_DEVICES_EXTENDED_TYPE_EMMC               ((BYTE)0Eh)
#define SMBIOS_ONBOARD_DEVICES_EXTENDED_TYPE_NVME_CONTROLLER    ((BYTE)0Fh)
#define SMBIOS_ONBOARD_DEVICES_EXTENDED_TYPE_UFS_CONTROLLER     ((BYTE)10h)
                    BYTE TypeOfDevice : 7; /* SMBIOS_ONBOARD_DEVICES_EXTENDED_TYPE_* */
                    BYTE Status : 1;
                };
            };
            BYTE TypeInstance;
            WORD SegmentGroupNumber;
            BYTE BusNumber;
            union
            {
                BYTE DeviceFunctionNumber;
                struct
                {
                    BYTE FunctionNumber : 3;
                    BYTE DeviceNumber : 5;
                };
            };
        } TYPE_41_ONBOARD_DEVICES_EXTENDED_INFORMATION;
        struct
        {
            BYTE VendorID[4];
            BYTE MajorSpecVersion;
            BYTE MinorSpecVersion;
            DWORD FirmwareVersion1;
            DWORD FirmwareVersion2;
            BYTE Description;
            union
            {
                QWORD Characteristics;
                struct
                {
                    QWORD Reserved0 : 1;
                    QWORD Reserved1 : 1;
                    QWORD NotSupported : 1;
                    QWORD FamilyConfigurableViaFirmwareUpdate : 1;
                    QWORD FamilyConfigurableViaPlatformSoftwareSupport : 1;
                    QWORD FamilyConfigurableViaOEMProprietaryMechanism : 1;
                    QWORD Reserved2 : 58;
                };
            };
            DWORD OEMDefined;
        } TYPE_43_TPM_DEVICE;
        struct
        {
            WORD ReferencedHandle;
            BYTE ProcessorSpecificBlock[];
        } TYPE_44_PROCESSOR_ADDITIONAL_INFO;
        struct
        {
            BYTE ComponentName;
            BYTE Version;
#define SMBIOS_FIRMWARE_INVENTORY_VERSION_FORMAT_FREE_FORM      ((BYTE)00h)
#define SMBIOS_FIRMWARE_INVENTORY_VERSION_FORMAT_MAJOR_MINOR    ((BYTE)01h)
#define SMBIOS_FIRMWARE_INVENTORY_VERSION_FORMAT_HEX_STRING_32  ((BYTE)02h)
#define SMBIOS_FIRMWARE_INVENTORY_VERSION_FORMAT_HEX_STRING_64  ((BYTE)03h)
            BYTE VersionFormat; /* SMBIOS_FIRMWARE_INVENTORY_VERSION_FORMAT_* */
            BYTE ID;
#define SMBIOS_FIRMWARE_INVENTORY_ID_FORMAT_FREE_FORM   ((BYTE)00h)
#define SMBIOS_FIRMWARE_INVENTORY_ID_FORMAT_UEFI_GUID   ((BYTE)01h)
            BYTE IDFormat;  /* SMBIOS_FIRMWARE_INVENTORY_ID_FORMAT_* */
            BYTE ReleaseDate;
            BYTE Manufacturer;
            BYTE LowestSupportedFirmwareVersion;    /* SMBIOS_FIRMWARE_INVENTORY_VERSION_FORMAT_* */
            QWORD ImageSize;
            union
            {
                WORD Characteristics;
                struct
                {
                    WORD Updatable : 1;
                    WORD WriteProtect : 1;
                    WORD Reserved : 14;
                };
            };
#define SMBIOS_FIRMWARE_INVENTORY_STATE_OTHER               ((BYTE)01h)
#define SMBIOS_FIRMWARE_INVENTORY_STATE_UNKNOWN             ((BYTE)02h)
#define SMBIOS_FIRMWARE_INVENTORY_STATE_DISABLED            ((BYTE)03h)
#define SMBIOS_FIRMWARE_INVENTORY_STATE_ENABLED             ((BYTE)04h)
#define SMBIOS_FIRMWARE_INVENTORY_STATE_ABSENT              ((BYTE)05h)
#define SMBIOS_FIRMWARE_INVENTORY_STATE_STANDBY_OFFLINE     ((BYTE)06h)
#define SMBIOS_FIRMWARE_INVENTORY_STATE_STANDBY_SPARE       ((BYTE)07h)
#define SMBIOS_FIRMWARE_INVENTORY_STATE_UNAVAILABLE_OFFLINE ((BYTE)08h)
            BYTE State; /* SMBIOS_FIRMWARE_INVENTORY_STATE_* */
            BYTE NumberOfAssociatedComponents;
            _Field_size_(NumberOfAssociatedComponents) WORD AssociatedComponentHandles[];
        } TYPE_45_FIRMWARE_INVENTORY_INFO;
    };
} SMBIOS_TABLE, *PSMBIOS_TABLE;

#pragma pack(pop)

#pragma endregion

#ifdef _MSC_VER
#pragma warning(default: 4200)
#else
#pragma pop_macro("_Field_size_")
#pragma pop_macro("_Field_size_bytes_")
#endif
