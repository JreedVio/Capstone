#include "AssetManager.h"
#include "VulkanRenderer.h"
#include <fstream>
#include "MeshComponent.h"
#include "ShaderComponent.h"
#include "MaterialComponent.h"
#include "TransformComponent.h"
#include "Actor.h"
#include <mutex>
#include "SceneManager.h"
#include "Scene.h"

#define BUFFER_SIZE 128

AssetManager* AssetManager::instance_(nullptr);

AssetManager::AssetManager() {
	//ReadManiFest();
	//OnCreate();
	renderer = renderer->VulkanRenderer::GetInstance();
}

AssetManager* AssetManager::GetInstance(){
	if (!instance_) {
		instance_ = new AssetManager();
	}

	return instance_;
}


AssetManager::~AssetManager(){
	//OnDestroy();
}


void AssetManager::LoadAssets(const char* fileName_) {
	XMLElement* fileData = ReadManiFest(LoadType::ASSETS, fileName_);
	if (!fileData) {
		Debug::FatalError("Fail to Load Asset " + std::string(fileName_), __FILE__, __LINE__);
		return;
	}
	//Add Components
	XMLElement* componentData = fileData->FirstChildElement("Component");
	AddComponentData(componentData);
	//Add Actor
	XMLElement* actorData = fileData->FirstChildElement("Actor");
	AddActorData(actorData);
	Debug::Info("Succeed to Load Asset " + std::string(fileName_), __FILE__, __LINE__);
}

void AssetManager::LoadScene(const char* sceneName_) {

	XMLElement* fileData = ReadManiFest(LoadType::SCENE, sceneName_);
	if (!fileData) {
		Debug::Info("Fail to Load Scene " + std::string(sceneName_), __FILE__, __LINE__);
		return;
	}
	//Load Room data 
	CreateRoom(fileData);
	

	Debug::Info("Succeed to Load Scene " + std::string(sceneName_), __FILE__, __LINE__);

}

void AssetManager::OnDestroy() {

	RemoveAllComponents();
}

XMLElement* AssetManager::ReadManiFest(LoadType loadType, const char* fileName_) {

	XMLElement* rootData = nullptr;
	bool status;
	//Read document with the given name
	char fileName[BUFFER_SIZE];
	strcpy_s(fileName, fileName_);
	strcat_s(fileName, ".xml");

	//Change the document type to load
	switch(loadType) {
	case(ASSETS):
		assetDoc.Clear();
		assetDoc.LoadFile(fileName);

		status = assetDoc.Error();
		if (status) {
			std::cout << assetDoc.ErrorIDToName(assetDoc.ErrorID()) << std::endl;
			return nullptr;
		}
		rootData = assetDoc.RootElement();
		break;
	case(SCENE):
		sceneDoc.Clear();
		sceneDoc.LoadFile(fileName);
		status = sceneDoc.Error();
		if (status) {
			std::cout << sceneDoc.ErrorIDToName(sceneDoc.ErrorID()) << std::endl;
			return nullptr;
		}
		rootData = sceneDoc.RootElement();
		break;
	}

	//Loop through the document to read data
	XMLElement* fileData = rootData->FirstChildElement(fileName_);

	return fileData;
}

void AssetManager::AddComponentData(XMLElement* componentData) {

	//Get First child element and check if it exists;
	XMLElement* componentFirstElement = componentData->FirstChildElement();
	while (componentFirstElement) {

		if (!componentFirstElement) {
			std::cerr << "No First Element \n";
		}
		else {
			//Get file name and add components		
			const char* componentType = componentFirstElement->Value();
			const char* componentName = componentFirstElement->FindAttribute("name")->Value();
			//Create component accordingly and add to asset manager
			if (strcmp(componentType, "Mesh") == 0) {
				const XMLAttribute* filenameAttribute = componentFirstElement->FindAttribute("filename");
				if (!filenameAttribute) {
					std::cerr << "No filename Attribute \n";
				}
				else {
					const char* filename = componentFirstElement->FindAttribute("filename")->Value();
					//renderer->LoadModel(componentName, filename);
					AddComponent<MeshComponent>(componentName, nullptr, filename);
				}
			}
			else if (strcmp(componentType, "Material") == 0) {
				const XMLAttribute* filenameAttribute = componentFirstElement->FindAttribute("filename");
				if (!componentFirstElement->FindAttribute("filename")) {
					std::cerr << "No filename Attribute \n";
				}
				else {
					const char* filename = componentFirstElement->FindAttribute("filename")->Value();
					AddComponent<MaterialComponent>(componentName, nullptr, filename);

				}
			}
			else if (strcmp(componentType, "Shader") == 0) {
				const XMLAttribute* vertFilenameAttribute = componentFirstElement->FindAttribute("vertFilename");
				const XMLAttribute* fragFilenameAttribute = componentFirstElement->FindAttribute("fragFilename");
				const XMLAttribute* geomFilenameAttribute = componentFirstElement->FindAttribute("geomFilename");

				if ((!vertFilenameAttribute) || (!fragFilenameAttribute)) {
					std::cerr << "No vertFilename or fragFilename Attribute \n";
				}
				else {
					//Get the file names
					const char* vertFile = vertFilenameAttribute->Value();
					const char* fragFile = fragFilenameAttribute->Value();

					AddComponent<ShaderComponent>(componentName, nullptr, vertFile, fragFile);
				}
			}
		}
		componentFirstElement = componentFirstElement->NextSiblingElement();
	}
	//Call OnCreate() for all components
	//CreateComponents();
}

bool AssetManager::CreateComponents(){

	for (auto component : componentCatalog) {
		component.second->OnCreate();
	}
	return true;
}

void AssetManager::AddActorData(XMLElement* actorData) {

	while (actorData) {
		//Get First child element and check if it exists;
		XMLElement* componentParentElement = actorData->FirstChildElement("Parent");
		XMLElement* componentMeshElement = actorData->FirstChildElement("Mesh");
		XMLElement* componentMaterialElement = actorData->FirstChildElement("Material");
		XMLElement* componentShaderElement = actorData->FirstChildElement("Shader");
		XMLElement* componentBodyElement = actorData->FirstChildElement("Body");
		XMLElement* componentAIElement = actorData->FirstChildElement("AI");
		if (!componentMeshElement || !componentMaterialElement || !componentShaderElement) {
			std::cerr << "Component Element not found\n";
		}
		else {

			Ref<Actor> newActor;
			//Get Actor name
			const char* actorName = actorData->FindAttribute("name")->Value();
			//Check actor data
			const char* parentName = componentParentElement->FindAttribute("componentName")->Value();	

			if (strcmp(parentName, "None") == 0) {
				newActor = std::make_shared<Actor>(nullptr);
			}
			else {
				newActor = std::make_shared<Actor>(GetComponent<Actor>(parentName).get());
			}

			//Get components to add to actor
			const char* meshName = componentMeshElement->FindAttribute("componentName")->Value();
			const char* materialName = componentMaterialElement->FindAttribute("componentName")->Value();
			const char* shaderName = componentShaderElement->FindAttribute("componentName")->Value();
			Ref<MeshComponent> actorMesh = GetComponent<MeshComponent>(meshName);
			Ref<MaterialComponent> actorMaterial = GetComponent<MaterialComponent>(materialName);
			Ref<ShaderComponent> actorShader = GetShader(shaderName);

			//Add all components to actor
			newActor->AddComponent<MeshComponent>(actorMesh);
			newActor->AddComponent<MaterialComponent>(actorMaterial);
			newActor->AddComponent<ShaderComponent>(actorShader);
			newActor->OnCreate();
			AddActor(actorName, newActor);
		}
		actorData = actorData->NextSiblingElement("Actor");
	}
}

bool AssetManager::CreateActors(){

	for (auto actor : actorList) {
		if (actor.second->OnCreate() == false) {
			return false;
		}
	}
	return true;
}

