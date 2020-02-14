
engine = None
try :
    import tinyrenderer_core_egl as tr_core_egl
    engine = tr_core_egl
except ImportError :
    print( 'ERROR >>> couldn\'t load headless-egl rendering backend' )