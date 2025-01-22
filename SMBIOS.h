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

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

/* Version Control */
#ifndef SMBIOS_VERSION
#define SMBIOS_VERSION 0x03080000 // 0x(Major)(Minor)(Revision)(WIP)
#endif
static_assert(SMBIOS_VERSION >= 0x02000000);

/* Basic Types */
typedef unsigned char       BYTE;
typedef unsigned short      WORD;
typedef unsigned long       DWORD;
typedef unsigned __int64    QWORD;

/* For code analysis */
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

#pragma endregion

#pragma region SMBIOS Specification

#pragma pack(push, 1)

/* aka RawSMBIOSData on Windows */
typedef struct _SMBIOS_RAW_DATA
{
    BYTE Used20CallingMethod;
    BYTE SMBIOSMajorVersion;
    BYTE SMBIOSMinorVersion;
    BYTE DmiRevision;
    DWORD Length;
    _Field_size_bytes_(Length) BYTE SMBIOSTableData[]; // SMBIOS_HEADER
} SMBIOS_RAW_DATA, *PSMBIOS_RAW_DATA;

typedef struct _SMBIOS_HEADER
{
    BYTE Type; // SMBIOS_TYPE_*
    BYTE Length;
    WORD Handle;
} SMBIOS_HEADER, *PSMBIOS_HEADER;
static_assert(sizeof(SMBIOS_HEADER) == 4);

#pragma region Platform Firmware Information (Type 0)

#define SMBIOS_TYPE_PLATFORM_FIRMWARE_INFORMATION ((BYTE)0)

#define SMBIOS_PLATFORM_FIRMWARE_EXTENDEDROMSIZE_UNIT_MB ((WORD)0b00) // Unit is MB
#define SMBIOS_PLATFORM_FIRMWARE_EXTENDEDROMSIZE_UNIT_GB ((WORD)0b01) // Unit is GB

typedef struct _SMBIOS_PLATFORM_FIRMWARE_INFORMATION
{
    SMBIOS_HEADER Header;
    BYTE Vendor;
    BYTE Version;
    WORD BIOSStartingAddressSegment;
    BYTE ReleaseDate;
    BYTE ROMSize;
    union
    {
        QWORD Value;
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
    } Characteristics;
#if SMBIOS_VERSION >= 0x02010000
    union
    {
        BYTE Value;
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
    } CharacteristicsExtensionByte1;
#if SMBIOS_VERSION >= 0x02030000
    union
    {
        BYTE Value;
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
    } CharacteristicsExtensionByte2;
#if SMBIOS_VERSION >= 0x02040000
    BYTE MajorRelease;
    BYTE MinorRelease;
    BYTE ECFirmwareMajorRelease;
    BYTE ECFirmwareMinorRelease;
#if SMBIOS_VERSION >= 0x03010000
    union
    {
        WORD Value;
        struct
        {
            WORD Size : 14; // 00:13 Size
            WORD Unit : 2;  // 14:15 Unit, SMBIOS_PLATFORM_FIRMWARE_EXTENDEDROMSIZE_UNIT_*
        };
    } ExtendedROMSize;
#endif // SMBIOS_VERSION >= 0x03010000
#endif // SMBIOS_VERSION >= 0x02040000
#endif // SMBIOS_VERSION >= 0x02030000
#endif // SMBIOS_VERSION >= 0x02010000
} SMBIOS_PLATFORM_FIRMWARE_INFORMATION, *PSMBIOS_PLATFORM_FIRMWARE_INFORMATION, SMBIOS_TYPE_0, *PSMBIOS_TYPE_0;

#pragma endregion

#pragma region System Information (Type 1)

#define SMBIOS_TYPE_SYSTEM_INFORMATION ((BYTE)1)

#define SMBIOS_SYSTEM_WAKEUPTYPE_RESERVED           ((BYTE)0x00) // Reserved
#define SMBIOS_SYSTEM_WAKEUPTYPE_OTHER              ((BYTE)0x01) // Other
#define SMBIOS_SYSTEM_WAKEUPTYPE_UNKNOW             ((BYTE)0x02) // Unknown
#define SMBIOS_SYSTEM_WAKEUPTYPE_APM_TIMER          ((BYTE)0x03) // APM Timer
#define SMBIOS_SYSTEM_WAKEUPTYPE_MODEM_RING         ((BYTE)0x04) // Modem Ring
#define SMBIOS_SYSTEM_WAKEUPTYPE_LAN_REMOTE         ((BYTE)0x05) // LAN Remote
#define SMBIOS_SYSTEM_WAKEUPTYPE_POWER_SWITCH       ((BYTE)0x06) // Power Switch
#define SMBIOS_SYSTEM_WAKEUPTYPE_PCI_PME            ((BYTE)0x07) // PCI PME#
#define SMBIOS_SYSTEM_WAKEUPTYPE_AC_POWER_RESTORED  ((BYTE)0x08) // AC Power Restored

typedef struct _SMBIOS_SYSTEM_INFORMATION
{
    SMBIOS_HEADER Header;
    BYTE Manufacturer;
    BYTE ProductName;
    BYTE Version;
    BYTE SerialNumber;
#if SMBIOS_VERSION >= 0x02010000
    BYTE UUID[16];
    BYTE WakeUpType; // SMBIOS_SYSTEM_WAKEUPTYPE_*
#if SMBIOS_VERSION >= 0x02040000
    BYTE SKUNumber;
    BYTE Family;
#endif // SMBIOS_VERSION >= 0x02040000
#endif // SMBIOS_VERSION >= 0x02010000
} SMBIOS_SYSTEM_INFORMATION, *PSMBIOS_SYSTEM_INFORMATION, SMBIOS_TYPE_1, *PSMBIOS_TYPE_1;

#pragma endregion

#pragma region Baseboard Information (Type 2)

#define SMBIOS_TYPE_BASEBOARD_INFORMATION ((BYTE)2)

#define SMBIOS_BASEBOARD_TYPE_UNKNOWN                   ((BYTE)0x01) // Unknown
#define SMBIOS_BASEBOARD_TYPE_OTHER                     ((BYTE)0x02) // Other
#define SMBIOS_BASEBOARD_TYPE_SERVER_BLADE              ((BYTE)0x03) // Server Blade
#define SMBIOS_BASEBOARD_TYPE_CONNECTIVITY_SWITCH       ((BYTE)0x04) // Connectivity Switch
#define SMBIOS_BASEBOARD_TYPE_SYSTEM_MANAGEMENT_MODULE  ((BYTE)0x05) // System Management Module
#define SMBIOS_BASEBOARD_TYPE_PROCESSOR_MODULE          ((BYTE)0x06) // Processor Module
#define SMBIOS_BASEBOARD_TYPE_IO_MODULE                 ((BYTE)0x07) // I/O Module
#define SMBIOS_BASEBOARD_TYPE_MEMORY_MODULE             ((BYTE)0x08) // Memory Module
#define SMBIOS_BASEBOARD_TYPE_DAUGHTER_BOARD            ((BYTE)0x09) // Daughter board
#define SMBIOS_BASEBOARD_TYPE_MOTHERBOARD               ((BYTE)0x0A) // Motherboard (includes processor, memory, and I/O)
#define SMBIOS_BASEBOARD_TYPE_PROCESSOR_MEMORY_MODULE   ((BYTE)0x0B) // Processor/Memory Module
#define SMBIOS_BASEBOARD_TYPE_PROCESSOR_IO_MODULE       ((BYTE)0x0C) // Processor/IO Module
#define SMBIOS_BASEBOARD_TYPE_INTERCONNECT_BOARD        ((BYTE)0x0D) // Interconnect board

typedef struct _SMBIOS_BASEBOARD_INFORMATION
{
    SMBIOS_HEADER Header;
    BYTE Manufacturer;
    BYTE Product;
    BYTE Version;
    BYTE SerialNumber;
    BYTE AssetTag;
    union
    {
        BYTE Value;
        struct
        {
            BYTE HostingBoard : 1;      // 00 The board is a hosting board (for example, a motherboard)
            BYTE RequiresAuxiliary : 1; // 01 The board requires at least one daughter board or auxiliary card to function properly
            BYTE Removable : 1;         // 02 The board is removable
            BYTE Replaceable : 1;       // 03 The board is replaceable
            BYTE HotSwappable : 1;      // 04 The board is s hot swappable
            BYTE Reserved : 3;          // 05:07 Reserved for future definition by this specification
        };
    } FeatureFlags;
    BYTE LocationInChassis;
    WORD ChassisHandle;
    BYTE BoardType; // SMBIOS_BASEBOARD_TYPE_*
    BYTE NumberOfContainedObjectHandles;
    _Field_size_(NumberOfContainedObjectHandles) WORD ContainedObjectHandles[];
} SMBIOS_BASEBOARD_INFORMATION, *PSMBIOS_BASEBOARD_INFORMATION, SMBIOS_TYPE_2, *PSMBIOS_TYPE_2;

#pragma endregion

#pragma region System Enclosure or Chassis (Type 3)

#define SMBIOS_TYPE_SYSTEM_ENCLOSURE_OR_CHASSIS ((BYTE)3)

#define SMBIOS_SYSTEM_ENCLOSURE_OR_CHASSIS_TYPE_OTHER                   ((BYTE)0x01) // Other
#define SMBIOS_SYSTEM_ENCLOSURE_OR_CHASSIS_TYPE_UNKNOWN                 ((BYTE)0x02) // Unknown
#define SMBIOS_SYSTEM_ENCLOSURE_OR_CHASSIS_TYPE_DESKTOP                 ((BYTE)0x03) // Desktop
#define SMBIOS_SYSTEM_ENCLOSURE_OR_CHASSIS_TYPE_LOW_PROFILE_DESKTOP     ((BYTE)0x04) // Low Profile Desktop
#define SMBIOS_SYSTEM_ENCLOSURE_OR_CHASSIS_TYPE_PIZZA_BOX               ((BYTE)0x05) // Pizza Box
#define SMBIOS_SYSTEM_ENCLOSURE_OR_CHASSIS_TYPE_MINI_TOWER              ((BYTE)0x06) // Mini Tower
#define SMBIOS_SYSTEM_ENCLOSURE_OR_CHASSIS_TYPE_TOWER                   ((BYTE)0x07) // Tower
#define SMBIOS_SYSTEM_ENCLOSURE_OR_CHASSIS_TYPE_PORTABLE                ((BYTE)0x08) // Portable
#define SMBIOS_SYSTEM_ENCLOSURE_OR_CHASSIS_TYPE_LAPTOP                  ((BYTE)0x09) // Laptop
#define SMBIOS_SYSTEM_ENCLOSURE_OR_CHASSIS_TYPE_NOTEBOOK                ((BYTE)0x0A) // Notebook
#define SMBIOS_SYSTEM_ENCLOSURE_OR_CHASSIS_TYPE_HAND_HELD               ((BYTE)0x0B) // Hand Held
#define SMBIOS_SYSTEM_ENCLOSURE_OR_CHASSIS_TYPE_DOCKING_STATION         ((BYTE)0x0C) // Docking Station
#define SMBIOS_SYSTEM_ENCLOSURE_OR_CHASSIS_TYPE_ALL_IN_ONE              ((BYTE)0x0D) // All in One
#define SMBIOS_SYSTEM_ENCLOSURE_OR_CHASSIS_TYPE_SUB_NOTEBOOK            ((BYTE)0x0E) // Sub Notebook
#define SMBIOS_SYSTEM_ENCLOSURE_OR_CHASSIS_TYPE_SPACE_SAVING            ((BYTE)0x0F) // Space-saving
#define SMBIOS_SYSTEM_ENCLOSURE_OR_CHASSIS_TYPE_LUNCH_BOX               ((BYTE)0x10) // Lunch Box
#define SMBIOS_SYSTEM_ENCLOSURE_OR_CHASSIS_TYPE_MAIN_SERVER_CHASSIS     ((BYTE)0x11) // Main Server Chassis
#define SMBIOS_SYSTEM_ENCLOSURE_OR_CHASSIS_TYPE_EXPANSION_CHASSIS       ((BYTE)0x12) // Expansion Chassis
#define SMBIOS_SYSTEM_ENCLOSURE_OR_CHASSIS_TYPE_SUB_CHASSIS             ((BYTE)0x13) // SubChassis
#define SMBIOS_SYSTEM_ENCLOSURE_OR_CHASSIS_TYPE_BUS_EXPANSION_CHASSIS   ((BYTE)0x14) // Bus Expansion Chassis
#define SMBIOS_SYSTEM_ENCLOSURE_OR_CHASSIS_TYPE_PERIPHERAL_CHASSIS      ((BYTE)0x15) // Peripheral Chassis
#define SMBIOS_SYSTEM_ENCLOSURE_OR_CHASSIS_TYPE_RAID_CHASSIS            ((BYTE)0x16) // RAID Chassis
#define SMBIOS_SYSTEM_ENCLOSURE_OR_CHASSIS_TYPE_RACK_MOUNT_CHASSIS      ((BYTE)0x17) // Rack Mount Chassis
#define SMBIOS_SYSTEM_ENCLOSURE_OR_CHASSIS_TYPE_SEALED_CASE_PC          ((BYTE)0x18) // Sealed-case PC
#define SMBIOS_SYSTEM_ENCLOSURE_OR_CHASSIS_TYPE_MULTI_SYSTEM_CHASSIS    ((BYTE)0x19) // Multi-system chassis
#define SMBIOS_SYSTEM_ENCLOSURE_OR_CHASSIS_TYPE_COMPACT_PCI             ((BYTE)0x1A) // Compact PCI
#define SMBIOS_SYSTEM_ENCLOSURE_OR_CHASSIS_TYPE_ADVANCED_TCA            ((BYTE)0x1B) // Advanced TCA
#define SMBIOS_SYSTEM_ENCLOSURE_OR_CHASSIS_TYPE_BLADE                   ((BYTE)0x1C) // Blade
#define SMBIOS_SYSTEM_ENCLOSURE_OR_CHASSIS_TYPE_BLADE_ENCLOSURE         ((BYTE)0x1D) // Blade Enclosure
#define SMBIOS_SYSTEM_ENCLOSURE_OR_CHASSIS_TYPE_TABLET                  ((BYTE)0x1E) // Tablet
#define SMBIOS_SYSTEM_ENCLOSURE_OR_CHASSIS_TYPE_CONVERTIBLE             ((BYTE)0x1F) // Convertible
#define SMBIOS_SYSTEM_ENCLOSURE_OR_CHASSIS_TYPE_DETACHABLE              ((BYTE)0x20) // Detachable
#define SMBIOS_SYSTEM_ENCLOSURE_OR_CHASSIS_TYPE_IOT_GATEWAY             ((BYTE)0x21) // IoT Gateway
#define SMBIOS_SYSTEM_ENCLOSURE_OR_CHASSIS_TYPE_EMBEDDED_PC             ((BYTE)0x22) // Embedded PC
#define SMBIOS_SYSTEM_ENCLOSURE_OR_CHASSIS_TYPE_MINI_PC                 ((BYTE)0x23) // Mini PC
#define SMBIOS_SYSTEM_ENCLOSURE_OR_CHASSIS_TYPE_STICK_PC                ((BYTE)0x24) // Stick PC

#define SMBIOS_SYSTEM_ENCLOSURE_OR_CHASSIS_STATE_OTHER              ((BYTE)0x01) // Other
#define SMBIOS_SYSTEM_ENCLOSURE_OR_CHASSIS_STATE_UNKNOWN            ((BYTE)0x02) // Unknown
#define SMBIOS_SYSTEM_ENCLOSURE_OR_CHASSIS_STATE_SAFE               ((BYTE)0x03) // Safe
#define SMBIOS_SYSTEM_ENCLOSURE_OR_CHASSIS_STATE_WARNING            ((BYTE)0x04) // Warning
#define SMBIOS_SYSTEM_ENCLOSURE_OR_CHASSIS_STATE_CRITICAL           ((BYTE)0x05) // Critical
#define SMBIOS_SYSTEM_ENCLOSURE_OR_CHASSIS_STATE_NON_RECOVERABLE    ((BYTE)0x06) // Non-recoverable

#define SMBIOS_SYSTEM_ENCLOSURE_OR_CHASSIS_SECURITY_STATE_OTHER                         ((BYTE)0x01) // Other
#define SMBIOS_SYSTEM_ENCLOSURE_OR_CHASSIS_SECURITY_STATE_UNKNOWN                       ((BYTE)0x02) // Unknown
#define SMBIOS_SYSTEM_ENCLOSURE_OR_CHASSIS_SECURITY_STATE_NONE                          ((BYTE)0x03) // None
#define SMBIOS_SYSTEM_ENCLOSURE_OR_CHASSIS_SECURITY_STATE_EXTERNAL_INTERFACE_LOCKED_OUT ((BYTE)0x04) // External interface locked out
#define SMBIOS_SYSTEM_ENCLOSURE_OR_CHASSIS_SECURITY_STATE_EXTERNAL_INTERFACE_ENABLED    ((BYTE)0x05) // External interface enabled

typedef struct _SMBIOS_SYSTEM_ENCLOSURE_OR_CHASSIS
{
    SMBIOS_HEADER Header;
    BYTE Manufacturer;
    union
    {
        BYTE Value;
        struct
        {
            BYTE Type : 7;          // 00:06: SMBIOS_SYSTEM_ENCLOSURE_OR_CHASSIS_TYPE_*
            BYTE ChassisLock : 1;   // 07 Chassis lock is present
        };
    } Type;
    BYTE Version;
    BYTE SerialNumber;
    BYTE AssetTagNumber;
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
    BYTE SKUNumber;
#endif // SMBIOS_VERSION >= 0x02070000
*/
#endif // SMBIOS_VERSION >= 0x02030000
#endif // SMBIOS_VERSION >= 0x02010000
} SMBIOS_SYSTEM_ENCLOSURE_OR_CHASSIS, *PSMBIOS_SYSTEM_ENCLOSURE_OR_CHASSIS, SMBIOS_TYPE_3, *PSMBIOS_TYPE_3;

#pragma endregion

#pragma region Processor Information (Type 4)

#define SMBIOS_TYPE_PROCESSOR_INFORMATION ((BYTE)4)

#define SMBIOS_PROCESSOR_TYPE_OTHER             ((BYTE)0x01) // Other
#define SMBIOS_PROCESSOR_TYPE_UNKNOWN           ((BYTE)0x02) // Unknown
#define SMBIOS_PROCESSOR_TYPE_CENTRAL_PROCESSOR ((BYTE)0x03) // Central Processor
#define SMBIOS_PROCESSOR_TYPE_MATH_PROCESSOR    ((BYTE)0x04) // Math Processor
#define SMBIOS_PROCESSOR_TYPE_DSP_PROCESSOR     ((BYTE)0x05) // DSP Processor
#define SMBIOS_PROCESSOR_TYPE_VIDEO_PROCESSOR   ((BYTE)0x06) // Video Processor

#define SMBIOS_PROCESSOR_FAMILY_OTHER                                                   ((BYTE)0x01)    // Other
#define SMBIOS_PROCESSOR_FAMILY_UNKNOWN                                                 ((BYTE)0x02)    // Unknown
#define SMBIOS_PROCESSOR_FAMILY_8086                                                    ((BYTE)0x03)    // 8086
#define SMBIOS_PROCESSOR_FAMILY_80286                                                   ((BYTE)0x04)    // 80286
#define SMBIOS_PROCESSOR_FAMILY_INTEL386_PROCESSOR                                      ((BYTE)0x05)    // Intel386™ processor
#define SMBIOS_PROCESSOR_FAMILY_INTEL486_PROCESSOR                                      ((BYTE)0x06)    // Intel486™ processor
#define SMBIOS_PROCESSOR_FAMILY_8087                                                    ((BYTE)0x07)    // 8087
#define SMBIOS_PROCESSOR_FAMILY_80287                                                   ((BYTE)0x08)    // 80287
#define SMBIOS_PROCESSOR_FAMILY_80387                                                   ((BYTE)0x09)    // 80387
#define SMBIOS_PROCESSOR_FAMILY_80487                                                   ((BYTE)0x0A)    // 80487
#define SMBIOS_PROCESSOR_FAMILY_INTEL_PENTIUM_PROCESSOR                                 ((BYTE)0x0B)    // Intel® Pentium® processor
#define SMBIOS_PROCESSOR_FAMILY_PENTIUM_PRO_PROCESSOR                                   ((BYTE)0x0C)    // Pentium® Pro processor
#define SMBIOS_PROCESSOR_FAMILY_PENTIUM_II_PROCESSOR                                    ((BYTE)0x0D)    // Pentium® II processor
#define SMBIOS_PROCESSOR_FAMILY_PENTIUM_PROCESSOR_WITH_MMX_TECHNOLOGY                   ((BYTE)0x0E)    // Pentium® processor with MMX™ technology
#define SMBIOS_PROCESSOR_FAMILY_INTEL_CELERON_PROCESSOR                                 ((BYTE)0x0F)    // Intel® Celeron® processor
#define SMBIOS_PROCESSOR_FAMILY_PENTIUM_II_XEON_PROCESSOR                               ((BYTE)0x10)    // Pentium® II Xeon® processor
#define SMBIOS_PROCESSOR_FAMILY_PENTIUM_III_PROCESSOR                                   ((BYTE)0x11)    // Pentium® III processor
#define SMBIOS_PROCESSOR_FAMILY_M1_FAMILY                                               ((BYTE)0x12)    // M1 Family
#define SMBIOS_PROCESSOR_FAMILY_M2_FAMILY                                               ((BYTE)0x13)    // M2 Family
#define SMBIOS_PROCESSOR_FAMILY_INTEL_CELERON_M_PROCESSOR                               ((BYTE)0x14)    // Intel® Celeron® M processor
#define SMBIOS_PROCESSOR_FAMILY_INTEL_PENTIUM_4_HT_PROCESSOR                            ((BYTE)0x15)    // Intel® Pentium® 4 HT processor
#define SMBIOS_PROCESSOR_FAMILY_INTEL_PROCESSOR                                         ((BYTE)0x16)    // Intel® Processor
#define SMBIOS_PROCESSOR_FAMILY_AMD_DURON_PROCESSOR_FAMILY_1                            ((BYTE)0x18)    // AMD Duron™ Processor Family [1]
#define SMBIOS_PROCESSOR_FAMILY_K5_FAMILY_1                                             ((BYTE)0x19)    // K5 Family [1]
#define SMBIOS_PROCESSOR_FAMILY_K6_FAMILY_1                                             ((BYTE)0x1A)    // K6 Family [1]
#define SMBIOS_PROCESSOR_FAMILY_K6_2_1                                                  ((BYTE)0x1B)    // K6-2 [1]
#define SMBIOS_PROCESSOR_FAMILY_K6_3_1                                                  ((BYTE)0x1C)    // K6-3 [1]
#define SMBIOS_PROCESSOR_FAMILY_AMD_ATHLON_PROCESSOR_FAMILY_1                           ((BYTE)0x1D)    // AMD Athlon™ Processor Family [1]
#define SMBIOS_PROCESSOR_FAMILY_AMD29000_FAMILY                                         ((BYTE)0x1E)    // AMD29000 Family
#define SMBIOS_PROCESSOR_FAMILY_K6_2_PLUS                                               ((BYTE)0x1F)    // K6-2+
#define SMBIOS_PROCESSOR_FAMILY_POWER_PC_FAMILY                                         ((BYTE)0x20)    // Power PC Family
#define SMBIOS_PROCESSOR_FAMILY_POWER_PC_601                                            ((BYTE)0x21)    // Power PC 601
#define SMBIOS_PROCESSOR_FAMILY_POWER_PC_603                                            ((BYTE)0x22)    // Power PC 603
#define SMBIOS_PROCESSOR_FAMILY_POWER_PC_603_PLUS                                       ((BYTE)0x23)    // Power PC 603+
#define SMBIOS_PROCESSOR_FAMILY_POWER_PC_604                                            ((BYTE)0x24)    // Power PC 604
#define SMBIOS_PROCESSOR_FAMILY_POWER_PC_620                                            ((BYTE)0x25)    // Power PC 620
#define SMBIOS_PROCESSOR_FAMILY_POWER_PC_X704                                           ((BYTE)0x26)    // Power PC x704
#define SMBIOS_PROCESSOR_FAMILY_POWER_PC_750                                            ((BYTE)0x27)    // Power PC 750
#define SMBIOS_PROCESSOR_FAMILY_INTEL_CORE_DUO_PROCESSOR                                ((BYTE)0x28)    // Intel® Core™ Duo processor
#define SMBIOS_PROCESSOR_FAMILY_INTEL_CORE_DUO_MOBILE_PROCESSOR                         ((BYTE)0x29)    // Intel® Core™ Duo mobile processor
#define SMBIOS_PROCESSOR_FAMILY_INTEL_CORE_SOLO_MOBILE_PROCESSOR                        ((BYTE)0x2A)    // Intel® Core™ Solo mobile processor
#define SMBIOS_PROCESSOR_FAMILY_INTEL_ATOM_PROCESSOR                                    ((BYTE)0x2B)    // Intel® Atom™ processor
#define SMBIOS_PROCESSOR_FAMILY_INTEL_CORE_M_PROCESSOR                                  ((BYTE)0x2C)    // Intel® Core™ M processor
#define SMBIOS_PROCESSOR_FAMILY_INTEL_CORE_M3_PROCESSOR                                 ((BYTE)0x2D)    // Intel® Core™ m3 processor
#define SMBIOS_PROCESSOR_FAMILY_INTEL_CORE_M5_PROCESSOR                                 ((BYTE)0x2E)    // Intel® Core™ m5 processor
#define SMBIOS_PROCESSOR_FAMILY_INTEL_CORE_M7_PROCESSOR                                 ((BYTE)0x2F)    // Intel® Core™ m7 processor
#define SMBIOS_PROCESSOR_FAMILY_ALPHA_FAMILY_2                                          ((BYTE)0x30)    // Alpha Family [2]
#define SMBIOS_PROCESSOR_FAMILY_ALPHA_21064                                             ((BYTE)0x31)    // Alpha 21064
#define SMBIOS_PROCESSOR_FAMILY_ALPHA_21066                                             ((BYTE)0x32)    // Alpha 21066
#define SMBIOS_PROCESSOR_FAMILY_ALPHA_21164                                             ((BYTE)0x33)    // Alpha 21164
#define SMBIOS_PROCESSOR_FAMILY_ALPHA_21164PC                                           ((BYTE)0x34)    // Alpha 21164PC
#define SMBIOS_PROCESSOR_FAMILY_ALPHA_21164A                                            ((BYTE)0x35)    // Alpha 21164a
#define SMBIOS_PROCESSOR_FAMILY_ALPHA_21264                                             ((BYTE)0x36)    // Alpha 21264
#define SMBIOS_PROCESSOR_FAMILY_ALPHA_21364                                             ((BYTE)0x37)    // Alpha 21364
#define SMBIOS_PROCESSOR_FAMILY_AMD_TURION_II_ULTRA_DUAL_CORE_MOBILE_M_PROCESSOR_FAMILY ((BYTE)0x38)    // AMD Turion™ II Ultra Dual-Core Mobile M Processor Family
#define SMBIOS_PROCESSOR_FAMILY_AMD_TURION_II_DUAL_CORE_MOBILE_M_PROCESSOR_FAMILY       ((BYTE)0x39)    // AMD Turion™ II Dual-Core Mobile M Processor Family
#define SMBIOS_PROCESSOR_FAMILY_AMD_ATHLON_II_DUAL_CORE_M_PROCESSOR_FAMILY              ((BYTE)0x3A)    // AMD Athlon™ II Dual-Core M Processor Family
#define SMBIOS_PROCESSOR_FAMILY_AMD_OPTERON_6100_SERIES_PROCESSOR                       ((BYTE)0x3B)    // AMD Opteron™ 6100 Series Processor
#define SMBIOS_PROCESSOR_FAMILY_AMD_OPTERON_4100_SERIES_PROCESSOR                       ((BYTE)0x3C)    // AMD Opteron™ 4100 Series Processor
#define SMBIOS_PROCESSOR_FAMILY_AMD_OPTERON_6200_SERIES_PROCESSOR                       ((BYTE)0x3D)    // AMD Opteron™ 6200 Series Processor
#define SMBIOS_PROCESSOR_FAMILY_AMD_OPTERON_4200_SERIES_PROCESSOR                       ((BYTE)0x3E)    // AMD Opteron™ 4200 Series Processor
#define SMBIOS_PROCESSOR_FAMILY_AMD_FX_SERIES_PROCESSOR                                 ((BYTE)0x3F)    // AMD FX™ Series Processor
#define SMBIOS_PROCESSOR_FAMILY_MIPS_FAMILY                                             ((BYTE)0x40)    // MIPS Family
#define SMBIOS_PROCESSOR_FAMILY_MIPS_R4000                                              ((BYTE)0x41)    // MIPS R4000
#define SMBIOS_PROCESSOR_FAMILY_MIPS_R4200                                              ((BYTE)0x42)    // MIPS R4200
#define SMBIOS_PROCESSOR_FAMILY_MIPS_R4400                                              ((BYTE)0x43)    // MIPS R4400
#define SMBIOS_PROCESSOR_FAMILY_MIPS_R4600                                              ((BYTE)0x44)    // MIPS R4600
#define SMBIOS_PROCESSOR_FAMILY_MIPS_R10000                                             ((BYTE)0x45)    // MIPS R10000
#define SMBIOS_PROCESSOR_FAMILY_AMD_C_SERIES_PROCESSOR                                  ((BYTE)0x46)    // AMD C-Series Processor
#define SMBIOS_PROCESSOR_FAMILY_AMD_E_SERIES_PROCESSOR                                  ((BYTE)0x47)    // AMD E-Series Processor
#define SMBIOS_PROCESSOR_FAMILY_AMD_A_SERIES_PROCESSOR                                  ((BYTE)0x48)    // AMD A-Series Processor
#define SMBIOS_PROCESSOR_FAMILY_AMD_G_SERIES_PROCESSOR                                  ((BYTE)0x49)    // AMD G-Series Processor
#define SMBIOS_PROCESSOR_FAMILY_AMD_Z_SERIES_PROCESSOR                                  ((BYTE)0x4A)    // AMD Z-Series Processor
#define SMBIOS_PROCESSOR_FAMILY_AMD_R_SERIES_PROCESSOR                                  ((BYTE)0x4B)    // AMD R-Series Processor
#define SMBIOS_PROCESSOR_FAMILY_AMD_OPTERON_4300_SERIES_PROCESSOR                       ((BYTE)0x4C)    // AMD Opteron™ 4300 Series Processor
#define SMBIOS_PROCESSOR_FAMILY_AMD_OPTERON_6300_SERIES_PROCESSOR                       ((BYTE)0x4D)    // AMD Opteron™ 6300 Series Processor
#define SMBIOS_PROCESSOR_FAMILY_AMD_OPTERON_3300_SERIES_PROCESSOR                       ((BYTE)0x4E)    // AMD Opteron™ 3300 Series Processor
#define SMBIOS_PROCESSOR_FAMILY_AMD_FIREPRO_SERIES_PROCESSOR                            ((BYTE)0x4F)    // AMD FirePro™ Series Processor
#define SMBIOS_PROCESSOR_FAMILY_SPARC_FAMILY                                            ((BYTE)0x50)    // SPARC Family
#define SMBIOS_PROCESSOR_FAMILY_SUPERSPARC                                              ((BYTE)0x51)    // SuperSPARC
#define SMBIOS_PROCESSOR_FAMILY_MICROSPARC_II                                           ((BYTE)0x52)    // microSPARC II
#define SMBIOS_PROCESSOR_FAMILY_MICROSPARC_IIEP                                         ((BYTE)0x53)    // microSPARC IIep
#define SMBIOS_PROCESSOR_FAMILY_ULTRASPARC                                              ((BYTE)0x54)    // UltraSPARC
#define SMBIOS_PROCESSOR_FAMILY_ULTRASPARC_II                                           ((BYTE)0x55)    // UltraSPARC II
#define SMBIOS_PROCESSOR_FAMILY_ULTRASPARC_Iii                                          ((BYTE)0x56)    // UltraSPARC Iii
#define SMBIOS_PROCESSOR_FAMILY_ULTRASPARC_III                                          ((BYTE)0x57)    // UltraSPARC III
#define SMBIOS_PROCESSOR_FAMILY_ULTRASPARC_IIIi                                         ((BYTE)0x58)    // UltraSPARC IIIi
#define SMBIOS_PROCESSOR_FAMILY_68040_FAMILY                                            ((BYTE)0x60)    // 68040 Family
#define SMBIOS_PROCESSOR_FAMILY_68XXX                                                   ((BYTE)0x61)    // 68xxx
#define SMBIOS_PROCESSOR_FAMILY_68000                                                   ((BYTE)0x62)    // 68000
#define SMBIOS_PROCESSOR_FAMILY_68010                                                   ((BYTE)0x63)    // 68010
#define SMBIOS_PROCESSOR_FAMILY_68020                                                   ((BYTE)0x64)    // 68020
#define SMBIOS_PROCESSOR_FAMILY_68030                                                   ((BYTE)0x65)    // 68030
#define SMBIOS_PROCESSOR_FAMILY_AMD_ATHLON_X4_QUAD_CORE_PROCESSOR_FAMILY                ((BYTE)0x66)    // AMD Athlon(TM) X4 Quad-Core Processor Family
#define SMBIOS_PROCESSOR_FAMILY_AMD_OPTERON_X1000_SERIES_PROCESSOR                      ((BYTE)0x67)    // AMD Opteron(TM) X1000 Series Processor
#define SMBIOS_PROCESSOR_FAMILY_AMD_OPTERON_X2000_SERIES_APU                            ((BYTE)0x68)    // AMD Opteron(TM) X2000 Series APU
#define SMBIOS_PROCESSOR_FAMILY_AMD_OPTERON_A_SERIES_PROCESSOR                          ((BYTE)0x69)    // AMD Opteron(TM) A-Series Processor
#define SMBIOS_PROCESSOR_FAMILY_AMD_OPTERON_X3000_SERIES_APU                            ((BYTE)0x6A)    // AMD Opteron(TM) X3000 Series APU
#define SMBIOS_PROCESSOR_FAMILY_AMD_ZEN_PROCESSOR_FAMILY                                ((BYTE)0x6B)    // AMD Zen Processor Family
#define SMBIOS_PROCESSOR_FAMILY_HOBBIT_FAMILY                                           ((BYTE)0x70)    // Hobbit Family
#define SMBIOS_PROCESSOR_FAMILY_CRUSOE_TM5000_FAMILY                                    ((BYTE)0x78)    // Crusoe™ TM5000 Family
#define SMBIOS_PROCESSOR_FAMILY_CRUSOE_TM3000_FAMILY                                    ((BYTE)0x79)    // Crusoe™ TM3000 Family
#define SMBIOS_PROCESSOR_FAMILY_EFFICEON_TM8000_FAMILY                                  ((BYTE)0x7A)    // Efficeon™ TM8000 Family
#define SMBIOS_PROCESSOR_FAMILY_WEITEK                                                  ((BYTE)0x80)    // Weitek
#define SMBIOS_PROCESSOR_FAMILY_ITANIUM_PROCESSOR                                       ((BYTE)0x82)    // Itanium™ processor
#define SMBIOS_PROCESSOR_FAMILY_AMD_ATHLON_64_PROCESSOR_FAMILY                          ((BYTE)0x83)    // AMD Athlon™ 64 Processor Family
#define SMBIOS_PROCESSOR_FAMILY_AMD_OPTERON_PROCESSOR_FAMILY                            ((BYTE)0x84)    // AMD Opteron™ Processor Family
#define SMBIOS_PROCESSOR_FAMILY_AMD_SEMPRON_PROCESSOR_FAMILY                            ((BYTE)0x85)    // AMD Sempron™ Processor Family
#define SMBIOS_PROCESSOR_FAMILY_AMD_TURION_64_MOBILE_TECHNOLOGY                         ((BYTE)0x86)    // AMD Turion™ 64 Mobile Technology
#define SMBIOS_PROCESSOR_FAMILY_DUAL_CORE_AMD_OPTERON_PROCESSOR_FAMILY                  ((BYTE)0x87)    // Dual-Core AMD Opteron™ Processor Family
#define SMBIOS_PROCESSOR_FAMILY_AMD_ATHLON_64_X2_DUAL_CORE_PROCESSOR_FAMILY             ((BYTE)0x88)    // AMD Athlon™ 64 X2 Dual-Core Processor Family
#define SMBIOS_PROCESSOR_FAMILY_AMD_TURION_64_X2_MOBILE_TECHNOLOGY                      ((BYTE)0x89)    // AMD Turion™ 64 X2 Mobile Technology
#define SMBIOS_PROCESSOR_FAMILY_QUAD_CORE_AMD_OPTERON_PROCESSOR_FAMILY                  ((BYTE)0x8A)    // Quad-Core AMD Opteron™ Processor Family
#define SMBIOS_PROCESSOR_FAMILY_THIRD_GENERATION_AMD_OPTERON_PROCESSOR_FAMILY           ((BYTE)0x8B)    // Third-Generation AMD Opteron™ Processor Family
#define SMBIOS_PROCESSOR_FAMILY_AMD_PHENOM_FX_QUAD_CORE_PROCESSOR_FAMILY                ((BYTE)0x8C)    // AMD Phenom™ FX Quad-Core Processor Family
#define SMBIOS_PROCESSOR_FAMILY_AMD_PHENOM_X4_QUAD_CORE_PROCESSOR_FAMILY                ((BYTE)0x8D)    // AMD Phenom™ X4 Quad-Core Processor Family
#define SMBIOS_PROCESSOR_FAMILY_AMD_PHENOM_X2_DUAL_CORE_PROCESSOR_FAMILY                ((BYTE)0x8E)    // AMD Phenom™ X2 Dual-Core Processor Family
#define SMBIOS_PROCESSOR_FAMILY_AMD_ATHLON_X2_DUAL_CORE_PROCESSOR_FAMILY                ((BYTE)0x8F)    // AMD Athlon™ X2 Dual-Core Processor Family
#define SMBIOS_PROCESSOR_FAMILY_PA_RISC_FAMILY                                          ((BYTE)0x90)    // PA-RISC Family
#define SMBIOS_PROCESSOR_FAMILY_PA_RISC_8500                                            ((BYTE)0x91)    // PA-RISC 8500
#define SMBIOS_PROCESSOR_FAMILY_PA_RISC_8000                                            ((BYTE)0x92)    // PA-RISC 8000
#define SMBIOS_PROCESSOR_FAMILY_PA_RISC_7300LC                                          ((BYTE)0x93)    // PA-RISC 7300LC
#define SMBIOS_PROCESSOR_FAMILY_PA_RISC_7200                                            ((BYTE)0x94)    // PA-RISC 7200
#define SMBIOS_PROCESSOR_FAMILY_PA_RISC_7100LC                                          ((BYTE)0x95)    // PA-RISC 7100LC
#define SMBIOS_PROCESSOR_FAMILY_PA_RISC_7100                                            ((BYTE)0x96)    // PA-RISC 7100
#define SMBIOS_PROCESSOR_FAMILY_V30_FAMILY                                              ((BYTE)0xA0)    // V30 Family
#define SMBIOS_PROCESSOR_FAMILY_QUAD_CORE_INTEL_XEON_PROCESSOR_3200_SERIES              ((BYTE)0xA1)    // Quad-Core Intel® Xeon® processor 3200 Series
#define SMBIOS_PROCESSOR_FAMILY_DUAL_CORE_INTEL_XEON_PROCESSOR_3000_SERIES              ((BYTE)0xA2)    // Dual-Core Intel® Xeon® processor 3000 Series
#define SMBIOS_PROCESSOR_FAMILY_QUAD_CORE_INTEL_XEON_PROCESSOR_5300_SERIES              ((BYTE)0xA3)    // Quad-Core Intel® Xeon® processor 5300 Series
#define SMBIOS_PROCESSOR_FAMILY_DUAL_CORE_INTEL_XEON_PROCESSOR_5100_SERIES              ((BYTE)0xA4)    // Dual-Core Intel® Xeon® processor 5100 Series
#define SMBIOS_PROCESSOR_FAMILY_DUAL_CORE_INTEL_XEON_PROCESSOR_5000_SERIES              ((BYTE)0xA5)    // Dual-Core Intel® Xeon® processor 5000 Series
#define SMBIOS_PROCESSOR_FAMILY_DUAL_CORE_INTEL_XEON_PROCESSOR_LV                       ((BYTE)0xA6)    // Dual-Core Intel® Xeon® processor LV
#define SMBIOS_PROCESSOR_FAMILY_DUAL_CORE_INTEL_XEON_PROCESSOR_ULV                      ((BYTE)0xA7)    // Dual-Core Intel® Xeon® processor ULV
#define SMBIOS_PROCESSOR_FAMILY_DUAL_CORE_INTEL_XEON_PROCESSOR_7100_SERIES              ((BYTE)0xA8)    // Dual-Core Intel® Xeon® processor 7100 Series
#define SMBIOS_PROCESSOR_FAMILY_QUAD_CORE_INTEL_XEON_PROCESSOR_5400_SERIES              ((BYTE)0xA9)    // Quad-Core Intel® Xeon® processor 5400 Series
#define SMBIOS_PROCESSOR_FAMILY_QUAD_CORE_INTEL_XEON_PROCESSOR                          ((BYTE)0xAA)    // Quad-Core Intel® Xeon® processor
#define SMBIOS_PROCESSOR_FAMILY_DUAL_CORE_INTEL_XEON_PROCESSOR_5200_SERIES              ((BYTE)0xAB)    // Dual-Core Intel® Xeon® processor 5200 Series
#define SMBIOS_PROCESSOR_FAMILY_DUAL_CORE_INTEL_XEON_PROCESSOR_7200_SERIES              ((BYTE)0xAC)    // Dual-Core Intel® Xeon® processor 7200 Series
#define SMBIOS_PROCESSOR_FAMILY_QUAD_CORE_INTEL_XEON_PROCESSOR_7300_SERIES              ((BYTE)0xAD)    // Quad-Core Intel® Xeon® processor 7300 Series
#define SMBIOS_PROCESSOR_FAMILY_QUAD_CORE_INTEL_XEON_PROCESSOR_7400_SERIES              ((BYTE)0xAE)    // Quad-Core Intel® Xeon® processor 7400 Series
#define SMBIOS_PROCESSOR_FAMILY_MULTI_CORE_INTEL_XEON_PROCESSOR_7400_SERIES             ((BYTE)0xAF)    // Multi-Core Intel® Xeon® processor 7400 Series
#define SMBIOS_PROCESSOR_FAMILY_PENTIUM_III_XEON_PROCESSOR                              ((BYTE)0xB0)    // Pentium® III Xeon® processor
#define SMBIOS_PROCESSOR_FAMILY_PENTIUM_III_PROCESSOR_WITH_INTEL_SPEEDSTEP_TECHNOLOGY   ((BYTE)0xB1)    // Pentium® III Processor with Intel® SpeedStep™ Technology
#define SMBIOS_PROCESSOR_FAMILY_PENTIUM_4_PROCESSOR                                     ((BYTE)0xB2)    // Pentium® 4 Processor
#define SMBIOS_PROCESSOR_FAMILY_INTEL_XEON_PROCESSOR                                    ((BYTE)0xB3)    // Intel® Xeon® processor
#define SMBIOS_PROCESSOR_FAMILY_AS400_FAMILY                                            ((BYTE)0xB4)    // AS400 Family
#define SMBIOS_PROCESSOR_FAMILY_INTEL_XEON_PROCESSOR_MP                                 ((BYTE)0xB5)    // Intel® Xeon® processor MP
#define SMBIOS_PROCESSOR_FAMILY_AMD_ATHLON_XP_PROCESSOR_FAMILY                          ((BYTE)0xB6)    // AMD Athlon™ XP Processor Family
#define SMBIOS_PROCESSOR_FAMILY_AMD_ATHLON_MP_PROCESSOR_FAMILY                          ((BYTE)0xB7)    // AMD Athlon™ MP Processor Family
#define SMBIOS_PROCESSOR_FAMILY_INTEL_ITANIUM_2_PROCESSOR                               ((BYTE)0xB8)    // Intel® Itanium® 2 processor
#define SMBIOS_PROCESSOR_FAMILY_INTEL_PENTIUM_M_PROCESSOR                               ((BYTE)0xB9)    // Intel® Pentium® M processor
#define SMBIOS_PROCESSOR_FAMILY_INTEL_CELERON_D_PROCESSOR                               ((BYTE)0xBA)    // Intel® Celeron® D processor
#define SMBIOS_PROCESSOR_FAMILY_INTEL_PENTIUM_D_PROCESSOR                               ((BYTE)0xBB)    // Intel® Pentium® D processor
#define SMBIOS_PROCESSOR_FAMILY_INTEL_PENTIUM_PROCESSOR_EXTREME_EDITION                 ((BYTE)0xBC)    // Intel® Pentium® Processor Extreme Edition
#define SMBIOS_PROCESSOR_FAMILY_INTEL_CORE_SOLO_PROCESSOR                               ((BYTE)0xBD)    // Intel® Core™ Solo Processor
#define SMBIOS_PROCESSOR_FAMILY_INTEL_CORE_2_DUO_PROCESSOR                              ((BYTE)0xBF)    // Intel® Core™ 2 Duo Processor
#define SMBIOS_PROCESSOR_FAMILY_INTEL_CORE_2_SOLO_PROCESSOR                             ((BYTE)0xC0)    // Intel® Core™ 2 Solo processor
#define SMBIOS_PROCESSOR_FAMILY_INTEL_CORE_2_EXTREME_PROCESSOR                          ((BYTE)0xC1)    // Intel® Core™ 2 Extreme processor
#define SMBIOS_PROCESSOR_FAMILY_INTEL_CORE_2_QUAD_PROCESSOR                             ((BYTE)0xC2)    // Intel® Core™ 2 Quad processor
#define SMBIOS_PROCESSOR_FAMILY_INTEL_CORE_2_EXTREME_MOBILE_PROCESSOR                   ((BYTE)0xC3)    // Intel® Core™ 2 Extreme mobile processor
#define SMBIOS_PROCESSOR_FAMILY_INTEL_CORE_2_DUO_MOBILE_PROCESSOR                       ((BYTE)0xC4)    // Intel® Core™ 2 Duo mobile processor
#define SMBIOS_PROCESSOR_FAMILY_INTEL_CORE_2_SOLO_MOBILE_PROCESSOR                      ((BYTE)0xC5)    // Intel® Core™ 2 Solo mobile processor
#define SMBIOS_PROCESSOR_FAMILY_INTEL_CORE_I7_PROCESSOR                                 ((BYTE)0xC6)    // Intel® Core™ i7 processor
#define SMBIOS_PROCESSOR_FAMILY_DUAL_CORE_INTEL_CELERON_PROCESSOR                       ((BYTE)0xC7)    // Dual-Core Intel® Celeron® processor
#define SMBIOS_PROCESSOR_FAMILY_IBM390_FAMILY                                           ((BYTE)0xC8)    // IBM390 Family
#define SMBIOS_PROCESSOR_FAMILY_G4                                                      ((BYTE)0xC9)    // G4
#define SMBIOS_PROCESSOR_FAMILY_G5                                                      ((BYTE)0xCA)    // G5
#define SMBIOS_PROCESSOR_FAMILY_ESA_390_G6                                              ((BYTE)0xCB)    // ESA/390 G6
#define SMBIOS_PROCESSOR_FAMILY_Z_ARCHITECTURE_BASE                                     ((BYTE)0xCC)    // z/Architecture base
#define SMBIOS_PROCESSOR_FAMILY_INTEL_CORE_I5_PROCESSOR                                 ((BYTE)0xCD)    // Intel® Core™ i5 processor
#define SMBIOS_PROCESSOR_FAMILY_INTEL_CORE_I3_PROCESSOR                                 ((BYTE)0xCE)    // Intel® Core™ i3 processor
#define SMBIOS_PROCESSOR_FAMILY_INTEL_CORE_I9_PROCESSOR                                 ((BYTE)0xCF)    // Intel® Core™ i9 processor
#define SMBIOS_PROCESSOR_FAMILY_INTEL_XEON_D_PROCESSOR_FAMILY                           ((BYTE)0xD0)    // Intel® Xeon® D Processor family
#define SMBIOS_PROCESSOR_FAMILY_VIA_C7_M_PROCESSOR_FAMILY                               ((BYTE)0xD2)    // VIA C7™-M Processor Family
#define SMBIOS_PROCESSOR_FAMILY_VIA_C7_D_PROCESSOR_FAMILY                               ((BYTE)0xD3)    // VIA C7™-D Processor Family
#define SMBIOS_PROCESSOR_FAMILY_VIA_C7_PROCESSOR_FAMILY                                 ((BYTE)0xD4)    // VIA C7™ Processor Family
#define SMBIOS_PROCESSOR_FAMILY_VIA_EDEN_PROCESSOR_FAMILY                               ((BYTE)0xD5)    // VIA Eden™ Processor Family
#define SMBIOS_PROCESSOR_FAMILY_MULTI_CORE_INTEL_XEON_PROCESSOR                         ((BYTE)0xD6)    // Multi-Core Intel® Xeon® processor
#define SMBIOS_PROCESSOR_FAMILY_DUAL_CORE_INTEL_XEON_PROCESSOR_3XXX_SERIES              ((BYTE)0xD7)    // Dual-Core Intel® Xeon® processor 3xxx Series
#define SMBIOS_PROCESSOR_FAMILY_QUAD_CORE_INTEL_XEON_PROCESSOR_3XXX_SERIES              ((BYTE)0xD8)    // Quad-Core Intel® Xeon® processor 3xxx Series
#define SMBIOS_PROCESSOR_FAMILY_VIA_NANO_PROCESSOR_FAMILY                               ((BYTE)0xD9)    // VIA Nano™ Processor Family
#define SMBIOS_PROCESSOR_FAMILY_DUAL_CORE_INTEL_XEON_PROCESSOR_5XXX_SERIES              ((BYTE)0xDA)    // Dual-Core Intel® Xeon® processor 5xxx Series
#define SMBIOS_PROCESSOR_FAMILY_QUAD_CORE_INTEL_XEON_PROCESSOR_5XXX_SERIES              ((BYTE)0xDB)    // Quad-Core Intel® Xeon® processor 5xxx Series
#define SMBIOS_PROCESSOR_FAMILY_DUAL_CORE_INTEL_XEON_PROCESSOR_7XXX_SERIES              ((BYTE)0xDD)    // Dual-Core Intel® Xeon® processor 7xxx Series
#define SMBIOS_PROCESSOR_FAMILY_QUAD_CORE_INTEL_XEON_PROCESSOR_7XXX_SERIES              ((BYTE)0xDE)    // Quad-Core Intel® Xeon® processor 7xxx Series
#define SMBIOS_PROCESSOR_FAMILY_MULTI_CORE_INTEL_XEON_PROCESSOR_7XXX_SERIES             ((BYTE)0xDF)    // Multi-Core Intel® Xeon® processor 7xxx Series
#define SMBIOS_PROCESSOR_FAMILY_MULTI_CORE_INTEL_XEON_PROCESSOR_3400_SERIES             ((BYTE)0xE0)    // Multi-Core Intel® Xeon® processor 3400 Series
#define SMBIOS_PROCESSOR_FAMILY_AMD_OPTERON_3000_SERIES_PROCESSOR                       ((BYTE)0xE4)    // AMD Opteron™ 3000 Series Processor
#define SMBIOS_PROCESSOR_FAMILY_AMD_SEMPRON_II_PROCESSOR                                ((BYTE)0xE5)    // AMD Sempron™ II Processor
#define SMBIOS_PROCESSOR_FAMILY_EMBEDDED_AMD_OPTERON_QUAD_CORE_PROCESSOR_FAMILY         ((BYTE)0xE6)    // Embedded AMD Opteron™ Quad-Core Processor Family
#define SMBIOS_PROCESSOR_FAMILY_AMD_PHENOM_TRIPLE_CORE_PROCESSOR_FAMILY                 ((BYTE)0xE7)    // AMD Phenom™ Triple-Core Processor Family
#define SMBIOS_PROCESSOR_FAMILY_AMD_TURION_ULTRA_DUAL_CORE_MOBILE_PROCESSOR_FAMILY      ((BYTE)0xE8)    // AMD Turion™ Ultra Dual-Core Mobile Processor Family
#define SMBIOS_PROCESSOR_FAMILY_AMD_TURION_DUAL_CORE_MOBILE_PROCESSOR_FAMILY            ((BYTE)0xE9)    // AMD Turion™ Dual-Core Mobile Processor Family
#define SMBIOS_PROCESSOR_FAMILY_AMD_ATHLON_DUAL_CORE_PROCESSOR_FAMILY                   ((BYTE)0xEA)    // AMD Athlon™ Dual-Core Processor Family
#define SMBIOS_PROCESSOR_FAMILY_AMD_SEMPRON_SI_PROCESSOR_FAMILY                         ((BYTE)0xEB)    // AMD Sempron™ SI Processor Family
#define SMBIOS_PROCESSOR_FAMILY_AMD_PHENOM_II_PROCESSOR_FAMILY                          ((BYTE)0xEC)    // AMD Phenom™ II Processor Family
#define SMBIOS_PROCESSOR_FAMILY_AMD_ATHLON_II_PROCESSOR_FAMILY                          ((BYTE)0xED)    // AMD Athlon™ II Processor Family
#define SMBIOS_PROCESSOR_FAMILY_SIX_CORE_AMD_OPTERON_PROCESSOR_FAMILY                   ((BYTE)0xEE)    // Six-Core AMD Opteron™ Processor Family
#define SMBIOS_PROCESSOR_FAMILY_AMD_SEMPRON_M_PROCESSOR_FAMILY                          ((BYTE)0xEF)    // AMD Sempron™ M Processor Family
#define SMBIOS_PROCESSOR_FAMILY_I860                                                    ((BYTE)0xFA)    // i860
#define SMBIOS_PROCESSOR_FAMILY_I960                                                    ((BYTE)0xFB)    // i960
#define SMBIOS_PROCESSOR_FAMILY_EXTENSION_INDICATOR                                     ((BYTE)0xFE)    // (See Processor Family 2)

