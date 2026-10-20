# Subsurface Scattering

## The idea
- Render the scene from the cameras perspective with front-face culling on
- In the light pass you can do tap/s around the coordinate to see the difference
    in the depth of that pixel and the depth in the front-face culled one
- Depending on the thickness use that to influence how much the light passes through
- Read (this presentation)[https://www.slideshare.net/slideshow/colin-barrebrisebois-gdc-2011-approximating-translucency-for-a-fast-cheap-and-convincing-subsurfacescattering-look-7170855/7170855]
    for some more information on what to do with the values we get
