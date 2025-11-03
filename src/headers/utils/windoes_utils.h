#pragma once
#include <windows.h>
#include <commdlg.h>
#include <fstream>
#include <string>
#include <vector>
#include <iostream>
#include <assimp/types.h>

class winUtil {
public:
   static std::wstring open_file_dialog(const wchar_t* filter);
   static std::string ShiftJISToUTF8(const char* sjisStr);
   static std::string DecodePmxTextureName(const aiString& aiStr);
};