#define SMBIOS_PROCESSOR_FAMILY_2_ARMV7                                                 ((WORD)0x0100) // ARMv7
#define SMBIOS_PROCESSOR_FAMILY_2_ARMV8                                                 ((WORD)0x0101) // ARMv8
#define SMBIOS_PROCESSOR_FAMILY_2_ARMV9                                                 ((WORD)0x0102) // ARMv9
#define SMBIOS_PROCESSOR_FAMILY_2_SH_3                                                  ((WORD)0x0104) // SH-3
#define SMBIOS_PROCESSOR_FAMILY_2_SH_4                                                  ((WORD)0x0105) // SH-4
#define SMBIOS_PROCESSOR_FAMILY_2_ARM                                                   ((WORD)0x0118) // ARM
#define SMBIOS_PROCESSOR_FAMILY_2_STRONGARM                                             ((WORD)0x0119) // StrongARM
#define SMBIOS_PROCESSOR_FAMILY_2_6X86                                                  ((WORD)0x012C) // 6x86
#define SMBIOS_PROCESSOR_FAMILY_2_MEDIAGX                                               ((WORD)0x012D) // MediaGX
#define SMBIOS_PROCESSOR_FAMILY_2_MII                                                   ((WORD)0x012E) // MII
#define SMBIOS_PROCESSOR_FAMILY_2_WINCHIP                                               ((WORD)0x0140) // WinChip
#define SMBIOS_PROCESSOR_FAMILY_2_DSP                                                   ((WORD)0x015E) // DSP
#define SMBIOS_PROCESSOR_FAMILY_2_VIDEO_PROCESSOR                                       ((WORD)0x01F4) // Video Processor
#define SMBIOS_PROCESSOR_FAMILY_2_RISC_V_RV32                                           ((WORD)0x0200) // RISC-V RV32
#define SMBIOS_PROCESSOR_FAMILY_2_RISC_V_RV64                                           ((WORD)0x0201) // RISC-V RV64
#define SMBIOS_PROCESSOR_FAMILY_2_RISC_V_RV128                                          ((WORD)0x0202) // RISC-V RV128
#define SMBIOS_PROCESSOR_FAMILY_2_LOONGARCH                                             ((WORD)0x0258) // LoongArch
#define SMBIOS_PROCESSOR_FAMILY_2_LOONGSON_1_PROCESSOR_FAMILY                           ((WORD)0x0259) // Loongson™ 1 Processor Family
#define SMBIOS_PROCESSOR_FAMILY_2_LOONGSON_2_PROCESSOR_FAMILY                           ((WORD)0x025A) // Loongson™ 2 Processor Family
#define SMBIOS_PROCESSOR_FAMILY_2_LOONGSON_3_PROCESSOR_FAMILY                           ((WORD)0x025B) // Loongson™ 3 Processor Family
#define SMBIOS_PROCESSOR_FAMILY_2_LOONGSON_2K_PROCESSOR_FAMILY                          ((WORD)0x025C) // Loongson™ 2K Processor Family
#define SMBIOS_PROCESSOR_FAMILY_2_LOONGSON_3A_PROCESSOR_FAMILY                          ((WORD)0x025D) // Loongson™ 3A Processor Family
#define SMBIOS_PROCESSOR_FAMILY_2_LOONGSON_3B_PROCESSOR_FAMILY                          ((WORD)0x025E) // Loongson™ 3B Processor Family
#define SMBIOS_PROCESSOR_FAMILY_2_LOONGSON_3C_PROCESSOR_FAMILY                          ((WORD)0x025F) // Loongson™ 3C Processor Family
#define SMBIOS_PROCESSOR_FAMILY_2_LOONGSON_3D_PROCESSOR_FAMILY                          ((WORD)0x0260) // Loongson™ 3D Processor Family
#define SMBIOS_PROCESSOR_FAMILY_2_LOONGSON_3E_PROCESSOR_FAMILY                          ((WORD)0x0261) // Loongson™ 3E Processor Family
#define SMBIOS_PROCESSOR_FAMILY_2_DUAL_CORE_LOONGSON_2K_PROCESSOR_2XXX_SERIES           ((WORD)0x0262) // Dual-Core Loongson™ 2K Processor 2xxx Series
#define SMBIOS_PROCESSOR_FAMILY_2_QUAD_CORE_LOONGSON_3A_PROCESSOR_5XXX_SERIES           ((WORD)0x026C) // Quad-Core Loongson™ 3A Processor 5xxx Series
#define SMBIOS_PROCESSOR_FAMILY_2_MULTI_CORE_LOONGSON_3A_PROCESSOR_5XXX_SERIES          ((WORD)0x026D) // Multi-Core Loongson™ 3A Processor 5xxx Series
#define SMBIOS_PROCESSOR_FAMILY_2_QUAD_CORE_LOONGSON_3B_PROCESSOR_5XXX_SERIES           ((WORD)0x026E) // Quad-Core Loongson™ 3B Processor 5xxx Series
#define SMBIOS_PROCESSOR_FAMILY_2_MULTI_CORE_LOONGSON_3B_PROCESSOR_5XXX_SERIES          ((WORD)0x026F) // Multi-Core Loongson™ 3B Processor 5xxx Series
#define SMBIOS_PROCESSOR_FAMILY_2_MULTI_CORE_LOONGSON_3C_PROCESSOR_5XXX_SERIES          ((WORD)0x0270) // Multi-Core Loongson™ 3C Processor 5xxx Series
#define SMBIOS_PROCESSOR_FAMILY_2_MULTI_CORE_LOONGSON_3D_PROCESSOR_5XXX_SERIES          ((WORD)0x0271) // Multi-Core Loongson™ 3D Processor 5xxx Series
#define SMBIOS_PROCESSOR_FAMILY_2_INTEL_CORE_3                                          ((WORD)0x0300) // Intel® Core™ 3
#define SMBIOS_PROCESSOR_FAMILY_2_INTEL_CORE_5                                          ((WORD)0x0301) // Intel® Core™ 5
#define SMBIOS_PROCESSOR_FAMILY_2_INTEL_CORE_7                                          ((WORD)0x0302) // Intel® Core™ 7
#define SMBIOS_PROCESSOR_FAMILY_2_INTEL_CORE_9                                          ((WORD)0x0303) // Intel® Core™ 9
#define SMBIOS_PROCESSOR_FAMILY_2_INTEL_CORE_ULTRA_3                                    ((WORD)0x0304) // Intel® Core™ Ultra 3
#define SMBIOS_PROCESSOR_FAMILY_2_INTEL_CORE_ULTRA_5                                    ((WORD)0x0305) // Intel® Core™ Ultra 5
#define SMBIOS_PROCESSOR_FAMILY_2_INTEL_CORE_ULTRA_7                                    ((WORD)0x0306) // Intel® Core™ Ultra 7
#define SMBIOS_PROCESSOR_FAMILY_2_INTEL_CORE_ULTRA_9                                    ((WORD)0x0307) // Intel® Core™ Ultra 9

#define SMBIOS_PROCESSOR_CPU_STATUS_UNKNOWN                 ((BYTE)0x0) // Unknown
#define SMBIOS_PROCESSOR_CPU_STATUS_ENABLED                 ((BYTE)0x1) // CPU Enabled
#define SMBIOS_PROCESSOR_CPU_STATUS_DISABLED_BY_USER        ((BYTE)0x2) // CPU Disabled by User through Firmware Setup
#define SMBIOS_PROCESSOR_CPU_STATUS_DISABLED_BY_FIRMWARE    ((BYTE)0x3) // CPU Disabled By firmware (POST Error)
#define SMBIOS_PROCESSOR_CPU_STATUS_IDLE                    ((BYTE)0x4) // CPU is Idle, waiting to be enabled
#define SMBIOS_PROCESSOR_CPU_STATUS_OTHER                   ((BYTE)0x7) // Other

#define SMBIOS_PROCESSOR_UPGRADE_OTHER                  ((BYTE)0x01) // Other
#define SMBIOS_PROCESSOR_UPGRADE_UNKNOWN                ((BYTE)0x02) // Unknown
#define SMBIOS_PROCESSOR_UPGRADE_DAUGHTER_BOARD         ((BYTE)0x03) // Daughter Board
#define SMBIOS_PROCESSOR_UPGRADE_ZIF_SOCKET             ((BYTE)0x04) // ZIF Socket
#define SMBIOS_PROCESSOR_UPGRADE_REPLACEABLE_PIGGY_BACK ((BYTE)0x05) // Replaceable Piggy Back
#define SMBIOS_PROCESSOR_UPGRADE_NONE                   ((BYTE)0x06) // None
#define SMBIOS_PROCESSOR_UPGRADE_LIF_SOCKET             ((BYTE)0x07) // LIF Socket
#define SMBIOS_PROCESSOR_UPGRADE_SLOT_1                 ((BYTE)0x08) // Slot 1
#define SMBIOS_PROCESSOR_UPGRADE_SLOT_2                 ((BYTE)0x09) // Slot 2
#define SMBIOS_PROCESSOR_UPGRADE_370_PIN_SOCKET         ((BYTE)0x0A) // 370-pin socket
#define SMBIOS_PROCESSOR_UPGRADE_SLOT_A                 ((BYTE)0x0B) // Slot A
#define SMBIOS_PROCESSOR_UPGRADE_SLOT_M                 ((BYTE)0x0C) // Slot M
#define SMBIOS_PROCESSOR_UPGRADE_SOCKET_423             ((BYTE)0x0D) // Socket 423
#define SMBIOS_PROCESSOR_UPGRADE_SOCKET_A               ((BYTE)0x0E) // Socket A (Socket 462)
#define SMBIOS_PROCESSOR_UPGRADE_SOCKET_478             ((BYTE)0x0F) // Socket 478
#define SMBIOS_PROCESSOR_UPGRADE_SOCKET_754             ((BYTE)0x10) // Socket 754
#define SMBIOS_PROCESSOR_UPGRADE_SOCKET_940             ((BYTE)0x11) // Socket 940
#define SMBIOS_PROCESSOR_UPGRADE_SOCKET_939             ((BYTE)0x12) // Socket 939
#define SMBIOS_PROCESSOR_UPGRADE_SOCKET_mPGA604         ((BYTE)0x13) // Socket mPGA604
#define SMBIOS_PROCESSOR_UPGRADE_SOCKET_LGA771          ((BYTE)0x14) // Socket LGA771
#define SMBIOS_PROCESSOR_UPGRADE_SOCKET_LGA775          ((BYTE)0x15) // Socket LGA775
#define SMBIOS_PROCESSOR_UPGRADE_SOCKET_S1              ((BYTE)0x16) // Socket S1
#define SMBIOS_PROCESSOR_UPGRADE_SOCKET_AM2             ((BYTE)0x17) // Socket AM2
#define SMBIOS_PROCESSOR_UPGRADE_SOCKET_F               ((BYTE)0x18) // Socket F (1207)
#define SMBIOS_PROCESSOR_UPGRADE_SOCKET_LGA1366         ((BYTE)0x19) // Socket LGA1366
#define SMBIOS_PROCESSOR_UPGRADE_SOCKET_G34             ((BYTE)0x1A) // Socket G34
#define SMBIOS_PROCESSOR_UPGRADE_SOCKET_AM3             ((BYTE)0x1B) // Socket AM3
#define SMBIOS_PROCESSOR_UPGRADE_SOCKET_C32             ((BYTE)0x1C) // Socket C32
#define SMBIOS_PROCESSOR_UPGRADE_SOCKET_LGA1156         ((BYTE)0x1D) // Socket LGA1156
#define SMBIOS_PROCESSOR_UPGRADE_SOCKET_LGA1567         ((BYTE)0x1E) // Socket LGA1567
#define SMBIOS_PROCESSOR_UPGRADE_SOCKET_PGA988A         ((BYTE)0x1F) // Socket PGA988A
#define SMBIOS_PROCESSOR_UPGRADE_SOCKET_BGA1288         ((BYTE)0x20) // Socket BGA1288
#define SMBIOS_PROCESSOR_UPGRADE_SOCKET_rPGA988B        ((BYTE)0x21) // Socket rPGA988B
#define SMBIOS_PROCESSOR_UPGRADE_SOCKET_BGA1023         ((BYTE)0x22) // Socket BGA1023
#define SMBIOS_PROCESSOR_UPGRADE_SOCKET_BGA1224         ((BYTE)0x23) // Socket BGA1224
#define SMBIOS_PROCESSOR_UPGRADE_SOCKET_LGA1155         ((BYTE)0x24) // Socket LGA1155
#define SMBIOS_PROCESSOR_UPGRADE_SOCKET_LGA1356         ((BYTE)0x25) // Socket LGA1356
#define SMBIOS_PROCESSOR_UPGRADE_SOCKET_LGA2011         ((BYTE)0x26) // Socket LGA2011
#define SMBIOS_PROCESSOR_UPGRADE_SOCKET_FS1             ((BYTE)0x27) // Socket FS1
#define SMBIOS_PROCESSOR_UPGRADE_SOCKET_FS2             ((BYTE)0x28) // Socket FS2
#define SMBIOS_PROCESSOR_UPGRADE_SOCKET_FM1             ((BYTE)0x29) // Socket FM1
#define SMBIOS_PROCESSOR_UPGRADE_SOCKET_FM2             ((BYTE)0x2A) // Socket FM2
#define SMBIOS_PROCESSOR_UPGRADE_SOCKET_LGA2011_3       ((BYTE)0x2B) // Socket LGA2011-3
#define SMBIOS_PROCESSOR_UPGRADE_SOCKET_LGA1356_3       ((BYTE)0x2C) // Socket LGA2011-3
#define SMBIOS_PROCESSOR_UPGRADE_SOCKET_LGA1150         ((BYTE)0x2D) // Socket LGA1150
#define SMBIOS_PROCESSOR_UPGRADE_SOCKET_BGA1168         ((BYTE)0x2E) // Socket BGA1168
#define SMBIOS_PROCESSOR_UPGRADE_SOCKET_BGA1234         ((BYTE)0x2F) // Socket BGA1234
#define SMBIOS_PROCESSOR_UPGRADE_SOCKET_BGA1364         ((BYTE)0x30) // Socket BGA1364
#define SMBIOS_PROCESSOR_UPGRADE_SOCKET_AM4             ((BYTE)0x31) // Socket AM4
#define SMBIOS_PROCESSOR_UPGRADE_SOCKET_LGA1151         ((BYTE)0x32) // Socket LGA1151
#define SMBIOS_PROCESSOR_UPGRADE_SOCKET_BGA1356         ((BYTE)0x33) // Socket BGA1356
#define SMBIOS_PROCESSOR_UPGRADE_SOCKET_BGA1440         ((BYTE)0x34) // Socket BGA1440
#define SMBIOS_PROCESSOR_UPGRADE_SOCKET_BGA1515         ((BYTE)0x35) // Socket BGA1515
#define SMBIOS_PROCESSOR_UPGRADE_SOCKET_LGA3647_1       ((BYTE)0x36) // Socket LGA3647-1
#define SMBIOS_PROCESSOR_UPGRADE_SOCKET_SP3             ((BYTE)0x37) // Socket SP3
#define SMBIOS_PROCESSOR_UPGRADE_SOCKET_SP3r2           ((BYTE)0x38) // Socket SP3r2
#define SMBIOS_PROCESSOR_UPGRADE_SOCKET_LGA2066         ((BYTE)0x39) // Socket LGA2066
#define SMBIOS_PROCESSOR_UPGRADE_SOCKET_BGA1392         ((BYTE)0x3A) // Socket BGA1392
#define SMBIOS_PROCESSOR_UPGRADE_SOCKET_BGA1510         ((BYTE)0x3B) // Socket BGA1510
#define SMBIOS_PROCESSOR_UPGRADE_SOCKET_BGA1528         ((BYTE)0x3C) // Socket BGA1528
#define SMBIOS_PROCESSOR_UPGRADE_SOCKET_LGA4189         ((BYTE)0x3D) // Socket LGA4189
#define SMBIOS_PROCESSOR_UPGRADE_SOCKET_LGA1200         ((BYTE)0x3E) // Socket LGA1200
#define SMBIOS_PROCESSOR_UPGRADE_SOCKET_LGA4677         ((BYTE)0x3F) // Socket LGA4677
#define SMBIOS_PROCESSOR_UPGRADE_SOCKET_LGA1700         ((BYTE)0x40) // Socket LGA1700
#define SMBIOS_PROCESSOR_UPGRADE_SOCKET_BGA1744         ((BYTE)0x41) // Socket BGA1744
#define SMBIOS_PROCESSOR_UPGRADE_SOCKET_BGA1781         ((BYTE)0x42) // Socket BGA1781
#define SMBIOS_PROCESSOR_UPGRADE_SOCKET_BGA1211         ((BYTE)0x43) // Socket BGA1211
#define SMBIOS_PROCESSOR_UPGRADE_SOCKET_BGA2422         ((BYTE)0x44) // Socket BGA2422
#define SMBIOS_PROCESSOR_UPGRADE_SOCKET_LGA1211         ((BYTE)0x45) // Socket LGA1211
#define SMBIOS_PROCESSOR_UPGRADE_SOCKET_LGA2422         ((BYTE)0x46) // Socket LGA2422
#define SMBIOS_PROCESSOR_UPGRADE_SOCKET_LGA5773         ((BYTE)0x47) // Socket LGA5773
#define SMBIOS_PROCESSOR_UPGRADE_SOCKET_BGA5773         ((BYTE)0x48) // Socket BGA5773
#define SMBIOS_PROCESSOR_UPGRADE_SOCKET_AM5             ((BYTE)0x49) // Socket AM5
#define SMBIOS_PROCESSOR_UPGRADE_SOCKET_SP5             ((BYTE)0x4A) // Socket SP5
#define SMBIOS_PROCESSOR_UPGRADE_SOCKET_SP6             ((BYTE)0x4B) // Socket SP6
#define SMBIOS_PROCESSOR_UPGRADE_SOCKET_BGA883          ((BYTE)0x4C) // Socket BGA883
#define SMBIOS_PROCESSOR_UPGRADE_SOCKET_BGA1190         ((BYTE)0x4D) // Socket BGA1190
#define SMBIOS_PROCESSOR_UPGRADE_SOCKET_BGA4129         ((BYTE)0x4E) // Socket BGA4129
#define SMBIOS_PROCESSOR_UPGRADE_SOCKET_LGA4710         ((BYTE)0x4F) // Socket LGA4710
#define SMBIOS_PROCESSOR_UPGRADE_SOCKET_LGA7529         ((BYTE)0x50) // Socket LGA7529
#define SMBIOS_PROCESSOR_UPGRADE_SOCKET_BGA1964         ((BYTE)0x51) // Socket BGA1964
#define SMBIOS_PROCESSOR_UPGRADE_SOCKET_BGA1792         ((BYTE)0x52) // Socket BGA1792
#define SMBIOS_PROCESSOR_UPGRADE_SOCKET_BGA2049         ((BYTE)0x53) // Socket BGA2049
#define SMBIOS_PROCESSOR_UPGRADE_SOCKET_BGA2551         ((BYTE)0x54) // Socket BGA2551
#define SMBIOS_PROCESSOR_UPGRADE_SOCKET_LGA1851         ((BYTE)0x55) // Socket LGA1851
#define SMBIOS_PROCESSOR_UPGRADE_SOCKET_BGA2114         ((BYTE)0x56) // Socket BGA2114
#define SMBIOS_PROCESSOR_UPGRADE_SOCKET_BGA2833         ((BYTE)0x57) // Socket BGA2833
#define SMBIOS_PROCESSOR_UPGRADE_INVALID                ((BYTE)0xFF) // Invalid

