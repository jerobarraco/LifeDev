improve ticks
	disable when possible 
	use activate deactivate on components
	increase interval if possible
	disable based on distance? or lower interval?
	JSIG significance manager https://github.com/tomlooman/ActionRoguelike/blob/master/Source/ActionRoguelike/Public/Components/SSignificanceComponent.h


automanage
	on audio and niagara components
	* add Activate/Deactivate on animator

# mesh
	set to static or stationary if possible
	disable cast shadows if possible
	enable 'use attach parent bounds' if possible 
	disable ticks
	disable overlap events
	disable physics if not required
	disable query if not required
	move/rotate on one single call, only 1nce per frame (e.g. setTransform instead of setLocation + setRotation )
	WPO position writes velocity to false
	WorldPositionOffsetDisableDistance (see quickmesh)
	; enable auto instancing https://forums.unrealengine.com/t/what-about-auto-instancing/131775/2?u=nande
	; seems to default to 1, but maybe better be sure. also it might only work with taa and no msaa
	r.MeshDrawCommands.DynamicInstancing=1
	NanitePixelProgrammableDistance = 600
		https://forums.unrealengine.com/t/nanite-pixel-programmable-distance-for-ism-component-huge-performance-wins/2278017/8
	bComputeFastBounds (teh compute once per frame will crash)

# lumen
	AmbientOcclusionStaticFraction set to false for lumen. can be changed on the post process.
mesh cards / surface cache
	https://dev.epicgames.com/documentation/en-us/unreal-engine/lumen-technical-details-in-unreal-engine
	r.Lumen.Visualize.CardPlacement 1
	* lumen mesh cards group id (Ray tracing group id ). it's on the mesh. it's global though.
	* light attachment as group

# material
	Quality switch node
	vertex interpolator
	mask + dither if possible
	no double sided 
	increase opacity
	saturate instead of clamp
	lower num samples
	DistanceCullFade to blend the cull
	-  min max draw distance
	RayTracingQualitySwitchReplace to make it look better with lumen https://dev.epicgames.com/documentation/en-us/unreal-engine/lumen-technical-details-in-unreal-engine

# culling
* use distance cull volumes
* showflag.distanceculledprimitives 1
* stat initviews
* 'freezerendering' to test culling
* remove "can be occluder" (use as occluder) on small things or with many holes. not only it won't occlude much, but it will also take time to process.

# light
* use spotlights instead of pointlights as they are faster
* light: can have a check "affects world"
* r.forward.lightgriddebug 1

## light culling
* automatic : r.MinScreenRadiusForLights 0.03 (disables the LIGHT at that radius)
* * maybe r.Shadow.RadiusThreshold should disable the shadow at that radius, but it's not working (it's overriden by scalability)
* maxDrawDistance MaxDistanceFadeRange
* if not using static lights, go to preferences and disable "allow static lights". it avoids issues.
r.AllowStaticLighting=False

## baked lights

* enable on settings 'allow static bakedlighting'
* disable "support hardware raytrace"
* on world settings remove world partition
* restart
* on world settings remove "force no precomputed light"

level streaming
* streaming volume + manual
* stat levels
* stat levels
* project settings > navigation > Update Overlaps Method During Level Streaming = never

animation
* fast pass
* warnaboutblueprintusage
* stat anim
* update rate optimization on distance for skeletal mesh 
* visibilitybasedanimtickoption
* bRenderAsStatic bPauseAnims bNoSkeletonUpdate

## shadows
* removed support for local fog volumes
* disable cast volumetric shadows on lights
* be intentional about casting shadows and dynamic lights, disable by default
* (not sure this is good) @raditsyz :A great tip for any other artists/tech artists is to tune your lights by disabling 'Exponential Falloff' on your lights and set your exponent to 2-3. You can use this to make better fill lights that don't need as high of an attenuation radius. This trick combined with an increased 'Indirect Lighting Intensity' can create some gains for you without losing too much light detail and accuracy, especially when it comes to fill lights.
* use contact shadows instead of dynamic ones
* * set contact shdow length in dir light settings
* * on mesh disable dynamdc shadows, enable contact shadows only
* on mesh set the invalidation to static if possible https://m.youtube.com/watch?v=tV4bpjm-bIg
* 

