#include "SMBIOS.h"
#include "SMBIOS.TypeInfo.h"

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <locale.h>

bool
GetSmbiosTableData(
    PSMBIOS_RAW_DATA* Data,
    unsigned int* DataSize);

#if defined(_WINDOWS)

#include <Windows.h>

bool
GetSmbiosTableData(
    PSMBIOS_RAW_DATA* Data,
    unsigned int* DataSize)
{
    UINT BufferSize, BytesWritten;
    PVOID Buffer;
    DWORD Error;

    BufferSize = GetSystemFirmwareTable('RSMB', 0, NULL, 0);
    if (BufferSize == 0)
    {
        printf("GetSystemFirmwareTable failed with: 0x%08lX\n", GetLastError());
        return false;
    }
    Buffer = malloc(BufferSize);
    if (Buffer == NULL)
    {
        printf("malloc failed to allocate %lu bytes buffer\n", BufferSize);
        return false;
    }
    BytesWritten = GetSystemFirmwareTable('RSMB', 0, Buffer, BufferSize);
    if (BytesWritten == 0)
    {
        Error = GetLastError();
    } else if (BytesWritten != BufferSize)
    {
        Error = ERROR_INVALID_DATA;
    } else
    {
        *Data = Buffer;
        *DataSize = BufferSize;
        return true;
    }

    printf("GetSystemFirmwareTable failed with: 0x%08lX\n", Error);
    return false;
}

#elif defined(_LINUX)

#error Not implemented

#else

#error No OS target specified, currently supports Windows (_WINDOWS) and Linux (_LINUX)

#endif

#define AddPtr(P, I) ((void*)((unsigned char*)(P) + (I))) // aka Add2Ptr
#define SubPtr(B, O) ((unsigned int)((size_t)(O) - (size_t)(B))) // aka PtrOffset
#ifndef NULL
#define NULL ((void *)0)
#endif

static const char* g_Strings[UCHAR_MAX] = { 0 };

