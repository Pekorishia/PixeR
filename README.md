# PixeR

PixeR is an image renderer that uses the ray tracing technique.

## Dependencies

    C++11

## How to use

Open the src folder and run the main or choose a scene from the scenes_files folder and run passing the scene file name.


```bash
g++ -std=c++11 main.cpp -o build && ./build
g++ -std=c++11 main.cpp -o build && ./build scenefile.txt
```


## Scene file format

### image creation specification:

Tags | Contents
------------ | -------------
name | the name of the image file (only accepts .ppm extension).
codification | the type of the image codification (only accepts "ascii").
width & height | the wiidth and height of the image (integer).
samples | the number of color samples retrieved from one pixel (integer). 
ray_depth* | the number of times that the rays will bounce (integer). 
t_min & t_max | the minimum and maximum distance that the image will show objects (float and "infinity"). 

*Only matters when using difuse and blinn-phong shader

**Example**: 

```txt
"image": { 
		    	"name": "newScene.ppm", 
		    	"codification": "ascii",
		    	"width": 1200,
		    	"height": 600,
		    	"samples": 1,
		    	"ray_depth": 1,
		    	"t_min": 0.001,
		    	"t_max": "infinity"
		  	},
```

### Shader creation specification:

Tags | Contents
------------ | -------------
Type | the type of the shader ("difuse", "depth", "normal", "blinnphong").
min_depth & max_depth* | the minimum and maximum distance that the shader will dye the image (float).
foreground & background* | the color that will dye the front and back part of the image(rgb).  

*Only needed when using depth shader

**Example**: 

```txt
"shader": {
			"type": "blinnphong"
		},
```
or

```txt
"shader": {
			"type": "depth",
			"min_depth": 0,
			"max_depth": 4,
			"foreground": {
							"r": 1,
							"g": 1,
							"b": 1
						},
			"background" : {
							"r": 0,
							"g": 0,
							"b": 0
							}
			},
```


### Camera creation specification:

Tags | Contents
------------ | -------------
lower_left_corner | the lower left point of the view plane (point).
horizontal & vertical | the horizontal and vertical vector that compose the view plane (vector).
origin | the point of the origin of the camera (point). 

**Example**: 

```txt
"camera": {
			"lower_left_corner":{
									"x": -2,
									"y": -1,
									"z": -1
								},
			"horizontal":{
							"x": 4,
							"y": 0,
							"z": 0
						}, 
			"vertical":{
							"x": 0,
							"y": 2,
							"z": 0
						}, 
			"origin":{
						"x": 0,
						"y": 0,
						"z": 0
					}
		},
```

### Scene creation specification:

Inside the scene we put the lights, ambient light, background and objects.

**Ambient Light**

Only the rgb of the ambient light.

**Example**: 

```txt
"ambient_light": {
					"r": 0.4,
					"g": 0.4,
					"b": 0.4
				},
```

**Lights**

Tags | Contents
------------ | -------------
direction | the direction of the light (vector).
intensity | the color intensity of the light (rgb).

**Example**: 

```txt
light": [ 
		{
			"direction": {
							"x": 20,
							"y": 10,
							"z": 5
						},
			"intensity": {
							"r": 1,
							"g": 1,
							"b": 1
						}
	   }
	   ],
```

**Background**

Tags | Contents
------------ | -------------
upper_left & upper_right | the left and right color of the upper part of the background (rgb).
lower_left & lower_right |the left and right color of the lower part of the background (rgb).

**Example**: 

```txt
"background": {
				"upper_left": {
								"r": 0.5,
								"g": 0.7,
								"b": 0.9
							},
				"upper_right": {
								"r": 0.5,
								"g": 0.3,
								"b": 0.9
							},
				"lower_left": {
								"r": 1,
								"g": 1,
								"b": 1
							},
				"lower_right": {
								"r": 1,
								"g": 1,
								"b": 1
							}
			},
```

**Objects**

The only object type created until now is sphere.


sphere Tags | Contents
------------ | -------------
material | the material of the sphere.
center | the center of the sphere (point).
radius | the radius of the sphere (float).