## shadow maps
	r.Shadow.Virtual.NonNanite.IncludeInCoarsePages 0
	r.Shadow.Virtual.UseFarShadowCulling 0
	set foliage (grass, etc) to not cast shadows on low lod https://www.youtube.com/watch?v=AobyMegpUMg
	r.Shadow.Virtual.MaxPhysicalPages (512),
	r.Shadow.Virtual.MaxPhysicalPagesSceneCapture (512),
	r.Shadow.Virtual.ResolutionLodBiasLocal (1.00), and
	r.Shadow.Virtual.ResolutionLodBiasDirectional (0.00)

	* try r.shadow.virtualcache.staticseparate
	  https://m.youtube.com/watch?v=BKaAzhMHJZ0

* enable on settings 'allow static bakedlighting'
* on world settings remove world partition
* reducing the DirectionalLOD and reduce vt sizes https://drive.google.com/file/d/18uiEkezcrznO6XK1IdEVUg9UzP8ZJh2L/view

## vsm
* https://dev.epicgames.com/documentation/en-us/unreal-engine/virtual-shadow-maps-in-unreal-engine
* * try to use stationary instead of movable
* https://dev.epicgames.com/documentation/en-us/unreal-engine/virtual-shadow-maps-in-unreal-engine#managing-cache-invalidations
* Use the following console variables in succession to enable stats:
  r.ShaderPrintEnable 1
  r.Shadow.Virtual.ShowStats 1 (or 2 to show only the page statistics)
* r.shadow.virtual.dynamicres.maxpagepoolloadfactor  0.85 might help reduce the size
* r.shadow.virtual.smrt.raycountlocal ( note this is controlled by scallability, so has to be changed there)
* * r.shadow.virtual.visualize.advanced 1 to see advanced visualizations including smrt

* r.shadow.virtual.cache.staticseparate 1 enabled by dufault but doubles vram

* * reduce max pshysical pages. max sure the lights dont une more than that
* r.shadow.virtual.resolutionlodbias[Local]
  < 1 detail, more than 1 rough detail
* r.shadow.virtual.resolutionlodbiaslocalmoving
* these values also exists for directional too
  ** optimize the ones for directional. since i rarelf use them
  r.shadow.virtual.resolutionlodbiasdirectional and moving

* adjust size of r.shadow.virtual.clipmap.firstlevel and lastlevel. affects mostly directional light (and memory)

* * reduce max pshysical pages. max sure the lights dont une more than that
* r.shadow.virtual.resolutionlodbias[Local]
  < 1 detail, more than 1 rough detail
* r.shadow.virtual.resolutionlodbiaslocalmoving
* these values also exists for directional too
  ** optimize the ones for directional. since i rarelf use them
  r.shadow.virtual.resolutionlodbiasdirectional and moving


r.shadow.virtual.smrt.raycountlocal (already set by scalability settings)

## megalights
https://www.youtube.com/watch?app=desktop&v=3PQga-2vLm4
* enable using postprocess volume (or globally, but not recommended)
* r.megalights.hairstrands.*  .HairVoxelTraces
* r.megalights.directionallights are not enabled by defaults. it has issues with lower light scenes. enabled with 1
* * ghosting and noise
* r.megalights.debug 1 + r.shaderprint.1
* r.forward.lightgriddebug 1

# pso precache
	; pso precaching https://www.tomlooman.com/psocaching-unreal-engine/
	; https://dev.epicgames.com/documentation/en-us/unreal-engine/pso-precaching-for-unreal-engine
	; https://www.youtube.com/watch?v=i35yf-wh3Bs
	; https://dev.epicgames.com/documentation/en-us/unreal-engine/optimizing-rendering-with-pso-caches-in-unreal-engine
	r.PSOPrecaching=1
	; don't emit until pso ready
	fx.Niagara.Emitter.ComputePSOPrecacheMode=1
	r.PSOPrecache.ProxyCreationWhenPSOReady=1
	; don't hitch use default material.  relies on the r.PSOPrecache.ProxyCreationWhenPSOReady=1
	;0 	Skip the draw until the PSO is ready.
	;1 	Fallback to the engine's default material until the PSO is ready.
	r.PSOPrecache.ProxyCreationDelayStrategy=0
	
	; keep this active for validation with 'stat psocache', Insights AND required for ExcludePrecachePSO cvar
	; r.PSOPrecache.Validation=2
	; additional detail in logging for "stat psocache"
	; r.PSOPrecache.Validation.TrackMinimalPSOs=1
	; settings below for bundled PSO steps to combine with PSO Precache
	; r.ShaderPipelineCache.ExcludePrecachePSO=1
	r.ShaderPipelineCache.Enabled=1
	; start up background compilation mode so we can run "hitchless" in a main menu (optional)
	r.ShaderPipelineCache.StartupMode=2
	
	; ~ pso precache

	https://dev.epicgames.com/community/learning/tutorials/xjzE/unreal-engine-epic-for-indies-game-engines-shader-stuttering-ue-s-solution
	https://www.youtube.com/live/i35yf-wh3Bs
	https://www.unrealengine.com/en-US/tech-blog/game-engines-and-shader-stuttering-unreal-engines-solution-to-the-problem
	Use r.PSOPrecache.Validation=2 as explained in the documentation to identify misses or late PSOs and understand the causes.
	r.PSOPrecache.Validation=2
	r.PSOPrecache.Validation.TrackMinimalPSOs=1
	Using the -clearPSODriverCache command-line argument during playtests 


	to test:
	Without any changes applied, run the packaged game with -trace=default -clearPSODriverCache and the Unreal Insights session browser open (InstallFolder/Engine/Binaries/Win64/UnrealInsights.exe).

	There are many more CVARs available in the different PSO related code files:
	RenderCore/ShaderPipelineCache.cpp
	Engine/PSOPrecache.cpp


