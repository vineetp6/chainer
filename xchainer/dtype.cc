#include "xchainer/dtype.h"

#include <cstring>
#include <ostream>
#include <sstream>

namespace xchainer {

std::ostream& operator<<(std::ostream& os, Dtype dtype) { return os << GetDtypeName(dtype); }

const char* GetDtypeName(Dtype dtype) {
    switch (dtype) {
        case Dtype::kBool:
            return "bool";
        case Dtype::kInt8:
            return "int8";
        case Dtype::kInt16:
            return "int16";
        case Dtype::kInt32:
            return "int32";
        case Dtype::kInt64:
            return "int64";
        case Dtype::kUInt8:
            return "uint8";
        case Dtype::kFloat32:
            return "float32";
        case Dtype::kFloat64:
            return "float64";
    }
    throw DtypeError("invalid dtype");
}

Dtype GetDtype(const std::string& name) {
    // We define an ad-hoc POD struct to comply with the coding guideline.
    // Note that std::tuple is not a POD type.
    struct Pair {
        const char* name;
        Dtype dtype;
    };

    static const Pair kMapping[] = {
        // full name
        {"bool", Dtype::kBool},
        {"int8", Dtype::kInt8},
        {"int16", Dtype::kInt16},
        {"int32", Dtype::kInt32},
        {"int64", Dtype::kInt64},
        {"uint8", Dtype::kUInt8},
        {"float32", Dtype::kFloat32},
        {"float64", Dtype::kFloat64},
        // character code
        {"?", Dtype::kBool},
        {"b", Dtype::kInt8},
        {"h", Dtype::kInt16},
        {"i", Dtype::kInt32},
        {"q", Dtype::kInt64},
        {"B", Dtype::kUInt8},
        {"f", Dtype::kFloat32},
        {"d", Dtype::kFloat64},
    };
    static_assert(std::is_pod<decltype(kMapping)>::value, "static variable must be POD to comply with the coding guideline");

    const char* cname = name.c_str();
    for (const Pair& pair : kMapping) {
        if (0 == std::strcmp(pair.name, cname)) {
            return pair.dtype;
        }
    }
    throw DtypeError("unknown dtype name: \"" + name + '"');
}

std::vector<Dtype> GetAllDtypes() {
    return {
        Dtype::kBool, Dtype::kInt8, Dtype::kInt16, Dtype::kInt32, Dtype::kInt64, Dtype::kUInt8, Dtype::kFloat32, Dtype::kFloat64,
    };
}

void CheckEqual(Dtype lhs, Dtype rhs) {
    if (lhs != rhs) {
        std::ostringstream ss;
        ss << "dtype mismatched: " << lhs << " != " << rhs;
        throw DtypeError(ss.str());
    }
}

}  // namespace xchainer
