#ifndef PIPELINE_ITEMS
#define PIPELINE_ITEMS

#include "System/GraphicsAPI/GraphicsPipeline.h"
#include "System/Window.h"

GraphicsPipelineItem getDLightPipelineItem(Window& win);
GraphicsPipelineItem getGBufferPipelineItem(Window& win);
GraphicsPipelineItem getSAOPipelineItem(Window& win);
GraphicsPipelineItem getSAOBlurHPipelineItem(Window& win);
GraphicsPipelineItem getSAOBlurPipelineItem(Window& win);
GraphicsPipelineItem getLightPassPipelineItem(Window& win);

#endif
