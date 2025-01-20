#include "SMBIOS.h"

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
#define SubPtr(B,O) ((unsigned int)((size_t)(O) - (size_t)(B))) // aka PtrOffset
#ifndef NULL
#define NULL ((void *)0)
#endif

static const char* g_Strings[UCHAR_MAX];

PSMBIOS_TABLE
PrintSmbiosTable(
    PSMBIOS_TABLE Table,
    void* StartOfData,
    void* EndOfData)
{
    PSMBIOS_TABLE NextTable;
    void* EndOfTable;
    unsigned short i;
    unsigned char *psz, StringCount;
    size_t MaxSize, Length;
    PSMBIOS_TABLE_TYPE_INFO TypeInfo;

    /* Get type information */
    TypeInfo = NULL;
    for (i = 0; i < _countof(SmbiosTableTypeInfo); i++)
    {
        if (SmbiosTableTypeInfo[i].Type == Table->Header.Type)
        {
            TypeInfo = &SmbiosTableTypeInfo[i];
            break;
        }
    }

    /* Build string index array and locate next table address */
    psz = AddPtr(Table, Table->Header.Length);
    StringCount = 0;
    NextTable = NULL;
    while (true)
    {
        if (psz + 2 >= (unsigned char*)EndOfData)
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
    printf("Handle: %hu, Offset: 0x%08lX, Length: 0x%hhX bytes, Total: 0x%lX bytes\n",
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
        printf("\t%hs: ", TypeInfo->Fields[i].Name);
        if (TypeInfo->Fields[i].Type == SmbiosDataTypeString)
        {
            BYTE Index = *(BYTE*)AddPtr(Table, TypeInfo->Fields[i].Offset);
            if (Index != 0 && Index <= StringCount)
            {
                puts(g_Strings[Index - 1]);
            } else
            {
                putchar('\n');
            }
        }
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
    void* EndOfData;

    /* SMBIOS use UTF-8 strings */
    setlocale(LC_ALL, "UTF-8");
    if (!GetSmbiosTableData(&Data, &DataSize))
    {
        puts("Get SMBIOS table failed\n");
        return ENODATA;
    }

    /* SMBIOS information */
    printf("SMBIOS Version: %lu.%lu\n", Data->SMBIOSMajorVersion, Data->SMBIOSMinorVersion);
    printf("DMI Revision: %lu\n", Data->DmiRevision);
    printf("Data Size: %lu bytes\n\n", Data->Length);
    EndOfData = AddPtr(Data->SMBIOSTableData, Data->Length);

    /* Print all tables */
    Table = (PSMBIOS_TABLE)Data->SMBIOSTableData;
    do
    {
        Table = PrintSmbiosTable(Table, Data->SMBIOSTableData, EndOfData);
    } while (Table != NULL);

    /* Cleanup and exit */
    free(Data);
    return 0;
}
