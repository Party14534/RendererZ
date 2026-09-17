#include "pipelineItems.h"
#include "System/GraphicsAPI/GraphicsPipeline.h"

GraphicsPipelineItem getDLightPipelineItem(Window& win) {
    Callback begin = [&win] {
        glViewport(0, 0, 
                win.dLightShadowBuffer.width,
                win.dLightShadowBuffer.height);
        api->setCullFace(false);
        
        win.dLightShadowBuffer.bind();
        glClear(GL_DEPTH_BUFFER_BIT);


        win.dLightShader->use();
        win.dLightShadowBuffer.bind();
        win.setDLightUniforms();
    };

    Callback end = [&win] {
        api->setCullFace(true);
        win.dLightShadowBuffer.unbind();
        win.dLightShadowBuffer.bindTexture(5);
        glViewport(0, 0, win.width, win.height);
    };


    return GraphicsPipelineItem::Geometry(
            win.dLightShader,
            begin,
            end,
            win.dLightVP);
}

GraphicsPipelineItem getGBufferPipelineItem(Window& win) {
    Callback begin = [&win] {
        win.gBuffer.bind();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        win.setGBufferUniforms();
    };

    Callback end = [&win] {
        win.gBuffer.unbind();
        win.gBuffer.bindTextures();
    };


    return GraphicsPipelineItem::Geometry(
            win.gBufferShader,
            begin,
            end,
            win.gBufferVP);
}

GraphicsPipelineItem getSAOPipelineItem(Window& win) {
    Callback begin = [&win] {
        win.saoBuffer.bind();
        glClear(GL_COLOR_BUFFER_BIT);
        api->setCullFace(false);
        win.saoPassShader->use();
        win.setSAOPassUniforms();
    };

    Callback end = [&win] {
        win.saoBuffer.unbind();
    };

    return GraphicsPipelineItem::PostProcessing(
            win.saoPassShader,
            begin,
            end,
            win.drawScreen);
}

GraphicsPipelineItem getSAOBlurHPipelineItem(Window& win) {
    Callback begin = [&win] {
        win.saoBlurHBuffer.bind();
        win.saoBuffer.bindTexture(3);
        glClear(GL_COLOR_BUFFER_BIT);
        win.setSAOBlurPassUniforms(Vec2(1., 0.));
    };

    Callback end = [&win] {
        win.saoBlurHBuffer.unbind();
    };

    return GraphicsPipelineItem::PostProcessing(
            win.saoBlurPassShader,
            begin,
            end,
            win.drawScreen);
}

GraphicsPipelineItem getSAOBlurPipelineItem(Window& win) {
    Callback begin = [&win] {
        win.saoBlurBuffer.bind();
        win.saoBlurHBuffer.bindTexture(3);
        glClear(GL_COLOR_BUFFER_BIT);
        win.setSAOBlurPassUniforms(Vec2(0., 1.));
    };

    Callback end = [&win] {
        win.saoBlurBuffer.unbind();
    };

    return GraphicsPipelineItem::PostProcessing(
            win.saoBlurPassShader,
            begin,
            end,
            win.drawScreen);
}

GraphicsPipelineItem getLightPassPipelineItem(Window& win) {
    Callback begin = [&win] {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        win.saoBlurBuffer.bindTexture(4);
        win.setLightPassUniforms();
    };

    Callback end = [&win] {
        api->setCullFace(true);
    };

    return GraphicsPipelineItem::PostProcessing(
            win.lightPassShader,
            begin,
            end,
            win.drawScreen);
}