typedef struct _SMBIOS_PROCESSOR_INFORMATION
{
    SMBIOS_HEADER Header;
    BYTE SocketDesignation;
    BYTE Type; // SMBIOS_PROCESSOR_TYPE_*
    BYTE Family; // SMBIOS_PROCESSOR_FAMILY_*
    BYTE Manufacturer;
    QWORD ID;
    BYTE Version;
    union
    {
        BYTE Value;
        struct
        {
            BYTE _5V : 1;           // 00 5V
            BYTE _3Dot3V : 1;       // 01 3.3V
            BYTE _2Dot9V : 1;       // 02 2.9V
            BYTE Reserved0 : 1;     // 03 Reserved, must be zero
            BYTE Reserved1 : 3;     // 04:06 Reserved, must be zero
            BYTE LegacyMode : 1;    // Should be 0
        };
        struct
        {
            BYTE Times10 : 7;       // Current voltage times 10
            BYTE NotLegacyMode : 1; // Should be 1
        };
    } Voltage; // Deprecated from version 3.8.0
    WORD ExternalClock;
    WORD MaxSpeed;
    WORD CurrentSpeed;
    union
    {
        BYTE Value;
        struct
        {
            BYTE CPUStatus : 3;             // 0:2 SMBIOS_PROCESSOR_CPU_STATUS_*
            BYTE Reserved2 : 3;             // 3:5 Reserved, must be zero
            BYTE CPUSocketPopulated : 1;    // 6 CPU Socket Populated
            BYTE Reserved3 : 1;             // 7 Reserved, must be zero
        };
    } Status;
    BYTE Upgrade; // SMBIOS_PROCESSOR_UPGRADE_*
#if SMBIOS_VERSION >= 0x02010000
    WORD L1CacheHandle;
    WORD L2CacheHandle;
    WORD L3CacheHandle;
#if SMBIOS_VERSION >= 0x02030000
    BYTE SerialNumber;
    BYTE AssetTag;
    BYTE PartNumber;
#if SMBIOS_VERSION >= 0x02050000
    BYTE CoreCount;
    BYTE CoreEnabled;
    BYTE ThreadCount;
    union
    {
        WORD Value;
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
    } Characteristics;
#if SMBIOS_VERSION >= 0x02060000
    WORD Family2; // SMBIOS_PROCESSOR_FAMILY_2_*
#if SMBIOS_VERSION >= 0x03000000
    WORD CoreCount2;
    WORD CoreEnabled2;
    WORD ThreadCount2;
#if SMBIOS_VERSION >= 0x03060000
    WORD ThreadEnabled;
#if SMBIOS_VERSION >= 0x03080000
    BYTE SocketType;
#endif // SMBIOS_VERSION >= 0x03080000
#endif // SMBIOS_VERSION >= 0x03060000
#endif // SMBIOS_VERSION >= 0x03000000
#endif // SMBIOS_VERSION >= 0x02060000
#endif // SMBIOS_VERSION >= 0x02050000
#endif // SMBIOS_VERSION >= 0x02030000
#endif // SMBIOS_VERSION >= 0x02010000
} SMBIOS_PROCESSOR_INFORMATION, *PSMBIOS_PROCESSOR_INFORMATION, SMBIOS_TYPE_4, *PSMBIOS_TYPE_4;

#pragma endregion

typedef union _SMBIOS_MEMORY_TYPE
{
    WORD Value;
    struct
    {
        WORD Other;         // 00 Other
        WORD Unknown;       // 01 Unknown
        WORD Standard;      // 02 Standard
        WORD FastPageMode;  // 03 Fast Page Mode
        WORD EDO;           // 04 EDO
        WORD Parity;        // 05 Parity
        WORD ECC;           // 06 ECC
        WORD SIMM;          // 07 SIMM
        WORD DIMM;          // 08 DIMM
        WORD BurstEDO;      // 09 Burst EDO
        WORD SDRAM;         // 10 SDRAM
        WORD Reserved;      // 11:15 Reserved, must be zero
    };
} SMBIOS_MEMORY_TYPE, *PSMBIOS_MEMORY_TYPE;

#define SMBIOS_MEMORY_ERROR_TYPE_OTHER                  ((BYTE)0x01) // Other
#define SMBIOS_MEMORY_ERROR_TYPE_UNKNOWN                ((BYTE)0x02) // Unknown
#define SMBIOS_MEMORY_ERROR_TYPE_OK                     ((BYTE)0x03) // OK
#define SMBIOS_MEMORY_ERROR_TYPE_BAD_READ               ((BYTE)0x04) // Bad read
#define SMBIOS_MEMORY_ERROR_TYPE_PARITY                 ((BYTE)0x05) // Parity error
#define SMBIOS_MEMORY_ERROR_TYPE_SINGLE_BIT             ((BYTE)0x06) // Single-bit error
#define SMBIOS_MEMORY_ERROR_TYPE_DOUBLE_BIT             ((BYTE)0x07) // Double-bit error
#define SMBIOS_MEMORY_ERROR_TYPE_MULTI_BIT              ((BYTE)0x08) // Multi-bit error
#define SMBIOS_MEMORY_ERROR_TYPE_NIBBLE                 ((BYTE)0x09) // Nibble error
#define SMBIOS_MEMORY_ERROR_TYPE_CHECKSUM               ((BYTE)0x0A) // Checksum error
#define SMBIOS_MEMORY_ERROR_TYPE_CRC                    ((BYTE)0x0B) // CRC error
#define SMBIOS_MEMORY_ERROR_TYPE_CORRECTED_SINGLE_BIT   ((BYTE)0x0C) // Corrected single-bit error
#define SMBIOS_MEMORY_ERROR_TYPE_CORRECTED              ((BYTE)0x0D) // Corrected error
#define SMBIOS_MEMORY_ERROR_TYPE_UNCORRECTABLE          ((BYTE)0x0E) // Uncorrectable error

#define SMBIOS_MEMORY_ERROR_GRANULARITY_OTHER                   ((BYTE)0x01) // Other
#define SMBIOS_MEMORY_ERROR_GRANULARITY_UNKNOWN                 ((BYTE)0x02) // Unknown
#define SMBIOS_MEMORY_ERROR_GRANULARITY_DEVICE_LEVEL            ((BYTE)0x03) // Device level
#define SMBIOS_MEMORY_ERROR_GRANULARITY_MEMORY_PARTITION_LEVEL  ((BYTE)0x04) // Memory partition level

#define SMBIOS_MEMORY_ERROR_OPERATION_OTHER           ((BYTE)0x01) // Other
#define SMBIOS_MEMORY_ERROR_OPERATION_UNKNOWN         ((BYTE)0x02) // Unknown
#define SMBIOS_MEMORY_ERROR_OPERATION_READ            ((BYTE)0x03) // Read
#define SMBIOS_MEMORY_ERROR_OPERATION_WRITE           ((BYTE)0x04) // Write
#define SMBIOS_MEMORY_ERROR_OPERATION_PARTIAL_WRITE   ((BYTE)0x05) // Partial write

#pragma region Memory Controller Information (Type 5, Obsolete)

#define SMBIOS_TYPE_MEMORY_CONTROLLER_INFORMATION ((BYTE)5)

#define SMBIOS_MEMORY_CONTROLLER_ERROR_DETECTING_METHOD_OTHER       ((BYTE)0x01) // Other
#define SMBIOS_MEMORY_CONTROLLER_ERROR_DETECTING_METHOD_UNKNOWN     ((BYTE)0x02) // Unknown
#define SMBIOS_MEMORY_CONTROLLER_ERROR_DETECTING_METHOD_NONE        ((BYTE)0x03) // None
#define SMBIOS_MEMORY_CONTROLLER_ERROR_DETECTING_METHOD_8BIT_PARITY ((BYTE)0x04) // 8-bit Parity
#define SMBIOS_MEMORY_CONTROLLER_ERROR_DETECTING_METHOD_32BIT_ECC   ((BYTE)0x05) // 32-bit ECC
#define SMBIOS_MEMORY_CONTROLLER_ERROR_DETECTING_METHOD_64BIT_ECC   ((BYTE)0x06) // 64-bit ECC
#define SMBIOS_MEMORY_CONTROLLER_ERROR_DETECTING_METHOD_128BIT_ECC  ((BYTE)0x07) // 128-bit ECC
#define SMBIOS_MEMORY_CONTROLLER_ERROR_DETECTING_METHOD_CRC         ((BYTE)0x08) // CRC

#define SMBIOS_MEMORY_CONTROLLER_INTERLEAVE_OTHER       ((BYTE)0x01) // Other
#define SMBIOS_MEMORY_CONTROLLER_INTERLEAVE_UNKNOWN     ((BYTE)0x02) // Unknown
#define SMBIOS_MEMORY_CONTROLLER_INTERLEAVE_ONE_WAY     ((BYTE)0x03) // One-Way Interleave
#define SMBIOS_MEMORY_CONTROLLER_INTERLEAVE_TWO_WAY     ((BYTE)0x04) // Two-Way Interleave
#define SMBIOS_MEMORY_CONTROLLER_INTERLEAVE_FOUR_WAY    ((BYTE)0x05) // Four-Way Interleave
#define SMBIOS_MEMORY_CONTROLLER_INTERLEAVE_EIGHT_WAY   ((BYTE)0x06) // Eight-Way Interleave
#define SMBIOS_MEMORY_CONTROLLER_INTERLEAVE_SIXTEEN_WAY ((BYTE)0x07) // Sixteen-Way Interleave

typedef struct _SMBIOS_MEMORY_CONTROLLER_INFORMATION
{
    SMBIOS_HEADER Header;
    BYTE ErrorDetectingMethod;  // SMBIOS_MEMORY_CONTROLLER_ERROR_DETECTING_METHOD_*
    union
    {
        BYTE Value;
        struct
        {
            BYTE Other : 1;     // 0 Other
            BYTE Unknown : 1;   // 1 Unknown
            BYTE None : 1;      // 2 None
            BYTE SingleBit : 1; // 3 Single-Bit Error Correcting
            BYTE DoubleBit : 1; // 4 Double-Bit Error Correcting
            BYTE Scrubbing : 1; // 5 Error Scrubbing
        };
    } ErrorCorrectingCapability;
    BYTE SupportedInterleave;   // SMBIOS_MEMORY_CONTROLLER_INTERLEAVE_*
    BYTE CurrentInterleave;     // SMBIOS_MEMORY_CONTROLLER_INTERLEAVE_*
    BYTE MaximumMemoryModuleSize;
    union
    {
        WORD Value;
        struct
        {
            WORD Other;     // 00 Other
            WORD Unknown;   // 01 Unknown
            WORD _70ns;     // 02 70ns
            WORD _60ns;     // 03 60ns
            WORD _50ns;     // 04 50ns
            WORD Reserved;  // 05:15 Reserved, must be zero
        };
    } SupportedSpeeds;
    SMBIOS_MEMORY_TYPE SupportedMemoryTypes;
    union
    {
        BYTE Value;
        struct
        {
            BYTE _5V;       // 0 5V
            BYTE _3Dot3V;   // 1 3.3V
            BYTE _2Dot9V;   // 2 2.9V
            BYTE Reserved;  // 3:7 Reserved, must be zero
        };
    } MemoryModuleVoltage;
    BYTE NumberOfAssociatedMemorySlots;
    _Field_size_(NumberOfAssociatedMemorySlots) WORD MemoryModuleConfigurationHandles[];
/*
#if SMBIOS_VERSION >= 0x02010000
    BYTE EnabledErrorCorrectingCapabilities;
#endif // SMBIOS_VERSION >= 0x02010000
*/
} SMBIOS_MEMORY_CONTROLLER_INFORMATION, *PSMBIOS_MEMORY_CONTROLLER_INFORMATION, SMBIOS_TYPE_5, *PSMBIOS_TYPE_5;

#pragma endregion Obsolete

#pragma region Memory Module Information (Type 6, Obsolete)

#define SMBIOS_TYPE_MEMORY_MODULE_INFORMATION ((BYTE)6)

#define SMBIOS_MEMORY_MODULE_SIZE_NOT_DETERMINABLE  ((BYTE)0x7D) // Not determinable (Installed Size only)
#define SMBIOS_MEMORY_MODULE_SIZE_NOT_ENABLED       ((BYTE)0x7E) // Module is installed, but no memory has been enabled
#define SMBIOS_MEMORY_MODULE_SIZE_NOT_INSTALLED     ((BYTE)0x7F) // Not installed

typedef union _SMBIOS_MEMORY_MODULE_SIZE
{
    BYTE Value;
    struct
    {
        BYTE Size : 7;          // 0:6 Indicates size (n), where 2**n is the size in MiB, or SMBIOS_MEMORY_MODULE_SIZE_*
        BYTE DoubleBank : 1;    // 7 Single- (0) or double-bank (1) connection
    };
} SMBIOS_MEMORY_MODULE_SIZE, *PSMBIOS_MEMORY_MODULE_SIZE;

typedef struct _SMBIOS_MEMORY_MODULE_INFORMATION
{
    SMBIOS_HEADER Header;
    BYTE SocketDesignation;
    BYTE BankConnections;
    BYTE CurrentSpeed;
    SMBIOS_MEMORY_TYPE CurrentMemoryType;
    SMBIOS_MEMORY_MODULE_SIZE InstalledSize;
    SMBIOS_MEMORY_MODULE_SIZE EnabledSize;
    union
    {
        BYTE Value;
        struct
        {
            BYTE UncorrectableErrors : 1;   // 0 Uncorrectable errors received for the module
            BYTE CorrectableErrors : 1;     // 1 Correctable errors received for the module
            BYTE UseEventLog : 1;           // 2 Error Status information should be obtained from the event log
            BYTE Reserved : 5;              // 3:7 Reserved, set to 0
        };
    } ErrorStatus;
} SMBIOS_MEMORY_MODULE_INFORMATION, *PSMBIOS_MEMORY_MODULE_INFORMATION, SMBIOS_TYPE_6, *PSMBIOS_TYPE_6;

#pragma endregion Obsolete

#pragma region Cache Information (Type 7)

#define SMBIOS_TYPE_CACHE_INFORMATION ((BYTE)7)

#define SMBIOS_CACHE_LOCATION_INTERNAL    ((WORD)0b00)
#define SMBIOS_CACHE_LOCATION_EXTERNAL    ((WORD)0b01)
#define SMBIOS_CACHE_LOCATION_RESERVED    ((WORD)0b10)
#define SMBIOS_CACHE_LOCATION_UNKNOWN     ((WORD)0b11)

#define SMBIOS_CACHE_OPERATIONAL_MODE_WRITE_THROUGH                 ((WORD)0b00)
#define SMBIOS_CACHE_OPERATIONAL_MODE_WRITE_BACK                    ((WORD)0b01)
#define SMBIOS_CACHE_OPERATIONAL_MODE_VARIES_WITH_MEMORY_ADDRESS    ((WORD)0b10)
#define SMBIOS_CACHE_OPERATIONAL_MODE_UNKNOWN                       ((WORD)0b11)

typedef union _SMBIOS_CACHE_SIZE
{
    WORD Value;
    struct
    {
        WORD Size : 15;             // 00:14 Max size in given granularity
        WORD _64KGranularity : 1;   // 15 0 – 1K granularity, 1 – 64K granularity
    };
} SMBIOS_CACHE_SIZE, *PSMBIOS_CACHE_SIZE;

typedef union _SMBIOS_CACHE_SIZE2
{
    DWORD Value;
    struct
    {
        DWORD Size : 31;            // 00:30 Max size in given granularity
        DWORD _64KGranularity : 1;  // 31 0 – 1K granularity, 1 – 64K granularity
    };
} SMBIOS_CACHE_SIZE2, *PSMBIOS_CACHE_SIZE2;

typedef struct _SMBIOS_CACHE_SRAM_TYPE
{
    WORD Other : 1;         // 00 Other
    WORD Unknown : 1;       // 01 Unknown
    WORD NonBurst : 1;      // 02 Non-Burst
    WORD Burst : 1;         // 03 Burst
    WORD PipelineBurst : 1; // 04 Pipeline Burst
    WORD Synchronous : 1;   // 05 Synchronous
    WORD Asynchronous : 1;  // 06 Asynchronous
    WORD Reserved : 9;      // 07:15 Reserved, must be zero
} SMBIOS_CACHE_SRAM_TYPE, *PSMBIOS_CACHE_SRAM_TYPE;

#define SMBIOS_CACHE_ERROR_CORRECTION_TYPE_OTHER            ((BYTE)0x01) // Other
#define SMBIOS_CACHE_ERROR_CORRECTION_TYPE_UNKNOWN          ((BYTE)0x02) // Unknown
#define SMBIOS_CACHE_ERROR_CORRECTION_TYPE_NONE             ((BYTE)0x03) // None
#define SMBIOS_CACHE_ERROR_CORRECTION_TYPE_PARITY           ((BYTE)0x04) // Parity
#define SMBIOS_CACHE_ERROR_CORRECTION_TYPE_SINGLE_BIT_ECC   ((BYTE)0x05) // Single-bit ECC
#define SMBIOS_CACHE_ERROR_CORRECTION_TYPE_MULTI_BIT_ECC    ((BYTE)0x06) // Multi-bit ECC 

#define SMBIOS_CACHE_SYSTEM_CACHE_TYPE_OTHER        ((BYTE)0x01) // Other
#define SMBIOS_CACHE_SYSTEM_CACHE_TYPE_UNKNOWN      ((BYTE)0x02) // Unknown
#define SMBIOS_CACHE_SYSTEM_CACHE_TYPE_INSTRUCTION  ((BYTE)0x03) // Instruction
#define SMBIOS_CACHE_SYSTEM_CACHE_TYPE_DATA         ((BYTE)0x04) // Data
#define SMBIOS_CACHE_SYSTEM_CACHE_TYPE_UNIFIED      ((BYTE)0x05) // Unified

#define SMBIOS_CACHE_ASSOCIATIVITY_OTHER            ((BYTE)0x01) // Other
#define SMBIOS_CACHE_ASSOCIATIVITY_UNKNOWN          ((BYTE)0x02) // Unknown
#define SMBIOS_CACHE_ASSOCIATIVITY_DIRECT_MAPPED    ((BYTE)0x03) // Direct Mapped
#define SMBIOS_CACHE_ASSOCIATIVITY_2_WAY            ((BYTE)0x04) // 2-way Set-Associative
#define SMBIOS_CACHE_ASSOCIATIVITY_4_WAY            ((BYTE)0x05) // 4-way Set-Associative
#define SMBIOS_CACHE_ASSOCIATIVITY_FULL             ((BYTE)0x06) // Fully Associative
#define SMBIOS_CACHE_ASSOCIATIVITY_8_WAY            ((BYTE)0x07) // 8-way Set-Associative
#define SMBIOS_CACHE_ASSOCIATIVITY_16_WAY           ((BYTE)0x08) // 16-way Set-Associative
#define SMBIOS_CACHE_ASSOCIATIVITY_12_WAY           ((BYTE)0x09) // 12-way Set-Associative
#define SMBIOS_CACHE_ASSOCIATIVITY_24_WAY           ((BYTE)0x0A) // 24-way Set-Associative
#define SMBIOS_CACHE_ASSOCIATIVITY_32_WAY           ((BYTE)0x0B) // 32-way Set-Associative
#define SMBIOS_CACHE_ASSOCIATIVITY_48_WAY           ((BYTE)0x0C) // 48-way Set-Associative
#define SMBIOS_CACHE_ASSOCIATIVITY_64_WAY           ((BYTE)0x0D) // 64-way Set-Associative
#define SMBIOS_CACHE_ASSOCIATIVITY_20_WAY           ((BYTE)0x0E) // 20-way Set-Associative

typedef struct _SMBIOS_CACHE_INFORMATION
{
    SMBIOS_HEADER Header;
    BYTE SocketDesignation;
    union
    {
        WORD Value;
        struct
        {
            WORD Level : 3;             // 00:02 1 through 8
            WORD Socketed : 1;          // 03 1b – Socketed, 0b – Not Socketed
            WORD Reserved0 : 1;         // 04 Reserved, must be zero
            WORD Location : 2;          // 05:06 SMBIOS_CACHE_LOCATION_*
            WORD Enabled : 1;           // 07 1b – Enabled, 0b – Disabled (at boot time)
            WORD OperationalMode : 2;   // 08:09 SMBIOS_CACHE_OPERATIONAL_MODE_*
            WORD Reserved1 : 6;         // 10:15 Reserved, must be zero
        };
    } Configuration;
    SMBIOS_CACHE_SIZE MaximumCacheSize;
    SMBIOS_CACHE_SIZE InstalledSize;
    SMBIOS_CACHE_SRAM_TYPE SupportedSRAMType;
    SMBIOS_CACHE_SRAM_TYPE CurrentSRAMType;
#if SMBIOS_VERSION >= 0x02010000
    BYTE CacheSpeed;            // In nanoseconds, the value is 0 if the speed is unknown
    BYTE ErrorCorrectionType;   // SMBIOS_CACHE_ERROR_CORRECTION_TYPE_*
    BYTE SystemCacheType;       // SMBIOS_CACHE_SYSTEM_CACHE_TYPE_*
    BYTE Associativity;         // SMBIOS_CACHE_ASSOCIATIVITY_*
#if SMBIOS_VERSION >= 0x03010000
    SMBIOS_CACHE_SIZE2 MaximumCacheSize2;
    SMBIOS_CACHE_SIZE2 InstalledCacheSize2;
#endif // SMBIOS_VERSION >= 0x03010000
#endif // SMBIOS_VERSION >= 0x02010000
} SMBIOS_CACHE_INFORMATION, *PSMBIOS_CACHE_INFORMATION, SMBIOS_TYPE_7, *PSMBIOS_TYPE_7;

#pragma endregion

#pragma region Port Connector Information (Type 8)

#define SMBIOS_TYPE_PORT_CONNECTOR_INFORMATION ((BYTE)8)

#define SMBIOS_PORT_CONNECTOR_TYPE_NONE                             ((BYTE)0x00) // None
#define SMBIOS_PORT_CONNECTOR_TYPE_CENTRONICS                       ((BYTE)0x01) // Centronics
#define SMBIOS_PORT_CONNECTOR_TYPE_MINI_CENTRONICS                  ((BYTE)0x02) // Mini Centronics
#define SMBIOS_PORT_CONNECTOR_TYPE_MINI_PROPRIETARY                 ((BYTE)0x03) // Proprietary
#define SMBIOS_PORT_CONNECTOR_TYPE_DB_25_PIN_MALE                   ((BYTE)0x04) // DB-25 pin male
#define SMBIOS_PORT_CONNECTOR_TYPE_DB_25_PIN_FEMALE                 ((BYTE)0x05) // DB-25 pin female
#define SMBIOS_PORT_CONNECTOR_TYPE_DB_15_PIN_MALE                   ((BYTE)0x06) // DB-15 pin male
#define SMBIOS_PORT_CONNECTOR_TYPE_DB_15_PIN_FEMALE                 ((BYTE)0x07) // DB-15 pin female
#define SMBIOS_PORT_CONNECTOR_TYPE_DB_9_PIN_MALE                    ((BYTE)0x08) // DB-9 pin male
#define SMBIOS_PORT_CONNECTOR_TYPE_DB_9_PIN_FEMALE                  ((BYTE)0x09) // DB-9 pin female
#define SMBIOS_PORT_CONNECTOR_TYPE_RJ_11                            ((BYTE)0x0A) // RJ-11
#define SMBIOS_PORT_CONNECTOR_TYPE_RJ_45                            ((BYTE)0x0B) // RJ-45
#define SMBIOS_PORT_CONNECTOR_TYPE_50_PIN_MINISCSI                  ((BYTE)0x0C) // 50-pin MiniSCSI
#define SMBIOS_PORT_CONNECTOR_TYPE_MINI_DIN                         ((BYTE)0x0D) // Mini-DIN
#define SMBIOS_PORT_CONNECTOR_TYPE_MICRO_DIN                        ((BYTE)0x0E) // Micro-DIN
#define SMBIOS_PORT_CONNECTOR_TYPE_PS_2                             ((BYTE)0x0F) // PS/2
#define SMBIOS_PORT_CONNECTOR_TYPE_INFRARED                         ((BYTE)0x10) // Infrared
#define SMBIOS_PORT_CONNECTOR_TYPE_HP_HIL                           ((BYTE)0x11) // HP-HIL
#define SMBIOS_PORT_CONNECTOR_TYPE_ACCESS_BUS                       ((BYTE)0x12) // Access Bus (USB)
#define SMBIOS_PORT_CONNECTOR_TYPE_SSA_SCSI                         ((BYTE)0x13) // SSA SCSI
#define SMBIOS_PORT_CONNECTOR_TYPE_CIRCULAR_DIN_8_MALE              ((BYTE)0x14) // Circular DIN-8 male
#define SMBIOS_PORT_CONNECTOR_TYPE_CIRCULAR_DIN_8_FEMALE            ((BYTE)0x15) // Circular DIN-8 female
#define SMBIOS_PORT_CONNECTOR_TYPE_ON_BOARD_IDE                     ((BYTE)0x16) // On Board IDE
#define SMBIOS_PORT_CONNECTOR_TYPE_ON_BOARD_FLOPPY                  ((BYTE)0x17) // On Board Floppy
#define SMBIOS_PORT_CONNECTOR_TYPE_9_PIN_DUAL_INLINE                ((BYTE)0x18) // 9-pin Dual Inline (pin 10 cut)
#define SMBIOS_PORT_CONNECTOR_TYPE_25_PIN_DUAL_INLINE               ((BYTE)0x19) // 25-pin Dual Inline (pin 26 cut)
#define SMBIOS_PORT_CONNECTOR_TYPE_50_PIN_DUAL_INLINE               ((BYTE)0x1A) // 50-pin Dual Inline
#define SMBIOS_PORT_CONNECTOR_TYPE_68_PIN_DUAL_INLINE               ((BYTE)0x1B) // 68-pin Dual Inline
#define SMBIOS_PORT_CONNECTOR_TYPE_ON_BOARD_SOUND_INPUT_FROM_CD_ROM ((BYTE)0x1C) // On Board Sound Input from CD-ROM
#define SMBIOS_PORT_CONNECTOR_TYPE_MINI_CENTRONICS_TYPE_14          ((BYTE)0x1D) // Mini-Centronics Type-14
#define SMBIOS_PORT_CONNECTOR_TYPE_MINI_CENTRONICS_TYPE_26          ((BYTE)0x1E) // Mini-Centronics Type-26
#define SMBIOS_PORT_CONNECTOR_TYPE_MINI_JACK                        ((BYTE)0x1F) // Mini-jack (headphones)
#define SMBIOS_PORT_CONNECTOR_TYPE_BNC                              ((BYTE)0x20) // BNC
#define SMBIOS_PORT_CONNECTOR_TYPE_1394                             ((BYTE)0x21) // 1394
#define SMBIOS_PORT_CONNECTOR_TYPE_SAS_SATA_PLUG_RECEPTACLE         ((BYTE)0x22) // SAS/SATA Plug Receptacle
#define SMBIOS_PORT_CONNECTOR_TYPE_USB_TYPE_C_RECEPTACLE            ((BYTE)0x23) // USB Type-C Receptacle
#define SMBIOS_PORT_CONNECTOR_TYPE_PC_98                            ((BYTE)0xA0) // PC-98
#define SMBIOS_PORT_CONNECTOR_TYPE_PC_98HIRESO                      ((BYTE)0xA1) // PC-98Hireso
#define SMBIOS_PORT_CONNECTOR_TYPE_PC_H98                           ((BYTE)0xA2) // PC-H98
#define SMBIOS_PORT_CONNECTOR_TYPE_PC_98NOTE                        ((BYTE)0xA3) // PC-98Note
#define SMBIOS_PORT_CONNECTOR_TYPE_PC_98FULL                        ((BYTE)0xA4) // PC-98Full
#define SMBIOS_PORT_CONNECTOR_TYPE_OTHER                            ((BYTE)0xFF) // Other

#define SMBIOS_PORT_CONNECTOR_PORT_TYPE_NONE                            ((BYTE)0x00) // None
#define SMBIOS_PORT_CONNECTOR_PORT_TYPE_PARALLEL_PORT_XT_AT_COMPATIBLE  ((BYTE)0x01) // Parallel Port XT/AT Compatible
#define SMBIOS_PORT_CONNECTOR_PORT_TYPE_PARALLEL_PORT_PS_2              ((BYTE)0x02) // Parallel Port PS/2
#define SMBIOS_PORT_CONNECTOR_PORT_TYPE_PARALLEL_PORT_ECP               ((BYTE)0x03) // Parallel Port ECP
#define SMBIOS_PORT_CONNECTOR_PORT_TYPE_PARALLEL_PORT_EPP               ((BYTE)0x04) // Parallel Port EPP
#define SMBIOS_PORT_CONNECTOR_PORT_TYPE_PARALLEL_PORT_ECP_EPP           ((BYTE)0x05) // Parallel Port ECP/EPP
#define SMBIOS_PORT_CONNECTOR_PORT_TYPE_SERIAL_PORT_XT_AT_COMPATIBLE    ((BYTE)0x06) // Serial Port XT/AT Compatible
#define SMBIOS_PORT_CONNECTOR_PORT_TYPE_SERIAL_PORT_16450_COMPATIBLE    ((BYTE)0x07) // Serial Port 16450 Compatible
#define SMBIOS_PORT_CONNECTOR_PORT_TYPE_SERIAL_PORT_16550_COMPATIBLE    ((BYTE)0x08) // Serial Port 16550 Compatible
#define SMBIOS_PORT_CONNECTOR_PORT_TYPE_SERIAL_PORT_16550A_COMPATIBLE   ((BYTE)0x09) // Serial Port 16550A Compatible
#define SMBIOS_PORT_CONNECTOR_PORT_TYPE_SCSI_PORT                       ((BYTE)0x0A) // SCSI Port
#define SMBIOS_PORT_CONNECTOR_PORT_TYPE_MIDI_PORT                       ((BYTE)0x0B) // MIDI Port
#define SMBIOS_PORT_CONNECTOR_PORT_TYPE_JOY_STICK_PORT                  ((BYTE)0x0C) // Joy Stick Port
#define SMBIOS_PORT_CONNECTOR_PORT_TYPE_KEYBOARD_PORT                   ((BYTE)0x0D) // Keyboard Port
#define SMBIOS_PORT_CONNECTOR_PORT_TYPE_MOUSE_PORT                      ((BYTE)0x0E) // Mouse Port
#define SMBIOS_PORT_CONNECTOR_PORT_TYPE_SSA_SCSI                        ((BYTE)0x0F) // SSA SCSI
#define SMBIOS_PORT_CONNECTOR_PORT_TYPE_USB                             ((BYTE)0x10) // USB
#define SMBIOS_PORT_CONNECTOR_PORT_TYPE_FIRE_WIRE                       ((BYTE)0x11) // FireWire (IEEE P1394)
#define SMBIOS_PORT_CONNECTOR_PORT_TYPE_PCMCIA_TYPE_I                   ((BYTE)0x12) // PCMCIA Type I2
#define SMBIOS_PORT_CONNECTOR_PORT_TYPE_PCMCIA_TYPE_II                  ((BYTE)0x13) // PCMCIA Type II
#define SMBIOS_PORT_CONNECTOR_PORT_TYPE_PCMCIA_TYPE_III                 ((BYTE)0x14) // PCMCIA Type III
#define SMBIOS_PORT_CONNECTOR_PORT_TYPE_CARD_BUS                        ((BYTE)0x15) // Card bus
#define SMBIOS_PORT_CONNECTOR_PORT_TYPE_ACCESS_BUS_PORT                 ((BYTE)0x16) // Access Bus Port
#define SMBIOS_PORT_CONNECTOR_PORT_TYPE_SCSI_2                          ((BYTE)0x17) // SCSI II
#define SMBIOS_PORT_CONNECTOR_PORT_TYPE_SCSI_WIDE                       ((BYTE)0x18) // SCSI Wide
#define SMBIOS_PORT_CONNECTOR_PORT_TYPE_PC_98                           ((BYTE)0x19) // PC-98
#define SMBIOS_PORT_CONNECTOR_PORT_TYPE_PC_98_Hireso                    ((BYTE)0x1A) // PC-98-Hireso
#define SMBIOS_PORT_CONNECTOR_PORT_TYPE_PC_H98                          ((BYTE)0x1B) // PC-H98
#define SMBIOS_PORT_CONNECTOR_PORT_TYPE_VIDEO_PORT                      ((BYTE)0x1C) // Video Port
#define SMBIOS_PORT_CONNECTOR_PORT_TYPE_AUDIO_PORT                      ((BYTE)0x1D) // Audio Port
#define SMBIOS_PORT_CONNECTOR_PORT_TYPE_MODEM_PORT                      ((BYTE)0x1E) // Modem Port
#define SMBIOS_PORT_CONNECTOR_PORT_TYPE_NETWORK_PORT                    ((BYTE)0x1F) // Network Port
#define SMBIOS_PORT_CONNECTOR_PORT_TYPE_SATA                            ((BYTE)0x20) // SATA
#define SMBIOS_PORT_CONNECTOR_PORT_TYPE_SAS                             ((BYTE)0x21) // SAS
#define SMBIOS_PORT_CONNECTOR_PORT_TYPE_MFDP                            ((BYTE)0x22) // MFDP (Multi-Function Display Port)
#define SMBIOS_PORT_CONNECTOR_PORT_TYPE_THUNDERBOLT                     ((BYTE)0x23) // Thunderbolt
#define SMBIOS_PORT_CONNECTOR_PORT_TYPE_8251_COMPATIBLE                 ((BYTE)0xA0) // 8251 Compatible
#define SMBIOS_PORT_CONNECTOR_PORT_TYPE_8251_FIFO_COMPATIBLE            ((BYTE)0xA1) // 8251 FIFO Compatible
#define SMBIOS_PORT_CONNECTOR_PORT_TYPE_OTHER                           ((BYTE)0xFF) // Other

typedef struct _SMBIOS_PORT_CONNECTOR_INFORMATION
{
    SMBIOS_HEADER Header;
    BYTE InternalReferenceDesignator;
    BYTE InternalConnectorType; // SMBIOS_PORT_CONNECTOR_TYPE_*
    BYTE ExternalReferenceDesignator;
    BYTE ExternalConnectorType; // SMBIOS_PORT_CONNECTOR_TYPE_*
    BYTE PortType;              // SMBIOS_PORT_CONNECTOR_PORT_TYPE_*
} SMBIOS_PORT_CONNECTOR_INFORMATION, *PSMBIOS_PORT_CONNECTOR_INFORMATION, SMBIOS_TYPE_8, *PSMBIOS_TYPE_8;

#pragma endregion

#pragma region System Slots (Type 9)

#define SMBIOS_TYPE_SYSTEM_SLOTS ((BYTE)9)

