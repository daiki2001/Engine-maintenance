#include "LoadTTF.h"
#include "./Header/Error.h"

namespace
{
static uint8_t tagInfo[4] = {};
}

namespace Engine
{
namespace Loader
{
const std::string LoadTTF::defaultFont = "./lib/FiraCode-Regular.ttf";

LoadTTF::LoadTTF() :
    tableDirectory{}
{
}

LoadTTF::~LoadTTF()
{
}

uint8_t LoadTTF::Read8(std::ifstream* file)
{
    if (!file)
    {
        return ErrorLog("フォントファイルの読み込みに失敗しました。", true);
    }

    uint8_t o = 0;
    file->read((char*)&o, sizeof(uint8_t));

    if (!file)
    {
        return ErrorLog("フォントファイルの読み込みに失敗しました。", true);
    }

    return o;
}

uint16_t LoadTTF::Read16(std::ifstream* file)
{
    if (!file)
    {
        return ErrorLog("フォントファイルの読み込みに失敗しました。", true);
    }

    uint8_t o[2] = {};
    for (size_t i = 0; i < 2; i++)
    {
        o[i] = Read8(file);
    }

    return static_cast<uint16_t>(o[0] << 8 | o[1]);
}

uint32_t LoadTTF::Read32(std::ifstream* file)
{
    if (!file)
    {
        return ErrorLog("フォントファイルの読み込みに失敗しました。", true);
    }

    uint8_t o[4] = {};
    for (size_t i = 0; i < 4; i++)
    {
        o[i] = Read8(file);
    }

    return static_cast<uint32_t>(o[0] << 24 | o[1] << 16 | o[2] << 8 | o[3]);
}

void LoadTTF::TagInfo(const LoadTTF::Tag& tag, uint8_t info[4])
{
    info[0] = static_cast<uint8_t>((tag >> 24) & 0xFF);
    info[1] = static_cast<uint8_t>((tag >> 16) & 0xFF);
    info[2] = static_cast<uint8_t>((tag >> 8) & 0xFF);
    info[3] = static_cast<uint8_t>(tag & 0xFF);
}

void LoadTTF::LoadFont(const std::string& filePath)
{
    using namespace std;

    string fontFile = filePath;
    if (filePath.empty())
    {
        fontFile = defaultFont;
    }

    ifstream file(fontFile, ios::in | ios::binary);
    if (!file)
    {
        ErrorLog("フォントファイルの読み込みに失敗しました。", true);
        return;
    }

    tableDirectory.sfntVersion = Read32(&file);
    tableDirectory.numTables = Read16(&file);
    tableDirectory.searchRange = Read16(&file);
    tableDirectory.entrySelector = Read16(&file);
    tableDirectory.rangeShift = Read16(&file);
    tableDirectory.tableRecords.resize(tableDirectory.numTables);
    for (uint16_t i = 0; i < tableDirectory.tableRecords.size(); i++)
    {
        tableDirectory.tableRecords[i] = ReadTableRecord(&file);
    }

    for (size_t i = 0; i < tableDirectory.tableRecords.size(); i++)
    {
        TagInfo(tableDirectory.tableRecords[i].tableTag, tagInfo);
        if (tableDirectory.tableRecords[i].tableTag == 'DSIG')
        {
            ReadDSIG(&file, tableDirectory.tableRecords[i]);
        }
    }
}

LoadTTF::TableRecord LoadTTF::ReadTableRecord(std::ifstream* file)
{
    if (!file)
    {
        ErrorLog("フォントファイルの読み込みに失敗しました。", true);
        return TableRecord{};
    }

    TableRecord o = {};
    o.tableTag = Read32(file);
    o.checksum = Read32(file);
    o.offset = Read32(file);
    o.length = Read32(file);
    return o;
}

void LoadTTF::ReadDSIG(std::ifstream* file, const TableRecord& tableRecord)
{
    return;
    uint32_t version = 0;       //Version number of the DSIG table (0x00000001)
    uint16_t numSignatures = 0; //Number of signatures in the table
    uint16_t flags = 0;         //permission flags\
                                  Bit 0: cannot be resigned\
                                  Bits 1 - 7 : Reserved(Set to 0)
}
} //Loader
} //Engine
