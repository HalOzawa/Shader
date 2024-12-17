#include "Model.h"

namespace Model
{
	std::vector<ModelData*> modelList;
}

int Model::Load(std::string filename)
{
	ModelData* pData = new ModelData;
	pData->filename_ = filename;
	pData->pFbx_ = nullptr;

	for (auto& e : modelList)
	{
		pData->pFbx_ = e->pFbx_;
		break;
	}
	if (pData->pFbx_ == nullptr)
	{
		pData->pFbx_ = new FBX;
		pData->pFbx_->Load(filename);
	}

	modelList.push_back(pData);
	return(modelList.size() - 1);
}

void Model::SetTransform(int hModel, Transform transform)
{
}

void Model::Draw(int hModel)
{
}

void Model::Release()
{
}
