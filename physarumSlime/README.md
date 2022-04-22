# physarumSlime

A [physarum](https://en.wikipedia.org/wiki/Physarum_polycephalum#Situational_behavior) slime simulation.

Inspired completely by [Sebastian Lague's video](https://www.youtube.com/watch?v=X-iSQQgOd1A) and also the comments in [this](https://www.youtube.com/watch?v=vVrQjbX2zLc) video.

## Screenshots

Using a 1660Ti graphics card, the following resolutions are displayed in the [screenshots/](screenshots/) folder:

v1: 2560x1440, 1 million agents.

v2: 3200x1800, 1.6 million agents.

## Simulation algorithm

The simulation works as follows:

- There are many "agents" each holding a position and angle.
- The agents move around at a constant speed, leaving trails which fade and diffuse over time.
- The agents steer towards directions with stronger trails, by sampling the trail strength at points in front of them.

Surprisingly, this simple algorithm is enough to create beautiful patterns (and also [solve shortest path problems](https://en.wikipedia.org/wiki/Physarum_polycephalum#Situational_behavior).

## Implementation nuances

#### v1

In v1, we implement the above algorithm like so:

- store a texture, agentBuffer, where each pixel represents an agent, and the r, g, b values of each pixel are the agents x position, y position and angle respectively. Note that we use a ping pong buffer to reduce screen flicker.
- store a texture, trailMapBuffer, which is essentially the screen i.e. contains the greyscale trail values. (not a ping pong buffer, to avoid screen flickering)
- in each update step:
  - activate a shader, agentShader, and draw agentBuffer onto itself (ping pong), which allows agentShader to be used to update each agent.
  - activate a shader, effectShader, and draw trailMapBuffer onto itself, which allows effectShader to be used to apply the fade and blur effects.
  - activate a shader, trailMapShader, and draw vertices representing each pixel position on agentBuffer onto trailMapBuffer, which allows trailMapShader to be used to draw the agents to the screen (by having the vertex shader output the value of agentBuffer at the vertex position)
- in each draw call:
  - activate a shader, colorShader, to apply a color lookup table based on the greyscale trailMapBuffer value (or just pass it through).
  - draw trailMapBuffer to the screen

In this way we achieve parallelization.

#### v2

The v1 code was not optimal, achieving 15fps on 5120x2880, 4M agents - this slowness was probably due to inefficient texture accesses by the vertex shader. In v2, we change the update method to use a compute shader instead, operating on ofBufferObjects instead of ofFbos. This achieves much better results, handling 8000x4500, 10M agents at around 25fps.

## Resources

In order of usefulness:

- [shader and ping pong buffer example](https://github.com/openframeworks/openFrameworks/blob/master/examples/gl/gpuParticleSystemExample)
- [compute shader walkthrough and a colorscheme](http://maciejmatyka.blogspot.com/2022/01/compute-shaders-in-open-frameworks.html)
- [algorithm description](https://cargocollective.com/sagejenson/physarum)
- [shader tutorial (official docs)](https://openframeworks.cc/ofBook/chapters/shaders.html)
  
  
