Material
########

A material represents the properties of an object that determine how it
interacts with light and therefore how it looks like when rendered. Our renderer
provides a simple material implementation that supports common shading models
like Lambertian, Blinn-Phong, and PBR.

Materials are considered resources in our renderer, similar to ``textures`` and
``shaders``, so they are commonly shared between various renderables. Below we
show how to create and setup a simple material:

.. code-block:: cpp

  #include <renderer/material/material_t.hpp>

  // Create a material associated to a given renderable (e.g. Mesh)
  mesh.material = std::make_shared<::renderer::Material>();
  mesh.material->type = ::renderer::eMaterialType::PHONG;
  mesh.material->ambient = Vec3(0.8F, 0.3F, 0.2F);
  mesh.material->diffuse = Vec3(0.8F, 0.3F, 0.2F);
  mesh.material->specular = Vec3(0.8F, 0.3F, 0.2F);
  mesh.material->albedoMap = ::renderer::TextureManager::GetTexture("PATH_TO_TEXTURE")

.. code-block:: python

  import renderer as rdr

  # Create a material associated to a given renderable (e.g. Mesh)
  mesh.material = rdr.Material()
  mesh.material.type = rdr.MaterialType.PHONG
  mesh.material.ambient = [0.8, 0.3, 0.2]
  mesh.material.diffuse = [0.8, 0.3, 0.2]
  mesh.material.specular = [0.8, 0.3, 0.2]
  mesh.material.albedoMap = rdr.TextureManager.GetTexture("PATH_TO_TEXTURE")


API Reference
-------------

.. doxygenclass:: renderer::Material
   :members:
