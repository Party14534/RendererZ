#include "GraphicsPipeline.h"
#include "Objects/Drawable.h"

/*
 * Pipeline
 */

GraphicsPipeline::GraphicsPipeline(std::span<GraphicsPipelineItem>& items)
    : items(items) { }

void GraphicsPipeline::runPipeline(std::span<IRenderable*> targets) {
    for(auto& item : items) {
        item.program->use();

        // Bind uniforms, framebuffers, etc.
        // Set viewport and clear needed buffers
        // Return a view and projection matrix
        item.beginCallback();
        bool isGeometry = item.drawType == GEOMETRY;

        if (isGeometry) {
            for (auto& target : item.usingDefaultTargets
                        ? targets
                        : item.pipelineTarget.targets)
            {
                target->draw(item.program, *item.VP);
            }
        } else {
            item.pipelineTarget.target->drawLightPass(item.program);
        }

        item.endCallback();
    }
}

/*
 * Pipeline Item
 */
GraphicsPipelineItem::GraphicsPipelineItem(
        std::shared_ptr<ShaderProgram> program,
        Callback begin, Callback end,
        PipelineItemTarget target,
        PipelineItemType drawType, const Mat4D* vp) :
    program(program), beginCallback(begin), endCallback(end),
    pipelineTarget(target), drawType(drawType), VP(vp)
{ }

GraphicsPipelineItem GraphicsPipelineItem::PostProcessing(
        std::shared_ptr<ShaderProgram> program,
        Callback begin, Callback end, Drawable& target)
{
    PipelineItemTarget t = PipelineItemTarget {
        .target = &target
    };

    return GraphicsPipelineItem(
            program, begin, end,
            PipelineItemTarget { .target = &target },
            POST_PROCESSING, nullptr);
}

GraphicsPipelineItem GraphicsPipelineItem::Geometry(
        std::shared_ptr<ShaderProgram> program,
        Callback begin, Callback end,
        const Mat4D& VP)
{
    GraphicsPipelineItem item = GraphicsPipelineItem(
            program, begin, end,
            {}, GEOMETRY, &VP);

    item.usingDefaultTargets = true;

    return item;
}

GraphicsPipelineItem GraphicsPipelineItem::Geometry(
        std::shared_ptr<ShaderProgram> program,
        Callback begin, Callback end, std::span<IRenderable*> targets,
        const Mat4D& VP)
{
    return GraphicsPipelineItem(
            program, begin, end,
            PipelineItemTarget {.targets = targets},
            GEOMETRY, &VP);
}
