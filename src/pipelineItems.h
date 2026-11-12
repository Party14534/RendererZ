#ifndef PIPELINE_ITEMS
#define PIPELINE_ITEMS

#include "System/GraphicsAPI/GraphicsPipeline.h"
#include "System/Window.h"

GraphicsPipelineItem getDLightPipelineItem(Window& win);
GraphicsPipelineItem getSAOPipelineItem(Window& win);
GraphicsPipelineItem getSAOBlurHPipelineItem(Window& win);
GraphicsPipelineItem getSAOBlurPipelineItem(Window& win);

// OLD
GraphicsPipelineItem getGBufferPipelineItem(Window& win);
GraphicsPipelineItem getLightPassPipelineItem(Window& win);

// NEW
GraphicsPipelineItem getGBufferPipelineItemPBR(Window& win);
GraphicsPipelineItem getLightPassPipelineItemPBR(Window& win);

#endif