#define SMBIOS_SYSTEM_SLOTS_TYPE_OTHER                                                  ((BYTE)0x01) // Other
#define SMBIOS_SYSTEM_SLOTS_TYPE_UNKNOWN                                                ((BYTE)0x02) // Unknown
#define SMBIOS_SYSTEM_SLOTS_TYPE_ISA                                                    ((BYTE)0x03) // ISA
#define SMBIOS_SYSTEM_SLOTS_TYPE_MCA                                                    ((BYTE)0x04) // MCA
#define SMBIOS_SYSTEM_SLOTS_TYPE_EISA                                                   ((BYTE)0x05) // EISA
#define SMBIOS_SYSTEM_SLOTS_TYPE_PCI                                                    ((BYTE)0x06) // PCI
#define SMBIOS_SYSTEM_SLOTS_TYPE_PC_CARD                                                ((BYTE)0x07) // PC Card (PCMCIA)
#define SMBIOS_SYSTEM_SLOTS_TYPE_VL_VESA                                                ((BYTE)0x08) // VL-VESA
#define SMBIOS_SYSTEM_SLOTS_TYPE_PROPRIETARY                                            ((BYTE)0x09) // Proprietary
#define SMBIOS_SYSTEM_SLOTS_TYPE_PROCESSOR_CARD_SLOT                                    ((BYTE)0x0A) // Processor Card Slot
#define SMBIOS_SYSTEM_SLOTS_TYPE_PROPRIETARY_MEMORY_CARD_SLOT                           ((BYTE)0x0B) // Proprietary Memory Card Slot
#define SMBIOS_SYSTEM_SLOTS_TYPE_IO_RISER_CARD_SLOT                                     ((BYTE)0x0C) // I/O Riser Card Slot
#define SMBIOS_SYSTEM_SLOTS_TYPE_NUBUS                                                  ((BYTE)0x0D) // NuBus
#define SMBIOS_SYSTEM_SLOTS_TYPE_PCI_66MHZ_CAPABLE                                      ((BYTE)0x0E) // PCI – 66MHz Capable
#define SMBIOS_SYSTEM_SLOTS_TYPE_AGP                                                    ((BYTE)0x0F) // AGP
#define SMBIOS_SYSTEM_SLOTS_TYPE_AGP_2X                                                 ((BYTE)0x10) // AGP 2X
#define SMBIOS_SYSTEM_SLOTS_TYPE_AGP_4X                                                 ((BYTE)0x11) // AGP 4X
#define SMBIOS_SYSTEM_SLOTS_TYPE_PCI_X                                                  ((BYTE)0x12) // PCI-X
#define SMBIOS_SYSTEM_SLOTS_TYPE_AGP_8X                                                 ((BYTE)0x13) // AGP 8X
#define SMBIOS_SYSTEM_SLOTS_TYPE_M2_SOCKET_1_DP                                         ((BYTE)0x14) // M.2 Socket 1-DP (Mechanical Key A)
#define SMBIOS_SYSTEM_SLOTS_TYPE_M2_SOCKET_1_SD                                         ((BYTE)0x15) // M.2 Socket 1-SD (Mechanical Key E)
#define SMBIOS_SYSTEM_SLOTS_TYPE_M2_SOCKET_2                                            ((BYTE)0x16) // M.2 Socket 2 (Mechanical Key B)
#define SMBIOS_SYSTEM_SLOTS_TYPE_M2_SOCKET_3                                            ((BYTE)0x17) // M.2 Socket 3 (Mechanical Key M)
#define SMBIOS_SYSTEM_SLOTS_TYPE_M2_SOCKET_MXM_TYPE_I                                   ((BYTE)0x18) // MXM Type I
#define SMBIOS_SYSTEM_SLOTS_TYPE_M2_SOCKET_MXM_TYPE_II                                  ((BYTE)0x19) // MXM Type II
#define SMBIOS_SYSTEM_SLOTS_TYPE_M2_SOCKET_MXM_TYPE_III_STANDARD                        ((BYTE)0x1A) // MXM Type III (standard connector)
#define SMBIOS_SYSTEM_SLOTS_TYPE_M2_SOCKET_MXM_TYPE_III_HE                              ((BYTE)0x1B) // MXM Type III (HE connector)
#define SMBIOS_SYSTEM_SLOTS_TYPE_M2_SOCKET_MXM_TYPE_IV                                  ((BYTE)0x1C) // MXM Type IV
#define SMBIOS_SYSTEM_SLOTS_TYPE_M2_SOCKET_MXM_3_TYPE_A                                 ((BYTE)0x1D) // MXM 3.0 Type A
#define SMBIOS_SYSTEM_SLOTS_TYPE_M2_SOCKET_MXM_3_TYPE_B                                 ((BYTE)0x1E) // MXM 3.0 Type B
#define SMBIOS_SYSTEM_SLOTS_TYPE_PCI_EXPRESS_GEN_2_SFF_8639                             ((BYTE)0x1F) // PCI Express Gen 2 SFF-8639 (U.2)
#define SMBIOS_SYSTEM_SLOTS_TYPE_PCI_EXPRESS_GEN_3_SFF_8639                             ((BYTE)0x20) // PCI Express Gen 3 SFF-8639 (U.2)
#define SMBIOS_SYSTEM_SLOTS_TYPE_PCI_EXPRESS_MINI_52_PIN_WITH_BOTTOM_SIDE_KEEP_OUTS     ((BYTE)0x21) // PCI Express Mini 52-pin (CEM spec. 2.0) with bottom-side keep-outs
#define SMBIOS_SYSTEM_SLOTS_TYPE_PCI_EXPRESS_MINI_52_PIN_WITHOUT_BOTTOM_SIDE_KEEP_OUTS  ((BYTE)0x22) // PCI Express Mini 52-pin (CEM spec. 2.0) without bottom-side keep-outs
#define SMBIOS_SYSTEM_SLOTS_TYPE_PCI_EXPRESS_MINI_76_PIN                                ((BYTE)0x23) // PCI Express Mini 76-pin (CEM spec. 2.0)
#define SMBIOS_SYSTEM_SLOTS_TYPE_PCI_EXPRESS_GEN_4_SFF_8639                             ((BYTE)0x24) // PCI Express Gen 4 SFF-8639 (U.2)
#define SMBIOS_SYSTEM_SLOTS_TYPE_PCI_EXPRESS_GEN_5_SFF_8639                             ((BYTE)0x25) // PCI Express Gen 5 SFF-8639 (U.2)
#define SMBIOS_SYSTEM_SLOTS_TYPE_OCP_NIC_3_SFF                                          ((BYTE)0x26) // OCP NIC 3.0 Small Form Factor (SFF)
#define SMBIOS_SYSTEM_SLOTS_TYPE_OCP_NIC_3_LFF                                          ((BYTE)0x27) // OCP NIC 3.0 Large Form Factor (LFF)
#define SMBIOS_SYSTEM_SLOTS_TYPE_OCP_NIC_PRIOR_TO_3                                     ((BYTE)0x28) // OCP NIC Prior to 3.0
#define SMBIOS_SYSTEM_SLOTS_TYPE_CXL_FLEXBUS_1                                          ((BYTE)0x30) // CXL Flexbus 1.0 (deprecated)
#define SMBIOS_SYSTEM_SLOTS_TYPE_PC_98_C20                                              ((BYTE)0xA0) // PC-98/C20
#define SMBIOS_SYSTEM_SLOTS_TYPE_PC_98_C24                                              ((BYTE)0xA1) // PC-98/C24
#define SMBIOS_SYSTEM_SLOTS_TYPE_PC_98_E                                                ((BYTE)0xA2) // PC-98/E
#define SMBIOS_SYSTEM_SLOTS_TYPE_PC_98_LOCAL_BUS                                        ((BYTE)0xA3) // PC-98/Local Bus
#define SMBIOS_SYSTEM_SLOTS_TYPE_PC_98_CARD                                             ((BYTE)0xA4) // PC-98/Card
#define SMBIOS_SYSTEM_SLOTS_TYPE_PCI_EXPRESS                                            ((BYTE)0xA5) // PCI Express
#define SMBIOS_SYSTEM_SLOTS_TYPE_PCI_EXPRESS_X1                                         ((BYTE)0xA6) // PCI Express x1
#define SMBIOS_SYSTEM_SLOTS_TYPE_PCI_EXPRESS_X2                                         ((BYTE)0xA7) // PCI Express x2
#define SMBIOS_SYSTEM_SLOTS_TYPE_PCI_EXPRESS_X4                                         ((BYTE)0xA8) // PCI Express x4
#define SMBIOS_SYSTEM_SLOTS_TYPE_PCI_EXPRESS_X8                                         ((BYTE)0xA9) // PCI Express x8
#define SMBIOS_SYSTEM_SLOTS_TYPE_PCI_EXPRESS_X16                                        ((BYTE)0xAA) // PCI Express x16
#define SMBIOS_SYSTEM_SLOTS_TYPE_PCI_EXPRESS_GEN_2                                      ((BYTE)0xAB) // PCI Express Gen 2
#define SMBIOS_SYSTEM_SLOTS_TYPE_PCI_EXPRESS_GEN_2_X1                                   ((BYTE)0xAC) // PCI Express Gen 2 x1
#define SMBIOS_SYSTEM_SLOTS_TYPE_PCI_EXPRESS_GEN_2_X2                                   ((BYTE)0xAD) // PCI Express Gen 2 x2
#define SMBIOS_SYSTEM_SLOTS_TYPE_PCI_EXPRESS_GEN_2_X4                                   ((BYTE)0xAE) // PCI Express Gen 2 x4
#define SMBIOS_SYSTEM_SLOTS_TYPE_PCI_EXPRESS_GEN_2_X8                                   ((BYTE)0xAF) // PCI Express Gen 2 x8
#define SMBIOS_SYSTEM_SLOTS_TYPE_PCI_EXPRESS_GEN_2_X16                                  ((BYTE)0xB0) // PCI Express Gen 2 x16
#define SMBIOS_SYSTEM_SLOTS_TYPE_PCI_EXPRESS_GEN_3                                      ((BYTE)0xB1) // PCI Express Gen 3
#define SMBIOS_SYSTEM_SLOTS_TYPE_PCI_EXPRESS_GEN_3_X1                                   ((BYTE)0xB2) // PCI Express Gen 3 x1
#define SMBIOS_SYSTEM_SLOTS_TYPE_PCI_EXPRESS_GEN_3_X2                                   ((BYTE)0xB3) // PCI Express Gen 3 x2
#define SMBIOS_SYSTEM_SLOTS_TYPE_PCI_EXPRESS_GEN_3_X4                                   ((BYTE)0xB4) // PCI Express Gen 3 x4
#define SMBIOS_SYSTEM_SLOTS_TYPE_PCI_EXPRESS_GEN_3_X8                                   ((BYTE)0xB5) // PCI Express Gen 3 x8
#define SMBIOS_SYSTEM_SLOTS_TYPE_PCI_EXPRESS_GEN_3_X16                                  ((BYTE)0xB6) // PCI Express Gen 3 x16
#define SMBIOS_SYSTEM_SLOTS_TYPE_PCI_EXPRESS_GEN_4                                      ((BYTE)0xB8) // PCI Express Gen 4
#define SMBIOS_SYSTEM_SLOTS_TYPE_PCI_EXPRESS_GEN_4_X1                                   ((BYTE)0xB9) // PCI Express Gen 4 x1
#define SMBIOS_SYSTEM_SLOTS_TYPE_PCI_EXPRESS_GEN_4_X2                                   ((BYTE)0xBA) // PCI Express Gen 4 x2
#define SMBIOS_SYSTEM_SLOTS_TYPE_PCI_EXPRESS_GEN_4_X4                                   ((BYTE)0xBB) // PCI Express Gen 4 x4
#define SMBIOS_SYSTEM_SLOTS_TYPE_PCI_EXPRESS_GEN_4_X8                                   ((BYTE)0xBC) // PCI Express Gen 4 x8
#define SMBIOS_SYSTEM_SLOTS_TYPE_PCI_EXPRESS_GEN_4_X16                                  ((BYTE)0xBD) // PCI Express Gen 4 x16
#define SMBIOS_SYSTEM_SLOTS_TYPE_PCI_EXPRESS_GEN_5                                      ((BYTE)0xBE) // PCI Express Gen 5
#define SMBIOS_SYSTEM_SLOTS_TYPE_PCI_EXPRESS_GEN_5_X1                                   ((BYTE)0xBF) // PCI Express Gen 5 x1
#define SMBIOS_SYSTEM_SLOTS_TYPE_PCI_EXPRESS_GEN_5_X2                                   ((BYTE)0xC0) // PCI Express Gen 5 x2
#define SMBIOS_SYSTEM_SLOTS_TYPE_PCI_EXPRESS_GEN_5_X4                                   ((BYTE)0xC1) // PCI Express Gen 5 x4
#define SMBIOS_SYSTEM_SLOTS_TYPE_PCI_EXPRESS_GEN_5_X8                                   ((BYTE)0xC2) // PCI Express Gen 5 x8
#define SMBIOS_SYSTEM_SLOTS_TYPE_PCI_EXPRESS_GEN_5_X16                                  ((BYTE)0xC3) // PCI Express Gen 5 x16
#define SMBIOS_SYSTEM_SLOTS_TYPE_PCI_EXPRESS_GEN_6_AND_BEYOND                           ((BYTE)0xC4) // PCI Express Gen 6 and Beyond
#define SMBIOS_SYSTEM_SLOTS_TYPE_EDSFF_E1S_E1L                                          ((BYTE)0xC5) // Enterprise and Datacenter 1U E1 Form Factor Slot
#define SMBIOS_SYSTEM_SLOTS_TYPE_EDSFF_E3S_E3L                                          ((BYTE)0xC6) // Enterprise and Datacenter 3" E3 Form Factor Slot

#define SMBIOS_SYSTEM_SLOTS_DATA_BUS_WIDTH_OTHER    ((BYTE)0x01) // Other
#define SMBIOS_SYSTEM_SLOTS_DATA_BUS_WIDTH_UNKNOWN  ((BYTE)0x02) // Unknown
#define SMBIOS_SYSTEM_SLOTS_DATA_BUS_WIDTH_8_BIT    ((BYTE)0x03) // 8 bit
#define SMBIOS_SYSTEM_SLOTS_DATA_BUS_WIDTH_16_BIT   ((BYTE)0x04) // 16 bit
#define SMBIOS_SYSTEM_SLOTS_DATA_BUS_WIDTH_32_BIT   ((BYTE)0x05) // 32 bit
#define SMBIOS_SYSTEM_SLOTS_DATA_BUS_WIDTH_64_BIT   ((BYTE)0x06) // 64 bit
#define SMBIOS_SYSTEM_SLOTS_DATA_BUS_WIDTH_128_BIT  ((BYTE)0x07) // 128 bit
#define SMBIOS_SYSTEM_SLOTS_DATA_BUS_WIDTH_1X       ((BYTE)0x08) // 1x or x1
#define SMBIOS_SYSTEM_SLOTS_DATA_BUS_WIDTH_2X       ((BYTE)0x09) // 2x or x2
#define SMBIOS_SYSTEM_SLOTS_DATA_BUS_WIDTH_4X       ((BYTE)0x0A) // 4x or x4
#define SMBIOS_SYSTEM_SLOTS_DATA_BUS_WIDTH_8X       ((BYTE)0x0B) // 8x or x8
#define SMBIOS_SYSTEM_SLOTS_DATA_BUS_WIDTH_12X      ((BYTE)0x0C) // 12x or x12
#define SMBIOS_SYSTEM_SLOTS_DATA_BUS_WIDTH_16X      ((BYTE)0x0D) // 16x or x16
#define SMBIOS_SYSTEM_SLOTS_DATA_BUS_WIDTH_32X      ((BYTE)0x0E) // 32x or x32

#define SMBIOS_SYSTEM_SLOTS_CURRENT_USAGE_OTHER         ((BYTE)0x01) // Other
#define SMBIOS_SYSTEM_SLOTS_CURRENT_USAGE_UNKNOWN       ((BYTE)0x02) // Unknown
#define SMBIOS_SYSTEM_SLOTS_CURRENT_USAGE_AVAILABLE     ((BYTE)0x03) // Available
#define SMBIOS_SYSTEM_SLOTS_CURRENT_USAGE_IN_USE        ((BYTE)0x04) // In use
#define SMBIOS_SYSTEM_SLOTS_CURRENT_USAGE_UNAVAILABLE   ((BYTE)0x05) // Unavailable

#define SMBIOS_SYSTEM_SLOTS_LENGTH_OTHER                        ((BYTE)0x01) // Other
#define SMBIOS_SYSTEM_SLOTS_LENGTH_UNKNOWN                      ((BYTE)0x02) // Unknown
#define SMBIOS_SYSTEM_SLOTS_LENGTH_SHORT                        ((BYTE)0x03) // Short Length
#define SMBIOS_SYSTEM_SLOTS_LENGTH_LONG                         ((BYTE)0x04) // Long Length
#define SMBIOS_SYSTEM_SLOTS_LENGTH_2DOT5_INCH_DRIVE_FORM_FACTOR ((BYTE)0x05) // 2.5" drive form factor
#define SMBIOS_SYSTEM_SLOTS_LENGTH_3DOT5_INCH_DRIVE_FORM_FACTOR ((BYTE)0x06) // 3.5" drive form factor

typedef struct _SMBIOS_SYSTEM_SLOTS
{
    SMBIOS_HEADER Header;
    BYTE Designation;

    BYTE Type;          // SMBIOS_SYSTEM_SLOTS_TYPE_*
    BYTE DataBusWidth;  // SMBIOS_SYSTEM_SLOTS_DATA_BUS_WIDTH_*
    BYTE CurrentUsage;  // SMBIOS_SYSTEM_SLOTS_CURRENT_USAGE_*
    BYTE Length;        // SMBIOS_SYSTEM_SLOTS_LENGTH_*
    WORD ID;
    union
    {
        BYTE Value;
        struct
        {
            BYTE Unknown : 1;                       // Characteristics unknown
            BYTE Provides5Dot0Volts : 1;            // Provides 5.0 volts
            BYTE Provides3Dot3Volts : 1;            // Provides 3.3 volts
            BYTE OpeningSharedWithAnother : 1;      // Slot’s opening is shared with another slot
            BYTE PCCardSupportsPCCard16 : 1;        // PC Card slot supports PC Card-16
            BYTE PCCardSupportsCardBus : 1;         // PC Card slot supports CardBus
            BYTE PCCardSupportsZoomVideo : 1;       // PC Card slot supports Zoom Video
            BYTE PCCardSupportsModemRingResume : 1; // PC Card slot supports Modem Ring Resume
        };
    } Characteristics1;
#if SMBIOS_VERSION >= 0x02010000
    union
    {
        BYTE Value;
        struct
        {
            BYTE PCISupportsPMESignal : 1;              // PCI slot supports Power Management Event (PME#) signal
            BYTE SupportsHotPlugDevices : 1;            // Slot supports hot-plug devices
            BYTE PCISupportsSMBusSignal : 1;            // PCI slot supports SMBus signal
            BYTE PCIeSupportsBifurcation : 1;           // PCIe slot supports bifurcation
            BYTE SupportsAsyncOrSurpriseRemoval : 1;    // Slot supports async/surprise removal
            BYTE FlexbusCXL1Capable : 1;                // Flexbus slot, CXL 1.0 capable
            BYTE FlexbusCXL2Capable : 1;                // Flexbus slot, CXL 2.0 capable
            BYTE FlexbusCXL3Capable : 1;                // Flexbus slot, CXL 3.0 capable
        };
    } Characteristics2;
#if SMBIOS_VERSION >= 0x02060000
    WORD SegmentGroupNumber;
    BYTE BusNumber;
    union
    {
        BYTE Value;
        struct
        {
            BYTE FunctionNumber : 3;
            BYTE DeviceNumber : 5;
        };
    } DeviceFunctionNumber;
#if SMBIOS_VERSION >= 0x03020000
    BYTE BaseDataBusWidth;
    BYTE PeerGroupingCount;
    _Field_size_bytes_(5 * PeerGroupingCount) BYTE PeerGroups[];
/*
#if SMBIOS_VERSION >= 0x03040000
    BYTE Information;
    BYTE PhysicalWidth;
    WORD Pitch;
#if SMBIOS_VERSION >= 0x03050000
    BYTE Height;
#endif // SMBIOS_VERSION >= 0x03050000
#endif // SMBIOS_VERSION >= 0x03040000
*/
#endif // SMBIOS_VERSION >= 0x03020000
#endif // SMBIOS_VERSION >= 0x02060000
#endif // SMBIOS_VERSION >= 0x02010000
} SMBIOS_SYSTEM_SLOTS, *PSMBIOS_SYSTEM_SLOTS, SMBIOS_TYPE_9, *PSMBIOS_TYPE_9;

#pragma endregion

#pragma region On Board Devices Information (Type 10, Obsolete)

#define SMBIOS_TYPE_ONBOARD_DEVICES_INFORMATION ((BYTE)10)

#define SMBIOS_ONBOARD_DEVICES_TYPE_OTHER           ((BYTE)0x01)
#define SMBIOS_ONBOARD_DEVICES_TYPE_UNKNOWN         ((BYTE)0x02)
#define SMBIOS_ONBOARD_DEVICES_TYPE_VIDEO           ((BYTE)0x03)
#define SMBIOS_ONBOARD_DEVICES_TYPE_SCSI_CONTROLLER ((BYTE)0x04)
#define SMBIOS_ONBOARD_DEVICES_TYPE_ETHERNET        ((BYTE)0x05)
#define SMBIOS_ONBOARD_DEVICES_TYPE_TOKEN_RING      ((BYTE)0x06)
#define SMBIOS_ONBOARD_DEVICES_TYPE_SOUND           ((BYTE)0x07)
#define SMBIOS_ONBOARD_DEVICES_TYPE_PATA_CONTROLLER ((BYTE)0x08)
#define SMBIOS_ONBOARD_DEVICES_TYPE_SATA_CONTROLLER ((BYTE)0x09)
#define SMBIOS_ONBOARD_DEVICES_TYPE_SAS_CONTROLLER  ((BYTE)0x0A)

typedef struct _SMBIOS_ONBOARD_DEVICES_ENTRY
{
    union
    {
        BYTE Value;
        struct
        {
            BYTE DeviceType : 7; // SMBIOS_ONBOARD_DEVICES_TYPE_*
            BYTE Enabled : 1;
        };
    } Type;
    BYTE Description;
} SMBIOS_ONBOARD_DEVICES_ENTRY, *PSMBIOS_ONBOARD_DEVICES_ENTRY;

typedef struct _SMBIOS_ONBOARD_DEVICES_INFORMATION
{
    SMBIOS_HEADER Header;
    SMBIOS_ONBOARD_DEVICES_ENTRY Devices[];
} SMBIOS_ONBOARD_DEVICES_INFORMATION, *PSMBIOS_ONBOARD_DEVICES_INFORMATION, SMBIOS_TYPE_10, *PSMBIOS_TYPE_10;

#pragma endregion Obsolete

#pragma region OEM Strings (Type 11)

#define SMBIOS_TYPE_OEM_STRINGS ((BYTE)11)

typedef struct _SMBIOS_OEM_STRINGS
{
    SMBIOS_HEADER Header;
    BYTE Count;
} SMBIOS_OEM_STRINGS, *PSMBIOS_OEM_STRINGS, SMBIOS_TYPE_11, *PSMBIOS_TYPE_11;

#pragma endregion

#pragma region System Configuration Options (Type 12)

#define SMBIOS_TYPE_SYSTEM_CONFIGURATION_OPTIONS ((BYTE)12)

typedef struct _SMBIOS_SYSTEM_CONFIGURATION_OPTIONS
{
    SMBIOS_HEADER Header;
    BYTE Count;
} SMBIOS_SYSTEM_CONFIGURATION_OPTIONS, *PSMBIOS_SYSTEM_CONFIGURATION_OPTIONS, SMBIOS_TYPE_12, *PSMBIOS_TYPE_12;

#pragma endregion

#pragma region Firmware Language Information (Type 13)

#define SMBIOS_TYPE_FIRMWARE_LANGUAGE_INFORMATION ((BYTE)13)

typedef struct _SMBIOS_FIRMWARE_LANGUAGE_INFORMATION
{
    SMBIOS_HEADER Header;
    BYTE InstallableLanguages;
#if SMBIOS_VERSION >= 0x02010000
    union
    {
        BYTE Value;
        struct
        {
            BYTE AbbreviatedFormat : 1;
            BYTE Reserved : 7;
        };
    } Flags;
#else
    BYTE Reserved0;
#endif // SMBIOS_VERSION >= 0x02010000
    BYTE Reserved1[15];
    BYTE CurrentLanguage;
} SMBIOS_FIRMWARE_LANGUAGE_INFORMATION, *PSMBIOS_FIRMWARE_LANGUAGE_INFORMATION, SMBIOS_TYPE_13, *PSMBIOS_TYPE_13;

#pragma endregion

#pragma region Group Associations (Type 14)

#define SMBIOS_TYPE_GROUP_ASSOCIATIONS ((BYTE)14)

typedef struct _SMBIOS_GROUP_ASSOCIATIONS_ENTRY
{
    BYTE Type;
    WORD Handle;
} SMBIOS_GROUP_ASSOCIATIONS_ENTRY, *PSMBIOS_GROUP_ASSOCIATIONS_ENTRY;

typedef struct _SMBIOS_GROUP_ASSOCIATIONS
{
    SMBIOS_HEADER Header;
    BYTE GroupName;
    SMBIOS_GROUP_ASSOCIATIONS_ENTRY Items[];
} SMBIOS_GROUP_ASSOCIATIONS, *PSMBIOS_GROUP_ASSOCIATIONS, SMBIOS_TYPE_14, *PSMBIOS_TYPE_14;

#pragma endregion

#pragma region System Event Log (Type 15)

#define SMBIOS_TYPE_SYSTEM_EVENT_LOG ((BYTE)15)

#define SMBIOS_SYSTEM_EVENT_LOG_ACCESSMETHOD_1_8BIT_INDEX_1_8BIT_DATA   ((BYTE)0x00) // Indexed I/O: 1 8-bit index port, 1 8-bit data port
#define SMBIOS_SYSTEM_EVENT_LOG_ACCESSMETHOD_2_8BIT_INDEX_1_8BIT_DATA   ((BYTE)0x01) // Indexed I/O: 2 8-bit index port, 1 8-bit data port
#define SMBIOS_SYSTEM_EVENT_LOG_ACCESSMETHOD_1_16BIT_INDEX_1_8BIT_DATA  ((BYTE)0x02) // Indexed I/O: 1 16-bit index port, 1 8-bit data port
#define SMBIOS_SYSTEM_EVENT_LOG_ACCESSMETHOD_MEMORY_MAPPED              ((BYTE)0x03) // Memory-mapped physical 32-bit address
#define SMBIOS_SYSTEM_EVENT_LOG_ACCESSMETHOD_NONVOLATILE_DATA           ((BYTE)0x04) // Available through General-Purpose NonVolatile Data functions

#define SMBIOS_SYSTEM_EVENT_LOG_HEADERFORMAT_NO_HEADER  ((BYTE)0x00) // No header
#define SMBIOS_SYSTEM_EVENT_LOG_HEADERFORMAT_TYPE_1     ((BYTE)0x01) // Type 1 log header

typedef struct _SMBIOS_SYSTEM_EVENT_LOG
{
    SMBIOS_HEADER Header;
    WORD AreaLength;
    WORD HeaderStartOffset;
    WORD DataStartOffset;
    BYTE AccessMethod; // SMBIOS_SYSTEM_EVENT_LOG_ACCESSMETHOD_*
    union
    {
        BYTE Value;
        struct
        {
            BYTE Valid : 1;     // 0 Log area valid
            BYTE Full : 1;      // 1 Log area full
            BYTE Reserved : 6;  // 2:7 Reserved, set to 0
        };
    } Status;
    DWORD ChangeToken;
    union
    {
        struct
        {
            WORD IndexAddr;
            WORD DataAddr;
        } IO;
        DWORD PhysicalAddr32;
        WORD GPNVHandle;
    } AccessMethodAddress;
#if SMBIOS_VERSION >= 0x02010000
    BYTE HeaderFormat; // SMBIOS_SYSTEM_EVENT_LOG_HEADERFORMAT_*
    BYTE NumberOfSupportedTypeDescriptors;
    BYTE LengthOfTypeDescriptor; // Currently hard-coded as 2
    _Field_size_bytes_(NumberOfSupportedTypeDescriptors * LengthOfTypeDescriptor) BYTE SupportedTypeDescriptors[];
#endif // SMBIOS_VERSION >= 0x02010000
} SMBIOS_SYSTEM_EVENT_LOG, *PSMBIOS_SYSTEM_EVENT_LOG, SMBIOS_TYPE_15, *PSMBIOS_TYPE_15;

// WIP

#pragma endregion WIP

#if SMBIOS_VERSION >= 0x02010000

#pragma region Physical Memory Array (Type 16)

#define SMBIOS_TYPE_PHYSICAL_MEMORY_ARRAY ((BYTE)16)

#define SMBIOS_PHYSICAL_MEMORY_ARRAY_LOCATION_OTHER                         ((BYTE)0x01) // Other
#define SMBIOS_PHYSICAL_MEMORY_ARRAY_LOCATION_UNKNOWN                       ((BYTE)0x02) // Unknown
#define SMBIOS_PHYSICAL_MEMORY_ARRAY_LOCATION_SYSTEM_BOARD_OR_MOTHERBOARD   ((BYTE)0x03) // System board or motherboard
#define SMBIOS_PHYSICAL_MEMORY_ARRAY_LOCATION_ISA_ADDON_CARD                ((BYTE)0x04) // ISA add-on card
#define SMBIOS_PHYSICAL_MEMORY_ARRAY_LOCATION_EISA_ADDON_CARD               ((BYTE)0x05) // EISA add-on card
#define SMBIOS_PHYSICAL_MEMORY_ARRAY_LOCATION_PCI_ADDON_CARD                ((BYTE)0x06) // PCI add-on card
#define SMBIOS_PHYSICAL_MEMORY_ARRAY_LOCATION_MCA_ADDON_CARD                ((BYTE)0x07) // MCA add-on card
#define SMBIOS_PHYSICAL_MEMORY_ARRAY_LOCATION_PCMCIA_ADDON_CARD             ((BYTE)0x08) // PCMCIA add-on card
#define SMBIOS_PHYSICAL_MEMORY_ARRAY_LOCATION_PROPRIETARY_ADDON_CARD        ((BYTE)0x09) // Proprietary add-on card
#define SMBIOS_PHYSICAL_MEMORY_ARRAY_LOCATION_NUBUS                         ((BYTE)0x0A) // NuBus
#define SMBIOS_PHYSICAL_MEMORY_ARRAY_LOCATION_PC_98_C20_ADDON_CARD          ((BYTE)0xA0) // PC-98/C20 add-on card
#define SMBIOS_PHYSICAL_MEMORY_ARRAY_LOCATION_PC_98_C24_ADDON_CARD          ((BYTE)0xA1) // PC-98/C24 add-on card
#define SMBIOS_PHYSICAL_MEMORY_ARRAY_LOCATION_PC_98_E_ADDON_CARD            ((BYTE)0xA2) // PC-98/E add-on card
#define SMBIOS_PHYSICAL_MEMORY_ARRAY_LOCATION_PC_98_LOCAL_BUS_ADDON_CARD    ((BYTE)0xA3) // PC-98/Local bus add-on card
#define SMBIOS_PHYSICAL_MEMORY_ARRAY_LOCATION_CXL_ADDON_CARD                ((BYTE)0xA4) // CXL add-on card

#define SMBIOS_PHYSICAL_MEMORY_ARRAY_USE_OTHER              ((BYTE)0x01) // Other
#define SMBIOS_PHYSICAL_MEMORY_ARRAY_USE_UNKNOWN            ((BYTE)0x02) // Unknown
#define SMBIOS_PHYSICAL_MEMORY_ARRAY_USE_SYSTEM_MEMORY      ((BYTE)0x03) // System memory
#define SMBIOS_PHYSICAL_MEMORY_ARRAY_USE_VIDEO_MEMORY       ((BYTE)0x04) // Video memory
#define SMBIOS_PHYSICAL_MEMORY_ARRAY_USE_FLASH_MEMORY       ((BYTE)0x05) // Flash memory
#define SMBIOS_PHYSICAL_MEMORY_ARRAY_USE_NON_VOLATILE_RAM   ((BYTE)0x06) // Non-volatile RAM
#define SMBIOS_PHYSICAL_MEMORY_ARRAY_USE_CACHE_MEMORY       ((BYTE)0x07) // Cache memory

#define SMBIOS_PHYSICAL_MEMORY_ARRAY_ERROR_CORRECTION_TYPE_OTHER            ((BYTE)0x01) // Other
#define SMBIOS_PHYSICAL_MEMORY_ARRAY_ERROR_CORRECTION_TYPE_UNKNOWN          ((BYTE)0x02) // Unknown
#define SMBIOS_PHYSICAL_MEMORY_ARRAY_ERROR_CORRECTION_TYPE_NONE             ((BYTE)0x03) // None
#define SMBIOS_PHYSICAL_MEMORY_ARRAY_ERROR_CORRECTION_TYPE_PARITY           ((BYTE)0x04) // Parity
#define SMBIOS_PHYSICAL_MEMORY_ARRAY_ERROR_CORRECTION_TYPE_SINGLE_BIT_ECC   ((BYTE)0x05) // Single-bit ECC
#define SMBIOS_PHYSICAL_MEMORY_ARRAY_ERROR_CORRECTION_TYPE_MULTI_BIT_ECC    ((BYTE)0x06) // Multi-bit ECC
#define SMBIOS_PHYSICAL_MEMORY_ARRAY_ERROR_CORRECTION_TYPE_CRC              ((BYTE)0x07) // CRC

typedef struct _SMBIOS_PHYSICAL_MEMORY_ARRAY
{
    SMBIOS_HEADER Header;
    BYTE Location;          // SMBIOS_PHYSICAL_MEMORY_ARRAY_LOCATION_*
    BYTE Use;               // SMBIOS_PHYSICAL_MEMORY_ARRAY_USE_*
    BYTE ErrorCorrection;   // SMBIOS_PHYSICAL_MEMORY_ARRAY_ERROR_CORRECTION_TYPE_*
    DWORD MaximumCapacity;
    WORD ErrorInformationHandle;
    WORD NumberOfMemoryDevices;
    QWORD ExtendedMaximumCapacity;
} SMBIOS_PHYSICAL_MEMORY_ARRAY, *PSMBIOS_PHYSICAL_MEMORY_ARRAY, SMBIOS_TYPE_16, *PSMBIOS_TYPE_16;

#pragma endregion

#pragma region Memory Device (Type 17)

#define SMBIOS_TYPE_MEMORY_DEVICE ((BYTE)17)

#define SMBIOS_MEMORY_DEVICE_FORM_FACTOR_OTHER              ((BYTE)0x01) // Other
#define SMBIOS_MEMORY_DEVICE_FORM_FACTOR_UNKNOWN            ((BYTE)0x02) // Unknown
#define SMBIOS_MEMORY_DEVICE_FORM_FACTOR_SIMM               ((BYTE)0x03) // SIMM
#define SMBIOS_MEMORY_DEVICE_FORM_FACTOR_SIP                ((BYTE)0x04) // SIP
#define SMBIOS_MEMORY_DEVICE_FORM_FACTOR_CHIP               ((BYTE)0x05) // Chip
#define SMBIOS_MEMORY_DEVICE_FORM_FACTOR_DIP                ((BYTE)0x06) // DIP
#define SMBIOS_MEMORY_DEVICE_FORM_FACTOR_ZIP                ((BYTE)0x07) // ZIP
#define SMBIOS_MEMORY_DEVICE_FORM_FACTOR_PROPRIETARY_CARD   ((BYTE)0x08) // Proprietary Card
#define SMBIOS_MEMORY_DEVICE_FORM_FACTOR_DIMM               ((BYTE)0x09) // DIMM
#define SMBIOS_MEMORY_DEVICE_FORM_FACTOR_TSOP               ((BYTE)0x0A) // TSOP
#define SMBIOS_MEMORY_DEVICE_FORM_FACTOR_ROW_OF_CHIPS       ((BYTE)0x0B) // Row of chips
#define SMBIOS_MEMORY_DEVICE_FORM_FACTOR_RIMM               ((BYTE)0x0C) // RIMM
#define SMBIOS_MEMORY_DEVICE_FORM_FACTOR_SODIMM             ((BYTE)0x0D) // SODIMM
#define SMBIOS_MEMORY_DEVICE_FORM_FACTOR_SRIMM              ((BYTE)0x0E) // SRIMM
#define SMBIOS_MEMORY_DEVICE_FORM_FACTOR_FB_DIMM            ((BYTE)0x0F) // FB-DIMM
#define SMBIOS_MEMORY_DEVICE_FORM_FACTOR_DIE                ((BYTE)0x10) // Die
#define SMBIOS_MEMORY_DEVICE_FORM_FACTOR_CAMM               ((BYTE)0x11) // CAMM

#define SMBIOS_MEMORY_DEVICE_TYPE_OTHER                         ((BYTE)0x01)
#define SMBIOS_MEMORY_DEVICE_TYPE_UNKNOWN                       ((BYTE)0x02)
#define SMBIOS_MEMORY_DEVICE_TYPE_DRAM                          ((BYTE)0x03)
#define SMBIOS_MEMORY_DEVICE_TYPE_EDRAM                         ((BYTE)0x04)
#define SMBIOS_MEMORY_DEVICE_TYPE_VRAM                          ((BYTE)0x05)
#define SMBIOS_MEMORY_DEVICE_TYPE_SRAM                          ((BYTE)0x06)
#define SMBIOS_MEMORY_DEVICE_TYPE_RAM                           ((BYTE)0x07)
#define SMBIOS_MEMORY_DEVICE_TYPE_ROM                           ((BYTE)0x08)
#define SMBIOS_MEMORY_DEVICE_TYPE_FLASH                         ((BYTE)0x09)
#define SMBIOS_MEMORY_DEVICE_TYPE_EEPROM                        ((BYTE)0x0A)
#define SMBIOS_MEMORY_DEVICE_TYPE_FEPROM                        ((BYTE)0x0B)
#define SMBIOS_MEMORY_DEVICE_TYPE_EPROM                         ((BYTE)0x0C)
#define SMBIOS_MEMORY_DEVICE_TYPE_CDRAM                         ((BYTE)0x0D)
#define SMBIOS_MEMORY_DEVICE_TYPE_3DRAM                         ((BYTE)0x0E)
#define SMBIOS_MEMORY_DEVICE_TYPE_SDRAM                         ((BYTE)0x0F)
#define SMBIOS_MEMORY_DEVICE_TYPE_SGRAM                         ((BYTE)0x10)
#define SMBIOS_MEMORY_DEVICE_TYPE_RDRAM                         ((BYTE)0x11)
#define SMBIOS_MEMORY_DEVICE_TYPE_DDR                           ((BYTE)0x12)
#define SMBIOS_MEMORY_DEVICE_TYPE_DDR2                          ((BYTE)0x13)
#define SMBIOS_MEMORY_DEVICE_TYPE_DDR2_FB_DIMM                  ((BYTE)0x14)
#define SMBIOS_MEMORY_DEVICE_TYPE_DDR3                          ((BYTE)0x18)
#define SMBIOS_MEMORY_DEVICE_TYPE_FBD2                          ((BYTE)0x19)
#define SMBIOS_MEMORY_DEVICE_TYPE_DDR4                          ((BYTE)0x1A)
#define SMBIOS_MEMORY_DEVICE_TYPE_LPDDR                         ((BYTE)0x1B)
#define SMBIOS_MEMORY_DEVICE_TYPE_LPDDR2                        ((BYTE)0x1C)
#define SMBIOS_MEMORY_DEVICE_TYPE_LPDDR3                        ((BYTE)0x1D)
#define SMBIOS_MEMORY_DEVICE_TYPE_LPDDR4                        ((BYTE)0x1E)
#define SMBIOS_MEMORY_DEVICE_TYPE_LOGICAL_NON_VOLATILE_DEVICE   ((BYTE)0x1F)
#define SMBIOS_MEMORY_DEVICE_TYPE_HBM                           ((BYTE)0x20)
#define SMBIOS_MEMORY_DEVICE_TYPE_HBM2                          ((BYTE)0x21)
#define SMBIOS_MEMORY_DEVICE_TYPE_DDR5                          ((BYTE)0x22)
#define SMBIOS_MEMORY_DEVICE_TYPE_LPDDR5                        ((BYTE)0x23)
#define SMBIOS_MEMORY_DEVICE_TYPE_HBM3                          ((BYTE)0x24)

#define SMBIOS_MEMORY_DEVICE_TECHNOLOGY_OTHER                           ((BYTE)0x01) // Other
#define SMBIOS_MEMORY_DEVICE_TECHNOLOGY_UNKNOWN                         ((BYTE)0x02) // Unknown
#define SMBIOS_MEMORY_DEVICE_TECHNOLOGY_DRAM                            ((BYTE)0x03) // DRAM
#define SMBIOS_MEMORY_DEVICE_TECHNOLOGY_NVDIMM_N                        ((BYTE)0x04) // NVDIMM-N
#define SMBIOS_MEMORY_DEVICE_TECHNOLOGY_NVDIMM_F                        ((BYTE)0x05) // NVDIMM-F
#define SMBIOS_MEMORY_DEVICE_TECHNOLOGY_NVDIMM_P                        ((BYTE)0x06) // NVDIMM-P
#define SMBIOS_MEMORY_DEVICE_TECHNOLOGY_INTEL_OPTANE_PERSISTENT_MEMORY  ((BYTE)0x07) // Intel® Optane™ persistent memory
#define SMBIOS_MEMORY_DEVICE_TECHNOLOGY_MRDIMM                          ((BYTE)0x08) // MRDIMM

