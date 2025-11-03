#include "../headers/utils/windoes_utils.h"


// Detect if byte stream looks like UTF-16 (lots of nulls)
static bool LooksLikeUTF16(const char* data, size_t len) {
    int nullCount = 0;
    for (size_t i = 0; i < len; ++i)
        if (data[i] == '\0') nullCount++;
    return (double)nullCount / len > 0.3;  // heuristic
}

// ---------------------------------------------------------------------------
// File Open Dialog (Wide API)
// ---------------------------------------------------------------------------
std::wstring winUtil::open_file_dialog(const wchar_t* filter)
{
    OPENFILENAMEW ofn{};
    wchar_t fileName[MAX_PATH] = L"";

    ofn.lStructSize = sizeof(ofn);
    ofn.lpstrFilter = filter;
    ofn.lpstrFile = fileName;
    ofn.nMaxFile = MAX_PATH;
    ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
    ofn.lpstrDefExt = nullptr;

    if (GetOpenFileNameW(&ofn))
        return std::wstring(fileName);

    return L"";
}

// ---------------------------------------------------------------------------
// Convert Shift-JIS → UTF-8
// ---------------------------------------------------------------------------
std::string winUtil::ShiftJISToUTF8(const char* sjisStr)
{
    if (!sjisStr || !*sjisStr)
        return "";

    int wideLen = MultiByteToWideChar(932, 0, sjisStr, -1, nullptr, 0);  // 932 = Shift-JIS
    if (wideLen == 0)
        return "";

    std::wstring wideStr(wideLen, L'\0');
    MultiByteToWideChar(932, 0, sjisStr, -1, &wideStr[0], wideLen);

    int utf8Len = WideCharToMultiByte(CP_UTF8, 0, wideStr.c_str(), -1, nullptr, 0, nullptr, nullptr);
    if (utf8Len == 0)
        return "";

    std::string utf8Str(utf8Len, '\0');
    WideCharToMultiByte(CP_UTF8, 0, wideStr.c_str(), -1, &utf8Str[0], utf8Len, nullptr, nullptr);

    if (!utf8Str.empty() && utf8Str.back() == '\0')
        utf8Str.pop_back();

    return utf8Str;
}

// ---------------------------------------------------------------------------
// Decode PMX texture names (handles UTF-16 or Shift-JIS)
// ---------------------------------------------------------------------------
std::string winUtil::DecodePmxTextureName(const aiString& aiStr)
{
    const char* bytes = aiStr.C_Str();
    size_t len = aiStr.length;

    if (len == 0)
        return "";

    // --- Case 1: UTF-16LE ---
    if (LooksLikeUTF16(bytes, len)) {
        const wchar_t* wstr = reinterpret_cast<const wchar_t*>(bytes);
        int wcharCount = static_cast<int>(len / 2);

        int utf8Len = WideCharToMultiByte(CP_UTF8, 0, wstr, wcharCount, nullptr, 0, nullptr, nullptr);
        std::string utf8Str(utf8Len, 0);
        WideCharToMultiByte(CP_UTF8, 0, wstr, wcharCount, &utf8Str[0], utf8Len, nullptr, nullptr);
        return utf8Str;
    }

    // --- Case 2: Shift-JIS ---
    return ShiftJISToUTF8(bytes);
}
