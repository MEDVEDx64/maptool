#ifndef REPLAYFILE_H
#define REPLAYFILE_H

#include <QString>

class ReplayFile
{
public:
    ReplayFile(QString replayPath);
    ~ReplayFile();

    enum MapType
    {
        Bit = 1,
        Lev = 2,
        Png = 3,
    };

    // Returns true on success
    bool upload(QString path);
    bool download(QString path);

    MapType getType();

private:
    char *m_replayPath;
};

#endif // REPLAYFILE_H
