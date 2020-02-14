
engine = None
try :
    import tinyrenderer_core as tr_core
    engine = tr_core
except ImportError :
    print( 'ERROR >>> couldn\'t load windowed-glfw rendering backend' )