void AssetManager::CreateRoom(XMLElement* roomData){

		//Get information for room

		//Get Actor location and store in the room

		//Get First child element and check if it exists;
		XMLElement* actorData = roomData->FirstChildElement("Actor");
		while (actorData) {

			//Get Actor from list
			const char* actorName = actorData->FindAttribute("name")->Value();
			//Get the data of the actor from the actor list
			Ref<Actor> actorData_ = GetActor(actorName);
			Ref<Actor> actor_ = std::make_shared<Actor>(*actorData_.get());
			//Get data for transform
			XMLElement* componentTransformElement = actorData->FirstChildElement("Transform");
			//Position data
			float posX = componentTransformElement->FloatAttribute("posX");
			float posY = componentTransformElement->FloatAttribute("posY");
			float posZ = componentTransformElement->FloatAttribute("posZ");
			//Orientation data
			float angle = componentTransformElement->FloatAttribute("angle");
			float axisX = componentTransformElement->FloatAttribute("axisX");
			float axisY = componentTransformElement->FloatAttribute("axisY");
			float axisZ = componentTransformElement->FloatAttribute("axisZ");
			Quaternion orientation = Quaternion();
			if (VMath::mag(Vec3(axisX, axisY, axisZ)) > VERY_SMALL) {
				orientation = QMath::angleAxisRotation(angle, Vec3(axisX, axisY, axisZ));
			}
			//Scale data
			float scaleX = componentTransformElement->FloatAttribute("scaleX");
			float scaleY = componentTransformElement->FloatAttribute("scaleY");
			float scaleZ = componentTransformElement->FloatAttribute("scaleZ");
			if (scaleX < VERY_SMALL) {
				scaleX = 0.0f;
			}
			if (scaleY < VERY_SMALL) {
				scaleY = 0.0f;
			}
			if (scaleZ < VERY_SMALL) {
				scaleZ = 0.0f;
			}
			//Set transform component
			Ref<TransformComponent> tranform = std::make_shared<TransformComponent>(actor_.get(), Vec3(posX, posY, posZ), orientation, Vec3(scaleX, scaleY, scaleZ));
			actor_->AddComponent(tranform);
			actor_->OnCreate();
			//Add Actor to current scene
			SceneManager::GetInstance()->GetCurrentScene()->AddActor(actorName, actor_);
			actorData = actorData->NextSiblingElement("Actor");
		}

}

bool AssetManager::OnCreate(){
	return true;
}



void AssetManager::RemoveAllComponents(){

	componentCatalog.clear();
	actorList.clear();
	//materialList.clear();
	//meshList.clear();
	shaderList.clear();
}


Ref<Actor> AssetManager::GetActor(const char* name_){
	for (auto actor_ : actorList) {
		if (strcmp(actor_.first, name_) == 0) {
			return actor_.second;
		}
	}
	//Debug message when fail
	std::string message = std::string(name_) + " -> Actor not found in AssetManager";
	Debug::FatalError(message, __FILE__, __LINE__);
	return nullptr;
}

Ref<MaterialComponent> AssetManager::GetMaterial(const char* name_) {

	Ref<MaterialComponent> material_ = GetComponent<MaterialComponent>(name_);
	if (material_) return material_;
	//Debug message when fail
	std::string message = std::string(name_) + " -> Material not found in AssetManager";
	Debug::FatalError(message, __FILE__, __LINE__);
	return nullptr;
}

Ref<MeshComponent> AssetManager::GetMesh(const char* name_) {

	Ref<MeshComponent> mesh_ = GetComponent<MeshComponent>(name_);
	if (mesh_) return mesh_;
	//Debug message when fail
	std::string message = std::string(name_) + " -> Model not found in AssetManager";
	Debug::FatalError(message, __FILE__, __LINE__);
	return nullptr;
}

Ref<ShaderComponent> AssetManager::GetShader(const char* name_) {

	for (auto shader_ : shaderList) {
		if (strcmp(shader_.first, name_) == 0) {
			return shader_.second;
		}
	}
	//Debug message when fail
	std::string message = std::string(name_) + " -> Pipeline not found in AssetManager";
	Debug::FatalError(message, __FILE__, __LINE__);
	return nullptr;
}