typedef struct _SMBIOS_MEMORY_DEVICE
{
    SMBIOS_HEADER Header;
    WORD PhysicalMemoryArrayHandle;
    WORD ErrorInformationHandle;
    WORD TotalWidth;
    WORD DataWidth;
    union
    {
        WORD Value; // 0 = no device, FFFFh = unknown, 7FFFh = see "ExtendedSize" field
        struct
        {
            WORD Size : 15;
            WORD Granularity : 1; // 0 = MB, 1 = KB
        };
    } Size;
    BYTE FormFactor; // SMBIOS_MEMORY_DEVICE_FORM_FACTOR_*
    BYTE DeviceSet;
    BYTE DeviceLocator;
    BYTE BankLocator;
    BYTE Type; // SMBIOS_MEMORY_DEVICE_TYPE_*
    union
    {
        WORD Value;
        struct
        {
            WORD Reserved : 1;      // 00 Reserved, set to 0
            WORD Other : 1;         // 01 Other
            WORD Unknow : 1;        // 02 Unknown
            WORD FastPaged : 1;     // 03 Fast-paged
            WORD StaticColumn : 1;  // 04 Static column
            WORD PseudoStatic : 1;  // 05 Pseudo-static
            WORD RAMBUS : 1;        // 06 RAMBUS
            WORD Synchronous : 1;   // 07 Synchronous
            WORD CMOS : 1;          // 08 CMOS
            WORD EDO : 1;           // 09 EDO
            WORD WindowDRAM : 1;    // 10 Window DRAM
            WORD CacheDRAM : 1;     // 11 Cache DRAM
            WORD NonVolatile : 1;   // 12 Non-volatile
            WORD Registered : 1;    // 13 Registered (Buffered)
            WORD Unbuffered : 1;    // 14 Unbuffered (Unregistered)
            WORD LRDIMM : 1;        // 15 LRDIMM
        };
    } TypeDetail;
#if SMBIOS_VERSION >= 0x02030000
    WORD Speed;
    BYTE Manufacturer;
    BYTE SerialNumber;
    BYTE AssetTag;
    BYTE PartNumber;
#if SMBIOS_VERSION >= 0x02060000
    union
    {
        BYTE Value;
        struct
        {
            BYTE Rank : 4;      // 0:3 0 for unknown rank information
            BYTE Reserved : 4;  // 4:7 Reserved
        };
    } Attributes;
#if SMBIOS_VERSION >= 0x02070000
    DWORD ExtendedSize;
    WORD ConfiguredMemorySpeed;
#if SMBIOS_VERSION >= 0x02080000
    WORD MinimumVoltage;
    WORD MaximumVoltage;
    WORD ConfiguredVoltage;
#if SMBIOS_VERSION >= 0x03020000
    BYTE Technology; // SMBIOS_MEMORY_DEVICE_TECHNOLOGY_*
    union
    {
        WORD Value;
        struct
        {
            WORD Reserved0 : 1;                         // 00 Reserved, set to 0
            WORD Other : 1;                             // 01 Other
            WORD Unknown : 1;                           // 02 Unknown
            WORD VolatileMemory : 1;                    // 03 Volatile memory
            WORD ByteAccessiblePersistentMemory : 1;    // 04 Byte-accessible persistent memory
            WORD BlockAccessiblePersistentMemory : 1;   // 05 Block-accessible persistent memory
            WORD Reserved1 : 10;                        // 06:15 Reserved, set to 0
        };
    } OperatingModeCapability;
    BYTE FirmwareVersion;
    WORD ModuleManufacturerID;
    WORD ModuleProductID;
    WORD SubsystemControllerManufacturerID;
    QWORD NonVolatileSize;
    QWORD VolatileSize;
    QWORD CacheSize;
    QWORD LogicalSize;
#if SMBIOS_VERSION >= 0x03030000
    DWORD ExtendedSpeed;
    DWORD ExtendedConfiguredSpeed;
#if SMBIOS_VERSION >= 0x03070000
    WORD PMIC0ManufacturerID;
    WORD PMIC0RevisionNumber;
    WORD RCDManufacturerID;
    WORD RCDRevisionNumber;
#endif // SMBIOS_VERSION >= 0x03070000
#endif // SMBIOS_VERSION >= 0x03030000
#endif // SMBIOS_VERSION >= 0x03020000
#endif // SMBIOS_VERSION >= 0x02080000
#endif // SMBIOS_VERSION >= 0x02070000
#endif // SMBIOS_VERSION >= 0x02060000
#endif // SMBIOS_VERSION >= 0x02030000
} SMBIOS_MEMORY_DEVICE, *PSMBIOS_MEMORY_DEVICE, SMBIOS_TYPE_17, *PSMBIOS_TYPE_17;

#pragma endregion

#pragma region 32-Bit Memory Error Information (Type 18)

#define SMBIOS_TYPE_32BIT_MEMORY_ERROR_INFORMATION ((BYTE)18)

typedef struct _SMBIOS_32BIT_MEMORY_ERROR_INFORMATION
{
    SMBIOS_HEADER Header;
    BYTE Type;          // SMBIOS_MEMORY_ERROR_TYPE_*
    BYTE Granularity;   // SMBIOS_MEMORY_ERROR_GRANULARITY_*
    BYTE Operation;     // SMBIOS_MEMORY_ERROR_OPERATION_*
    DWORD VendorSyndrome;
    DWORD MemoryArrayErrorAddress;
    DWORD DeviceErrorAddress;
    DWORD Resolution;
} SMBIOS_32BIT_MEMORY_ERROR_INFORMATION, *PSMBIOS_32BIT_MEMORY_ERROR_INFORMATION, SMBIOS_TYPE_18, *PSMBIOS_TYPE_18;

#pragma endregion

#pragma region Memory Array Mapped Address (Type 19)

#define SMBIOS_TYPE_MEMORY_ARRAY_MAPPED_ADDRESS ((BYTE)19)

typedef struct _SMBIOS_MEMORY_ARRAY_MAPPED_ADDRESS
{
    SMBIOS_HEADER Header;
    DWORD StartingAddress;
    DWORD EndingAddress;
    WORD MemoryArrayHandle;
    BYTE PartitionWidth;
#if SMBIOS_VERSION >= 0x02070000
    QWORD ExtendedStartingAddress;
    QWORD ExtendedEndingAddress;
#endif // SMBIOS_VERSION >= 0x02070000
} SMBIOS_MEMORY_ARRAY_MAPPED_ADDRESS, *PSMBIOS_MEMORY_ARRAY_MAPPED_ADDRESS, SMBIOS_TYPE_19, *PSMBIOS_TYPE_19;

#pragma endregion

#pragma region Memory Device Mapped Address (Type 20)

#define SMBIOS_TYPE_MEMORY_DEVICE_MAPPED_ADDRESS ((BYTE)20)

typedef struct _SMBIOS_MEMORY_DEVICE_MAPPED_ADDRESS
{
    SMBIOS_HEADER Header;
    DWORD StartingAddress;
    DWORD EndingAddress;
    WORD MemoryDeviceHandle;
    WORD MemoryArrayMappedAddressHandle;
    BYTE PartitionRowPosition;
    BYTE InterleavePosition;
    BYTE InterleavedDataDepth;
#if SMBIOS_VERSION >= 0x02070000
    QWORD ExtendedStartingAddress;
    QWORD ExtendedEndingAddress;
#endif // SMBIOS_VERSION >= 0x02070000
} SMBIOS_MEMORY_DEVICE_MAPPED_ADDRESS, *PSMBIOS_MEMORY_DEVICE_MAPPED_ADDRESS, SMBIOS_TYPE_20, *PSMBIOS_TYPE_20;

#pragma endregion

#pragma region Built-in Pointing Device (Type 21)

#define SMBIOS_TYPE_BUILTIN_POINTING_DEVICE ((BYTE)21)

#define SMBIOS_BUILTIN_POINTING_DEVICE_TYPE_OTHER           ((BYTE)0x01) // Other
#define SMBIOS_BUILTIN_POINTING_DEVICE_TYPE_UNKNOWN         ((BYTE)0x02) // Unknown
#define SMBIOS_BUILTIN_POINTING_DEVICE_TYPE_MOUSE           ((BYTE)0x03) // Mouse
#define SMBIOS_BUILTIN_POINTING_DEVICE_TYPE_TRACK_BALL      ((BYTE)0x04) // Track Ball
#define SMBIOS_BUILTIN_POINTING_DEVICE_TYPE_TRACK_POINT     ((BYTE)0x05) // Track Point
#define SMBIOS_BUILTIN_POINTING_DEVICE_TYPE_GLIDE_POINT     ((BYTE)0x06) // Glide Point
#define SMBIOS_BUILTIN_POINTING_DEVICE_TYPE_TOUCH_PAD       ((BYTE)0x07) // Touch Pad
#define SMBIOS_BUILTIN_POINTING_DEVICE_TYPE_TOUCH_SCREEN    ((BYTE)0x08) // Touch Screen
#define SMBIOS_BUILTIN_POINTING_DEVICE_TYPE_OPTICAL_SENSOR  ((BYTE)0x09) // Optical Sensor

#define SMBIOS_BUILTIN_POINTING_DEVICE_INTERFACE_OTHER                  ((BYTE)0x01) // Other
#define SMBIOS_BUILTIN_POINTING_DEVICE_INTERFACE_UNKNOWN                ((BYTE)0x02) // Unknown
#define SMBIOS_BUILTIN_POINTING_DEVICE_INTERFACE_SERIAL                 ((BYTE)0x03) // Serial
#define SMBIOS_BUILTIN_POINTING_DEVICE_INTERFACE_PS2                    ((BYTE)0x04) // PS/2
#define SMBIOS_BUILTIN_POINTING_DEVICE_INTERFACE_INFRARED               ((BYTE)0x05) // Infrared
#define SMBIOS_BUILTIN_POINTING_DEVICE_INTERFACE_HP_HIL                 ((BYTE)0x06) // HP-HIL
#define SMBIOS_BUILTIN_POINTING_DEVICE_INTERFACE_BUS_MOUSE              ((BYTE)0x07) // Bus mouse
#define SMBIOS_BUILTIN_POINTING_DEVICE_INTERFACE_ADB                    ((BYTE)0x08) // ADB (Apple Desktop Bus)
#define SMBIOS_BUILTIN_POINTING_DEVICE_INTERFACE_BUS_MOUSE_DB9          ((BYTE)0xA0) // Bus mouse DB-9
#define SMBIOS_BUILTIN_POINTING_DEVICE_INTERFACE_BUS_MOUSE_MICRO_DIN    ((BYTE)0xA1) // Bus mouse Bus mouse micro-DIN
#define SMBIOS_BUILTIN_POINTING_DEVICE_INTERFACE_USB                    ((BYTE)0xA2) // USB
#define SMBIOS_BUILTIN_POINTING_DEVICE_INTERFACE_I2C                    ((BYTE)0xA3) // I2C
#define SMBIOS_BUILTIN_POINTING_DEVICE_INTERFACE_SPI                    ((BYTE)0xA4) // SPI

typedef struct _SMBIOS_BUILTIN_POINTING_DEVICE
{
    SMBIOS_HEADER Header;
    BYTE Type;      // SMBIOS_BUILTIN_POINTING_DEVICE_TYPE_*
    BYTE Interface; // SMBIOS_BUILTIN_POINTING_DEVICE_INTERFACE_*
    BYTE NumberOfButtons;
} SMBIOS_BUILTIN_POINTING_DEVICE, *PSMBIOS_BUILTIN_POINTING_DEVICE, SMBIOS_TYPE_21, *PSMBIOS_TYPE_21;

#pragma endregion

#pragma region Portable Battery (Type 22)

#define SMBIOS_TYPE_PORTABLE_BATTERY ((BYTE)22)

#define SMBIOS_PORTABLE_BATTERY_DEVICE_CHEMISTRY_OTHER                   ((BYTE)0x01) // Other
#define SMBIOS_PORTABLE_BATTERY_DEVICE_CHEMISTRY_UNKNOWN                 ((BYTE)0x02) // Unknown
#define SMBIOS_PORTABLE_BATTERY_DEVICE_CHEMISTRY_LEAD_ACID               ((BYTE)0x03) // Lead Acid
#define SMBIOS_PORTABLE_BATTERY_DEVICE_CHEMISTRY_NICKEL_CADMIUM          ((BYTE)0x04) // Nickel Cadmium
#define SMBIOS_PORTABLE_BATTERY_DEVICE_CHEMISTRY_NICKEL_METAL_HYDRIDE    ((BYTE)0x05) // Nickel metal hydride
#define SMBIOS_PORTABLE_BATTERY_DEVICE_CHEMISTRY_LITHIUM_ION             ((BYTE)0x06) // Lithium-ion
#define SMBIOS_PORTABLE_BATTERY_DEVICE_CHEMISTRY_ZINC_AIR                ((BYTE)0x07) // Zinc air
#define SMBIOS_PORTABLE_BATTERY_DEVICE_CHEMISTRY_LITHIUM_POLYMER         ((BYTE)0x08) // Lithium Polymer

typedef struct _SMBIOS_PORTABLE_BATTERY
{
    SMBIOS_HEADER Header;
    BYTE Location;
    BYTE Manufacturer;
    BYTE ManufactureDate;
    BYTE SerialNumber;
    BYTE DeviceName;
    BYTE DeviceChemistry;   // SMBIO_PORTABLE_BATTERY_DEVICE_CHEMISTRY_*
    WORD DesignCapacity;    // Design capacity of the battery in milliwatt-hours, or 0 if unknown
    WORD DesignVoltage;     // Design voltage of the battery in millivolts, or 0 if unknown
    BYTE SBDSVersionNumber;
    BYTE MaximumError;
#if SMBIOS_VERSION >= 0x02020000
    WORD SBDSSerialNumber;
    union
    {
        WORD Value;
        struct
        {
            WORD Day : 5;   // 00:04 Date, in the range 1 to 31
            WORD Month : 4; // 05:08 Month, in the range 1 to 12
            WORD Year : 7;  // 09:15 Year, biased by 1980, in the range 0 to 127
        };
    } SBDSManufactureDate;
    BYTE SBDSDeviceChemistry;
    BYTE DesignCapacityMultiplier;
    DWORD OEMSpecific;
#endif // SMBIOS_VERSION >= 0x02020000
} SMBIOS_PORTABLE_BATTERY, *PSMBIOS_PORTABLE_BATTERY, SMBIOS_TYPE_22, *PSMBIOS_TYPE_22;

#pragma endregion

#if SMBIOS_VERSION >= 0x02020000

#pragma region System Reset (Type 23)

#define SMBIOS_TYPE_SYSTEM_RESET ((BYTE)23)

#define SMBIOS_SYSTEM_RESET_BOOT_OPTION_RESERVED            ((BYTE)0b00) // Reserved, do not use
#define SMBIOS_SYSTEM_RESET_BOOT_OPTION_OPERATING_SYSTEM    ((BYTE)0b01) // Operating system
#define SMBIOS_SYSTEM_RESET_BOOT_OPTION_SYSTEM_UTILITIES    ((BYTE)0b10) // System utilities
#define SMBIOS_SYSTEM_RESET_BOOT_OPTION_DO_NOT_REBOOT       ((BYTE)0b11) // Do not reboot

typedef struct _SMBIOS_SYSTEM_RESET
{
    SMBIOS_HEADER Header;
    union
    {
        BYTE Value;
        struct
        {
            BYTE Status : 1;                        // 0 Identifies whether (1) or not (0) the system reset is enabled by the user
            BYTE BootOption : 2;                    // 1:2 SMBIOS_SYSTEM_RESET_BOOT_OPTION_*
            BYTE BootOptionOnLimit : 2;             // 3:4 SMBIOS_SYSTEM_RESET_BOOT_OPTION_*
            BYTE SystemContainsAWatchdogTimer : 1;  // 5 True (1) or False (0)
            BYTE Reserved : 2;                      // 6:7 Reserved for future assignment by this specification; set to 00b
        };
    } Capabilities;
    WORD ResetCount;
    WORD ResetLimit;
    WORD TimerInterval;
    WORD Timeout;
} SMBIOS_SYSTEM_RESET, *PSMBIOS_SYSTEM_RESET, SMBIOS_TYPE_23, *PSMBIOS_TYPE_23;

#pragma endregion

#pragma region Hardware Security (Type 24)

#define SMBIOS_TYPE_HARDWARE_SECURITY ((BYTE)24)

#define SMBIOS_HARDWARE_SECURITY_SETTINGS_STATUS_DISABLED           ((BYTE)0b00) // Disabled
#define SMBIOS_HARDWARE_SECURITY_SETTINGS_STATUS_ENABLED            ((BYTE)0b01) // Enabled
#define SMBIOS_HARDWARE_SECURITY_SETTINGS_STATUS_NOT_IMPLEMENTED    ((BYTE)0b10) // Not Implemented
#define SMBIOS_HARDWARE_SECURITY_SETTINGS_STATUS_UNKNOWN            ((BYTE)0b11) // Unknown

typedef struct _SMBIOS_HARDWARE_SECURITY
{
    SMBIOS_HEADER Header;
    union
    {
        BYTE Value;
        struct
        {
            BYTE FrontPanelResetStatus : 2;         // SMBIOS_HARDWARE_SECURITY_SETTINGS_STATUS_*
            BYTE AdministratorPasswordStatus : 2;   // SMBIOS_HARDWARE_SECURITY_SETTINGS_STATUS_*
            BYTE KeyboardPasswordStatus : 2;        // SMBIOS_HARDWARE_SECURITY_SETTINGS_STATUS_*
            BYTE PowerOnPasswordStatus : 2;         // SMBIOS_HARDWARE_SECURITY_SETTINGS_STATUS_*
        };
    } Settings;
} SMBIOS_HARDWARE_SECURITY, *PSMBIOS_HARDWARE_SECURITY, SMBIOS_TYPE_24, *PSMBIOS_TYPE_24;

#pragma endregion

#pragma region System Power Controls (Type 25)

#define SMBIOS_TYPE_SYSTEM_POWER_CONTROLS ((BYTE)25)

typedef struct _SMBIOS_SYSTEM_POWER_CONTROLS
{
    SMBIOS_HEADER Header;
    BYTE NextScheduledPowerOnMonth;         // 01h to 12h
    BYTE NextScheduledPowerOnDayOfMonth;    // 01h to 31h
    BYTE NextScheduledPowerOnHour;          // 00h to 23h
    BYTE NextScheduledPowerOnMinute;        // 00h to 59h
    BYTE NextScheduledPowerOnSecond;        // 00h to 59h
} SMBIOS_SYSTEM_POWER_CONTROLS, *PSMBIOS_SYSTEM_POWER_CONTROLS, SMBIOS_TYPE_25, *PSMBIOS_TYPE_25;

#pragma endregion

#define SMBIOS_PROBE_STATUS_OTHER           ((BYTE)0b001) // Other
#define SMBIOS_PROBE_STATUS_UNKNOWN         ((BYTE)0b010) // Unknown
#define SMBIOS_PROBE_STATUS_OK              ((BYTE)0b011) // OK
#define SMBIOS_PROBE_STATUS_NON_CRITICAL    ((BYTE)0b100) // Non-critical
#define SMBIOS_PROBE_STATUS_CRITICAL        ((BYTE)0b101) // Critical
#define SMBIOS_PROBE_STATUS_NON_RECOVERABLE ((BYTE)0b110) // Non-recoverable

#pragma region Voltage Probe (Type 26)

#define SMBIOS_TYPE_VOLTAGE_PROBE ((BYTE)26)

#define SMBIOS_VOLTAGE_PROBE_LOCATION_OTHER                     ((BYTE)0b00001) // Other
#define SMBIOS_VOLTAGE_PROBE_LOCATION_UNKNOWN                   ((BYTE)0b00010) // Unknown
#define SMBIOS_VOLTAGE_PROBE_LOCATION_PROCESSOR                 ((BYTE)0b00011) // Processor
#define SMBIOS_VOLTAGE_PROBE_LOCATION_DISK                      ((BYTE)0b00100) // Disk
#define SMBIOS_VOLTAGE_PROBE_LOCATION_PERIPHERAL_BAY            ((BYTE)0b00101) // Peripheral Bay
#define SMBIOS_VOLTAGE_PROBE_LOCATION_SYSTEM_MANAGEMENT_MODULE  ((BYTE)0b00110) // System Management Module
#define SMBIOS_VOLTAGE_PROBE_LOCATION_MOTHERBOARD               ((BYTE)0b00111) // Motherboard
#define SMBIOS_VOLTAGE_PROBE_LOCATION_MEMORY_MODULE             ((BYTE)0b01000) // Memory Module
#define SMBIOS_VOLTAGE_PROBE_LOCATION_PROCESSOR_MODULE          ((BYTE)0b01001) // Processor Module
#define SMBIOS_VOLTAGE_PROBE_LOCATION_POWER_UNIT                ((BYTE)0b01010) // Power Unit
#define SMBIOS_VOLTAGE_PROBE_LOCATION_ADDIN_CARD                ((BYTE)0b01011) // Add-in Card

typedef struct _SMBIOS_VOLTAGE_PROBE
{
    SMBIOS_HEADER Header;
    BYTE Description;
    union
    {
        BYTE Value;
        struct
        {
            BYTE Location : 5;  // 0:4 SMBIOS_VOLTAGE_PROBE_LOCATION_*
            BYTE Status : 3;    // 5:7 SMBIOS_PROBE_STATUS_*
        };
    } LocationAndStatus;
    WORD MaximumValue;
    WORD MinimumValue;
    WORD Resolution;
    WORD Tolerance;
    WORD Accuracy;
    DWORD OEMDefined;
    WORD NominalValue; // Present only if the structure’s length is larger than 14h
} SMBIOS_VOLTAGE_PROBE, *PSMBIOS_VOLTAGE_PROBE, SMBIOS_TYPE_26, *PSMBIOS_TYPE_26;

#pragma endregion

#pragma region Cooling Device (Type 27)

#define SMBIOS_TYPE_COOLING_DEVICE ((BYTE)27)

#define SMBIOS_COOLING_DEVICE_TYPE_OTHER                    ((BYTE)0b00001) // Other
#define SMBIOS_COOLING_DEVICE_TYPE_UNKNOWN                  ((BYTE)0b00010) // Unknown
#define SMBIOS_COOLING_DEVICE_TYPE_FAN                      ((BYTE)0b00011) // Fan
#define SMBIOS_COOLING_DEVICE_TYPE_CENTRIFUGAL_BLOWER       ((BYTE)0b00100) // Centrifugal Blower
#define SMBIOS_COOLING_DEVICE_TYPE_CHIP_FAN                 ((BYTE)0b00101) // Chip Fan
#define SMBIOS_COOLING_DEVICE_TYPE_CABINET_FAN              ((BYTE)0b00110) // Cabinet Fan
#define SMBIOS_COOLING_DEVICE_TYPE_POWER_SUPPLY_FAN         ((BYTE)0b00111) // Power Supply Fan
#define SMBIOS_COOLING_DEVICE_TYPE_HEAT_PIPE                ((BYTE)0b01000) // Heat Pipe
#define SMBIOS_COOLING_DEVICE_TYPE_INTEGRATED_REFRIGERATION ((BYTE)0b01001) // Integrated Refrigeration
#define SMBIOS_COOLING_DEVICE_TYPE_ACTIVE_COOLING           ((BYTE)0b10000) // Active Cooling
#define SMBIOS_COOLING_DEVICE_TYPE_PASSIVE_COOLING          ((BYTE)0b10001) // Passive Cooling

typedef struct _SMBIOS_COOLING_DEVICE
{
    SMBIOS_HEADER Header;
    WORD TemperatureProbeHandle;
    union
    {
        BYTE Value;
        struct
        {
            BYTE Type : 5;      // 0:4 SMBIOS_COOLING_DEVICE_TYPE_*
            BYTE Status : 3;    // 5:7 SMBIOS_PROBE_STATUS_*
        };
    } DeviceTypeAndStatus;
    BYTE CoolingUnitGroup;
    DWORD OEMDefined;
    WORD NominalSpeed;
#if SMBIOS_VERSION >= 0x02070000
    BYTE Description;
#endif // SMBIOS_VERSION >= 0x02070000
} SMBIOS_COOLING_DEVICE, *PSMBIOS_COOLING_DEVICE, SMBIOS_TYPE_27, *PSMBIOS_TYPE_27;

#pragma endregion

#pragma region Temperature Probe (Type 28)

#define SMBIOS_TYPE_TEMPERATURE_PROBE ((BYTE)28)

#define SMBIOS_TEMPERATURE_PROBE_LOCATION_OTHER                     ((BYTE)0b00001) // Other
#define SMBIOS_TEMPERATURE_PROBE_LOCATION_UNKNOWN                   ((BYTE)0b00010) // Unknown
#define SMBIOS_TEMPERATURE_PROBE_LOCATION_PROCESSOR                 ((BYTE)0b00011) // Processor
#define SMBIOS_TEMPERATURE_PROBE_LOCATION_DISK                      ((BYTE)0b00100) // Disk
#define SMBIOS_TEMPERATURE_PROBE_LOCATION_PERIPHERAL_BAY            ((BYTE)0b00101) // Peripheral Bay
#define SMBIOS_TEMPERATURE_PROBE_LOCATION_SYSTEM_MANAGEMENT_MODULE  ((BYTE)0b00110) // System Management Module
#define SMBIOS_TEMPERATURE_PROBE_LOCATION_MOTHERBOARD               ((BYTE)0b00111) // Motherboard
#define SMBIOS_TEMPERATURE_PROBE_LOCATION_MEMORY_MODULE             ((BYTE)0b01000) // Memory Module
#define SMBIOS_TEMPERATURE_PROBE_LOCATION_PROCESSOR_MODULE          ((BYTE)0b01001) // Processor Module
#define SMBIOS_TEMPERATURE_PROBE_LOCATION_POWER_UNIT                ((BYTE)0b01010) // Power Unit
#define SMBIOS_TEMPERATURE_PROBE_LOCATION_ADDIN_CARD                ((BYTE)0b01011) // Add-in Card
#define SMBIOS_TEMPERATURE_PROBE_LOCATION_FRONT_PANEL_BOARD         ((BYTE)0b01100) // Front Panel Board
#define SMBIOS_TEMPERATURE_PROBE_LOCATION_BACK_PANEL_BOARD          ((BYTE)0b01101) // Back Panel Board
#define SMBIOS_TEMPERATURE_PROBE_LOCATION_POWER_SYSTEM_BOARD        ((BYTE)0b01110) // Power System Board
#define SMBIOS_TEMPERATURE_PROBE_LOCATION_DRIVE_BACK_PLANE          ((BYTE)0b01111) // Drive Back Plane

typedef struct _SMBIOS_TEMPERATURE_PROBE
{
    SMBIOS_HEADER Header;
    BYTE Description;
    union
    {
        BYTE Value;
        struct
        {
            BYTE Location : 5;  // 0:4 SMBIOS_TEMPERATURE_PROBE_LOCATION_*
            BYTE Status : 3;    // 5:7 SMBIOS_PROBE_STATUS_*
        };
    } LocationAndStatus;
    WORD MaximumValue;
    WORD MinimumValue;
    WORD Resolution;
    WORD Tolerance;
    WORD Accuracy;
    DWORD OEMDefined;
    WORD NominalValue;
} SMBIOS_TEMPERATURE_PROBE, *PSMBIOS_TEMPERATURE_PROBE, SMBIOS_TYPE_28, *PSMBIOS_TYPE_28;

#pragma endregion

#pragma region Electrical Current Probe (Type 29)

#define SMBIOS_TYPE_ELECTRICAL_CURRENT_PROBE ((BYTE)29)

#define SMBIOS_ELECTRICAL_CURRENT_PROBE_LOCATION_OTHER                      ((BYTE)0b00001) // Other
#define SMBIOS_ELECTRICAL_CURRENT_PROBE_LOCATION_UNKNOWN                    ((BYTE)0b00010) // Unknown
#define SMBIOS_ELECTRICAL_CURRENT_PROBE_LOCATION_PROCESSOR                  ((BYTE)0b00011) // Processor
#define SMBIOS_ELECTRICAL_CURRENT_PROBE_LOCATION_DISK                       ((BYTE)0b00100) // Disk
#define SMBIOS_ELECTRICAL_CURRENT_PROBE_LOCATION_PERIPHERAL_BAY             ((BYTE)0b00101) // Peripheral Bay
#define SMBIOS_ELECTRICAL_CURRENT_PROBE_LOCATION_SYSTEM_MANAGEMENT_MODULE   ((BYTE)0b00110) // System Management Module
#define SMBIOS_ELECTRICAL_CURRENT_PROBE_LOCATION_MOTHERBOARD                ((BYTE)0b00111) // Motherboard
#define SMBIOS_ELECTRICAL_CURRENT_PROBE_LOCATION_MEMORY_MODULE              ((BYTE)0b01000) // Memory Module
#define SMBIOS_ELECTRICAL_CURRENT_PROBE_LOCATION_PROCESSOR_MODULE           ((BYTE)0b01001) // Processor Module
#define SMBIOS_ELECTRICAL_CURRENT_PROBE_LOCATION_POWER_UNIT                 ((BYTE)0b01010) // Power Unit
#define SMBIOS_ELECTRICAL_CURRENT_PROBE_LOCATION_ADDIN_CARD                 ((BYTE)0b01011) // Add-in Card

typedef struct _SMBIOS_ELECTRICAL_CURRENT_PROBE
{
    SMBIOS_HEADER Header;
    BYTE Description;
    union
    {
        BYTE LocationAndStatus;
        struct
        {
            BYTE Location : 5;  /* SMBIOS_ELECTRICAL_CURRENT_PROBE_LOCATION_* */
            BYTE Status : 3;    // 5:7 SMBIOS_PROBE_STATUS_*
        };
    };
    WORD MaximumValue;
    WORD MinimumValue;
    WORD Resolution;
    WORD Tolerance;
    WORD Accuracy;
    DWORD OEMDefined;
    WORD NominalValue;
} SMBIOS_ELECTRICAL_CURRENT_PROBE, *PSMBIOS_ELECTRICAL_CURRENT_PROBE, SMBIOS_TYPE_29, *PSMBIOS_TYPE_29;

#pragma endregion

#pragma region Out-of-Band Remote Access (Type 30)

#define SMBIOS_TYPE_OUT_OF_BAND_REMOTE_ACCESS ((BYTE)30)

typedef struct _SMBIOS_OUT_OF_BAND_REMOTE_ACCESS
{
    SMBIOS_HEADER Header;
    BYTE ManufacturerName;
    union
    {
        BYTE Value;
        struct
        {
            BYTE InboundConnectionEnabled : 1;  // 0  Inbound Connection Enabled
            BYTE OutboundConnectionEnabled : 1; // 1  Outbound Connection Enabled
            BYTE Reserved : 6;                  // 2:7 Reserved for future definition by this specification; set to all zeros
        };
    } Connections;
} SMBIOS_OUT_OF_BAND_REMOTE_ACCESS, *PSMBIOS_OUT_OF_BAND_REMOTE_ACCESS, SMBIOS_TYPE_30, *PSMBIOS_TYPE_30;

#pragma endregion

#if SMBIOS_VERSION >= 0x02030000

#pragma region Boot Integrity Services (BIS) Entry Point (Type 31)

#define SMBIOS_TYPE_BOOT_INTEGRITY_SERVICES_ENTRY_POINT ((BYTE)31)

#pragma endregion Reserved

#pragma region System Boot Information (Type 32)

#define SMBIOS_TYPE_SYSTEM_BOOT_INFORMATION ((BYTE)32)

#define SMBIOS_SYSTEM_BOOT_STATUS_NO_ERROR                              ((BYTE)0) // No errors detected
#define SMBIOS_SYSTEM_BOOT_STATUS_NO_BOOTABLE_MEDIA                     ((BYTE)1) // No bootable media
#define SMBIOS_SYSTEM_BOOT_STATUS_OS_FAILED_TO_LOAD                     ((BYTE)2) // "normal" operating system failed to load
#define SMBIOS_SYSTEM_BOOT_STATUS_FIRMWARE_DETECTED_HARDWARE_FAILURE    ((BYTE)3) // Firmware-detected hardware failure
#define SMBIOS_SYSTEM_BOOT_STATUS_OS_DETECTED_HARDWARE_FAILURE          ((BYTE)4) // Operating system-detected hardware failure
#define SMBIOS_SYSTEM_BOOT_STATUS_USER_REQUESTED_BOOT                   ((BYTE)5) // User-requested boot
#define SMBIOS_SYSTEM_BOOT_STATUS_SYSTEM_SECURITY_VIOLATION             ((BYTE)6) // System security violation
#define SMBIOS_SYSTEM_BOOT_STATUS_PREVIOUSLY_REQUESTED_IMAGE            ((BYTE)7) // Previously requested image
#define SMBIOS_SYSTEM_BOOT_STATUS_SYSTEM_WATCHDOG_TIMER_EXPIRED         ((BYTE)8) // System watchdog timer expired

typedef struct _SMBIOS_SYSTEM_BOOT_INFORMATION
{
    SMBIOS_HEADER Header;
    BYTE Reserved[6];
    struct
    {
        BYTE Status; // SMBIOS_SYSTEM_BOOT_STATUS_*
        BYTE AdditionalData[9];
    } Status;
} SMBIOS_SYSTEM_BOOT_INFORMATION, *PSMBIOS_SYSTEM_BOOT_INFORMATION, SMBIOS_TYPE_32, *PSMBIOS_TYPE_32;

#pragma endregion

#pragma region 64-Bit Memory Error Information (Type 33)

#define SMBIOS_TYPE_64BIT_MEMORY_ERROR_INFORMATION ((BYTE)33)

typedef struct _SMBIOS_64BIT_MEMORY_ERROR_INFORMATION
{
    SMBIOS_HEADER Header;
    BYTE Type;          // SMBIOS_MEMORY_ERROR_TYPE_*
    BYTE Granularity;   // SMBIOS_MEMORY_ERROR_GRANULARITY_*
    BYTE Operation;     // SMBIOS_MEMORY_ERROR_OPERATION_*
    DWORD VendorSyndrome;
    QWORD MemoryArrayErrorAddress;
    QWORD DeviceErrorAddress;
    DWORD Resolution;
} SMBIOS_64BIT_MEMORY_ERROR_INFORMATION, *PSMBIOS_64BIT_MEMORY_ERROR_INFORMATION, SMBIOS_TYPE_33, *PSMBIOS_TYPE_33;

#pragma endregion

#pragma region Management Device (Type 34)

#define SMBIOS_TYPE_MANAGEMENT_DEVICE ((BYTE)34)

#define SMBIOS_MANAGEMENT_DEVICE_TYPE_OTHER                         ((BYTE)0x01) // Other
#define SMBIOS_MANAGEMENT_DEVICE_TYPE_UNKNOWN                       ((BYTE)0x02) // Unknown
#define SMBIOS_MANAGEMENT_DEVICE_TYPE_NATIONAL_SEMICONDUCTOR_LM75   ((BYTE)0x03) // National Semiconductor LM75
#define SMBIOS_MANAGEMENT_DEVICE_TYPE_NATIONAL_SEMICONDUCTOR_LM78   ((BYTE)0x04) // National Semiconductor LM78
#define SMBIOS_MANAGEMENT_DEVICE_TYPE_NATIONAL_SEMICONDUCTOR_LM79   ((BYTE)0x05) // National Semiconductor LM79
#define SMBIOS_MANAGEMENT_DEVICE_TYPE_NATIONAL_SEMICONDUCTOR_LM80   ((BYTE)0x06) // National Semiconductor LM80
#define SMBIOS_MANAGEMENT_DEVICE_TYPE_NATIONAL_SEMICONDUCTOR_LM81   ((BYTE)0x07) // National Semiconductor LM81
#define SMBIOS_MANAGEMENT_DEVICE_TYPE_ANALOG_DEVICES_ADM9240        ((BYTE)0x08) // Analog Devices ADM9240
#define SMBIOS_MANAGEMENT_DEVICE_TYPE_DALLAS_SEMICONDUCTOR_DS1780   ((BYTE)0x09) // Dallas Semiconductor DS1780
#define SMBIOS_MANAGEMENT_DEVICE_TYPE_MAXIM_1617                    ((BYTE)0x0A) // Maxim 1617
#define SMBIOS_MANAGEMENT_DEVICE_TYPE_GENESYS_GL518SM               ((BYTE)0x0B) // Genesys GL518SM
#define SMBIOS_MANAGEMENT_DEVICE_TYPE_WINBOND_W83781D               ((BYTE)0x0C) // Winbond W83781D
#define SMBIOS_MANAGEMENT_DEVICE_TYPE_HOLTEK_HT82H791               ((BYTE)0x0D) // Holtek HT82H791

#define SMBIOS_MANAGEMENT_DEVICE_ADDRESS_TYPE_OTHER     ((BYTE)0x01) // Other
#define SMBIOS_MANAGEMENT_DEVICE_ADDRESS_TYPE_UNKNOWN   ((BYTE)0x02) // Unknown
#define SMBIOS_MANAGEMENT_DEVICE_ADDRESS_TYPE_IO_PORT   ((BYTE)0x03) // I/O Port 
#define SMBIOS_MANAGEMENT_DEVICE_ADDRESS_TYPE_MEMORY    ((BYTE)0x04) // Memory
#define SMBIOS_MANAGEMENT_DEVICE_ADDRESS_TYPE_SMBUS     ((BYTE)0x05) // SM Bus

