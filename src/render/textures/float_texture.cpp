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

#include <akari/core/plugin.h>
#include <akari/core/spectrum.h>
#include <akari/render/texture.h>
#include <akari/plugins/float_texture.h>

namespace akari {
    class FloatTexture final : public Texture {
      public:
        [[refl]] Float value = 1;
        FloatTexture() = default;
        explicit FloatTexture(Float value) : value(value) {}
        AKR_IMPLS(Texture)
        Spectrum evaluate(const ShadingPoint &sp) const override { return Spectrum(value); }
        Float average_luminance() const override { return value; }
    };

    AKR_EXPORT std::shared_ptr<Texture> create_float_texture(Float v) { return std::make_shared<FloatTexture>(v); }
#include "generated/FloatTexture.hpp"
    AKR_EXPORT_PLUGIN(p) {

    }
} // namespace akari