#include "font_linux.hpp"
#include <stdexcept>

#ifdef __APPLE__
#include <CoreText/CoreText.h>
#include <iostream>
#include <map>

namespace fontlist {

static int trans_weight(float weight) {
    // my subjective mapping
    if (weight < 0)
        return 400 + 250 * weight;
    else
        return 400 + weight / 0.0014;
}

static std::string to_string(CFStringRef str) {
    char buf[256];
    if (!CFStringGetCString(str, buf, sizeof(buf), kCFStringEncodingUTF8))
        throw std::runtime_error("fontlist: failed to run CFStringGetCStringPtr()");
    return buf;
}

static std::string getFontAttributeStr(CTFontDescriptorRef descriptor, CFStringRef attr) {
    auto fontFamilyName = (CFStringRef)CTFontDescriptorCopyAttribute(descriptor, attr);
    if (!fontFamilyName)
        throw std::runtime_error("fontlist: failed to run CTFontDescriptorCopyAttribute()");
    std::string str = to_string(fontFamilyName);
    CFRelease(fontFamilyName);
    return str;
}

static std::string getFontAttributePath(CTFontDescriptorRef descriptor, CFStringRef attr) {
    CFURLRef path = (CFURLRef)CTFontDescriptorCopyAttribute(descriptor, kCTFontURLAttribute);
    if (!path)
        throw std::runtime_error("fontlist: failed to run CTFontDescriptorCopyAttribute()");
    char pathBuffer[1024];
    if (!CFURLGetFileSystemRepresentation(path, true, (UInt8 *)pathBuffer, sizeof(pathBuffer)))
        throw std::runtime_error("fontlist: failed to run CFURLGetFileSystemRepresentation()");
    CFRelease(path);
    return pathBuffer;
}

std::vector<fontfamily> enumerate_font_macos_coretext() {
    std::vector<fontfamily> font_array;

    auto fontcollection = CTFontCollectionCreateFromAvailableFonts(nullptr);
    auto fontdescriptors = CTFontCollectionCreateMatchingFontDescriptors(fontcollection);
    size_t count = CFArrayGetCount(fontdescriptors);

    std::map<std::string, fontfamily> fontfamily_map;

    for (size_t i = 0; i < count; i++) {
        font fontDat;

        CTFontDescriptorRef descriptor = static_cast<CTFontDescriptorRef>(CFArrayGetValueAtIndex(fontdescriptors, i));
        fontDat.file = getFontAttributePath(descriptor, kCTFontURLAttribute);
        std::string fontFamilyName = getFontAttributeStr(descriptor, kCTFontFamilyNameAttribute);

        CFDictionaryRef dict = (CFDictionaryRef)CTFontDescriptorCopyAttribute(descriptor, kCTFontTraitsAttribute);
        if (!dict)
            throw std::runtime_error("fontlist: failed to run CTFontDescriptorCopyAttribute()");

        CFNumberRef weight = (CFNumberRef)CFDictionaryGetValue(dict, kCTFontWeightTrait);
        float weightNum;
        CFNumberGetValue(weight, kCFNumberFloatType, &weightNum);
        fontDat.weight = trans_weight(weightNum);
        CFRelease(weight);

        CFNumberRef symbolic = (CFNumberRef)CFDictionaryGetValue(dict, kCTFontSymbolicTrait);
        long long symbolicNum;
        CFNumberGetValue(symbolic, kCFNumberLongLongType, &symbolicNum);
        fontDat.style = (symbolicNum & kCTFontTraitItalic) ? fontstyle::italic : fontstyle::normal;
        CFRelease(symbolic);

        CFRelease(dict);

        fontfamily_map[fontFamilyName].name = fontFamilyName;
        fontfamily_map[fontFamilyName].fonts.push_back(fontDat);
    }

    for (auto &[name, family] : fontfamily_map) {
        font_array.push_back(std::move(family));
    }

    return font_array;
}

} // namespace fontlist
#endif