static
PSMBIOS_TABLE
PrintSmbiosTable(
    PSMBIOS_TABLE Table,
    void* StartOfData,
    void* EndOfData)
{
    PSMBIOS_TABLE NextTable;
    void* EndOfTable;
    WORD i;
    const char *psz;
    BYTE StringCount;
    size_t MaxSize, Length;
    PSMBIOS_TYPE_INFO TypeInfo;
    QWORD BitFieldValue, Value;
    WORD BitFieldSize = 0, ValueSize;

    /* Get type information */
    TypeInfo = NULL;
    for (i = 0; i < _countof(SmbiosTypeInfo); i++)
    {
        if (SmbiosTypeInfo[i].Type == Table->Header.Type)
        {
            TypeInfo = &SmbiosTypeInfo[i];
            break;
        }
    }

    /* Build string index array and locate next table address */
    psz = AddPtr(Table, Table->Header.Length);
    StringCount = 0;
    NextTable = NULL;
    while (true)
    {
        if (psz + 2 >= (const char*)EndOfData)
        {
            break;
        }
        if (psz[0] == '\0' && psz[1] == '\0')
        {
            NextTable = (PSMBIOS_TABLE)(psz + 2);
            break;
        }

        MaxSize = SubPtr(psz, EndOfData);
        Length = strnlen(psz, MaxSize);
        if (Length == MaxSize)
        {
            break;
        }
        g_Strings[StringCount++] = psz;
        psz += Length + 1;
        if (psz[0] == '\0')
        {
            NextTable = (PSMBIOS_TABLE)(psz + 1);
            break;
        }
    }
    if ((void*)NextTable >= EndOfData)
    {
        NextTable = NULL;
    }
    EndOfTable = NextTable == NULL ? EndOfData : NextTable;

    /* Print table header */
    if (TypeInfo != NULL)
    {
        printf("[Type %hhu: %hs]\n", Table->Header.Type, TypeInfo->Name);
    } else
    {
        printf("[Type %hhu (Unrecognized)]\n", Table->Header.Type);
    }
    printf("Handle: 0x%04hX, Offset: 0x%08lX, Length: 0x%02hhX bytes, Total: 0x%lX bytes\n",
           Table->Header.Handle,
           SubPtr(StartOfData, Table),
           Table->Header.Length,
           SubPtr(Table, EndOfTable));

    /* Print table fields */
    if (TypeInfo == NULL)
    {
        goto _Exit;
    }
    for (i = 0; i < TypeInfo->FieldCount; i++)
    {
        Value = 0;
        if (TypeInfo->Fields[i].IsBitField)
        {
            if (BitFieldSize == 0)
            {
                if (i == 0 ||
                    TypeInfo->Fields[i - 1].IsBitField ||
                    TypeInfo->Fields[i - 1].Type != SmbiosDataTypeUInt)
                {
                    break;
                }
                BitFieldSize = TypeInfo->Fields[i - 1].Size;
                BitFieldValue = 0;
                memcpy(&BitFieldValue, AddPtr(Table, TypeInfo->Fields[i - 1].Offset), TypeInfo->Fields[i - 1].Size);
            }
            putchar('\t');
            if (TypeInfo->Fields[i].Type == SmbiosDataTypeBit)
            {
                printf("%02hhu [%hc] %hs",
                       TypeInfo->Fields[i].Offset,
                       _bittest64(&BitFieldValue, TypeInfo->Fields[i].Offset) ? 'x' : ' ',
                       TypeInfo->Fields[i].Name);
            } else if (TypeInfo->Fields[i].Type == SmbiosDataTypeUInt || TypeInfo->Fields[i].Type == SmbiosDataTypeEnum)
            {
                printf("%02hhu:%02hhu %hs: ",
                       TypeInfo->Fields[i].Offset,
                       TypeInfo->Fields[i].Offset + TypeInfo->Fields[i].Size - 1,
                       TypeInfo->Fields[i].Name);
                Value = BitFieldValue;
                Value >>= TypeInfo->Fields[i].Offset;
                Value &= ((QWORD)1 << TypeInfo->Fields[i].Size) - 1;
            }
        } else
        {
            if (TypeInfo->Fields[i].Offset + TypeInfo->Fields[i].Size > Table->Header.Length)
            {
                break;
            }
            BitFieldSize = 0;
            printf("0x%04hX %hs: ", TypeInfo->Fields[i].Offset, TypeInfo->Fields[i].Name);
        }

        if (TypeInfo->Fields[i].Type == SmbiosDataTypeString && TypeInfo->Fields[i].Size == sizeof(BYTE))
        {
            BYTE Index = *(BYTE*)AddPtr(Table, TypeInfo->Fields[i].Offset);
            if (Index != 0 && Index <= StringCount)
            {
                printf("0x%02hhX \"%hs\"", Index, g_Strings[(BYTE)(Index - 1)]);
            }
        } else if (TypeInfo->Fields[i].Type == SmbiosDataTypeUInt || TypeInfo->Fields[i].Type == SmbiosDataTypeEnum)
        {
            if (TypeInfo->Fields[i].IsBitField)
            {
                ValueSize = BitFieldSize;
            } else
            {
                Value = 0;
                memcpy(&Value, AddPtr(Table, TypeInfo->Fields[i].Offset), TypeInfo->Fields[i].Size);
                ValueSize = TypeInfo->Fields[i].Size;
            }
            if (ValueSize == sizeof(BYTE))
            {
                printf("0x%02hhX", (BYTE)Value);
            } else if (ValueSize == sizeof(WORD))
            {
                printf("0x%04hX", (WORD)Value);
            } else if (ValueSize == sizeof(DWORD))
            {
                printf("0x%08lX", (DWORD)Value);
            } else if (ValueSize == sizeof(QWORD))
            {
                printf("0x%016llX", Value);
            }
            if (TypeInfo->Fields[i].Type == SmbiosDataTypeEnum)
            {
                WORD j;
                for (j = 0; j < TypeInfo->Fields[i].AdditionalInfo.Enum.Count; j++)
                {
                    if (TypeInfo->Fields[i].AdditionalInfo.Enum.Values[j].Value == Value)
                    {
                        printf(" (%hs)", TypeInfo->Fields[i].AdditionalInfo.Enum.Values[j].Name);
                        break;
                    }
                }
            }
        } else if (TypeInfo->Fields[i].Type == SmbiosDataTypeRaw || TypeInfo->Fields[i].Type == SmbiosDataTypeOther)
        {
            BYTE j, *p;
            p = AddPtr(Table, TypeInfo->Fields[i].Offset);
            for (j = 0; j < TypeInfo->Fields[i].Size; j++)
            {
                printf("%02X", p[j]);
                if (j != TypeInfo->Fields[i].Size - 1)
                {
                    putchar(' ');
                }
            }
        } else if (TypeInfo->Fields[i].Type == SmbiosDataTypeUuid && TypeInfo->Fields[i].Size == 16)
        {
            PBYTE Uuid = (PBYTE)AddPtr(Table, TypeInfo->Fields[i].Offset);
            printf("%02X%02X%02X%02X-%02X%02X-%02X%02X-%02X%02X-%02X%02X%02X%02X%02X%02X",
                   Uuid[0], Uuid[1], Uuid[2], Uuid[3], Uuid[4], Uuid[5], Uuid[6], Uuid[7],
                   Uuid[8], Uuid[9], Uuid[10], Uuid[11], Uuid[12], Uuid[13], Uuid[14], Uuid[15]);
        }
        putchar('\n');
    }

_Exit:
    putchar('\n');
    return NextTable;
}

int
main()
{
    PSMBIOS_RAW_DATA Data;
    unsigned int DataSize;
    PSMBIOS_TABLE Table;

#ifdef _WINDOWS
    SetConsoleOutputCP(CP_UTF8);
#endif
    setlocale(LC_ALL, ".UTF-8");

    if (!GetSmbiosTableData(&Data, &DataSize))
    {
        puts("Get SMBIOS table failed\n");
        return ENODATA;
    }
    printf("SMBIOS Version: %hhu.%hhu\n", Data->SMBIOSMajorVersion, Data->SMBIOSMinorVersion);
    printf("DMI Revision: %hhu\n", Data->DmiRevision);
    printf("Data Size: %lu bytes\n\n", Data->Length);
    Table = (PSMBIOS_TABLE)Data->SMBIOSTableData;
    do
    {
        Table = PrintSmbiosTable(Table, Data->SMBIOSTableData, AddPtr(Data->SMBIOSTableData, Data->Length));
    } while (Table != NULL);
    free(Data);
    return 0;
}