Material Tags | Contents
------------ | -------------
type | the type of the material ("matted", "blinnphong", "metal").
albedo | the color and difuse component of the material (rgb).
mirrow* | the specular reflexion component of the material (rgb).
specular* | the specular component of the material (rgb).
ambient* | the ambient component of the material (rgb).
alpha* | the specular exponent (float).
fuzz** | the fuzziness of the material (float).

*Only matters for the blinnphong material
**Only matters for the metal material

**Example**: 

```txt
"spheres": [
			{
				"material": {
								"type" : "matted",
								"albedo": {
											"r": 0.0,
											"g": 0.3,
											"b": 0.8
										}
							}, 
				"center": {
								"x": 0,
								"y": 0,
								"z": -1
							}, 
				"radius": 0.4
			},
```

or

```txt
"spheres": [
			{
				"material": {
								"type" : "metal",
								"albedo": {
											"r": 0.0,
											"g": 0.3,
											"b": 0.8
										},
								"fuzz": 0.3
							}, 
				"center": {
								"x": 0,
								"y": 0,
								"z": -1
							}, 
				"radius": 0.4
			},
```

or

```txt
"spheres": [
			{
				"material": {
								"type" : "blinnphong",
								"albedo": {
											"r": 0.0,
											"g": 0.3,
											"b": 0.8
										},
								"mirrow": {
											"r": 0.1,
											"g": 0.1,
											"b": 0.1
										},
								"specular": {
											"r": 0.9,
											"g": 0.9,
											"b": 0.9
										},
								"ambient": {
											"r": 0.1,
											"g": 0.1,
											"b": 0.1
										},
								"alpha": 64
							}, 
				"center": {
								"x": 0,
								"y": 0,
								"z": -1
							}, 
				"radius": 0.4
			}
		   ]
```

### Full scene file example:

```txt{
	"image": { 
		    	"name": "newScene.ppm",
		    	"codification": "ascii",
		    	"width": 1200,
		    	"height": 600,
		    	"samples": 1,
		    	"ray_depth": 1,
		    	"t_min": 0.001,
		    	"t_max": "infinity"
		  	},
	
	"shader": {
				"type": "blinnphong"
			},
	"camera": {
				"lower_left_corner":{
										"x": -2,
										"y": -1,
										"z": -1
									},
				"horizontal":{
								"x": 4,
								"y": 0,
								"z": 0
							}, 
				"vertical":{
								"x": 0,
								"y": 2,
								"z": 0
							}, 
				"origin":{
							"x": 0,
							"y": 0,
							"z": 0
						}
			},
	"scene": {
			"light" : [ 
						{
							"direction": {
											"x": 20,
											"y": 10,
											"z": 5
										},
							"intensity": {
											"r": 1,
											"g": 1,
											"b": 1
										}
					   }
					 ],
			"ambient_light": {
								"r": 0.4,
								"g": 0.4,
								"b": 0.4
							},
			"background": {
							"upper_left": {
											"r": 0.5,
											"g": 0.7,
											"b": 0.9
										},
							"upper_right": {
											"r": 0.5,
											"g": 0.3,
											"b": 0.9
										},
							"lower_left": {
											"r": 1,
											"g": 1,
											"b": 1
										},
							"lower_right": {
											"r": 1,
											"g": 1,
											"b": 1
										}
						},
			"objects": {
						"spheres": [
									{
										"material": {
														"type" : "blinnphong",
														"albedo": {
																	"r": 0.0,
																	"g": 0.3,
																	"b": 0.8
																},
														"specular": {
																	"r": 0.9,
																	"g": 0.9,
																	"b": 0.9
																},
														"ambient": {
																	"r": 0.1,
																	"g": 0.1,
																	"b": 0.1
																},
														"alpha": 64
													}, 
										"center": {
														"x": 0,
														"y": 0,
														"z": -1
													}, 
										"radius": 0.4
									},
									{
										"material": {
														"type" : "blinnphong",
														"albedo": {
																	"r": 0.4,
																	"g": 0.4,
																	"b": 0.4
																},
														"specular": {
																	"r": 1,
																	"g": 1,
																	"b": 1
																},
														"ambient": {
																	"r": 0.1,
																	"g": 0.1,
																	"b": 0.1
																},
														"alpha": 5
													}, 
										"center": {
														"x": 0,
														"y": -100.5,
														"z": -1
													}, 
										"radius": 100
									}
								]
						}
      		} 
 }
```