#!/usr/bin/env python

from tinyrenderer.core import engine
import tinymath as tm
import numpy as np
import gc

## check that materials don't grab ownership of ref. albedo nor specular maps (check logs for allocs/deallocs) ...
## nor create extra refs pyobjects for the argument texture-maps (check gc-count)
def test_materialConstructor() :
    albedoMap = engine.TextureManager.GetCachedTexture( 'img_wooden_container' )
    specularMap = engine.TextureManager.GetCachedTexture( 'img_wooden_container_specular' )

    print( 'num_referrers(albedo): {}'.format( len( gc.get_referrers( albedoMap ) ) ) )
    print( 'num_referrers(specular): {}'.format( len( gc.get_referrers( specularMap ) ) ) )

    material = engine.Material( 'dummy', engine.MaterialType.LAMBERT,
                                [0.,0.,1.], [0.,0.,1.], [0.,0.,1.], 32., albedoMap, specularMap )

    print( 'num_referrers(albedo): {}'.format( len( gc.get_referrers( albedoMap ) ) ) )
    print( 'num_referrers(specular): {}'.format( len( gc.get_referrers( specularMap ) ) ) )

## check that materials can be created with None as null-refs for texture-maps
def test_materialConstructorNoneMaps() :
    material = engine.Material( 'dummy', engine.MaterialType.LAMBERT,
                                [0.,0.,1.], [0.,0.,1.], [0.,0.,1.], 32., None, None )

## check that materials don't grab ownership of provided ref. shaders for bind method (check logs for allocs/deallocs)
## nor create extra refs pyobjects for the argument shader (check gc-count)
def test_materialBindShader() :
    shader = engine.ShaderManager.GetCachedShader( 'engine_no_shadows_no_fog' )

    print( 'num_referrers(shader): {}'.format( len( gc.get_referrers( shader ) ) ) )

    material = engine.Material( 'dummy', engine.MaterialType.LAMBERT,
                                [0.,0.,1.], [0.,0.,1.], [0.,0.,1.], 32., None, None )
    material.bind( shader )

    print( 'num_referrers(shader): {}'.format( len( gc.get_referrers( shader ) ) ) )

## check that returned texture-map refs. obtained from material.(albedoMap|specularMap) are not owned by returned pyobject (::reference policy should work)
def test_materialTexMapGetters() :
    albedoMap = engine.TextureManager.GetCachedTexture( 'img_wooden_container' )
    specularMap = engine.TextureManager.GetCachedTexture( 'img_wooden_container_specular' )

    print( 'num_referrers(albedo): {}'.format( len( gc.get_referrers( albedoMap ) ) ) )
    print( 'num_referrers(specular): {}'.format( len( gc.get_referrers( specularMap ) ) ) )

    material = engine.Material( 'dummy', engine.MaterialType.LAMBERT,
                                [0.,0.,1.], [0.,0.,1.], [0.,0.,1.], 32., albedoMap, specularMap )

    print( 'num_referrers(albedo): {}'.format( len( gc.get_referrers( albedoMap ) ) ) )
    print( 'num_referrers(specular): {}'.format( len( gc.get_referrers( specularMap ) ) ) )

    ref_albedoMap = material.albedoMap
    ref_specularMap = material.specularMap

    print( 'num_referrers(albedo): {}'.format( len( gc.get_referrers( albedoMap ) ) ) )
    print( 'num_referrers(specular): {}'.format( len( gc.get_referrers( specularMap ) ) ) )

    del ref_albedoMap
    del ref_specularMap

    print( 'num_referrers(albedo): {}'.format( len( gc.get_referrers( albedoMap ) ) ) )
    print( 'num_referrers(specular): {}'.format( len( gc.get_referrers( specularMap ) ) ) )

## check that passed texture-map refs. to material setters [material.(albedoMap|specularMap) = texture] are not owned by the material (check logs for allocs|deallocs)
def test_materialTexMapSetters() :
    albedoMap = engine.TextureManager.GetCachedTexture( 'img_wooden_container' )
    specularMap = engine.TextureManager.GetCachedTexture( 'img_wooden_container_specular' )

    print( '1. num_referrers(albedo): {}'.format( len( gc.get_referrers( albedoMap ) ) ) )
    print( '1. num_referrers(specular): {}'.format( len( gc.get_referrers( specularMap ) ) ) )

    material = engine.Material( 'dummy', engine.MaterialType.LAMBERT,
                                [0.,0.,1.], [0.,0.,1.], [0.,0.,1.], 32., albedoMap, specularMap )

    print( '2. num_referrers(albedo): {}'.format( len( gc.get_referrers( albedoMap ) ) ) )
    print( '2. num_referrers(specular): {}'.format( len( gc.get_referrers( specularMap ) ) ) )

    new_albedoMap = engine.TextureManager.GetCachedTexture( 'img_smiley' )
    new_specularMap = engine.TextureManager.GetCachedTexture( 'img_wooden_container_specular' )

    print( '3. num_referrers(albedo): {}'.format( len( gc.get_referrers( albedoMap ) ) ) )
    print( '3. num_referrers(specular): {}'.format( len( gc.get_referrers( specularMap ) ) ) )
    print( '3. num_referrers(new_albedo): {}'.format( len( gc.get_referrers( new_albedoMap ) ) ) )
    print( '3. num_referrers(new_specular): {}'.format( len( gc.get_referrers( new_specularMap ) ) ) )

    material.albedoMap = new_albedoMap
    material.specularMap = new_specularMap

    print( 'material.albedoMap: {}'.format( material.albedoMap.name ) )
    print( 'material.specularMap: {}'.format( material.specularMap.name ) )

    print( '4. num_referrers(albedo): {}'.format( len( gc.get_referrers( albedoMap ) ) ) )
    print( '4. num_referrers(specular): {}'.format( len( gc.get_referrers( specularMap ) ) ) )
    print( '4. num_referrers(new_albedo): {}'.format( len( gc.get_referrers( new_albedoMap ) ) ) )
    print( '4. num_referrers(new_specular): {}'.format( len( gc.get_referrers( new_specularMap ) ) ) )

if __name__ == '__main__' :
    ## create an application (need the context and other modules initialized)
    app = engine.Application()

    _ = input( 'Press ENTER to continue: start test_materialConstructor' )
    test_materialConstructor()
    _ = input( 'Press ENTER to continue: finished test_materialConstructor ...' )

    _ = input( 'Press ENTER to continue: start test_materialConstructorNoneMaps' )
    test_materialConstructorNoneMaps()
    _ = input( 'Press ENTER to continue: finished test_materialConstructorNoneMaps ...' )

    _ = input( 'Press ENTER to continue: start test_materialBindShader' )
    test_materialBindShader()
    _ = input( 'Press ENTER to continue: finished test_materialBindShader ...' )

    _ = input( 'Press ENTER to continue: start test_materialTexMapGetters' )
    test_materialTexMapGetters()
    _ = input( 'Press ENTER to continue: finished test_materialTexMapGetters ...' )

    _ = input( 'Press ENTER to continue: start test_materialTexMapSetters' )
    test_materialTexMapSetters()
    _ = input( 'Press ENTER to continue: finished test_materialTexMapSetters ...' )
