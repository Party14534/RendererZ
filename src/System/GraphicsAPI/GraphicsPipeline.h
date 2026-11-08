#ifndef GRAPHICS_PIPELINE_H
#define GRAPHICS_PIPELINE_H

#include "math.h"
#include "../../Objects/Drawable.h"
#include "../../Shaders/Shaders.h"
#include "../FrameBuffer/FrameBuffer.h"
#include "GraphicsAPI.h"
#include "../../global.h"
#include <span>
#include <chrono>

/*
 * 
 */

typedef std::function<const void()> Callback;

enum PipelineClearOption {
    DEPTH_BUFFER = GL_DEPTH_BUFFER_BIT,
    COLOR_BUFFER = GL_COLOR_BUFFER_BIT
};

enum PipelineItemType {
    POST_PROCESSING = 0,
    GEOMETRY
};

struct Viewport {
    u32 x, y, width, height;
};

union PipelineItemTarget {
    std::span<IRenderable*> targets;
    Drawable* target;
};

struct GraphicsPipelineItem {
    shared<ShaderProgram> program;
    Callback beginCallback;
    Callback endCallback;
    PipelineItemTarget pipelineTarget;
    PipelineItemType drawType;
    const Mat4D* VP;
    bool usingDefaultTargets = false;

    GraphicsPipelineItem();

    static GraphicsPipelineItem PostProcessing (
            shared<ShaderProgram> program,
            Callback begin, Callback end, Drawable& target);

    // Not using default targets
    static GraphicsPipelineItem Geometry (
            shared<ShaderProgram> program,
            Callback begin, Callback end, std::span<IRenderable*> targets,
            const Mat4D& VP);

    // Using default targets
    static GraphicsPipelineItem Geometry (
            shared<ShaderProgram> program,
            Callback begin, Callback end,
            const Mat4D& VP);

    private:
        GraphicsPipelineItem(shared<ShaderProgram> program,
                            Callback begin, Callback end,
                            PipelineItemTarget pipelineTarget,
                            PipelineItemType drawType, const Mat4D* vp);
};

struct GraphicsPipeline {
    std::span<GraphicsPipelineItem>& items;

    GraphicsPipeline(std::span<GraphicsPipelineItem>& items);

    void runPipeline(std::span<IRenderable*> targets);
};

#endif
