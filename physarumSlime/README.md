# physarumSlime

A [physarum](https://en.wikipedia.org/wiki/Physarum_polycephalum#Situational_behavior) slime simulation.

Inspired completely by [Sebastian Lague's video](https://www.youtube.com/watch?v=X-iSQQgOd1A)

## Screenshots
Have a look at the screenshots folder if you don't want to compile the source code yourself.

The images in the folder were run with resolution 2560x1440 and 1 million agents, managing around 30fps using a 1660Ti graphics card.

## Simulation algorithm
The simulation works as follows:
- There are many "agents" each holding a position and angle.
- The agents move around at a constant speed, leaving trails which fade and diffuse over time.
- The agents steer towards directions with stronger trails, by sampling the trail strength at points in front of them.

Surprisingly, this simple algorithm is enough to create beatiful patterns.

## Implementation nuances
The implementation of said algorithm is like so:
- store a texture, agentBuffer, where each pixel represents an agent, and the r, g, b values of each pixel are the agents x position, y position and angle respectively. Note that we use a ping pong buffer for better performance.
- store a texture, trailMapBuffer, which is essentially the screen i.e. contains the greyscale trail values. (not a ping pong buffer, to avoid screen flickering)
- in each update step:
  - activate a shader, agentShader, and draw agentBuffer onto itself (ping pong), which allows agentShader to be used to update each agent.
  - activate a shader, effectShader, and draw trailMapBuffer onto itself, which allows effectShader to be used to apply the fade and blur effects.
  - activate a shader, trailMapShader, and draw vertices representing each pixel position on agentBuffer onto trailMapBuffer, which allows trailMapShader to be used to draw the agents to the screen (by having the vertex shader output the value of agentBuffer at the vertex position)
- in each draw call:
  - activate a shader, colorShader, to apply a color lookup table based on the greyscale trailMapBuffer value (or just pass it through).
  - draw trailMapBuffer to the screen

In this way, GPU parallelization is acheived and so performance is very good.

## Resources
In order of usefulness:
- [shader and ping pong buffer example](https://github.com/openframeworks/openFrameworks/blob/master/examples/gl/gpuParticleSystemExample)
- [algorithm description](https://cargocollective.com/sagejenson/physarum)
- [shader tutorial (official docs)](https://openframeworks.cc/ofBook/chapters/shaders.html)
- [other useful stuff and a colorscheme](http://maciejmatyka.blogspot.com/2022/01/compute-shaders-in-open-frameworks.html)
