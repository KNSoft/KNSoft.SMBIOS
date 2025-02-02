#pragma once

#include "SMBIOS.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

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
    const char8_t* Name;
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

#define SMBIOS_DEFINE_FIELD(Type, Name, Field, FieldType, ...) { Name, false, (WORD)offsetof(SMBIOS_TYPE_##Type, Field), (BYTE)sizeof(((SMBIOS_TYPE_##Type*)0)->Field), FieldType, __VA_ARGS__ }
#define SMBIOS_DEFINE_BIT_FIELD(Name, BitOffset, BitSize, FieldType, ...) { Name, true, BitOffset, BitSize, FieldType, __VA_ARGS__ }
#define SMBIOS_FIELD_ENUM_VALUES(EnumName) { _countof(EnumName), EnumName }

#define SMBIOS_DEFINE_FIELD_STRING(Type, Name, Field) SMBIOS_DEFINE_FIELD(Type, Name, Field, SmbiosDataTypeString)
#define SMBIOS_DEFINE_FIELD_UINT(Type, Name, Field) SMBIOS_DEFINE_FIELD(Type, Name, Field, SmbiosDataTypeUInt)
#define SMBIOS_DEFINE_FIELD_ENUM(Type, Name, Field, EnumName) SMBIOS_DEFINE_FIELD(Type, Name, Field, SmbiosDataTypeEnum, SMBIOS_FIELD_ENUM_VALUES(EnumName))
#define SMBIOS_DEFINE_FIELD_BIT(Name, BitOffset) SMBIOS_DEFINE_BIT_FIELD(Name, BitOffset, 1, SmbiosDataTypeBit)

#define SMBIOS_DEFINE_TYPE(Type, Name) { Type, Name, _countof(SmbiosType##Type##FieldInfo), SmbiosType##Type##FieldInfo }