typedef struct _SMBIOS_MANAGEMENT_DEVICE
{
    SMBIOS_HEADER Header;
    BYTE Description;
    BYTE Type;          // SMBIOS_MANAGEMENT_DEVICE_TYPE_*
    DWORD Address;
    BYTE AddressType;   // SMBIOS_MANAGEMENT_DEVICE_ADDRESS_TYPE_*
} SMBIOS_MANAGEMENT_DEVICE, *PSMBIOS_MANAGEMENT_DEVICE, SMBIOS_TYPE_34, *PSMBIOS_TYPE_34;

#pragma endregion

#pragma region Management Device Component (Type 35)

#define SMBIOS_TYPE_MANAGEMENT_DEVICE_COMPONENT ((BYTE)35)

typedef struct _SMBIOS_MANAGEMENT_DEVICE_COMPONENT
{
    SMBIOS_HEADER Header;
    BYTE Description;
    WORD ManagementDeviceHandle;
    WORD ComponentHandle;
    WORD ThresholdHandle;
} SMBIOS_MANAGEMENT_DEVICE_COMPONENT, *PSMBIOS_MANAGEMENT_DEVICE_COMPONENT, SMBIOS_TYPE_35, *PSMBIOS_TYPE_35;

#pragma endregion

#pragma region Management Device Threshold Data (Type 36)

#define SMBIOS_TYPE_MANAGEMENT_DEVICE_THRESHOLD_DATA ((BYTE)36)

typedef struct _SMBIOS_MANAGEMENT_DEVICE_THRESHOLD_DATA
{
    SMBIOS_HEADER Header;
    WORD LowerThresholdNonCritical;
    WORD UpperThresholdNonCritical;
    WORD LowerThresholdCritical;
    WORD UpperThresholdCritical;
    WORD LowerThresholdNonRecoverable;
    WORD UpperThresholdNonRecoverable;
} SMBIOS_MANAGEMENT_DEVICE_THRESHOLD_DATA, *PSMBIOS_MANAGEMENT_DEVICE_THRESHOLD_DATA, SMBIOS_TYPE_36, *PSMBIOS_TYPE_36;

#pragma endregion

#pragma region Memory Channel (Type 37)

#define SMBIOS_TYPE_MEMORY_CHANNEL ((BYTE)37)

#define SMBIOS_MEMORY_CHANNEL_TYPE_OTHER        ((BYTE)0x01) // Other
#define SMBIOS_MEMORY_CHANNEL_TYPE_UNKNOWN      ((BYTE)0x02) // Unknown
#define SMBIOS_MEMORY_CHANNEL_TYPE_RAMBUS       ((BYTE)0x03) // Rambus
#define SMBIOS_MEMORY_CHANNEL_TYPE_SYNC_LINK    ((BYTE)0x04) // SyncLink

typedef struct _SMBIOS_MEMORY_CHANNEL_DEVICE
{
    BYTE Load;
    WORD Handle;
} SMBIOS_MEMORY_CHANNEL_DEVICE, *PSMBIOS_MEMORY_CHANNEL_DEVICE;

typedef struct _SMBIOS_MEMORY_CHANNEL
{
    SMBIOS_HEADER Header;
    BYTE Type; // SMBIOS_MEMORY_CHANNEL_TYPE_*
    BYTE MaximumLoad;
    BYTE MemoryDeviceCount;
    _Field_size_(MemoryDeviceCount) SMBIOS_MEMORY_CHANNEL_DEVICE MemoryDevices[];
} SMBIOS_MEMORY_CHANNEL, *PSMBIOS_MEMORY_CHANNEL, SMBIOS_TYPE_37, *PSMBIOS_TYPE_37;

#pragma endregion

#pragma region IPMI Device Information (Type 38)

#define SMBIOS_TYPE_IPMI_DEVICE_INFORMATION ((BYTE)38)

#define SMBIOS_IPMI_DEVICE_INTERFACE_TYPE_UNKONWN   ((BYTE)0x00) // Unknown
#define SMBIOS_IPMI_DEVICE_INTERFACE_TYPE_KCS       ((BYTE)0x01) // KCS: Keyboard Controller Style
#define SMBIOS_IPMI_DEVICE_INTERFACE_TYPE_SMIC      ((BYTE)0x02) // SMIC: Server Management Interface Chip
#define SMBIOS_IPMI_DEVICE_INTERFACE_TYPE_BT        ((BYTE)0x03) // BT: Block Transfer
#define SMBIOS_IPMI_DEVICE_INTERFACE_TYPE_SSIF      ((BYTE)0x04) // SSIF: SMBus System Interface

#define SMBIOS_IPMI_DEVICE_BASEADDRESS_REGISTER_SPACING_SUCCESSIVE  ((BYTE)0b00) // On successive byte boundaries
#define SMBIOS_IPMI_DEVICE_BASEADDRESS_REGISTER_SPACING_32BIT       ((BYTE)0b01) // On 32-bit boundaries
#define SMBIOS_IPMI_DEVICE_BASEADDRESS_REGISTER_SPACING_16BIT       ((BYTE)0b10) // On 16-bit boundaries
#define SMBIOS_IPMI_DEVICE_BASEADDRESS_REGISTER_SPACING_RESERVED    ((BYTE)0b11) // Reserved

typedef struct _SMBIOS_IPMI_DEVICE_INFORMATION
{
    SMBIOS_HEADER Header;
    BYTE Type; // SMBIOS_IPMI_DEVICE_INTERFACE_TYPE_*
    union
    {
        BYTE Value;
        struct
        {
            BYTE Minor : 4; // 0:3 Least significant bits
            BYTE Major : 4; // 4:7 Most significant digit
        };
    } SpecificationRevision;
    BYTE I2CTargetAddress;
    BYTE NVStorageDeviceAddress;
    union
    {
        QWORD Value;
        struct
        {
            QWORD IOSpace : 1;  // 00 1 The address is in I/O space, or 0 the address is memory-mapped
            QWORD Address : 63; // 01:63 Address
        };
    } BaseAddress;
    union
    {
        BYTE Value;
        struct
        {
            BYTE InterruptTriggerMode : 1;  // 0 1 = level, 0 = edge
            BYTE InterruptPolarity : 1;     // 1 1 = active high, 0 = active low
            BYTE Reserved0 : 1;             // 2 Reserved. Return as 0b
            BYTE InterruptInfo : 1;         // 3 1 = specified, 0 = not specified
            BYTE LSBAddress : 1;            // 4 LS-bit for addresses
            BYTE Reserved1 : 1;             // 5 Reserved. Return as 0b
            BYTE RegisterSpacing : 2;       // 6:7 SMBIOS_IPMI_DEVICE_BASEADDRESS_REGISTER_SPACING_*
        };
    } BaseAddressInfo;
} SMBIOS_IPMI_DEVICE_INFORMATION, *PSMBIOS_IPMI_DEVICE_INFORMATION, SMBIOS_TYPE_38, *PSMBIOS_TYPE_38;

#pragma endregion

#if SMBIOS_VERSION >= 0x02030100

#pragma region System Power Supply (Type 39)

#define SMBIOS_TYPE_SYSTEM_POWER_SUPPLY ((BYTE)39)

#define SMBIOS_SYSTEM_POWER_SUPPLY_INPUT_VOLTAGE_RANGE_SWITCHING_OTHER          ((WORD)0b0001) // Other
#define SMBIOS_SYSTEM_POWER_SUPPLY_INPUT_VOLTAGE_RANGE_SWITCHING_UNKNOWN        ((WORD)0b0010) // Unknown
#define SMBIOS_SYSTEM_POWER_SUPPLY_INPUT_VOLTAGE_RANGE_SWITCHING_MANUAL         ((WORD)0b0011) // Manual
#define SMBIOS_SYSTEM_POWER_SUPPLY_INPUT_VOLTAGE_RANGE_SWITCHING_AUTO_SWITCH    ((WORD)0b0100) // Auto-switch
#define SMBIOS_SYSTEM_POWER_SUPPLY_INPUT_VOLTAGE_RANGE_SWITCHING_WIDE_RANGE     ((WORD)0b0101) // Wide range
#define SMBIOS_SYSTEM_POWER_SUPPLY_INPUT_VOLTAGE_RANGE_SWITCHING_NOT_APPLICABLE ((WORD)0b0110) // Not applicable

#define SMBIOS_SYSTEM_POWER_SUPPLY_STATUS_OTHER         ((WORD)0b001) // Other
#define SMBIOS_SYSTEM_POWER_SUPPLY_STATUS_UNKNOWN       ((WORD)0b010) // Unknown
#define SMBIOS_SYSTEM_POWER_SUPPLY_STATUS_OK            ((WORD)0b011) // OK
#define SMBIOS_SYSTEM_POWER_SUPPLY_STATUS_NON_CRITICAL  ((WORD)0b100) // Non-critical
#define SMBIOS_SYSTEM_POWER_SUPPLY_STATUS_CRITICAL      ((WORD)0b101) // Critical; power supply has failed and has been taken off-line

#define SMBIOS_SYSTEM_POWER_SUPPLY_TYPE_OTHER       ((WORD)0b0001) // Other
#define SMBIOS_SYSTEM_POWER_SUPPLY_TYPE_UNKNOWN     ((WORD)0b0010) // Unknown
#define SMBIOS_SYSTEM_POWER_SUPPLY_TYPE_LINEAR      ((WORD)0b0011) // Linear
#define SMBIOS_SYSTEM_POWER_SUPPLY_TYPE_SWITCHING   ((WORD)0b0100) // Switching
#define SMBIOS_SYSTEM_POWER_SUPPLY_TYPE_BATTERY     ((WORD)0b0101) // Battery
#define SMBIOS_SYSTEM_POWER_SUPPLY_TYPE_UPS         ((WORD)0b0110) // UPS
#define SMBIOS_SYSTEM_POWER_SUPPLY_TYPE_CONVERTER   ((WORD)0b0111) // Converter
#define SMBIOS_SYSTEM_POWER_SUPPLY_TYPE_REGULATOR   ((WORD)0b1000) // Regulator

typedef struct _SMBIOS_SYSTEM_POWER_SUPPLY
{
    SMBIOS_HEADER Header;
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
        WORD Value;
        struct
        {
            WORD HotReplaceable : 1;                // 00 Power supply is hot-replaceable
            WORD Present : 1;                       // 01 Power supply is present 
            WORD UnpluggedFromTheWall : 1;          // 02 Power supply is unplugged from the wall
            WORD InputVoltageRangeSwitching : 4;    // 03:06 SMBIOS_SYSTEM_POWER_SUPPLY_INPUT_VOLTAGE_RANGE_SWITCHING_*
            WORD Status : 3;                        // 07:09 SMBIOS_SYSTEM_POWER_SUPPLY_STATUS_*
            WORD Type : 4;                          // 10:13 SMBIOS_SYSTEM_POWER_SUPPLY_TYPE_*
            WORD Reserved : 2;                      // 14:15 Reserved; set to 00b
        };
    } Characteristics;
    WORD InputVoltageProbeHandle;
    WORD CoolingDeviceHandle;
    WORD InputCurrentProbeHandle;
} SMBIOS_SYSTEM_POWER_SUPPLY, *PSMBIOS_SYSTEM_POWER_SUPPLY, SMBIOS_TYPE_39, *PSMBIOS_TYPE_39;

#pragma endregion

#if SMBIOS_VERSION >= 0x02060000

#pragma region Additional Information (Type 40)

#define SMBIOS_TYPE_ADDITIONAL_INFORMATION ((BYTE)40)

typedef struct _SMBIOS_ADDITIONAL_INFORMATION_ENTRY
{
    BYTE Length;
    WORD ReferencedHandle;
    BYTE ReferencedOffset;
    BYTE String;
    BYTE Value[1]; // Varies
} SMBIOS_ADDITIONAL_INFORMATION_ENTRY, *PSMBIOS_ADDITIONAL_INFORMATION_ENTRY;

typedef struct _SMBIOS_ADDITIONAL_INFORMATION
{
    SMBIOS_HEADER Header;
    BYTE Count;
    _Field_size_(Count) SMBIOS_ADDITIONAL_INFORMATION_ENTRY Entries[];
} SMBIOS_ADDITIONAL_INFORMATION, *PSMBIOS_ADDITIONAL_INFORMATION, SMBIOS_TYPE_40, *PSMBIOS_TYPE_40;

#pragma endregion

#pragma region Onboard Devices Extended Information (Type 41)

#define SMBIOS_TYPE_ONBOARD_DEVICES_EXTENDED_INFORMATION ((BYTE)41)

#define SMBIOS_ONBOARD_DEVICES_EXTENDED_TYPE_OTHER              ((BYTE)0x01) // Other
#define SMBIOS_ONBOARD_DEVICES_EXTENDED_TYPE_UNKNOWN            ((BYTE)0x02) // Unknown
#define SMBIOS_ONBOARD_DEVICES_EXTENDED_TYPE_VIDEO              ((BYTE)0x03) // Video
#define SMBIOS_ONBOARD_DEVICES_EXTENDED_TYPE_SCSI_CONTROLLER    ((BYTE)0x04) // SCSI Controller
#define SMBIOS_ONBOARD_DEVICES_EXTENDED_TYPE_ETHERNET           ((BYTE)0x05) // Ethernet
#define SMBIOS_ONBOARD_DEVICES_EXTENDED_TYPE_TOKEN_RING         ((BYTE)0x06) // Token Ring
#define SMBIOS_ONBOARD_DEVICES_EXTENDED_TYPE_SOUND              ((BYTE)0x07) // Sound
#define SMBIOS_ONBOARD_DEVICES_EXTENDED_TYPE_PATA_CONTROLLER    ((BYTE)0x08) // PATA Controller
#define SMBIOS_ONBOARD_DEVICES_EXTENDED_TYPE_SATA_CONTROLLER    ((BYTE)0x09) // SATA Controller
#define SMBIOS_ONBOARD_DEVICES_EXTENDED_TYPE_SAS_CONTROLLER     ((BYTE)0x0A) // SAS Controller
#define SMBIOS_ONBOARD_DEVICES_EXTENDED_TYPE_WIRELESS_LAN       ((BYTE)0x0B) // Wireless LAN
#define SMBIOS_ONBOARD_DEVICES_EXTENDED_TYPE_BLUETOOTH          ((BYTE)0x0C) // Bluetooth
#define SMBIOS_ONBOARD_DEVICES_EXTENDED_TYPE_WWAN               ((BYTE)0x0D) // WWAN
#define SMBIOS_ONBOARD_DEVICES_EXTENDED_TYPE_EMMC               ((BYTE)0x0E) // eMMC (embedded Multi-Media Controller)
#define SMBIOS_ONBOARD_DEVICES_EXTENDED_TYPE_NVME_CONTROLLER    ((BYTE)0x0F) // NVMe Controller
#define SMBIOS_ONBOARD_DEVICES_EXTENDED_TYPE_UFS_CONTROLLER     ((BYTE)0x10) // UFS Controller

typedef struct _SMBIOS_ONBOARD_DEVICES_EXTENDED_INFORMATION
{
    SMBIOS_HEADER Header;
    BYTE ReferenceDesignation;
    union
    {
        BYTE Value;
        struct
        {
            BYTE DeviceType : 7;    // 0:6 SMBIOS_ONBOARD_DEVICES_EXTENDED_TYPE_*
            BYTE Status : 1;        // 7 1 – Device Enabled, 0 – Device Disabled
        };
    } Type;
    BYTE TypeInstance;
    WORD SegmentGroupNumber;
    BYTE BusNumber;
    union
    {
        BYTE Value;
        struct
        {
            BYTE FunctionNumber : 3;    // 0：2 Function number
            BYTE DeviceNumber : 5;      // 3：7 Device number
        };
    } DeviceFunctionNumber;
} SMBIOS_ONBOARD_DEVICES_EXTENDED_INFORMATION, *PSMBIOS_ONBOARD_DEVICES_EXTENDED_INFORMATION, SMBIOS_TYPE_41, *PSMBIOS_TYPE_41;

#pragma endregion

#pragma region Management Controller Host Interface (Type 42)

#define SMBIOS_TYPE_MANAGEMENT_CONTROLLER_HOST_INTERFACE ((BYTE)42)

#define SMBIOS_MANAGEMENT_CONTROLLER_HOST_INTERFACE_PROTOCOL_TYPE_RESERVED_0        ((BYTE)0x00) // Reserved
#define SMBIOS_MANAGEMENT_CONTROLLER_HOST_INTERFACE_PROTOCOL_TYPE_RESERVED_1        ((BYTE)0x01) // Reserved
#define SMBIOS_MANAGEMENT_CONTROLLER_HOST_INTERFACE_PROTOCOL_TYPE_IPMI              ((BYTE)0x02) // IPMI
#define SMBIOS_MANAGEMENT_CONTROLLER_HOST_INTERFACE_PROTOCOL_TYPE_MCTP              ((BYTE)0x03) // MCTP
#define SMBIOS_MANAGEMENT_CONTROLLER_HOST_INTERFACE_PROTOCOL_TYPE_REDFISH_OVER_IP   ((BYTE)0x04) // Redfish over IP
#define SMBIOS_MANAGEMENT_CONTROLLER_HOST_INTERFACE_PROTOCOL_TYPE_OEM_DEFINED       ((BYTE)0xF0) // OEM-defined

typedef struct _SMBIOS_MANAGEMENT_CONTROLLER_HOST_INTERFACE_PROTOCOL_RECORD
{
    BYTE Type; // SMBIOS_MANAGEMENT_CONTROLLER_HOST_INTERFACE_PROTOCOL_TYPE_*
    BYTE TypeSpecificDataLength;
    _Field_size_bytes_(TypeSpecificDataLength) BYTE TypeSpecificData[];
} SMBIOS_MANAGEMENT_CONTROLLER_HOST_INTERFACE_PROTOCOL_RECORD, *PSMBIOS_MANAGEMENT_CONTROLLER_HOST_INTERFACE_PROTOCOL_RECORD;

typedef struct _SMBIOS_MANAGEMENT_CONTROLLER_HOST_INTERFACE
{
    SMBIOS_HEADER Header;
    BYTE Type; // 00h–3Fh MCTP Host Interfaces, 40h Network Host Interface, F0h OEM-defined, others Reserved
    BYTE TypeSpecificDataLength;
    _Field_size_bytes_(TypeSpecificDataLength) BYTE TypeSpecificData[];
/*
    BYTE NumberOfProtocolRecords;
    SMBIOS_MANAGEMENT_CONTROLLER_HOST_INTERFACE_PROTOCOL_RECORD ProtocolRecords[];
*/
} SMBIOS_MANAGEMENT_CONTROLLER_HOST_INTERFACE, *PSMBIOS_MANAGEMENT_CONTROLLER_HOST_INTERFACE, SMBIOS_TYPE_42, *PSMBIOS_TYPE_42;

#pragma endregion

#pragma region TPM Device (Type 43)

#define SMBIOS_TYPE_TPM_DEVICE ((BYTE)43)

typedef struct _SMBIOS_TPM_DEVICE
{
    SMBIOS_HEADER Header;
    BYTE VendorID[4];
    BYTE MajorSpecVersion;
    BYTE MinorSpecVersion;
    DWORD FirmwareVersion1;
    DWORD FirmwareVersion2;
    BYTE Description;
    union
    {
        QWORD Value;
        struct
        {
            QWORD Reserved0 : 1;                                    // 00 Reserved
            QWORD Reserved1 : 1;                                    // 01 Reserved
            QWORD NotSupported : 1;                                 // 02 Characteristics are not supported
            QWORD FamilyConfigurableViaFirmwareUpdate : 1;          // 03 Family configurable via firmware update
            QWORD FamilyConfigurableViaPlatformSoftwareSupport : 1; // 04 Family configurable via platform software support
            QWORD FamilyConfigurableViaOEMProprietaryMechanism : 1; // 05 Family configurable via OEM proprietary mechanism
            QWORD Reserved2 : 58;                                   // 06:63 Reserved
        };
    } Characteristics;
    DWORD OEMDefined;
} SMBIOS_TPM_DEVICE, *PSMBIOS_TPM_DEVICE, SMBIOS_TYPE_43, *PSMBIOS_TYPE_43;

#pragma endregion

#pragma region Processor Additional Information (Type 44)

#define SMBIOS_TYPE_PROCESSOR_ADDITIONAL_INFORMATION ((BYTE)44)

#define SMBIOS_PROCESSOR_ARCHITECTURE_TYPE_RESERVED     ((BYTE)0x00) // Reserved
#define SMBIOS_PROCESSOR_ARCHITECTURE_TYPE_X86          ((BYTE)0x01) // IA32 (x86)
#define SMBIOS_PROCESSOR_ARCHITECTURE_TYPE_X64          ((BYTE)0x02) // x64 (x86-64, Intel64, AMD64, EM64T)
#define SMBIOS_PROCESSOR_ARCHITECTURE_TYPE_IA64         ((BYTE)0x03) // Intel® Itanium® architecture
#define SMBIOS_PROCESSOR_ARCHITECTURE_TYPE_ARM32        ((BYTE)0x04) // 32-bit ARM (Aarch32)
#define SMBIOS_PROCESSOR_ARCHITECTURE_TYPE_ARM64        ((BYTE)0x05) // 64-bit ARM (Aarch64)
#define SMBIOS_PROCESSOR_ARCHITECTURE_TYPE_RISCV32      ((BYTE)0x06) // 32-bit RISC-V (RV32) 
#define SMBIOS_PROCESSOR_ARCHITECTURE_TYPE_RISCV64      ((BYTE)0x07) // 64-bit RISC-V (RV64)
#define SMBIOS_PROCESSOR_ARCHITECTURE_TYPE_RISCV128     ((BYTE)0x08) // 128-bit RISC-V (RV128)
#define SMBIOS_PROCESSOR_ARCHITECTURE_TYPE_LOONGARCH32  ((BYTE)0x09) // 32-bit LoongArch (LoongArch32)
#define SMBIOS_PROCESSOR_ARCHITECTURE_TYPE_LOONGARCH64  ((BYTE)0x0A) // 64-bit LoongArch (LoongArch64)

typedef struct _SMBIOS_PROCESSOR_SPECIFIC_BLOCK
{
    BYTE Length;
    BYTE Type; // SMBIOS_PROCESSOR_ARCHITECTURE_TYPE_*
    _Field_size_bytes_(Length) BYTE Data[1]; // Varies
} SMBIOS_PROCESSOR_SPECIFIC_BLOCK, *PSMBIOS_PROCESSOR_SPECIFIC_BLOCK;

typedef struct _SMBIOS_PROCESSOR_ADDITIONAL_INFORMATION
{
    SMBIOS_HEADER Header;
    WORD ReferencedHandle;
    SMBIOS_PROCESSOR_SPECIFIC_BLOCK ProcessorSpecificBlock[];
} SMBIOS_PROCESSOR_ADDITIONAL_INFORMATION, *PSMBIOS_PROCESSOR_ADDITIONAL_INFORMATION, SMBIOS_TYPE_44, *PSMBIOS_TYPE_44;

#pragma endregion

#if SMBIOS_VERSION >= 0x03050000

#pragma region Firmware Inventory Information (Type 45)

#define SMBIOS_TYPE_FIRMWARE_INVENTORY_INFORMATION ((BYTE)45)

#define SMBIOS_FIRMWARE_INVENTORY_VERSION_FORMAT_FREE_FORM      ((BYTE)0x00) // The format is a free-form string
#define SMBIOS_FIRMWARE_INVENTORY_VERSION_FORMAT_MAJOR_MINOR    ((BYTE)0x01) // The format is "MAJOR.MINOR"
#define SMBIOS_FIRMWARE_INVENTORY_VERSION_FORMAT_HEX_STRING_32  ((BYTE)0x02) // The format is a C-style hexadecimal string representation of the 32-bit numeric value
#define SMBIOS_FIRMWARE_INVENTORY_VERSION_FORMAT_HEX_STRING_64  ((BYTE)0x03) // The format is a C-style hexadecimal string representation of the 64-bit numeric value

#define SMBIOS_FIRMWARE_INVENTORY_ID_FORMAT_FREE_FORM   ((BYTE)0x00) // The format is a free-form string
#define SMBIOS_FIRMWARE_INVENTORY_ID_FORMAT_UEFI_GUID   ((BYTE)0x01) // The format is a string representation of the UEFI ESRT FwClass GUID or the UEFI Firmware Management Protocol ImageTypeId

#define SMBIOS_FIRMWARE_INVENTORY_STATE_OTHER               ((BYTE)0x01) // Other
#define SMBIOS_FIRMWARE_INVENTORY_STATE_UNKNOWN             ((BYTE)0x02) // Unknown
#define SMBIOS_FIRMWARE_INVENTORY_STATE_DISABLED            ((BYTE)0x03) // Disabled
#define SMBIOS_FIRMWARE_INVENTORY_STATE_ENABLED             ((BYTE)0x04) // Enabled
#define SMBIOS_FIRMWARE_INVENTORY_STATE_ABSENT              ((BYTE)0x05) // Absent
#define SMBIOS_FIRMWARE_INVENTORY_STATE_STANDBY_OFFLINE     ((BYTE)0x06) // StandbyOffline
#define SMBIOS_FIRMWARE_INVENTORY_STATE_STANDBY_SPARE       ((BYTE)0x07) // StandbySpare
#define SMBIOS_FIRMWARE_INVENTORY_STATE_UNAVAILABLE_OFFLINE ((BYTE)0x08) // UnavailableOffline

typedef struct _SMBIOS_FIRMWARE_INVENTORY_INFORMATION
{
    SMBIOS_HEADER Header;
    BYTE ComponentName;
    BYTE Version;
    BYTE VersionFormat;                         // SMBIOS_FIRMWARE_INVENTORY_VERSION_FORMAT_*, 04h–7Fh Available for future assignment by this specification, 80h-FFh Firmware Vendor/OEM-specific
    BYTE ID;
    BYTE IDFormat;                              // SMBIOS_FIRMWARE_INVENTORY_ID_FORMAT_*, 02h–7Fh Available for future assignment by this specification, 80h-FFh Firmware Vendor/OEM-specific
    BYTE ReleaseDate;
    BYTE Manufacturer;
    BYTE LowestSupportedFirmwareVersion;  // SMBIOS_FIRMWARE_INVENTORY_VERSION_FORMAT_*
    QWORD ImageSize;
    union
    {
        WORD Value;
        struct
        {
            WORD Updatable : 1;     // 00 Updatable
            WORD WriteProtect : 1;  // 01 Write-Protect
            WORD Reserved : 14;     // 02:15 Reserved
        };
    } Characteristics;
    BYTE State; // SMBIOS_FIRMWARE_INVENTORY_STATE_*
    BYTE NumberOfAssociatedComponents;
    _Field_size_(NumberOfAssociatedComponents) WORD AssociatedComponentHandles[];
} SMBIOS_FIRMWARE_INVENTORY_INFORMATION, *PSMBIOS_FIRMWARE_INVENTORY_INFORMATION, SMBIOS_TYPE_45, *PSMBIOS_TYPE_45;

#pragma endregion

#pragma region String Property (Type 46)

#define SMBIOS_TYPE_STRING_PROPERTY ((BYTE)46)

#define SMBIOS_STRING_PROPERTY_ID_RESERVED          ((WORD)0) // Reserved – do not use
#define SMBIOS_STRING_PROPERTY_ID_UEFI_DEVICE_PATH  ((WORD)1) // UEFI device path

typedef struct _SMBIOS_STRING_PROPERTY
{
    SMBIOS_HEADER Header;
    WORD ID; // SMBIOS_STRING_PROPERTY_ID_*
    BYTE String;
    WORD ParentHandle;
} SMBIOS_STRING_PROPERTY, *PSMBIOS_STRING_PROPERTY, SMBIOS_TYPE_46, *PSMBIOS_TYPE_46;

#pragma endregion

#endif // SMBIOS_VERSION >= 0x03050000

#endif // SMBIOS_VERSION >= 0x02060000

#endif // SMBIOS_VERSION >= 0x02030100

#endif // SMBIOS_VERSION >= 0x02030000

#pragma region Inactive (Type 126)

#define SMBIOS_TYPE_INACTIVE ((BYTE)126)

#pragma endregion Inactive

#pragma region End-of-Table (Type 127)

#define SMBIOS_TYPE_END_OF_TABLE ((BYTE)127)

#pragma endregion End-of-Table

#endif // SMBIOS_VERSION >= 0x02020000

#endif // SMBIOS_VERSION >= 0x02010000

typedef union _SMBIOS_TABLE
{
    SMBIOS_HEADER Header;
    SMBIOS_TYPE_0 PlatformFirmwareInformation;
    SMBIOS_TYPE_1 SystemInformation;
    SMBIOS_TYPE_2 BaseboardInformation;
    SMBIOS_TYPE_3  SystemEnclosureOrChassis;
    SMBIOS_TYPE_4 ProcessorInformation;
    SMBIOS_TYPE_5 MemoryControllerInformation; // Obsolete
    SMBIOS_TYPE_6 MemoryModuleInformation; // Obsolete
    SMBIOS_TYPE_7 CacheInformation;
    SMBIOS_TYPE_8 PortConnectorInformation;
    SMBIOS_TYPE_9 SystemSlots;
    SMBIOS_TYPE_10 OnBoardDevices; // Obsolete
    SMBIOS_TYPE_11 OEMStrings;
    SMBIOS_TYPE_12 SystemConfigurationOptions;
    SMBIOS_TYPE_13 FirmwareLanguageInformation;
    SMBIOS_TYPE_14 GroupAssociations;
    SMBIOS_TYPE_15 SystemEventLog; // WIP
#if SMBIOS_VERSION >= 0x02010000
    SMBIOS_TYPE_16 PhysicalMemoryArray;
    SMBIOS_TYPE_17 MemoryDevice;
    SMBIOS_TYPE_18 _32BitMemoryErrorInformation;
    SMBIOS_TYPE_19 MemoryArrayMappedAddress;
    SMBIOS_TYPE_20 MemoryDeviceMappedAddress;
    SMBIOS_TYPE_21 BuiltinPointingDevice;
    SMBIOS_TYPE_22 PortableBattery;
#if SMBIOS_VERSION >= 0x02020000
    SMBIOS_TYPE_23 SystemReset;
    SMBIOS_TYPE_24 HardwareSecurity;
    SMBIOS_TYPE_25 SystemPowerControls;
    SMBIOS_TYPE_26 VoltageProbe;
    SMBIOS_TYPE_27 CoolingDevice;
    SMBIOS_TYPE_28 TemperatureProbe;
    SMBIOS_TYPE_29 ElectricalCurrentProbe;
    SMBIOS_TYPE_30 OutOfBandRemoteAccess;
#if SMBIOS_VERSION >= 0x02030000
    SMBIOS_TYPE_32 SystemBootInformation;
    SMBIOS_TYPE_33 _64BitMemoryErrorInformation;
    SMBIOS_TYPE_34 ManagementDevice;
    SMBIOS_TYPE_35 ManagementDeviceComponent;
    SMBIOS_TYPE_36 ManagementDeviceThresholdData;
    SMBIOS_TYPE_37 MemoryChannel;
    SMBIOS_TYPE_38 IPMIDeviceInformation;
#if SMBIOS_VERSION >= 0x02030100
    SMBIOS_TYPE_39 SystemPowerSupply;
#if SMBIOS_VERSION >= 0x02060000
    SMBIOS_TYPE_40 AdditionalInformation;
    SMBIOS_TYPE_41 OnboardDevicesExtendedInformation;
    SMBIOS_TYPE_42 ManagementControllerHostInterface;
    SMBIOS_TYPE_43 TPMDevice;
    SMBIOS_TYPE_44 ProcessorAdditionalInformation;
#if SMBIOS_VERSION >= 0x03050000
    SMBIOS_TYPE_45 FirmwareInventoryInformation;
    SMBIOS_TYPE_46 StringProperty;
#endif // SMBIOS_VERSION >= 0x03050000
#endif // SMBIOS_VERSION >= 0x02060000
#endif // SMBIOS_VERSION >= 0x02030100
#endif // SMBIOS_VERSION >= 0x02030000
#endif // SMBIOS_VERSION >= 0x02020000
#endif // SMBIOS_VERSION >= 0x02010000
} SMBIOS_TABLE, *PSMBIOS_TABLE;

#pragma pack(pop)

#pragma endregion

#pragma region SMBIOS Type Data

#if !defined(SMBIOS_NO_TYPE_DATA)

typedef enum _SMBIOS_DATA_TYPE
{
    SmbiosDataTypeOther,
    SmbiosDataTypeString,
    SmbiosDataTypeUInt,
    SmbiosDataTypeBit,
    SmbiosDataTypeEnum,
    SmbiosDataTypeUuid,
} SMBIOS_DATA_TYPE, *PSMBIOS_DATA_TYPE;

typedef struct _SMBIOS_FIELD_ENUM
{
    const char* Name;
    QWORD Value;
} SMBIOS_FIELD_ENUM, *PSMBIOS_FIELD_ENUM;

typedef struct _SMBIOS_FIELD_TYPE_INFO
{
    const char* Name;
    BYTE IsBitField;
    WORD Offset;
    BYTE Size;
    SMBIOS_DATA_TYPE Type;
    union
    {
        struct
        {
            WORD Count;
            _Field_size_(Count) PSMBIOS_FIELD_ENUM Values;
        } Enum;
    } AdditionalInfo;
} SMBIOS_FIELD_TYPE_INFO, *PSMBIOS_FIELD_TYPE_INFO;

typedef struct _SMBIOS_TYPE_INFO
{
    BYTE Type;          // Type number in SMBIOS spec.
    const char* Name;   // Type name in SMBIOS spec.
    WORD FieldCount;    // Number of Fields
    _Field_size_(FieldCount) PSMBIOS_FIELD_TYPE_INFO Fields;  // Field information in type structure
} SMBIOS_TYPE_INFO, *PSMBIOS_TYPE_INFO;

#define SMBIOS_DEFINE_FIELD(Type, Name, Field, FieldType, ...) { Name, false, offsetof(SMBIOS_TYPE_##Type, Field), sizeof(((SMBIOS_TYPE_##Type*)0)->Field), FieldType, __VA_ARGS__ }
#define SMBIOS_DEFINE_BIT_FIELD(Name, BitOffset, BitSize, FieldType, ...) { Name, true, BitOffset, BitSize, FieldType, __VA_ARGS__ }
#define SMBIOS_FIELD_ENUM_VALUES(EnumName) { _countof(EnumName), EnumName }

#define SMBIOS_DEFINE_FIELD_STRING(Type, Name, Field) SMBIOS_DEFINE_FIELD(Type, Name, Field, SmbiosDataTypeString)
#define SMBIOS_DEFINE_FIELD_UINT(Type, Name, Field) SMBIOS_DEFINE_FIELD(Type, Name, Field, SmbiosDataTypeUInt)
#define SMBIOS_DEFINE_FIELD_ENUM(Type, Name, Field, EnumName) SMBIOS_DEFINE_FIELD(Type, Name, Field, SmbiosDataTypeEnum, SMBIOS_FIELD_ENUM_VALUES(EnumName))
#define SMBIOS_DEFINE_FIELD_BIT(Name, BitOffset) SMBIOS_DEFINE_BIT_FIELD(Name, BitOffset, 1, SmbiosDataTypeBit)

#define SMBIOS_DEFINE_TYPE(Type, Name) { Type, Name, _countof(SmbiosType##Type##FieldInfo), SmbiosType##Type##FieldInfo }

__declspec(selectany)
SMBIOS_FIELD_ENUM SmbiosType0ExtendedROMSizeUnitEnum[] = {
    { "MB", SMBIOS_PLATFORM_FIRMWARE_EXTENDEDROMSIZE_UNIT_MB },
    { "GB", SMBIOS_PLATFORM_FIRMWARE_EXTENDEDROMSIZE_UNIT_GB },
};

