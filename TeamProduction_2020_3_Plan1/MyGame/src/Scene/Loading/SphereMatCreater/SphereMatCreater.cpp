#include "SphereMatCreater.h"

#include <TktkDirectX11Wrapper/Graphics/Material/Material.h>
#include "../../../Ids/MaterialId.h"

void SphereMatCreater::create()
{
	for (int i = static_cast<int>(MaterialId::Sphere_1); i <= static_cast<int>(MaterialId::Sphere_10); i++)
	{
		tktk::Material::duplicate(i, tktk::SystemMaterialId::Sphere);
	}
}
