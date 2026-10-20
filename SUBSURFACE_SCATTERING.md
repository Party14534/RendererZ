# Subsurface Scattering

## The idea
- Render the scene from the cameras perspective with front-face culling on
- In the light pass you can do tap/s around the coordinate to see the difference
    in the depth of that pixel and the depth in the front-face culled one
- Depending on the thickness use that to influence how much the light passes through