* perf: disable water shadow vsm 8:53 https://m.youtube.com/watch?v=JJK61A66dzU
; they add overhead and are not needed
r.Water.SingleLayer.ShaderSupportVSMFiltering=0
r.Water.SingleLayer.VSMFiltering=0


# collisions
	https://m.youtube.com/watch?v=xIQI6nXFygA
	make sure the collisions for movement block only pawn. and the ones for interact does that.
	disable generate overlaps events if not used.
	
	don't use collision if not needed (remove from mesh)
	prefer in order: sphere, capsule, box, convex https://www.youtube.com/watch?app=desktop&v=HaVTYSnGvxA

	check "NeverNeedsCookedCollisionData" on each mesh
	
	use default overlap during streaming to only movable. then override on each actor if needed
	https://youtu.be/HaVTYSnGvxA?t=1141
	[/Script/Engine.Actor]
	; option used when UpdateOverlapsMethodDuringLevelStreaming is set to UseConfigDefault. Options are: AlwaysUpdate, OnlyUpdateMovable, NeverUpdate.
	DefaultUpdateOverlapsMethodDuringLevelStreaming=OnlyUpdateMovable
	
	[/Script/Engine.TriggerVolume]
	DefaultUpdateOverlapsMethodDuringLevelStreaming=AlwaysUpdate

# niagara

https://m.youtube.com/watch?v=c2MH20OPSw0 17:34
on niagara system activate culling. set the "effect type" on the system.

# nanite
* r.Nanite.MaxPixelsPerEdge ( see nanite vis mode for triangles or clusters) (1 to 30)

# gc
[ConsoleVariables]
; optimize gc. requires usage of tobjectptr https://youtu.be/d2LWbjSjsv8?t=765
;https://dev.epicgames.com/documentation/en-us/unreal-engine/incremental-garbage-collection-in-unreal-engine
; notice this is still experimental
gc.AllowIncrementalReachability=1
gc.AllowIncrementalGather=1
gc.IncrementalReachabilityTimeLimit=0.002

; this is a safety net so that the story step will not skip gc due to some missing async load See Story.cpp:127
gc.PerformGCWhileAsyncLoading=1

gc clustering (where and how?)
aactor::CanBeClusterRoot and CanBeInCluster

console command : obj list -countsort
https://dev.epicgames.com/community/learning/knowledge-base/ePKR/unreal-engine-garbage-collector-internals
https://bbkgl-github-io.translate.goog/2021/08/28/UE4-GC%E6%9C%BA%E5%88%B6%E8%A7%A3%E6%9E%904/?_x_tr_sl=auto&_x_tr_tl=en&_x_tr_hl=en


# var
* size map
* statistics window
* cmd ToggleForceDefaultMaterial
* cmd dumphitches
* stat none
* postprocess: use "AfterTonemapping" (in theory)*1
* write velocity after base pass
* pixel programmable pipeline
* perf visualizers
* nanite overdraw
* vsm nanite overdraw
  *1
  https://dev.epicgames.com/documentation/en-us/unreal-engine/post-process-materials-in-unreal-engine
  After Tonemapping 	This option indicates that post processing will take place after tonemapping and color grading has been completed. It is the preferred location for performance since the color is LDR and requires less precision and bandwidth. When this option is selected, the SceneTexture expression's Post Process Inputs 2 and 3 are used to control where Scene Color is in the pipeline. Input 2 applies scene color before tonemapping. Input 3 applies scene color after tonemapping.