__declspec(selectany)
SMBIOS_FIELD_TYPE_INFO SmbiosType0FieldInfo[] = {
    SMBIOS_DEFINE_FIELD_STRING(0, "Vendor", Vendor),
    SMBIOS_DEFINE_FIELD_STRING(0, "Firmware Version", Version),
    SMBIOS_DEFINE_FIELD_UINT(0, "BIOS Starting Address Segment", BIOSStartingAddressSegment),
    SMBIOS_DEFINE_FIELD_STRING(0, "Firmware Release Date", ReleaseDate),
    SMBIOS_DEFINE_FIELD_UINT(0, "Firmware ROM Size", ROMSize),
    SMBIOS_DEFINE_FIELD_UINT(0, "Firmware Characteristics", Characteristics),
    SMBIOS_DEFINE_FIELD_BIT("Reserved", 0),
    SMBIOS_DEFINE_FIELD_BIT("Reserved", 1),
    SMBIOS_DEFINE_FIELD_BIT("Unknown", 2),
    SMBIOS_DEFINE_FIELD_BIT("Firmware Characteristics are not supported", 3),
    SMBIOS_DEFINE_FIELD_BIT("ISA is supported", 4),
    SMBIOS_DEFINE_FIELD_BIT("MCA is supported", 5),
    SMBIOS_DEFINE_FIELD_BIT("EISA is supported", 6),
    SMBIOS_DEFINE_FIELD_BIT("PCI is supported", 7),
    SMBIOS_DEFINE_FIELD_BIT("PC card (PCMCIA) is supported", 8),
    SMBIOS_DEFINE_FIELD_BIT("Plug and Play is supported", 9),
    SMBIOS_DEFINE_FIELD_BIT("APM is supported", 10),
    SMBIOS_DEFINE_FIELD_BIT("Firmware is upgradeable (Flash)", 11),
    SMBIOS_DEFINE_FIELD_BIT("Firmware shadowing is allowed", 12),
    SMBIOS_DEFINE_FIELD_BIT("VL-VESA is supported", 13),
    SMBIOS_DEFINE_FIELD_BIT("ESCD support is available", 14),
    SMBIOS_DEFINE_FIELD_BIT("Boot from CD is supported", 15),
    SMBIOS_DEFINE_FIELD_BIT("Selectable boot is supported", 16),
    SMBIOS_DEFINE_FIELD_BIT("Firmware ROM is socketed (e.g., PLCC or SOP socket)", 17),
    SMBIOS_DEFINE_FIELD_BIT("Boot from PC card (PCMCIA) is supported", 18),
    SMBIOS_DEFINE_FIELD_BIT("EDD specification is supported", 19),
    SMBIOS_DEFINE_FIELD_BIT("Int 13h — Japanese floppy for NEC 9800 1.2 MB (3.5\", 1K bytes/sector, 360 RPM) is supported", 20),
    SMBIOS_DEFINE_FIELD_BIT("Int 13h — Japanese floppy for Toshiba 1.2 MB (3.5\", 360 RPM) is supported", 21),
    SMBIOS_DEFINE_FIELD_BIT("Int 13h — 5.25\" / 360 KB floppy services are supported", 22),
    SMBIOS_DEFINE_FIELD_BIT("Int 13h — 5.25\" / 1.2 MB floppy services are supported", 23),
    SMBIOS_DEFINE_FIELD_BIT("Int 13h — 3.5\" / 720 KB floppy services are supported", 24),
    SMBIOS_DEFINE_FIELD_BIT("Int 13h — 3.5\" / 2.88 MB floppy services are supported", 25),
    SMBIOS_DEFINE_FIELD_BIT("Int 5h, print screen service is supported", 26),
    SMBIOS_DEFINE_FIELD_BIT("Int 9h, 8042 keyboard services are supported", 27),
    SMBIOS_DEFINE_FIELD_BIT("Int 14h, serial services are supported", 28),
    SMBIOS_DEFINE_FIELD_BIT("Int 17h, printer services are supported", 29),
    SMBIOS_DEFINE_FIELD_BIT("Int 10h, CGA/Mono Video Services are supported", 30),
    SMBIOS_DEFINE_FIELD_BIT("NEC PC-98", 31),
    SMBIOS_DEFINE_FIELD_UINT(0, "Firmware Characteristics Extension Byte 1", CharacteristicsExtensionByte1),
    SMBIOS_DEFINE_FIELD_BIT("ACPI is supported", 0),
    SMBIOS_DEFINE_FIELD_BIT("USB Legacy is supported", 1),
    SMBIOS_DEFINE_FIELD_BIT("AGP is supported", 2),
    SMBIOS_DEFINE_FIELD_BIT("I2O boot is supported", 3),
    SMBIOS_DEFINE_FIELD_BIT("LS-120 SuperDisk boot is supported", 4),
    SMBIOS_DEFINE_FIELD_BIT("ATAPI ZIP drive boot is supported", 5),
    SMBIOS_DEFINE_FIELD_BIT("1394 boot is supported", 6),
    SMBIOS_DEFINE_FIELD_BIT("Smart battery is supported", 7),
    SMBIOS_DEFINE_FIELD_UINT(0, "Firmware Characteristics Extension Byte 2", CharacteristicsExtensionByte2),
    SMBIOS_DEFINE_FIELD_BIT("BIOS Boot Specification is supported", 0),
    SMBIOS_DEFINE_FIELD_BIT("Function key-initiated network service boot is supported", 1),
    SMBIOS_DEFINE_FIELD_BIT("Enable targeted content distribution", 2),
    SMBIOS_DEFINE_FIELD_BIT("UEFI Specification is supported", 3),
    SMBIOS_DEFINE_FIELD_BIT("SMBIOS table describes a virtual machine", 4),
    SMBIOS_DEFINE_FIELD_BIT("Manufacturing mode is supported", 5),
    SMBIOS_DEFINE_FIELD_BIT("Manufacturing mode is enabled", 6),
    SMBIOS_DEFINE_FIELD_BIT("Reserved", 7),
    SMBIOS_DEFINE_FIELD_UINT(0, "Platform Firmware Major Release", MajorRelease),
    SMBIOS_DEFINE_FIELD_UINT(0, "Platform Firmware Minor Release", MinorRelease),
    SMBIOS_DEFINE_FIELD_UINT(0, "Embedded Controller Firmware Major Release", ECFirmwareMajorRelease),
    SMBIOS_DEFINE_FIELD_UINT(0, "Embedded Controller Firmware Minor Release", ECFirmwareMinorRelease),
    SMBIOS_DEFINE_FIELD_UINT(0, "Extended Firmware ROM Size", ExtendedROMSize.Value),
    SMBIOS_DEFINE_BIT_FIELD("Size", 0, 14, SmbiosDataTypeUInt),
    SMBIOS_DEFINE_BIT_FIELD("Unit", 14, 2, SmbiosDataTypeEnum, SMBIOS_FIELD_ENUM_VALUES(SmbiosType0ExtendedROMSizeUnitEnum)),
};

__declspec(selectany)
SMBIOS_FIELD_ENUM SmbiosType1WakeUpTypeEnum[] = {
    { "Reserved", SMBIOS_SYSTEM_WAKEUPTYPE_RESERVED },
    { "Other", SMBIOS_SYSTEM_WAKEUPTYPE_OTHER },
    { "Unknown", SMBIOS_SYSTEM_WAKEUPTYPE_UNKNOW },
    { "APM Timer", SMBIOS_SYSTEM_WAKEUPTYPE_APM_TIMER },
    { "Modem Ring", SMBIOS_SYSTEM_WAKEUPTYPE_MODEM_RING },
    { "LAN Remote", SMBIOS_SYSTEM_WAKEUPTYPE_LAN_REMOTE },
    { "Power Switch", SMBIOS_SYSTEM_WAKEUPTYPE_POWER_SWITCH },
    { "PCI PME#", SMBIOS_SYSTEM_WAKEUPTYPE_PCI_PME },
    { "AC Power Restored", SMBIOS_SYSTEM_WAKEUPTYPE_AC_POWER_RESTORED },
};

__declspec(selectany)
SMBIOS_FIELD_TYPE_INFO SmbiosType1FieldInfo[] = {
    SMBIOS_DEFINE_FIELD_STRING(1, "Manufacturer", Manufacturer),
    SMBIOS_DEFINE_FIELD_STRING(1, "Product Name", ProductName),
    SMBIOS_DEFINE_FIELD_STRING(1, "Version", Version),
    SMBIOS_DEFINE_FIELD_STRING(1, "Serial Number", SerialNumber),
    SMBIOS_DEFINE_FIELD(1, "UUID", UUID, SmbiosDataTypeUuid),
    SMBIOS_DEFINE_FIELD_ENUM(1, "Wake-up Type", WakeUpType, SmbiosType1WakeUpTypeEnum),
    SMBIOS_DEFINE_FIELD_STRING(1, "SKU Number", SKUNumber),
    SMBIOS_DEFINE_FIELD_STRING(1, "Family", Family),
};

__declspec(selectany)
SMBIOS_FIELD_ENUM SmbiosType2BoardTypeEnum[] = {
    { "Unknown", SMBIOS_BASEBOARD_TYPE_UNKNOWN },
    { "Other", SMBIOS_BASEBOARD_TYPE_OTHER },
    { "Server Blade", SMBIOS_BASEBOARD_TYPE_SERVER_BLADE },
    { "Connectivity Switch", SMBIOS_BASEBOARD_TYPE_CONNECTIVITY_SWITCH },
    { "System Management Module", SMBIOS_BASEBOARD_TYPE_SYSTEM_MANAGEMENT_MODULE },
    { "Processor Module", SMBIOS_BASEBOARD_TYPE_PROCESSOR_MODULE },
    { "I/O Module", SMBIOS_BASEBOARD_TYPE_IO_MODULE },
    { "Memory Module", SMBIOS_BASEBOARD_TYPE_MEMORY_MODULE },
    { "Daughter board", SMBIOS_BASEBOARD_TYPE_DAUGHTER_BOARD },
    { "Motherboard (includes processor, memory, and I/O)", SMBIOS_BASEBOARD_TYPE_MOTHERBOARD },
    { "Processor/Memory Module", SMBIOS_BASEBOARD_TYPE_PROCESSOR_MEMORY_MODULE },
    { "Processor/IO Module", SMBIOS_BASEBOARD_TYPE_PROCESSOR_IO_MODULE },
    { "Interconnect board", SMBIOS_BASEBOARD_TYPE_INTERCONNECT_BOARD },
};

__declspec(selectany)
SMBIOS_FIELD_TYPE_INFO SmbiosType2FieldInfo[] = {
    SMBIOS_DEFINE_FIELD_STRING(2, "Manufacturer", Manufacturer),
    SMBIOS_DEFINE_FIELD_STRING(2, "Product", Product),
    SMBIOS_DEFINE_FIELD_STRING(2, "Version", Version),
    SMBIOS_DEFINE_FIELD_STRING(2, "Serial Number", SerialNumber),
    SMBIOS_DEFINE_FIELD_STRING(2, "Asset Tag", AssetTag),
    SMBIOS_DEFINE_FIELD(2, "Feature Flags", FeatureFlags, SmbiosDataTypeUInt),
    SMBIOS_DEFINE_FIELD_BIT("The board is a hosting board (for example, a motherboard)", 0),
    SMBIOS_DEFINE_FIELD_BIT("The board requires at least one daughter board or auxiliary card to function properly", 1),
    SMBIOS_DEFINE_FIELD_BIT("The board is removable", 2),
    SMBIOS_DEFINE_FIELD_BIT("The board is replaceable", 3),
    SMBIOS_DEFINE_FIELD_BIT("The board is s hot swappable", 4),
    SMBIOS_DEFINE_FIELD_STRING(2, "Location in Chassis", LocationInChassis),
    SMBIOS_DEFINE_FIELD(2, "Chassis Handle", ChassisHandle, SmbiosDataTypeUInt),
    SMBIOS_DEFINE_FIELD_ENUM(2, "Board Type", BoardType, SmbiosType2BoardTypeEnum),
    SMBIOS_DEFINE_FIELD(2, "Number of Contained Object Handles", NumberOfContainedObjectHandles, SmbiosDataTypeUInt),
};

__declspec(selectany)
SMBIOS_FIELD_ENUM SmbiosType3TypeEnum[] = {
    { "Other", SMBIOS_SYSTEM_ENCLOSURE_OR_CHASSIS_TYPE_OTHER },
    { "Unknown", SMBIOS_SYSTEM_ENCLOSURE_OR_CHASSIS_TYPE_UNKNOWN },
    { "Desktop", SMBIOS_SYSTEM_ENCLOSURE_OR_CHASSIS_TYPE_DESKTOP },
    { "Low Profile Desktop", SMBIOS_SYSTEM_ENCLOSURE_OR_CHASSIS_TYPE_LOW_PROFILE_DESKTOP },
    { "Pizza Box", SMBIOS_SYSTEM_ENCLOSURE_OR_CHASSIS_TYPE_PIZZA_BOX },
    { "Mini Tower", SMBIOS_SYSTEM_ENCLOSURE_OR_CHASSIS_TYPE_MINI_TOWER },
    { "Tower", SMBIOS_SYSTEM_ENCLOSURE_OR_CHASSIS_TYPE_TOWER },
    { "Portable", SMBIOS_SYSTEM_ENCLOSURE_OR_CHASSIS_TYPE_PORTABLE },
    { "Laptop", SMBIOS_SYSTEM_ENCLOSURE_OR_CHASSIS_TYPE_LAPTOP },
    { "Notebook", SMBIOS_SYSTEM_ENCLOSURE_OR_CHASSIS_TYPE_NOTEBOOK },
    { "Hand Held", SMBIOS_SYSTEM_ENCLOSURE_OR_CHASSIS_TYPE_HAND_HELD },
    { "Docking Station", SMBIOS_SYSTEM_ENCLOSURE_OR_CHASSIS_TYPE_DOCKING_STATION },
    { "All in One", SMBIOS_SYSTEM_ENCLOSURE_OR_CHASSIS_TYPE_ALL_IN_ONE },
    { "Sub Notebook", SMBIOS_SYSTEM_ENCLOSURE_OR_CHASSIS_TYPE_SUB_NOTEBOOK },
    { "Space-saving", SMBIOS_SYSTEM_ENCLOSURE_OR_CHASSIS_TYPE_SPACE_SAVING },
    { "Lunch Box", SMBIOS_SYSTEM_ENCLOSURE_OR_CHASSIS_TYPE_LUNCH_BOX },
    { "Main Server Chassis", SMBIOS_SYSTEM_ENCLOSURE_OR_CHASSIS_TYPE_MAIN_SERVER_CHASSIS },
    { "Expansion Chassis", SMBIOS_SYSTEM_ENCLOSURE_OR_CHASSIS_TYPE_EXPANSION_CHASSIS },
    { "SubChassis", SMBIOS_SYSTEM_ENCLOSURE_OR_CHASSIS_TYPE_SUB_CHASSIS },
    { "Bus Expansion Chassis", SMBIOS_SYSTEM_ENCLOSURE_OR_CHASSIS_TYPE_BUS_EXPANSION_CHASSIS },
    { "Peripheral Chassis", SMBIOS_SYSTEM_ENCLOSURE_OR_CHASSIS_TYPE_PERIPHERAL_CHASSIS },
    { "RAID Chassis", SMBIOS_SYSTEM_ENCLOSURE_OR_CHASSIS_TYPE_RAID_CHASSIS },
    { "Rack Mount Chassis", SMBIOS_SYSTEM_ENCLOSURE_OR_CHASSIS_TYPE_RACK_MOUNT_CHASSIS },
    { "Sealed-case PC", SMBIOS_SYSTEM_ENCLOSURE_OR_CHASSIS_TYPE_SEALED_CASE_PC },
    { "Multi-system chassis", SMBIOS_SYSTEM_ENCLOSURE_OR_CHASSIS_TYPE_MULTI_SYSTEM_CHASSIS },
    { "Compact PCI", SMBIOS_SYSTEM_ENCLOSURE_OR_CHASSIS_TYPE_COMPACT_PCI },
    { "Advanced TCA", SMBIOS_SYSTEM_ENCLOSURE_OR_CHASSIS_TYPE_ADVANCED_TCA },
    { "Blade", SMBIOS_SYSTEM_ENCLOSURE_OR_CHASSIS_TYPE_BLADE },
    { "Blade Enclosure", SMBIOS_SYSTEM_ENCLOSURE_OR_CHASSIS_TYPE_BLADE_ENCLOSURE },
    { "Tablet", SMBIOS_SYSTEM_ENCLOSURE_OR_CHASSIS_TYPE_TABLET },
    { "Convertible", SMBIOS_SYSTEM_ENCLOSURE_OR_CHASSIS_TYPE_CONVERTIBLE },
    { "Detachable", SMBIOS_SYSTEM_ENCLOSURE_OR_CHASSIS_TYPE_DETACHABLE },
    { "IoT Gateway", SMBIOS_SYSTEM_ENCLOSURE_OR_CHASSIS_TYPE_IOT_GATEWAY },
    { "Embedded PC", SMBIOS_SYSTEM_ENCLOSURE_OR_CHASSIS_TYPE_EMBEDDED_PC },
    { "Mini PC", SMBIOS_SYSTEM_ENCLOSURE_OR_CHASSIS_TYPE_MINI_PC },
    { "Stick PC", SMBIOS_SYSTEM_ENCLOSURE_OR_CHASSIS_TYPE_STICK_PC }
};

__declspec(selectany)
SMBIOS_FIELD_ENUM SmbiosType3StateEnum[] = {
    { "Other", SMBIOS_SYSTEM_ENCLOSURE_OR_CHASSIS_STATE_OTHER },
    { "Unknown", SMBIOS_SYSTEM_ENCLOSURE_OR_CHASSIS_STATE_UNKNOWN },
    { "Safe", SMBIOS_SYSTEM_ENCLOSURE_OR_CHASSIS_STATE_SAFE },
    { "Warning", SMBIOS_SYSTEM_ENCLOSURE_OR_CHASSIS_STATE_WARNING },
    { "Critical", SMBIOS_SYSTEM_ENCLOSURE_OR_CHASSIS_STATE_CRITICAL },
    { "Non-recoverable", SMBIOS_SYSTEM_ENCLOSURE_OR_CHASSIS_STATE_NON_RECOVERABLE },
};

__declspec(selectany)
SMBIOS_FIELD_ENUM SmbiosType3SecurityStatusEnum[] = {
    { "Other", SMBIOS_SYSTEM_ENCLOSURE_OR_CHASSIS_SECURITY_STATE_OTHER },
    { "Unknown", SMBIOS_SYSTEM_ENCLOSURE_OR_CHASSIS_SECURITY_STATE_UNKNOWN },
    { "None", SMBIOS_SYSTEM_ENCLOSURE_OR_CHASSIS_SECURITY_STATE_NONE },
    { "External interface locked out", SMBIOS_SYSTEM_ENCLOSURE_OR_CHASSIS_SECURITY_STATE_EXTERNAL_INTERFACE_LOCKED_OUT },
    { "External interface enabled", SMBIOS_SYSTEM_ENCLOSURE_OR_CHASSIS_SECURITY_STATE_EXTERNAL_INTERFACE_ENABLED },
};

__declspec(selectany)
SMBIOS_FIELD_TYPE_INFO SmbiosType3FieldInfo[] = {
    SMBIOS_DEFINE_FIELD_STRING(3, "Manufacturer", Manufacturer),
    SMBIOS_DEFINE_FIELD_UINT(3, "Type", Type.Value),
    SMBIOS_DEFINE_BIT_FIELD("Type", 0, 7, SmbiosDataTypeEnum, SMBIOS_FIELD_ENUM_VALUES(SmbiosType3SecurityStatusEnum)),
    SMBIOS_DEFINE_FIELD_BIT("Chassis lock is present", 7),
    SMBIOS_DEFINE_FIELD_STRING(3, "Version", Version),
    SMBIOS_DEFINE_FIELD_STRING(3, "Serial Number", SerialNumber),
    SMBIOS_DEFINE_FIELD_STRING(3, "Asset Tag Number", AssetTagNumber),
    SMBIOS_DEFINE_FIELD_ENUM(3, "Boot-up State", BootUpState, SmbiosType3StateEnum),
    SMBIOS_DEFINE_FIELD_ENUM(3, "Power Supply State", PowerSupplyState, SmbiosType3StateEnum),
    SMBIOS_DEFINE_FIELD_ENUM(3, "Thermal State", ThermalState, SmbiosType3StateEnum),
    SMBIOS_DEFINE_FIELD_ENUM(3, "Security Status", SecurityStatus, SmbiosType3SecurityStatusEnum),
    SMBIOS_DEFINE_FIELD_UINT(3, "OEM-defined", OEMDefined),
    SMBIOS_DEFINE_FIELD_UINT(3, "Height", Height),
    SMBIOS_DEFINE_FIELD_UINT(3, "Number of Power Cords", NumberOfPowerCords),
    SMBIOS_DEFINE_FIELD_UINT(3, "Contained Element Count", ContainedElementCount),
    SMBIOS_DEFINE_FIELD_UINT(3, "Contained Element Record Length", ContainedElementRecordLength),
};

__declspec(selectany)
SMBIOS_FIELD_ENUM SmbiosType4TypeEnum[] = {
    { "Other", SMBIOS_PROCESSOR_TYPE_OTHER },
    { "Unknown", SMBIOS_PROCESSOR_TYPE_UNKNOWN },
    { "Central Processor", SMBIOS_PROCESSOR_TYPE_CENTRAL_PROCESSOR },
    { "Math Processor", SMBIOS_PROCESSOR_TYPE_MATH_PROCESSOR },
    { "DSP Processor", SMBIOS_PROCESSOR_TYPE_DSP_PROCESSOR },
    { "Video Processor", SMBIOS_PROCESSOR_TYPE_VIDEO_PROCESSOR },
};

__declspec(selectany)
SMBIOS_FIELD_ENUM SmbiosType4FamilyEnum[] = {
    { u8"Other", SMBIOS_PROCESSOR_FAMILY_OTHER },
    { u8"Unknown", SMBIOS_PROCESSOR_FAMILY_UNKNOWN },
    { u8"8086", SMBIOS_PROCESSOR_FAMILY_8086 },
    { u8"80286", SMBIOS_PROCESSOR_FAMILY_80286 },
    { u8"Intel386™ processor", SMBIOS_PROCESSOR_FAMILY_INTEL386_PROCESSOR },
    { u8"Intel486™ processor", SMBIOS_PROCESSOR_FAMILY_INTEL486_PROCESSOR },
    { u8"8087", SMBIOS_PROCESSOR_FAMILY_8087 },
    { u8"80287", SMBIOS_PROCESSOR_FAMILY_80287 },
    { u8"80387", SMBIOS_PROCESSOR_FAMILY_80387 },
    { u8"80487", SMBIOS_PROCESSOR_FAMILY_80487 },
    { u8"Intel® Pentium® processor", SMBIOS_PROCESSOR_FAMILY_INTEL_PENTIUM_PROCESSOR },
    { u8"Pentium® Pro processor", SMBIOS_PROCESSOR_FAMILY_PENTIUM_PRO_PROCESSOR },
    { u8"Pentium® II processor", SMBIOS_PROCESSOR_FAMILY_PENTIUM_II_PROCESSOR },
    { u8"Pentium® processor with MMX™ technology", SMBIOS_PROCESSOR_FAMILY_PENTIUM_PROCESSOR_WITH_MMX_TECHNOLOGY },
    { u8"Intel® Celeron® processor", SMBIOS_PROCESSOR_FAMILY_INTEL_CELERON_PROCESSOR },
    { u8"Pentium® II Xeon® processor", SMBIOS_PROCESSOR_FAMILY_PENTIUM_II_XEON_PROCESSOR },
    { u8"Pentium® III processor", SMBIOS_PROCESSOR_FAMILY_PENTIUM_III_PROCESSOR },
    { u8"M1 Family", SMBIOS_PROCESSOR_FAMILY_M1_FAMILY },
    { u8"M2 Family", SMBIOS_PROCESSOR_FAMILY_M2_FAMILY },
    { u8"Intel® Celeron® M processor", SMBIOS_PROCESSOR_FAMILY_INTEL_CELERON_M_PROCESSOR },
    { u8"Intel® Pentium® 4 HT processor", SMBIOS_PROCESSOR_FAMILY_INTEL_PENTIUM_4_HT_PROCESSOR },
    { u8"Intel® Processor", SMBIOS_PROCESSOR_FAMILY_INTEL_PROCESSOR },
    { u8"AMD Duron™ Processor Family [1]", SMBIOS_PROCESSOR_FAMILY_AMD_DURON_PROCESSOR_FAMILY_1 },
    { u8"K5 Family [1]", SMBIOS_PROCESSOR_FAMILY_K5_FAMILY_1 },
    { u8"K6 Family [1]", SMBIOS_PROCESSOR_FAMILY_K6_FAMILY_1 },
    { u8"K6-2 [1]", SMBIOS_PROCESSOR_FAMILY_K6_2_1 },
    { u8"K6-3 [1]", SMBIOS_PROCESSOR_FAMILY_K6_3_1 },
    { u8"AMD Athlon™ Processor Family [1]", SMBIOS_PROCESSOR_FAMILY_AMD_ATHLON_PROCESSOR_FAMILY_1 },
    { u8"AMD29000 Family", SMBIOS_PROCESSOR_FAMILY_AMD29000_FAMILY },
    { u8"K6-2+", SMBIOS_PROCESSOR_FAMILY_K6_2_PLUS },
    { u8"Power PC Family", SMBIOS_PROCESSOR_FAMILY_POWER_PC_FAMILY },
    { u8"Power PC 601", SMBIOS_PROCESSOR_FAMILY_POWER_PC_601 },
    { u8"Power PC 603", SMBIOS_PROCESSOR_FAMILY_POWER_PC_603 },
    { u8"Power PC 603+", SMBIOS_PROCESSOR_FAMILY_POWER_PC_603_PLUS },
    { u8"Power PC 604", SMBIOS_PROCESSOR_FAMILY_POWER_PC_604 },
    { u8"Power PC 620", SMBIOS_PROCESSOR_FAMILY_POWER_PC_620 },
    { u8"Power PC x704", SMBIOS_PROCESSOR_FAMILY_POWER_PC_X704 },
    { u8"Power PC 750", SMBIOS_PROCESSOR_FAMILY_POWER_PC_750 },
    { u8"Intel® Core™ Duo processor", SMBIOS_PROCESSOR_FAMILY_INTEL_CORE_DUO_PROCESSOR },
    { u8"Intel® Core™ Duo mobile processor", SMBIOS_PROCESSOR_FAMILY_INTEL_CORE_DUO_MOBILE_PROCESSOR },
    { u8"Intel® Core™ Solo mobile processor", SMBIOS_PROCESSOR_FAMILY_INTEL_CORE_SOLO_MOBILE_PROCESSOR },
    { u8"Intel® Atom™ processor", SMBIOS_PROCESSOR_FAMILY_INTEL_ATOM_PROCESSOR },
    { u8"Intel® Core™ M processor", SMBIOS_PROCESSOR_FAMILY_INTEL_CORE_M_PROCESSOR },
    { u8"Intel® Core™ m3 processor", SMBIOS_PROCESSOR_FAMILY_INTEL_CORE_M3_PROCESSOR },
    { u8"Intel® Core™ m5 processor", SMBIOS_PROCESSOR_FAMILY_INTEL_CORE_M5_PROCESSOR },
    { u8"Intel® Core™ m7 processor", SMBIOS_PROCESSOR_FAMILY_INTEL_CORE_M7_PROCESSOR },
    { u8"Alpha Family [2]", SMBIOS_PROCESSOR_FAMILY_ALPHA_FAMILY_2 },
    { u8"Alpha 21064", SMBIOS_PROCESSOR_FAMILY_ALPHA_21064 },
    { u8"Alpha 21066", SMBIOS_PROCESSOR_FAMILY_ALPHA_21066 },
    { u8"Alpha 21164", SMBIOS_PROCESSOR_FAMILY_ALPHA_21164 },
    { u8"Alpha 21164PC", SMBIOS_PROCESSOR_FAMILY_ALPHA_21164PC },
    { u8"Alpha 21164a", SMBIOS_PROCESSOR_FAMILY_ALPHA_21164A },
    { u8"Alpha 21264", SMBIOS_PROCESSOR_FAMILY_ALPHA_21264 },
    { u8"Alpha 21364", SMBIOS_PROCESSOR_FAMILY_ALPHA_21364 },
    { u8"AMD Turion™ II Ultra Dual-Core Mobile M Processor Family", SMBIOS_PROCESSOR_FAMILY_AMD_TURION_II_ULTRA_DUAL_CORE_MOBILE_M_PROCESSOR_FAMILY },
    { u8"AMD Turion™ II Dual-Core Mobile M Processor Family", SMBIOS_PROCESSOR_FAMILY_AMD_TURION_II_DUAL_CORE_MOBILE_M_PROCESSOR_FAMILY },
    { u8"AMD Athlon™ II Dual-Core M Processor Family", SMBIOS_PROCESSOR_FAMILY_AMD_ATHLON_II_DUAL_CORE_M_PROCESSOR_FAMILY },
    { u8"AMD Opteron™ 6100 Series Processor", SMBIOS_PROCESSOR_FAMILY_AMD_OPTERON_6100_SERIES_PROCESSOR },
    { u8"AMD Opteron™ 4100 Series Processor", SMBIOS_PROCESSOR_FAMILY_AMD_OPTERON_4100_SERIES_PROCESSOR },
    { u8"AMD Opteron™ 6200 Series Processor", SMBIOS_PROCESSOR_FAMILY_AMD_OPTERON_6200_SERIES_PROCESSOR },
    { u8"AMD Opteron™ 4200 Series Processor", SMBIOS_PROCESSOR_FAMILY_AMD_OPTERON_4200_SERIES_PROCESSOR },
    { u8"AMD FX™ Series Processor", SMBIOS_PROCESSOR_FAMILY_AMD_FX_SERIES_PROCESSOR },
    { u8"MIPS Family", SMBIOS_PROCESSOR_FAMILY_MIPS_FAMILY },
    { u8"MIPS R4000", SMBIOS_PROCESSOR_FAMILY_MIPS_R4000 },
    { u8"MIPS R4200", SMBIOS_PROCESSOR_FAMILY_MIPS_R4200 },
    { u8"MIPS R4400", SMBIOS_PROCESSOR_FAMILY_MIPS_R4400 },
    { u8"MIPS R4600", SMBIOS_PROCESSOR_FAMILY_MIPS_R4600 },
    { u8"MIPS R10000", SMBIOS_PROCESSOR_FAMILY_MIPS_R10000 },
    { u8"AMD C-Series Processor", SMBIOS_PROCESSOR_FAMILY_AMD_C_SERIES_PROCESSOR },
    { u8"AMD E-Series Processor", SMBIOS_PROCESSOR_FAMILY_AMD_E_SERIES_PROCESSOR },
    { u8"AMD A-Series Processor", SMBIOS_PROCESSOR_FAMILY_AMD_A_SERIES_PROCESSOR },
    { u8"AMD G-Series Processor", SMBIOS_PROCESSOR_FAMILY_AMD_G_SERIES_PROCESSOR },
    { u8"AMD Z-Series Processor", SMBIOS_PROCESSOR_FAMILY_AMD_Z_SERIES_PROCESSOR },
    { u8"AMD R-Series Processor", SMBIOS_PROCESSOR_FAMILY_AMD_R_SERIES_PROCESSOR },
    { u8"AMD Opteron™ 4300 Series Processor", SMBIOS_PROCESSOR_FAMILY_AMD_OPTERON_4300_SERIES_PROCESSOR },
    { u8"AMD Opteron™ 6300 Series Processor", SMBIOS_PROCESSOR_FAMILY_AMD_OPTERON_6300_SERIES_PROCESSOR },
    { u8"AMD Opteron™ 3300 Series Processor", SMBIOS_PROCESSOR_FAMILY_AMD_OPTERON_3300_SERIES_PROCESSOR },
    { u8"AMD FirePro™ Series Processor", SMBIOS_PROCESSOR_FAMILY_AMD_FIREPRO_SERIES_PROCESSOR },
    { u8"SPARC Family", SMBIOS_PROCESSOR_FAMILY_SPARC_FAMILY },
    { u8"SuperSPARC", SMBIOS_PROCESSOR_FAMILY_SUPERSPARC },
    { u8"microSPARC II", SMBIOS_PROCESSOR_FAMILY_MICROSPARC_II },
    { u8"microSPARC IIep", SMBIOS_PROCESSOR_FAMILY_MICROSPARC_IIEP },
    { u8"UltraSPARC", SMBIOS_PROCESSOR_FAMILY_ULTRASPARC },
    { u8"UltraSPARC II", SMBIOS_PROCESSOR_FAMILY_ULTRASPARC_II },
    { u8"UltraSPARC Iii", SMBIOS_PROCESSOR_FAMILY_ULTRASPARC_Iii },
    { u8"UltraSPARC III", SMBIOS_PROCESSOR_FAMILY_ULTRASPARC_III },
    { u8"UltraSPARC IIIi", SMBIOS_PROCESSOR_FAMILY_ULTRASPARC_IIIi },
    { u8"68040 Family", SMBIOS_PROCESSOR_FAMILY_68040_FAMILY },
    { u8"68xxx", SMBIOS_PROCESSOR_FAMILY_68XXX },
    { u8"68000", SMBIOS_PROCESSOR_FAMILY_68000 },
    { u8"68010", SMBIOS_PROCESSOR_FAMILY_68010 },
    { u8"68020", SMBIOS_PROCESSOR_FAMILY_68020 },
    { u8"68030", SMBIOS_PROCESSOR_FAMILY_68030 },
    { u8"AMD Athlon(TM) X4 Quad-Core Processor Family", SMBIOS_PROCESSOR_FAMILY_AMD_ATHLON_X4_QUAD_CORE_PROCESSOR_FAMILY },
    { u8"AMD Opteron(TM) X1000 Series Processor", SMBIOS_PROCESSOR_FAMILY_AMD_OPTERON_X1000_SERIES_PROCESSOR },
    { u8"AMD Opteron(TM) X2000 Series APU", SMBIOS_PROCESSOR_FAMILY_AMD_OPTERON_X2000_SERIES_APU },
    { u8"AMD Opteron(TM) A-Series Processor", SMBIOS_PROCESSOR_FAMILY_AMD_OPTERON_A_SERIES_PROCESSOR },
    { u8"AMD Opteron(TM) X3000 Series APU", SMBIOS_PROCESSOR_FAMILY_AMD_OPTERON_X3000_SERIES_APU },
    { u8"AMD Zen Processor Family", SMBIOS_PROCESSOR_FAMILY_AMD_ZEN_PROCESSOR_FAMILY },
    { u8"Hobbit Family", SMBIOS_PROCESSOR_FAMILY_HOBBIT_FAMILY },
    { u8"Crusoe™ TM5000 Family", SMBIOS_PROCESSOR_FAMILY_CRUSOE_TM5000_FAMILY },
    { u8"Crusoe™ TM3000 Family", SMBIOS_PROCESSOR_FAMILY_CRUSOE_TM3000_FAMILY },
    { u8"Efficeon™ TM8000 Family", SMBIOS_PROCESSOR_FAMILY_EFFICEON_TM8000_FAMILY },
    { u8"Weitek", SMBIOS_PROCESSOR_FAMILY_WEITEK },
    { u8"Itanium™ processor", SMBIOS_PROCESSOR_FAMILY_ITANIUM_PROCESSOR },
    { u8"AMD Athlon™ 64 Processor Family", SMBIOS_PROCESSOR_FAMILY_AMD_ATHLON_64_PROCESSOR_FAMILY },
    { u8"AMD Opteron™ Processor Family", SMBIOS_PROCESSOR_FAMILY_AMD_OPTERON_PROCESSOR_FAMILY },
    { u8"AMD Sempron™ Processor Family", SMBIOS_PROCESSOR_FAMILY_AMD_SEMPRON_PROCESSOR_FAMILY },
    { u8"AMD Turion™ 64 Mobile Technology", SMBIOS_PROCESSOR_FAMILY_AMD_TURION_64_MOBILE_TECHNOLOGY },
    { u8"Dual-Core AMD Opteron™ Processor Family", SMBIOS_PROCESSOR_FAMILY_DUAL_CORE_AMD_OPTERON_PROCESSOR_FAMILY },
    { u8"AMD Athlon™ 64 X2 Dual-Core Processor Family", SMBIOS_PROCESSOR_FAMILY_AMD_ATHLON_64_X2_DUAL_CORE_PROCESSOR_FAMILY },
    { u8"AMD Turion™ 64 X2 Mobile Technology", SMBIOS_PROCESSOR_FAMILY_AMD_TURION_64_X2_MOBILE_TECHNOLOGY },
    { u8"Quad-Core AMD Opteron™ Processor Family", SMBIOS_PROCESSOR_FAMILY_QUAD_CORE_AMD_OPTERON_PROCESSOR_FAMILY },
    { u8"Third-Generation AMD Opteron™ Processor Family", SMBIOS_PROCESSOR_FAMILY_THIRD_GENERATION_AMD_OPTERON_PROCESSOR_FAMILY },
    { u8"AMD Phenom™ FX Quad-Core Processor Family", SMBIOS_PROCESSOR_FAMILY_AMD_PHENOM_FX_QUAD_CORE_PROCESSOR_FAMILY },
    { u8"AMD Phenom™ X4 Quad-Core Processor Family", SMBIOS_PROCESSOR_FAMILY_AMD_PHENOM_X4_QUAD_CORE_PROCESSOR_FAMILY },
    { u8"AMD Phenom™ X2 Dual-Core Processor Family", SMBIOS_PROCESSOR_FAMILY_AMD_PHENOM_X2_DUAL_CORE_PROCESSOR_FAMILY },
    { u8"AMD Athlon™ X2 Dual-Core Processor Family", SMBIOS_PROCESSOR_FAMILY_AMD_ATHLON_X2_DUAL_CORE_PROCESSOR_FAMILY },
    { u8"PA-RISC Family", SMBIOS_PROCESSOR_FAMILY_PA_RISC_FAMILY },
    { u8"PA-RISC 8500", SMBIOS_PROCESSOR_FAMILY_PA_RISC_8500 },
    { u8"PA-RISC 8000", SMBIOS_PROCESSOR_FAMILY_PA_RISC_8000 },
    { u8"PA-RISC 7300LC", SMBIOS_PROCESSOR_FAMILY_PA_RISC_7300LC },
    { u8"PA-RISC 7200", SMBIOS_PROCESSOR_FAMILY_PA_RISC_7200 },
    { u8"PA-RISC 7100LC", SMBIOS_PROCESSOR_FAMILY_PA_RISC_7100LC },
    { u8"PA-RISC 7100", SMBIOS_PROCESSOR_FAMILY_PA_RISC_7100 },
    { u8"V30 Family", SMBIOS_PROCESSOR_FAMILY_V30_FAMILY },
    { u8"Quad-Core Intel® Xeon® processor 3200 Series", SMBIOS_PROCESSOR_FAMILY_QUAD_CORE_INTEL_XEON_PROCESSOR_3200_SERIES },
    { u8"Dual-Core Intel® Xeon® processor 3000 Series", SMBIOS_PROCESSOR_FAMILY_DUAL_CORE_INTEL_XEON_PROCESSOR_3000_SERIES },
    { u8"Quad-Core Intel® Xeon® processor 5300 Series", SMBIOS_PROCESSOR_FAMILY_QUAD_CORE_INTEL_XEON_PROCESSOR_5300_SERIES },
    { u8"Dual-Core Intel® Xeon® processor 5100 Series", SMBIOS_PROCESSOR_FAMILY_DUAL_CORE_INTEL_XEON_PROCESSOR_5100_SERIES },
    { u8"Dual-Core Intel® Xeon® processor 5000 Series", SMBIOS_PROCESSOR_FAMILY_DUAL_CORE_INTEL_XEON_PROCESSOR_5000_SERIES },
    { u8"Dual-Core Intel® Xeon® processor LV", SMBIOS_PROCESSOR_FAMILY_DUAL_CORE_INTEL_XEON_PROCESSOR_LV },
    { u8"Dual-Core Intel® Xeon® processor ULV", SMBIOS_PROCESSOR_FAMILY_DUAL_CORE_INTEL_XEON_PROCESSOR_ULV },
    { u8"Dual-Core Intel® Xeon® processor 7100 Series", SMBIOS_PROCESSOR_FAMILY_DUAL_CORE_INTEL_XEON_PROCESSOR_7100_SERIES },
    { u8"Quad-Core Intel® Xeon® processor 5400 Series", SMBIOS_PROCESSOR_FAMILY_QUAD_CORE_INTEL_XEON_PROCESSOR_5400_SERIES },
    { u8"Quad-Core Intel® Xeon® processor", SMBIOS_PROCESSOR_FAMILY_QUAD_CORE_INTEL_XEON_PROCESSOR },
    { u8"Dual-Core Intel® Xeon® processor 5200 Series", SMBIOS_PROCESSOR_FAMILY_DUAL_CORE_INTEL_XEON_PROCESSOR_5200_SERIES },
    { u8"Dual-Core Intel® Xeon® processor 7200 Series", SMBIOS_PROCESSOR_FAMILY_DUAL_CORE_INTEL_XEON_PROCESSOR_7200_SERIES },
    { u8"Quad-Core Intel® Xeon® processor 7300 Series", SMBIOS_PROCESSOR_FAMILY_QUAD_CORE_INTEL_XEON_PROCESSOR_7300_SERIES },
    { u8"Quad-Core Intel® Xeon® processor 7400 Series", SMBIOS_PROCESSOR_FAMILY_QUAD_CORE_INTEL_XEON_PROCESSOR_7400_SERIES },
    { u8"Multi-Core Intel® Xeon® processor 7400 Series", SMBIOS_PROCESSOR_FAMILY_MULTI_CORE_INTEL_XEON_PROCESSOR_7400_SERIES },
    { u8"Pentium® III Xeon® processor", SMBIOS_PROCESSOR_FAMILY_PENTIUM_III_XEON_PROCESSOR },
    { u8"Pentium® III Processor with Intel® SpeedStep™ Technology", SMBIOS_PROCESSOR_FAMILY_PENTIUM_III_PROCESSOR_WITH_INTEL_SPEEDSTEP_TECHNOLOGY },
    { u8"Pentium® 4 Processor", SMBIOS_PROCESSOR_FAMILY_PENTIUM_4_PROCESSOR },
    { u8"Intel® Xeon® processor", SMBIOS_PROCESSOR_FAMILY_INTEL_XEON_PROCESSOR },
    { u8"AS400 Family", SMBIOS_PROCESSOR_FAMILY_AS400_FAMILY },
    { u8"Intel® Xeon® processor MP", SMBIOS_PROCESSOR_FAMILY_INTEL_XEON_PROCESSOR_MP },
    { u8"AMD Athlon™ XP Processor Family", SMBIOS_PROCESSOR_FAMILY_AMD_ATHLON_XP_PROCESSOR_FAMILY },
    { u8"AMD Athlon™ MP Processor Family", SMBIOS_PROCESSOR_FAMILY_AMD_ATHLON_MP_PROCESSOR_FAMILY },
    { u8"Intel® Itanium® 2 processor", SMBIOS_PROCESSOR_FAMILY_INTEL_ITANIUM_2_PROCESSOR },
    { u8"Intel® Pentium® M processor", SMBIOS_PROCESSOR_FAMILY_INTEL_PENTIUM_M_PROCESSOR },
    { u8"Intel® Celeron® D processor", SMBIOS_PROCESSOR_FAMILY_INTEL_CELERON_D_PROCESSOR },
    { u8"Intel® Pentium® D processor", SMBIOS_PROCESSOR_FAMILY_INTEL_PENTIUM_D_PROCESSOR },
    { u8"Intel® Pentium® Processor Extreme Edition", SMBIOS_PROCESSOR_FAMILY_INTEL_PENTIUM_PROCESSOR_EXTREME_EDITION },
    { u8"Intel® Core™ Solo Processor", SMBIOS_PROCESSOR_FAMILY_INTEL_CORE_SOLO_PROCESSOR },
    { u8"Intel® Core™ 2 Duo Processor", SMBIOS_PROCESSOR_FAMILY_INTEL_CORE_2_DUO_PROCESSOR },
    { u8"Intel® Core™ 2 Solo processor", SMBIOS_PROCESSOR_FAMILY_INTEL_CORE_2_SOLO_PROCESSOR },
    { u8"Intel® Core™ 2 Extreme processor", SMBIOS_PROCESSOR_FAMILY_INTEL_CORE_2_EXTREME_PROCESSOR },
    { u8"Intel® Core™ 2 Quad processor", SMBIOS_PROCESSOR_FAMILY_INTEL_CORE_2_QUAD_PROCESSOR },
    { u8"Intel® Core™ 2 Extreme mobile processor", SMBIOS_PROCESSOR_FAMILY_INTEL_CORE_2_EXTREME_MOBILE_PROCESSOR },
    { u8"Intel® Core™ 2 Duo mobile processor", SMBIOS_PROCESSOR_FAMILY_INTEL_CORE_2_DUO_MOBILE_PROCESSOR },
    { u8"Intel® Core™ 2 Solo mobile processor", SMBIOS_PROCESSOR_FAMILY_INTEL_CORE_2_SOLO_MOBILE_PROCESSOR },
    { u8"Intel® Core™ i7 processor", SMBIOS_PROCESSOR_FAMILY_INTEL_CORE_I7_PROCESSOR },
    { u8"Dual-Core Intel® Celeron® processor", SMBIOS_PROCESSOR_FAMILY_DUAL_CORE_INTEL_CELERON_PROCESSOR },
    { u8"IBM390 Family", SMBIOS_PROCESSOR_FAMILY_IBM390_FAMILY },
    { u8"G4", SMBIOS_PROCESSOR_FAMILY_G4 },
    { u8"G5", SMBIOS_PROCESSOR_FAMILY_G5 },
    { u8"ESA/390 G6", SMBIOS_PROCESSOR_FAMILY_ESA_390_G6 },
    { u8"z/Architecture base", SMBIOS_PROCESSOR_FAMILY_Z_ARCHITECTURE_BASE },
    { u8"Intel® Core™ i5 processor", SMBIOS_PROCESSOR_FAMILY_INTEL_CORE_I5_PROCESSOR },
    { u8"Intel® Core™ i3 processor", SMBIOS_PROCESSOR_FAMILY_INTEL_CORE_I3_PROCESSOR },
    { u8"Intel® Core™ i9 processor", SMBIOS_PROCESSOR_FAMILY_INTEL_CORE_I9_PROCESSOR },
    { u8"Intel® Xeon® D Processor family", SMBIOS_PROCESSOR_FAMILY_INTEL_XEON_D_PROCESSOR_FAMILY },
    { u8"VIA C7™-M Processor Family", SMBIOS_PROCESSOR_FAMILY_VIA_C7_M_PROCESSOR_FAMILY },
    { u8"VIA C7™-D Processor Family", SMBIOS_PROCESSOR_FAMILY_VIA_C7_D_PROCESSOR_FAMILY },
    { u8"VIA C7™ Processor Family", SMBIOS_PROCESSOR_FAMILY_VIA_C7_PROCESSOR_FAMILY },
    { u8"VIA Eden™ Processor Family", SMBIOS_PROCESSOR_FAMILY_VIA_EDEN_PROCESSOR_FAMILY },
    { u8"Multi-Core Intel® Xeon® processor", SMBIOS_PROCESSOR_FAMILY_MULTI_CORE_INTEL_XEON_PROCESSOR },
    { u8"Dual-Core Intel® Xeon® processor 3xxx Series", SMBIOS_PROCESSOR_FAMILY_DUAL_CORE_INTEL_XEON_PROCESSOR_3XXX_SERIES },
    { u8"Quad-Core Intel® Xeon® processor 3xxx Series", SMBIOS_PROCESSOR_FAMILY_QUAD_CORE_INTEL_XEON_PROCESSOR_3XXX_SERIES },
    { u8"VIA Nano™ Processor Family", SMBIOS_PROCESSOR_FAMILY_VIA_NANO_PROCESSOR_FAMILY },
    { u8"Dual-Core Intel® Xeon® processor 5xxx Series", SMBIOS_PROCESSOR_FAMILY_DUAL_CORE_INTEL_XEON_PROCESSOR_5XXX_SERIES },
    { u8"Quad-Core Intel® Xeon® processor 5xxx Series", SMBIOS_PROCESSOR_FAMILY_QUAD_CORE_INTEL_XEON_PROCESSOR_5XXX_SERIES },
    { u8"Dual-Core Intel® Xeon® processor 7xxx Series", SMBIOS_PROCESSOR_FAMILY_DUAL_CORE_INTEL_XEON_PROCESSOR_7XXX_SERIES },
    { u8"Quad-Core Intel® Xeon® processor 7xxx Series", SMBIOS_PROCESSOR_FAMILY_QUAD_CORE_INTEL_XEON_PROCESSOR_7XXX_SERIES },
    { u8"Multi-Core Intel® Xeon® processor 7xxx Series", SMBIOS_PROCESSOR_FAMILY_MULTI_CORE_INTEL_XEON_PROCESSOR_7XXX_SERIES },
    { u8"Multi-Core Intel® Xeon® processor 3400 Series", SMBIOS_PROCESSOR_FAMILY_MULTI_CORE_INTEL_XEON_PROCESSOR_3400_SERIES },
    { u8"AMD Opteron™ 3000 Series Processor", SMBIOS_PROCESSOR_FAMILY_AMD_OPTERON_3000_SERIES_PROCESSOR },
    { u8"AMD Sempron™ II Processor", SMBIOS_PROCESSOR_FAMILY_AMD_SEMPRON_II_PROCESSOR },
    { u8"Embedded AMD Opteron™ Quad-Core Processor Family", SMBIOS_PROCESSOR_FAMILY_EMBEDDED_AMD_OPTERON_QUAD_CORE_PROCESSOR_FAMILY },
    { u8"AMD Phenom™ Triple-Core Processor Family", SMBIOS_PROCESSOR_FAMILY_AMD_PHENOM_TRIPLE_CORE_PROCESSOR_FAMILY },
    { u8"AMD Turion™ Ultra Dual-Core Mobile Processor Family", SMBIOS_PROCESSOR_FAMILY_AMD_TURION_ULTRA_DUAL_CORE_MOBILE_PROCESSOR_FAMILY },
    { u8"AMD Turion™ Dual-Core Mobile Processor Family", SMBIOS_PROCESSOR_FAMILY_AMD_TURION_DUAL_CORE_MOBILE_PROCESSOR_FAMILY },
    { u8"AMD Athlon™ Dual-Core Processor Family", SMBIOS_PROCESSOR_FAMILY_AMD_ATHLON_DUAL_CORE_PROCESSOR_FAMILY },
    { u8"AMD Sempron™ SI Processor Family", SMBIOS_PROCESSOR_FAMILY_AMD_SEMPRON_SI_PROCESSOR_FAMILY },
    { u8"AMD Phenom™ II Processor Family", SMBIOS_PROCESSOR_FAMILY_AMD_PHENOM_II_PROCESSOR_FAMILY },
    { u8"AMD Athlon™ II Processor Family", SMBIOS_PROCESSOR_FAMILY_AMD_ATHLON_II_PROCESSOR_FAMILY },
    { u8"Six-Core AMD Opteron™ Processor Family", SMBIOS_PROCESSOR_FAMILY_SIX_CORE_AMD_OPTERON_PROCESSOR_FAMILY },
    { u8"AMD Sempron™ M Processor Family", SMBIOS_PROCESSOR_FAMILY_AMD_SEMPRON_M_PROCESSOR_FAMILY },
    { u8"i860", SMBIOS_PROCESSOR_FAMILY_I860 },
    { u8"i960", SMBIOS_PROCESSOR_FAMILY_I960 },
    { u8"(See Processor Family 2)", SMBIOS_PROCESSOR_FAMILY_EXTENSION_INDICATOR },
};

