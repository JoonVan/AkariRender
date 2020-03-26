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

#include <Akari/Core/MemoryArena.hpp>
#include <Akari/Render/BSDF.h>
#include <Akari/Render/Interaction.h>

namespace Akari {
    SurfaceInteraction::SurfaceInteraction(const glm::vec3 &wo, const glm::vec3 &p, const Akari::Triangle &triangle,
                                           const Akari::Intersection &intersection, Akari::MemoryArena &arena)
        : Interaction(wo, p, triangle.Ng) {
        sp.texCoords = triangle.InterpolatedTexCoord(intersection.uv);
        Ns = triangle.InterpolatedNormal(intersection.uv);
        Ng = triangle.Ng;
        bsdf = arena.alloc<BSDF>(Ng, Ns);
    }
} // namespace Akari