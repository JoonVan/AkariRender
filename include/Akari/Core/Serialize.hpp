// MIT License
//
// Copyright (c) 2019 椎名深雪
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#ifndef AKARIRENDER_SERIALIZE_HPP
#define AKARIRENDER_SERIALIZE_HPP
#include <Akari/Core/Akari.h>
#include <Akari/Core/Platform.h>
#include <Akari/Core/detail/serialize-impl.hpp>
#include <json.hpp>
namespace Akari {
    class AKR_EXPORT SerializeContext : public Serialize::Context {
      public:
        Class *GetClass(const std::string &s) override;
    };
} // namespace Akari
namespace nlohmann {
    template <> struct adl_serializer<Akari::fs::path> {
        static void from_json(const json &j, Akari::fs::path &path) { path = Akari::fs::path(j.get<std::string>()); }

        static void to_json(json &j, const Akari::fs::path &path) { j = path.string(); }
    };
} // namespace nlohmann
#endif // AKARIRENDER_SERIALIZE_HPP
