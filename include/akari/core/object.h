// MIT License
//
// Copyright (c) 2020 椎名深雪
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

#ifndef AKARIRENDER_OBJECT_H
#define AKARIRENDER_OBJECT_H

#include <akari/core/class.h>
#include <akari/core/platform.h>
namespace akari {

    class AKR_EXPORT Object {
      public:
        [[nodiscard]] virtual Class *GetClass() const = 0;
        virtual ~Object()=default;
    };
    namespace serialize {
        class OutputArchive;
        class InputArchive;
    }
    class AKR_EXPORT Serializable : public Object {
      public:
        template <class Archive> void save(Archive &) const {}

        template <class Archive> void load(Archive &) {}
    };

} // namespace akari

#endif // AKARIRENDER_OBJECT_H