__declspec(selectany)
SMBIOS_FIELD_ENUM SmbiosType4CpuStatusEnum[] = {
    { "Unknown", SMBIOS_PROCESSOR_CPU_STATUS_UNKNOWN },
    { "CPU Enabled", SMBIOS_PROCESSOR_CPU_STATUS_ENABLED },
    { "CPU Disabled by User through Firmware Setup", SMBIOS_PROCESSOR_CPU_STATUS_DISABLED_BY_USER },
    { "CPU Disabled By firmware (POST Error)", SMBIOS_PROCESSOR_CPU_STATUS_DISABLED_BY_FIRMWARE },
    { "CPU is Idle, waiting to be enabled", SMBIOS_PROCESSOR_CPU_STATUS_IDLE },
    { "Other", SMBIOS_PROCESSOR_CPU_STATUS_OTHER },
};

__declspec(selectany)
SMBIOS_FIELD_ENUM SmbiosType4UpgradeEnum[] = {
    { "Other", SMBIOS_PROCESSOR_UPGRADE_OTHER },
    { "Unknown", SMBIOS_PROCESSOR_UPGRADE_UNKNOWN },
    { "Daughter Board", SMBIOS_PROCESSOR_UPGRADE_DAUGHTER_BOARD },
    { "ZIF Socket", SMBIOS_PROCESSOR_UPGRADE_ZIF_SOCKET },
    { "Replaceable Piggy Back", SMBIOS_PROCESSOR_UPGRADE_REPLACEABLE_PIGGY_BACK },
    { "None", SMBIOS_PROCESSOR_UPGRADE_NONE },
    { "LIF Socket", SMBIOS_PROCESSOR_UPGRADE_LIF_SOCKET },
    { "Slot 1", SMBIOS_PROCESSOR_UPGRADE_SLOT_1 },
    { "Slot 2", SMBIOS_PROCESSOR_UPGRADE_SLOT_2 },
    { "370-pin socket", SMBIOS_PROCESSOR_UPGRADE_370_PIN_SOCKET },
    { "Slot A", SMBIOS_PROCESSOR_UPGRADE_SLOT_A },
    { "Slot M", SMBIOS_PROCESSOR_UPGRADE_SLOT_M },
    { "Socket 423", SMBIOS_PROCESSOR_UPGRADE_SOCKET_423 },
    { "Socket A (Socket 462)", SMBIOS_PROCESSOR_UPGRADE_SOCKET_A },
    { "Socket 478", SMBIOS_PROCESSOR_UPGRADE_SOCKET_478 },
    { "Socket 754", SMBIOS_PROCESSOR_UPGRADE_SOCKET_754 },
    { "Socket 940", SMBIOS_PROCESSOR_UPGRADE_SOCKET_940 },
    { "Socket 939", SMBIOS_PROCESSOR_UPGRADE_SOCKET_939 },
    { "Socket mPGA604", SMBIOS_PROCESSOR_UPGRADE_SOCKET_mPGA604 },
    { "Socket LGA771", SMBIOS_PROCESSOR_UPGRADE_SOCKET_LGA771 },
    { "Socket LGA775", SMBIOS_PROCESSOR_UPGRADE_SOCKET_LGA775 },
    { "Socket S1", SMBIOS_PROCESSOR_UPGRADE_SOCKET_S1 },
    { "Socket AM2", SMBIOS_PROCESSOR_UPGRADE_SOCKET_AM2 },
    { "Socket F (1207)", SMBIOS_PROCESSOR_UPGRADE_SOCKET_F },
    { "Socket LGA1366", SMBIOS_PROCESSOR_UPGRADE_SOCKET_LGA1366 },
    { "Socket G34", SMBIOS_PROCESSOR_UPGRADE_SOCKET_G34 },
    { "Socket AM3", SMBIOS_PROCESSOR_UPGRADE_SOCKET_AM3 },
    { "Socket C32", SMBIOS_PROCESSOR_UPGRADE_SOCKET_C32 },
    { "Socket LGA1156", SMBIOS_PROCESSOR_UPGRADE_SOCKET_LGA1156 },
    { "Socket LGA1567", SMBIOS_PROCESSOR_UPGRADE_SOCKET_LGA1567 },
    { "Socket PGA988A", SMBIOS_PROCESSOR_UPGRADE_SOCKET_PGA988A },
    { "Socket BGA1288", SMBIOS_PROCESSOR_UPGRADE_SOCKET_BGA1288 },
    { "Socket rPGA988B", SMBIOS_PROCESSOR_UPGRADE_SOCKET_rPGA988B },
    { "Socket BGA1023", SMBIOS_PROCESSOR_UPGRADE_SOCKET_BGA1023 },
    { "Socket BGA1224", SMBIOS_PROCESSOR_UPGRADE_SOCKET_BGA1224 },
    { "Socket LGA1155", SMBIOS_PROCESSOR_UPGRADE_SOCKET_LGA1155 },
    { "Socket LGA1356", SMBIOS_PROCESSOR_UPGRADE_SOCKET_LGA1356 },
    { "Socket LGA2011", SMBIOS_PROCESSOR_UPGRADE_SOCKET_LGA2011 },
    { "Socket FS1", SMBIOS_PROCESSOR_UPGRADE_SOCKET_FS1 },
    { "Socket FS2", SMBIOS_PROCESSOR_UPGRADE_SOCKET_FS2 },
    { "Socket FM1", SMBIOS_PROCESSOR_UPGRADE_SOCKET_FM1 },
    { "Socket FM2", SMBIOS_PROCESSOR_UPGRADE_SOCKET_FM2 },
    { "Socket LGA2011-3", SMBIOS_PROCESSOR_UPGRADE_SOCKET_LGA2011_3 },
    { "Socket LGA2011-3", SMBIOS_PROCESSOR_UPGRADE_SOCKET_LGA1356_3 },
    { "Socket LGA1150", SMBIOS_PROCESSOR_UPGRADE_SOCKET_LGA1150 },
    { "Socket BGA1168", SMBIOS_PROCESSOR_UPGRADE_SOCKET_BGA1168 },
    { "Socket BGA1234", SMBIOS_PROCESSOR_UPGRADE_SOCKET_BGA1234 },
    { "Socket BGA1364", SMBIOS_PROCESSOR_UPGRADE_SOCKET_BGA1364 },
    { "Socket AM4", SMBIOS_PROCESSOR_UPGRADE_SOCKET_AM4 },
    { "Socket LGA1151", SMBIOS_PROCESSOR_UPGRADE_SOCKET_LGA1151 },
    { "Socket BGA1356", SMBIOS_PROCESSOR_UPGRADE_SOCKET_BGA1356 },
    { "Socket BGA1440", SMBIOS_PROCESSOR_UPGRADE_SOCKET_BGA1440 },
    { "Socket BGA1515", SMBIOS_PROCESSOR_UPGRADE_SOCKET_BGA1515 },
    { "Socket LGA3647-1", SMBIOS_PROCESSOR_UPGRADE_SOCKET_LGA3647_1 },
    { "Socket SP3", SMBIOS_PROCESSOR_UPGRADE_SOCKET_SP3 },
    { "Socket SP3r2", SMBIOS_PROCESSOR_UPGRADE_SOCKET_SP3r2 },
    { "Socket LGA2066", SMBIOS_PROCESSOR_UPGRADE_SOCKET_LGA2066 },
    { "Socket BGA1392", SMBIOS_PROCESSOR_UPGRADE_SOCKET_BGA1392 },
    { "Socket BGA1510", SMBIOS_PROCESSOR_UPGRADE_SOCKET_BGA1510 },
    { "Socket BGA1528", SMBIOS_PROCESSOR_UPGRADE_SOCKET_BGA1528 },
    { "Socket LGA4189", SMBIOS_PROCESSOR_UPGRADE_SOCKET_LGA4189 },
    { "Socket LGA1200", SMBIOS_PROCESSOR_UPGRADE_SOCKET_LGA1200 },
    { "Socket LGA4677", SMBIOS_PROCESSOR_UPGRADE_SOCKET_LGA4677 },
    { "Socket LGA1700", SMBIOS_PROCESSOR_UPGRADE_SOCKET_LGA1700 },
    { "Socket BGA1744", SMBIOS_PROCESSOR_UPGRADE_SOCKET_BGA1744 },
    { "Socket BGA1781", SMBIOS_PROCESSOR_UPGRADE_SOCKET_BGA1781 },
    { "Socket BGA1211", SMBIOS_PROCESSOR_UPGRADE_SOCKET_BGA1211 },
    { "Socket BGA2422", SMBIOS_PROCESSOR_UPGRADE_SOCKET_BGA2422 },
    { "Socket LGA1211", SMBIOS_PROCESSOR_UPGRADE_SOCKET_LGA1211 },
    { "Socket LGA2422", SMBIOS_PROCESSOR_UPGRADE_SOCKET_LGA2422 },
    { "Socket LGA5773", SMBIOS_PROCESSOR_UPGRADE_SOCKET_LGA5773 },
    { "Socket BGA5773", SMBIOS_PROCESSOR_UPGRADE_SOCKET_BGA5773 },
    { "Socket AM5", SMBIOS_PROCESSOR_UPGRADE_SOCKET_AM5 },
    { "Socket SP5", SMBIOS_PROCESSOR_UPGRADE_SOCKET_SP5 },
    { "Socket SP6", SMBIOS_PROCESSOR_UPGRADE_SOCKET_SP6 },
    { "Socket BGA883", SMBIOS_PROCESSOR_UPGRADE_SOCKET_BGA883 },
    { "Socket BGA1190", SMBIOS_PROCESSOR_UPGRADE_SOCKET_BGA1190 },
    { "Socket BGA4129", SMBIOS_PROCESSOR_UPGRADE_SOCKET_BGA4129 },
    { "Socket LGA4710", SMBIOS_PROCESSOR_UPGRADE_SOCKET_LGA4710 },
    { "Socket LGA7529", SMBIOS_PROCESSOR_UPGRADE_SOCKET_LGA7529 },
    { "Socket BGA1964", SMBIOS_PROCESSOR_UPGRADE_SOCKET_BGA1964 },
    { "Socket BGA1792", SMBIOS_PROCESSOR_UPGRADE_SOCKET_BGA1792 },
    { "Socket BGA2049", SMBIOS_PROCESSOR_UPGRADE_SOCKET_BGA2049 },
    { "Socket BGA2551", SMBIOS_PROCESSOR_UPGRADE_SOCKET_BGA2551 },
    { "Socket LGA1851", SMBIOS_PROCESSOR_UPGRADE_SOCKET_LGA1851 },
    { "Socket BGA2114", SMBIOS_PROCESSOR_UPGRADE_SOCKET_BGA2114 },
    { "Socket BGA2833", SMBIOS_PROCESSOR_UPGRADE_SOCKET_BGA2833 },
    { "Invalid", SMBIOS_PROCESSOR_UPGRADE_INVALID },
};

__declspec(selectany)
SMBIOS_FIELD_ENUM SmbiosType4Family2Enum[] = {
    { u8"ARMv7", SMBIOS_PROCESSOR_FAMILY_2_ARMV7 },
    { u8"ARMv8", SMBIOS_PROCESSOR_FAMILY_2_ARMV8 },
    { u8"ARMv9", SMBIOS_PROCESSOR_FAMILY_2_ARMV9 },
    { u8"SH-3", SMBIOS_PROCESSOR_FAMILY_2_SH_3 },
    { u8"SH-4", SMBIOS_PROCESSOR_FAMILY_2_SH_4 },
    { u8"ARM", SMBIOS_PROCESSOR_FAMILY_2_ARM },
    { u8"StrongARM", SMBIOS_PROCESSOR_FAMILY_2_STRONGARM },
    { u8"6x86", SMBIOS_PROCESSOR_FAMILY_2_6X86 },
    { u8"MediaGX", SMBIOS_PROCESSOR_FAMILY_2_MEDIAGX },
    { u8"MII", SMBIOS_PROCESSOR_FAMILY_2_MII },
    { u8"WinChip", SMBIOS_PROCESSOR_FAMILY_2_WINCHIP },
    { u8"DSP", SMBIOS_PROCESSOR_FAMILY_2_DSP },
    { u8"Video Processor", SMBIOS_PROCESSOR_FAMILY_2_VIDEO_PROCESSOR },
    { u8"RISC-V RV32", SMBIOS_PROCESSOR_FAMILY_2_RISC_V_RV32 },
    { u8"RISC-V RV64", SMBIOS_PROCESSOR_FAMILY_2_RISC_V_RV64 },
    { u8"RISC-V RV128", SMBIOS_PROCESSOR_FAMILY_2_RISC_V_RV128 },
    { u8"LoongArch", SMBIOS_PROCESSOR_FAMILY_2_LOONGARCH },
    { u8"Loongson™ 1 Processor Family", SMBIOS_PROCESSOR_FAMILY_2_LOONGSON_1_PROCESSOR_FAMILY },
    { u8"Loongson™ 2 Processor Family", SMBIOS_PROCESSOR_FAMILY_2_LOONGSON_2_PROCESSOR_FAMILY },
    { u8"Loongson™ 3 Processor Family", SMBIOS_PROCESSOR_FAMILY_2_LOONGSON_3_PROCESSOR_FAMILY },
    { u8"Loongson™ 2K Processor Family", SMBIOS_PROCESSOR_FAMILY_2_LOONGSON_2K_PROCESSOR_FAMILY },
    { u8"Loongson™ 3A Processor Family", SMBIOS_PROCESSOR_FAMILY_2_LOONGSON_3A_PROCESSOR_FAMILY },
    { u8"Loongson™ 3B Processor Family", SMBIOS_PROCESSOR_FAMILY_2_LOONGSON_3B_PROCESSOR_FAMILY },
    { u8"Loongson™ 3C Processor Family", SMBIOS_PROCESSOR_FAMILY_2_LOONGSON_3C_PROCESSOR_FAMILY },
    { u8"Loongson™ 3D Processor Family", SMBIOS_PROCESSOR_FAMILY_2_LOONGSON_3D_PROCESSOR_FAMILY },
    { u8"Loongson™ 3E Processor Family", SMBIOS_PROCESSOR_FAMILY_2_LOONGSON_3E_PROCESSOR_FAMILY },
    { u8"Dual-Core Loongson™ 2K Processor 2xxx Series", SMBIOS_PROCESSOR_FAMILY_2_DUAL_CORE_LOONGSON_2K_PROCESSOR_2XXX_SERIES },
    { u8"Quad-Core Loongson™ 3A Processor 5xxx Series", SMBIOS_PROCESSOR_FAMILY_2_QUAD_CORE_LOONGSON_3A_PROCESSOR_5XXX_SERIES },
    { u8"Multi-Core Loongson™ 3A Processor 5xxx Series", SMBIOS_PROCESSOR_FAMILY_2_MULTI_CORE_LOONGSON_3A_PROCESSOR_5XXX_SERIES },
    { u8"Quad-Core Loongson™ 3B Processor 5xxx Series", SMBIOS_PROCESSOR_FAMILY_2_QUAD_CORE_LOONGSON_3B_PROCESSOR_5XXX_SERIES },
    { u8"Multi-Core Loongson™ 3B Processor 5xxx Series", SMBIOS_PROCESSOR_FAMILY_2_MULTI_CORE_LOONGSON_3B_PROCESSOR_5XXX_SERIES },
    { u8"Multi-Core Loongson™ 3C Processor 5xxx Series", SMBIOS_PROCESSOR_FAMILY_2_MULTI_CORE_LOONGSON_3C_PROCESSOR_5XXX_SERIES },
    { u8"Multi-Core Loongson™ 3D Processor 5xxx Series", SMBIOS_PROCESSOR_FAMILY_2_MULTI_CORE_LOONGSON_3D_PROCESSOR_5XXX_SERIES },
    { u8"Intel® Core™ 3", SMBIOS_PROCESSOR_FAMILY_2_INTEL_CORE_3 },
    { u8"Intel® Core™ 5", SMBIOS_PROCESSOR_FAMILY_2_INTEL_CORE_5 },
    { u8"Intel® Core™ 7", SMBIOS_PROCESSOR_FAMILY_2_INTEL_CORE_7 },
    { u8"Intel® Core™ 9", SMBIOS_PROCESSOR_FAMILY_2_INTEL_CORE_9 },
    { u8"Intel® Core™ Ultra 3", SMBIOS_PROCESSOR_FAMILY_2_INTEL_CORE_ULTRA_3 },
    { u8"Intel® Core™ Ultra 5", SMBIOS_PROCESSOR_FAMILY_2_INTEL_CORE_ULTRA_5 },
    { u8"Intel® Core™ Ultra 7", SMBIOS_PROCESSOR_FAMILY_2_INTEL_CORE_ULTRA_7 },
    { u8"Intel® Core™ Ultra 9", SMBIOS_PROCESSOR_FAMILY_2_INTEL_CORE_ULTRA_9 },
};

__declspec(selectany)
SMBIOS_FIELD_TYPE_INFO SmbiosType4FieldInfo[] = {
    SMBIOS_DEFINE_FIELD_STRING(4, "Socket Designation", SocketDesignation),
    SMBIOS_DEFINE_FIELD_ENUM(4, "Processor Type", Type, SmbiosType4TypeEnum),
    SMBIOS_DEFINE_FIELD_ENUM(4, "Processor Family", Family, SmbiosType4FamilyEnum),
    SMBIOS_DEFINE_FIELD_STRING(4, "Processor Manufacturer", Manufacturer),
    SMBIOS_DEFINE_FIELD_UINT(4, "Processor ID", ID),
    SMBIOS_DEFINE_FIELD_STRING(4, "Processor Version", Version),
    SMBIOS_DEFINE_FIELD_UINT(4, "Voltage", Voltage),
    SMBIOS_DEFINE_FIELD_UINT(4, "External Clock", ExternalClock),
    SMBIOS_DEFINE_FIELD_UINT(4, "Max Speed", MaxSpeed),
    SMBIOS_DEFINE_FIELD_UINT(4, "Current Speed", CurrentSpeed),
    SMBIOS_DEFINE_FIELD_UINT(4, "Status", Status.Value),
    SMBIOS_DEFINE_BIT_FIELD("CPU Status", 0, 3, SmbiosDataTypeEnum, SMBIOS_FIELD_ENUM_VALUES(SmbiosType4CpuStatusEnum)),
    SMBIOS_DEFINE_BIT_FIELD("Reserved", 3, 3, SmbiosDataTypeUInt),
    SMBIOS_DEFINE_FIELD_BIT("CPU Socket Populated", 6),
    SMBIOS_DEFINE_FIELD_BIT("Reserved", 7),
    SMBIOS_DEFINE_FIELD_ENUM(4, "Processor Upgrade", Upgrade, SmbiosType4UpgradeEnum),
    SMBIOS_DEFINE_FIELD_UINT(4, "L1 Cache Handle", L1CacheHandle),
    SMBIOS_DEFINE_FIELD_UINT(4, "L2 Cache Handle", L2CacheHandle),
    SMBIOS_DEFINE_FIELD_UINT(4, "L3 Cache Handle", L3CacheHandle),
    SMBIOS_DEFINE_FIELD_STRING(4, "Serial Number", SerialNumber),
    SMBIOS_DEFINE_FIELD_STRING(4, "Asset Tag", AssetTag),
    SMBIOS_DEFINE_FIELD_STRING(4, "Part Number", PartNumber),
    SMBIOS_DEFINE_FIELD_UINT(4, "Core Count", CoreCount),
    SMBIOS_DEFINE_FIELD_UINT(4, "Core Enabled", CoreEnabled),
    SMBIOS_DEFINE_FIELD_UINT(4, "Thread Count", ThreadCount),
    SMBIOS_DEFINE_FIELD_UINT(4, "Processor Characteristics", Characteristics.Value),
    SMBIOS_DEFINE_FIELD_BIT("Reserved", 0),
    SMBIOS_DEFINE_FIELD_BIT("Unknown", 1),
    SMBIOS_DEFINE_FIELD_BIT("64-bit Capable", 2),
    SMBIOS_DEFINE_FIELD_BIT("Multi-Core", 3),
    SMBIOS_DEFINE_FIELD_BIT("Hardware Thread", 4),
    SMBIOS_DEFINE_FIELD_BIT("Execute Protection", 5),
    SMBIOS_DEFINE_FIELD_BIT("Enhanced Virtualization", 6),
    SMBIOS_DEFINE_FIELD_BIT("Power/Performance Control", 7),
    SMBIOS_DEFINE_FIELD_BIT("128-bit Capable", 8),
    SMBIOS_DEFINE_FIELD_BIT("Arm64 SoC ID", 9),
    SMBIOS_DEFINE_BIT_FIELD("Reserved", 10, 6, SmbiosDataTypeUInt),
    SMBIOS_DEFINE_FIELD_ENUM(4, "Processor Family 2", Family2, SmbiosType4Family2Enum),
    SMBIOS_DEFINE_FIELD_UINT(4, "Core Count 2", CoreCount2),
    SMBIOS_DEFINE_FIELD_UINT(4, "Core Enabled 2", CoreEnabled2),
    SMBIOS_DEFINE_FIELD_UINT(4, "Thread Count 2", ThreadCount2),
    SMBIOS_DEFINE_FIELD_UINT(4, "Thread Enabled", ThreadEnabled),
    SMBIOS_DEFINE_FIELD_STRING(4, "Socket Type", SocketType),
};

__declspec(selectany)
SMBIOS_TYPE_INFO SmbiosTypeInfo[] = {
    SMBIOS_DEFINE_TYPE(0, "Platform Firmware Information"),
    SMBIOS_DEFINE_TYPE(1, "System Information"),
    SMBIOS_DEFINE_TYPE(2, "Baseboard Information"),
    SMBIOS_DEFINE_TYPE(3, "System Enclosure or Chassis"),
    SMBIOS_DEFINE_TYPE(4, "Processor Information"),
    { 5, "Memory Controller Information (Obsolete)", 0, (void*)0 },
    { 6, "Memory Module Information (Obsolete)", 0, (void*)0 },
    { 7, "Cache Information", 0, (void*)0 },
    { 8, "Port Connector Information", 0, (void*)0 },
    { 9, "System Slots", 0, (void*)0 },
    { 10, "On Board Devices Information (Obsolete)", 0, (void*)0 },
    { 11, "OEM Strings", 0, (void*)0 },
    { 12, "System Configuration Options", 0, (void*)0 },
    { 13, "Firmware Language Information", 0, (void*)0 },
    { 14, "Group Associations", 0, (void*)0 },
    { 15, "System Event Log", 0, (void*)0 },
    { 16, "Physical Memory Array", 0, (void*)0 },
    { 17, "Memory Device", 0, (void*)0 },
    { 18, "32-Bit Memory Error Information", 0, (void*)0 },
    { 19, "Memory Array Mapped Address", 0, (void*)0 },
    { 20, "Memory Device Mapped Address", 0, (void*)0 },
    { 21, "Built-in Pointing Device", 0, (void*)0 },
    { 22, "Portable Battery", 0, (void*)0 },
    { 23, "System Reset", 0, (void*)0 },
    { 24, "Hardware Security", 0, (void*)0 },
    { 25, "System Power Controls", 0, (void*)0 },
    { 26, "Voltage Probe", 0, (void*)0 },
    { 27, "Cooling Device", 0, (void*)0 },
    { 28, "Temperature Probe", 0, (void*)0 },
    { 29, "Electrical Current Probe", 0, (void*)0 },
    { 30, "Out-of-Band Remote Access", 0, (void*)0 },
    { 31, "oot Integrity Services (BIS) Entry Point", 0, (void*)0 },
    { 32, "System Boot Information", 0, (void*)0 },
    { 33, "64-Bit Memory Error Information", 0, (void*)0 },
    { 34, "Management Device", 0, (void*)0 },
    { 35, "Management Device Component", 0, (void*)0 },
    { 36, "Management Device Threshold Data", 0, (void*)0 },
    { 37, "Memory Channel", 0, (void*)0 },
    { 38, "IPMI Device Information", 0, (void*)0 },
    { 39, "System Power Supply", 0, (void*)0 },
    { 40, "Additional Information", 0, (void*)0 },
    { 41, "Onboard Devices Extended Information", 0, (void*)0 },
    { 42, "Management Controller Host Interface", 0, (void*)0 },
    { 43, "TPM Device", 0, (void*)0 },
    { 44, "Processor Additional Information", 0, (void*)0 },
    { 45, "Firmware Inventory Information", 0, (void*)0 },
    { 46, "String Property", 0, (void*)0 },
    { 126, "Inactive", 0, (void*)0 },
    { 127, "End-of-Table", 0, (void*)0 },
};

#endif

#pragma endregion

#ifdef _MSC_VER
#pragma warning(default: 4200)
#else
#pragma pop_macro("_Field_size_")
#pragma pop_macro("_Field_size_bytes_")
#endif
