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

#include <akari/core/logger.h>
#include <akari/core/parallel.h>
#include <akari/core/plugin.h>
#include <akari/render/integrator.h>
#include <future>
#include <mutex>

namespace akari {
    struct RTDebugRenderTask : RenderTask {
        RenderContext ctx;
        std::mutex mutex;
        std::condition_variable filmAvailable, done;
        std::future<void> future;
        int spp;
        RTDebugRenderTask(const RenderContext &ctx, int spp) : ctx(ctx), spp(spp) {}
        bool has_film_update() override { return false; }
        std::shared_ptr<const Film> film_update() override { return ctx.camera->GetFilm(); }
        bool is_done() override { return false; }
        bool wait_event(Event event) override {
            if (event == Event::EFILM_AVAILABLE) {
                std::unique_lock<std::mutex> lock(mutex);

                filmAvailable.wait(lock, [=]() { return has_film_update() || is_done(); });
                return has_film_update();
            } else if (event == Event::ERENDER_DONE) {
                std::unique_lock<std::mutex> lock(mutex);

                done.wait(lock, [=]() { return is_done(); });
                return true;
            }
            return false;
        }
        void Wait() override {
            future.wait();
            done.notify_all();
        }
        static std::string PrintVec3(const vec3 &v) { return fmt::format("{} {} {}", v.x, v.y, v.z); }
        Spectrum Li(Ray ray, Sampler *sampler, MemoryArena &arena) {
            auto scene = ctx.scene;

            Spectrum Li(0), beta(1);
            for (int depth = 0; depth < 5; depth++) {
                Intersection intersection(ray);
                if (scene->intersect(ray, &intersection)) {
                    auto &mesh = scene->get_mesh(intersection.meshId);
                    int group = mesh.get_primitive_group(intersection.primId);
                    const auto &materialSlot = mesh.get_material_slot(group);
                    auto material = materialSlot.material;
                    if (!material) {
                        debug("no material!!\n");
                        break;
                    }
                    Triangle triangle{};
                    mesh.get_triangle(intersection.primId, &triangle);
                    vec3 p = ray.At(intersection.t);
                    SurfaceInteraction si(&materialSlot, -ray.d, p, triangle, intersection, arena);
                    si.compute_scattering_functions(arena, TransportMode::EImportance, 1.0f);
                    BSDFSample bsdfSample(sampler->next1d(), sampler->next2d(), si);
                    si.bsdf->sample(bsdfSample);
                    //                                    Debug("pdf:{}\n",bsdfSample.pdf);
                    assert(bsdfSample.pdf >= 0);
                    if (bsdfSample.pdf <= 0) {
                        break;
                    }
                    //                                    Debug("wi: {}\n",PrintVec3(bsdfSample.wi));
                    auto wiW =bsdfSample.wi;
                    beta *= bsdfSample.f * abs(dot(wiW, si.Ns)) / bsdfSample.pdf;
                    ray = si.spawn_dir(wiW);
                } else {
                    Li += beta * Spectrum(1);
                    break;
                }
            }
            return Li;
        }
        void Start() override {
            future = std::async(std::launch::async, [=]() {
                auto beginTime = std::chrono::high_resolution_clock::now();

                auto scene = ctx.scene;
                auto &camera = ctx.camera;
                auto &_sampler = ctx.sampler;
                auto film = camera->GetFilm();
                auto nTiles = ivec2(film->resolution() + ivec2(TileSize - 1)) / ivec2(TileSize);
                parallel_for_2d(nTiles, [=](ivec2 tilePos, uint32_t tid) {
                    (void) tid;
                    MemoryArena arena;
                    Bounds2i tileBounds = Bounds2i{tilePos * (int) TileSize, (tilePos + ivec2(1)) * (int) TileSize};
                    auto tile = film->tile(tileBounds);
                    auto sampler = _sampler->clone();
                    for (int y = tile.bounds.p_min.y; y < tile.bounds.p_max.y; y++) {
                        for (int x = tile.bounds.p_min.x; x < tile.bounds.p_max.x; x++) {
                            sampler->set_sample_index(x + y * film->resolution().x);
                            for (int s = 0; s < spp; s++) {
                                sampler->start_next_sample();
                                CameraSample sample;
                                camera->generate_ray(sampler->next2d(), sampler->next2d(), ivec2(x, y), &sample);
                                auto Li = this->Li(sample.primary, sampler.get(), arena);
                                arena.reset();
                                tile.add_sample(ivec2(x, y), Li, 1.0f);
                            }
                        }
                    }
                    std::lock_guard<std::mutex> lock(mutex);
                    film->merge_tile(tile);
                });
                auto endTime = std::chrono::high_resolution_clock::now();
                std::chrono::duration<double> elapsed = (endTime - beginTime);
                info("Rendering done in {} secs, traced {} rays, {} M rays/sec\n", elapsed.count(),
                     scene->GetRayCounter(), scene->GetRayCounter() / elapsed.count() / 1e6);
            });
        }
    };

    class RTDebug : public Integrator {
        int spp = 16;

      public:
        AKR_DECL_COMP()
        AKR_SER(spp)
        std::shared_ptr<RenderTask> create_render_task(const RenderContext &ctx) override {
            return std::make_shared<RTDebugRenderTask>(ctx, spp);
        }
    };
    AKR_EXPORT_PLUGIN(RTDebug, p){}
} // namespace akari
