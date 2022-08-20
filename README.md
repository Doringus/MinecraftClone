# MinecraftClone
My second try to create something that looks like minecraft with C++ and OpenGl.
Completed features:
- OpenGl chunk and skybox renderer
- Thread pool for parallel chunk generation
- Greedy meshing algorithm
- Two world generators
- Chunk serialization (boost.serialization)
- Dynamic chunk loading and storing on disk

World generators
================
For procedural generation were used FastNoise libs. 
### Dummy generator
Basic generator based on 3 noises.
- 2d simplex noise for height map
- 2d simpex noise for temperature
- 2d simplex noise for humidity
Biomes are placed according to temperature, humidity values
### Cliffs generator
More advanced generator based on this video https://www.youtube.com/watch?v=ob3VwY4JyzE&ab_channel=Jfokus. For general terrain shaping was used one 3d-perlin noise. To control it were used 3 2d-perlin noises: erosion, continentalness and pv. They infuence on 3d noise. For biomes again were used hunidity and temperature 2d noises. 
![](https://i.imgur.com/diz9qeg.png)
