#pragma once
#include <fstream>
#include <string>
#include <array>
#include <vector>

//uint8;          //8-bit unsigned integer.
//int8;           //8-bit signed integer.
//uint16;         //16-bit unsigned integer.
//int16;          //16-bit signed integer.
//uint24;         //24-bit unsigned integer.
//uint32;         //32-bit unsigned integer.
//int32;          //32-bit unsigned integer.
//Fixed;          //32-bit signed fixed-point number (16.16)
//FWORD;          //int16 that describes a quantity in font design units.
//UFWORD;         //uint16 that describes a quantity in font design units.
//F2DOT14;        //16-bit signed fixed number with the low 14 bits of fraction (2.14).
//LONGDATETIME;   //Date and time represented in number of seconds since 12:00 midnight, January 1, 1904, UTC. The value is represented as a signed 64-bit integer.
//Tag;            //Array of four uint8s (length = 32 bits) used to identify a table, design-variation axis, script, language system, feature, or baseline
//Offset16;       //Short offset to a table, same as uint16, NULL offset = 0x0000
//Offset24;       //24-bit offset to a table, same as uint24, NULL offset = 0x000000
//Offset32;       //Long offset to a table, same as uint32, NULL offset = 0x00000000
//Version16Dot16; //Packed 32-bit value with major and minor version numbers. (See Table Version Numbers.)

namespace Engine
{
namespace Loader
{
class LoadTTF
{
public: //定数
    static const std::string defaultFont;

public: //エイリアス
    using Tag = uint32_t;

public: //サブクラス
    struct TableRecord
    {
        Tag tableTag = {};     //Table identifier.
        uint32_t checksum = 0; //Checksum for this table.
        uint32_t offset = 0;   //Offset from beginning of font file.
        uint32_t length = 0;   //Length of this table.
    };

    struct TableDirectory
    {
        uint32_t sfntVersion;                       //0x00010000 or 0x4F54544F ('OTTO') — see below.
        uint16_t numTables;                         //Number of tables.
        uint16_t searchRange;                       //Maximum power of 2 less than or equal to numTables, times 16 ((2**floor(log2(numTables))) * 16, where “**” is an exponentiation operator).
        uint16_t entrySelector;                     //Log2 of the maximum power of 2 less than or equal to numTables (log2(searchRange/16), which is equal to floor(log2(numTables))).
        uint16_t rangeShift;                        //numTables times 16, minus searchRange ((numTables * 16) - searchRange).
        std::vector<TableRecord> tableRecords = {}; //Table records array—one for each top-level table in the font
    };

public: //静的メンバ関数
    // 8Bit読み取り
    static uint8_t Read8(std::ifstream* file);
    // 16Bit読み取り
    static uint16_t Read16(std::ifstream* file);
    // 32Bit読み取り
    static uint32_t Read32(std::ifstream* file);

    static void TagInfo(const LoadTTF::Tag& tag, uint8_t info[4]);

public: //メンバ関数
    LoadTTF();
    ~LoadTTF();

    // フォントファイルの読み込み
    void LoadFont(const std::string& filePath = "");
    // TableRecordの読み取り
    TableRecord ReadTableRecord(std::ifstream* file);

    // DSIGの読み取り
    void ReadDSIG(std::ifstream* file, const TableRecord& tableRecord);

private: //メンバ変数
    TableDirectory tableDirectory;
};
} //Loader
} //Engine