* screen percentage and upscaling
 https://dev.epicgames.com/documentation/en-us/unreal-engine/screen-percentage-with-temporal-upscale-in-unreal-engine

## stats
* use `stat game` to see cpu usage. profile in standalone at least, better in develop. lower screen scale to detect cpu bounds. gpu times are always larger than cpu since it waits for cpu. https://forums.unrealengine.com/t/performance-issue-with-ue4-27-2/2666278/2?u=nande
* r.nanite.showstats 1/0, r.shadow.virtual.showstats (etc?)
* nanitestats
* stat none
* stat unitgraph, unit, fps,
* stat initviews for culling
* show collision and stat for collision game https://dev.epicgames.com/documentation/en-us/unreal-engine/review-collision-in-your-unreal-engine-game
* r.shaderPrint 1 and r.SHaderPrint.Zoom 1

# package size
https://cobracode.notion.site/How-to-reduce-Package-Size-for-Unreal-Engine-Game-Builds-7993544e050041f9800512acd3500cfc
* disable plugins
* use shipping
* disable prerequisite installer
* use for distrib
* specify maps to cook
* forward shading (i'm not using this because i use more advanced features)
* discard unused quality (warning will break the ability to change the scalability settings )
* default pak rules (too much trouble too little gain for me, too much risk also, can break core features now or in the future, for only 3 mb)


# hitches
	https://m.youtube.com/watch?v=HaVTYSnGvxA
	
	use packed instance stactic meshes
	could use cell transformers but only with world partition
	
	disable generate overlaps events if not used.
	(is it used for the interact?)
	
	skip update overlap on stream
	
	set up a pso cache strategy
	* have to have a loading screen for the global shaders
	fShaderpipelinecache::NumPrecompilesRemaining, wait for 0
	
	-clearPSODriverCache command line. add to launch.
	check psocachebuster plugin
	p
	r.psocachevallidation=2
	
	try the tools > chaos visual debugger
	
	
	also try using gc clusters on step sequences
	@37
	
	on interact, maybe step, or maybe put a warning.
	
	change this (allows to call beginplay staggered in multiple frames) in defaultEnigne @ [ConsoleVariables]
	s.AdaptiveAddToWorld.Enabled=1

	ensure the object count is sane
	obj list -countsort


# cpp microopts
cpp
* emplace
* constness
* constexpr
* pass by ref
* move
* unlikely
* & instead oy && when it makes sense
* branchless

 https://www.youtube.com/watch?v=g-WPhYREFjk

	likely/unlikely >> really good. except if it changes in the future. use for fundamental things (like checking if a value is null)

	if (b) a+= c >> a += bool(b)*c

	a && b >> a & b (ensure they are boolean, and also group "< == <= etc")
	important: also make sure that the combination is more predictable than each one

	return ternary is optimal. use it.
		return b ? c : d;

extracting a bool makes no difference. the compiler undoes it. though it's nice readability. so still do it. 
	bool x = whatever;
	if (x)

use emplace when possible
	K k;
	A.add(K)
	K.something
	instead A.Emplace(...)

	or
	K& k = A.Emplace();
	k.something...

use reserve on arrays when possible

DONT:
	// void(ALTeachMan::* X[] )() = {&ALTeachMan::DeInitFeat, &ALTeachMan::InitFeat};
	// (this->*X[Enabled])();
	// the cpu can't optimize the jump to function. so there's no advantage. the branch predictor will be blind anyway.
	
	return by value is faster than move under the right conditions.
	since that's really hard to keep in mind, and also because you can inhibit the compiler to generate the move constructors.
	and because i don't think it'll work with bps. i wont. also i don't like it.


# links
optimization on a budget https://m.youtube.com/watch?v=G51QWcitCII&t=174s

profiling and optimizing in ue4 
https://m.youtube.com/watch?v=EbXakIuZPFo

optimize ue5 rendering
https://m.youtube.com/watch?v=dj4kNnj4FAQ

optimize game thread
https://m.youtube.com/watch?v=KxREK-DYu70

multithreading
https://m.youtube.com/watch?v=XJMyNM8xmS0

Preempting Challenges in AAA Unreal Engine Development | GDC 2025
https://www.youtube.com/watch?v=vgsZGZ0csVQ
