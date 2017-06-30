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
    enum class EnumT : UnderlyingType { __VA_ARGS__, Unset }; \
    static inline const size_t Count = UnderlyingType(EnumT::Unset) + 1; \
  private: \
    EnumT val; \
    static inline const std::array<std::string, Count> _names = internalGetArr<Count>(#__VA_ARGS__); \
  public: \
    EnumType() : val(EnumT::Unset) {} \
    EnumType(EnumT other) : val(other) {}; \
    operator EnumT() const { return val; } \
    operator UnderlyingType() const { return toUnderlying(); } \
    UnderlyingType toUnderlying() const { return static_cast<UnderlyingType>(val); } \
    static EnumType parse(const std::string &name) { \
        if (auto it = std::find(_names.begin(), _names.end(), name); it != _names.end()) \
            return static_cast<EnumT>(it - _names.begin()); \
        else return EnumT::Unset; \
    } \
    const std::string &str() const { return _names[size_t(toUnderlying())]; } \
}; \
namespace std { \
    template <> struct hash<EnumType> { \
        size_t operator()(const EnumType &x) const { \
            return static_cast<size_t>(x.toUnderlying()); \
        } \
    }; \
}

#undef _NUM_ARGS
#undef NUM_ARGS
#undef EXPAND
#undef FIRSTARG
#undef RESTARGS
#undef FOREACH
#undef FOREACH_
#undef FOREACH__
#undef FOREACH_1
#undef FOREACH_2
#undef FOREACH_3
#undef STRINGIFY
