#include "replayfile.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

ReplayFile::ReplayFile(QString replayPath)
{
    QByteArray bytes = replayPath.toUtf8();
    m_replayPath = (char*)malloc(bytes.length());
    strcpy(m_replayPath, bytes.constData());
}

ReplayFile::~ReplayFile()
{
    free(m_replayPath);
}

typedef struct
{
    char h[2];
    int16_t version;

    struct
    {
        int32_t length;
        int32_t type;
    } map;

    char data[0];
}

WAgame;

bool ReplayFile::upload(QString path)
{
    int len = sizeof(WAgame);
    void *buffer = malloc(33554432); // 32M
    WAgame *wg = (WAgame*)buffer;

    FILE *rf = fopen(m_replayPath, "rb");
    if(!rf)
    {
        free(buffer);
        return false;
    }

    const char *p = path.toUtf8().constData();
    FILE *mf = fopen(p, "rb");
    if(!mf)
    {
        fclose(rf);
        free(buffer);
        return false;
    }

    fread(buffer, len, 1, rf);

    fseek(mf, 0, SEEK_END);
    long fl = ftell(mf);
    fseek(mf, 0, SEEK_SET);

    len += fl;
    fread((void*)wg->data, fl, 1, mf);
    fclose(mf);

    fseek(rf, wg->map.length - 4, SEEK_CUR);
    long restDataPos = ftell(rf);
    fseek(rf, 0, SEEK_END);
    long restDataLen = ftell(rf) - restDataPos;
    len += restDataLen;
    fseek(rf, restDataPos, SEEK_SET);
    fread((void*)((long)buffer + sizeof(WAgame) + fl), restDataLen, 1, rf);
    fclose(rf);

    wg->map.length = fl + 4;
    if(path.endsWith(".png"))
        wg->map.type = MapType::Png;
    else if(path.endsWith(".bit"))
        wg->map.type = MapType::Bit;
    else
        wg->map.type = MapType::Lev;

    rf = fopen(m_replayPath, "wb");
    if(!rf)
    {
        free(buffer);
        return false;
    }

    fwrite(buffer, len, 1, rf);
    fclose(rf);

    free(buffer);
    return true;
}
