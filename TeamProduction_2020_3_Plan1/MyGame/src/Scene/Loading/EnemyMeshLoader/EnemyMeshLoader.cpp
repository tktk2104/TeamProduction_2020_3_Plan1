#include "EnemyMeshLoader.h"

#include <TktkDirectX11Wrapper/Graphics/Mesh/Maker/SphereMeshMaker.h>
#include <TktkDirectX11Wrapper/Graphics/Mesh/ConstantBufferData/MonoColorConstantBufferData.h>
#include <TktkDirectX11Wrapper/Graphics/Material/Material.h>
#include <TktkDirectX11Wrapper/Graphics/ConstantBuffer/Asset/SystemConstantBufferId.h>

#include "../../../Ids/MeshId.h"
#include "../../../Ids/MaterialId.h"

void EnemyMeshLoader::load()
{
	tktk::SphereMeshMaker::make(MeshId::Enemy_1, MaterialId::Enemy_1);

	tktk::Material::getDataPtr(MaterialId::Enemy_1)->getParametersRef().setConstantBufferSetData(
		tktk::SystemConstantBufferId::MonoColor,
		tktk::MonoColorConstantBufferData({ Color::red })
	);
}
