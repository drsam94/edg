// (c) Samuel Donow 2017
#pragma once
#include<array>
#include<string>
#include<cstddef>
#include<algorithm>

class EnumInternal {
  protected:
    template<size_t N>
    static constexpr std::array<std::string, N> internalGetArr(const char *str) {
        std::array<std::string, N> ret;
        const char *bptr = str;
        const char *eptr = str;
        size_t i = 0;
        while (i < N) {
            while (*bptr == '\n' || *bptr == ' ' || *bptr == ',') bptr++;
            while (eptr <= bptr || (*eptr != '\n' && *eptr != ' '
                                && *eptr != ',' && *eptr != '\0')) eptr++;
            ret[i++] = std::string(bptr, eptr - bptr);
            bptr = eptr;
        }
        return ret;
    }
};

#define ENUM(EnumType, UnderlyingType, ...) \
class EnumType : EnumInternal { \
  public: \
    enum EnumT { __VA_ARGS__, Unset }; \
    static inline const size_t Count = UnderlyingType(EnumT::Unset) + 1; \
  private: \
    UnderlyingType val; \
    static inline const std::array<std::string, Count> _names = internalGetArr<Count>(#__VA_ARGS__); \
  public: \
    EnumType() : val(EnumT::Unset) {} \
    constexpr EnumType(UnderlyingType ul) : val(ul) {} \
    constexpr EnumType(EnumT other) : val(other) {}; \
    operator EnumT() const { return static_cast<EnumT>(val); } \
    explicit operator UnderlyingType() const { return toUnderlying(); } \
    UnderlyingType toUnderlying() const { return val; } \
    static EnumType parse(const std::string &name) { \
        if (auto it = std::find(_names.begin(), _names.end(), name); it != _names.end()) \
            return static_cast<UnderlyingType>(it - _names.begin()); \
        else return EnumT::Unset; \
    } \
    const std::string &str() const { return _names[size_t(toUnderlying())]; } \
    bool valid() const { return static_cast<EnumT>(val) != EnumT::Unset; } \
}; \
namespace std { \
    template <> struct hash<EnumType> { \
        size_t operator()(const EnumType &x) const { \
            return static_cast<size_t>(x.toUnderlying()); \
        } \
    }; \
}
