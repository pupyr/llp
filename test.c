#include <windows.h>
#include <winreg.h>
#include <stdio.h>
#include <inttypes.h>
#include <stddef.h>
#include "client.h"

#define REG_ADD_1 HKEY_CURRENT_USER
#define R_PATH "Software\\Microsoft\\Windows\\CurrentVersion\\"
#define MAX_DATA_LENGTH 16383

typedef struct {
    DWORD type;
    DWORD length;
    union {
        unsigned char lpData[MAX_DATA_LENGTH];
        int32_t asInt32;
        int64_t asInt64;
    };
} ValueInfo;

typedef void(*childKeyProc)(HKEY parentKey, char *subkeyName, void* state);
typedef void(*childValueProc)(char *valueName, ValueInfo* value, void* state);

void forEachValue(HKEY key, childValueProc proc, void* state)
{
    ValueInfo value;

    TCHAR lpValueName[16383];
    DWORD lpcchValueName = 16383;
    long index = 0;
    value.length = MAX_DATA_LENGTH;
    memset(value.lpData, 0, MAX_DATA_LENGTH);
    long RC = RegEnumValue(key, index, lpValueName, &lpcchValueName, NULL, &value.type, value.lpData, &value.length);
    if (RC == ERROR_SUCCESS) {
        while (RC == ERROR_SUCCESS) {
            proc(lpValueName, &value, state);
            index++;
            lpcchValueName = 16383;
            value.length = MAX_DATA_LENGTH;
            memset(value.lpData, 0, MAX_DATA_LENGTH);
            RC = RegEnumValue(key, index, lpValueName, &lpcchValueName, NULL, &value.type, value.lpData, &value.length);
        }
    } else {
        value.length = MAX_DATA_LENGTH;
        memset(value.lpData, 0, MAX_DATA_LENGTH);
        RC = RegGetValue(key, NULL, NULL, RRF_RT_ANY,  &value.type, value.lpData, &value.length);
        if (RC == ERROR_SUCCESS) {
            proc(NULL, &value, state);
        }
    }
}

void forEachSubkey(HKEY key, childKeyProc proc, void* state)
{
    long index = 0;

    char name[255];
    DWORD TMP=255;

    long RC = RegEnumKeyEx (key,index,name,&TMP,0,NULL,NULL,NULL);
    while (RC == ERROR_SUCCESS) {
        proc(key, name, state);
        index++;
        TMP = 255;
        RC = RegEnumKeyEx (key,index,name,&TMP,0,NULL,NULL,NULL);
    }
}

typedef struct {
    int depth;
} traverseState;

void printIndent(traverseState *state)
{
    for (int i = 0; i < state->depth; i++)
        printf("  ");
}

void printValueProc(char *valueName, ValueInfo* value, traverseState* state)
{
    printIndent(state);
    printf("%s = ", strlen(valueName) == 0 ? "(Default)" : valueName);
    switch (value->type) {
        case REG_DWORD:
        case REG_DWORD_BIG_ENDIAN:
        // case REG_DWORD_LITTLE_ENDIAN:
            printf ("%d", value->asInt32);
            break;
        case REG_QWORD:
        // case REG_QWORD_LITTLE_ENDIAN:
            printf ("%ul", value->asInt64);
            break;
        case REG_EXPAND_SZ:
        case REG_LINK:
        case REG_SZ:
            printf ("\"%s\"", value->lpData);
            break;
        case REG_MULTI_SZ:
        case REG_BINARY:
        case REG_NONE:
        default:
            printf("<UNSUPPORTED TYPE>");
            break;
    }
    printf("\n");
}

void printNodeName(HKEY parentKey, char *subkeyName, traverseState* state)
{
    printIndent(state);

    printf("%s\\\n", subkeyName);

    traverseState s = { state->depth + 1};

    HKEY subkey;
    long  rc = RegOpenKeyEx(parentKey,subkeyName,0,KEY_ALL_ACCESS, &subkey);
    if (rc == ERROR_SUCCESS) {
        forEachValue(subkey, (childValueProc)printValueProc, &s);
        forEachSubkey(subkey, (childKeyProc)printNodeName, &s);
        RegCloseKey(subkey);
    }
}

int main ()
{
    traverseState s = { 0 };
    forEachSubkey(REG_ADD_1, (childKeyProc)&printNodeName, &s);
    return 0;

}
