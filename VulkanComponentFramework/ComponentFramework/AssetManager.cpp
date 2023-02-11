#include "AssetManager.h"
#include "VulkanRenderer.h"
#include <math.h>
#include <fstream>
#include "MeshComponent.h"
#include "ShaderComponent.h"
#include "MaterialComponent.h"
#include "TransformComponent.h"
#include "Actor.h"
#include <mutex>
#include "SceneManager.h"
#include "Scene.h"
#include "Room.h"
#include "RoomScene.h"

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

Scene* AssetManager::LoadRoom(const char* sceneName_) {

	XMLElement* fileData = ReadManiFest(LoadType::SCENE, sceneName_);
	if (!fileData) {
		Debug::Info("Fail to Load Scene " + std::string(sceneName_), __FILE__, __LINE__);
		return nullptr;
	}
	//Load Room data 
	Scene* room_ = CreateRoom(fileData);

	Debug::Info("Succeed to Load Scene " + std::string(sceneName_), __FILE__, __LINE__);
	return room_;
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
			//newActor->OnCreate();
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

Scene* AssetManager::CreateRoom(XMLElement* roomData){

	//Ref<Room> room_ = std::make_shared<Room>(100, 100, 1000);
	//Get information for room
	XMLElement* sizeData = roomData->FirstChildElement("Size");
	float width = sizeData->FloatAttribute("x");
	float length = sizeData->FloatAttribute("y");
	float height = sizeData->FloatAttribute("z");
	Ref<Room> room_ = std::make_shared<Room>(width, length, height);
	Scene* scene_ = new RoomScene(renderer, room_);
	//Add light data
	XMLElement* lightData = roomData->FirstChildElement("LightActor");
	XMLElement* light_ = lightData->FirstChildElement("Light");
	while (light_) {
		//Get position of the light
		float posX = light_->FloatAttribute("posX");
		float posY = light_->FloatAttribute("posY");
		float posZ = light_->FloatAttribute("posZ");
		//Get diffuse if the light
		float r_ = light_->FloatAttribute("r");
		float g_ = light_->FloatAttribute("g");
		float b_ = light_->FloatAttribute("b");
		float a_ = light_->FloatAttribute("a");
		//Add light to the scene
		scene_->AddLight(Vec4(posX, posY, posZ, 0.0f), Vec4(r_, g_, b_, a_));
		light_ = light_->NextSiblingElement("Light");
	}

	//Get possible actor in the room
	//Store a temp list for refecrenced actor
	std::unordered_map<const char*, Ref<Actor>> actorRefList;
	XMLElement* referenceData = roomData->FirstChildElement("ReferenceActor");
	XMLElement* refActorData = referenceData->FirstChildElement("RefName");
	while (refActorData) {
		//Get the actor data and add it to the reference list
		const char* refActorName_ = refActorData->FindAttribute("name")->Value();
		Ref<Actor> actorData_ = GetActor(refActorName_);
		actorRefList[refActorName_] = actorData_;

		refActorData = refActorData->NextSiblingElement("RefName");
	}

	//Get First child element and check if it exists;
	XMLElement* actorData = roomData->FirstChildElement("Actor");

	while (actorData) {

		//Get Actor ref from list
		const char* actorRefName = actorData->FindAttribute("ref")->Value();
		const char* actorName = actorData->FindAttribute("name")->Value();
		//If Ref name is Random, it means that it is generated randomly
		Ref<Actor> actorData_;
		if (strcmp(actorRefName, "Random") == 0) {
			//Generate a random number, and use this as index to grab actor data from list
			int listSize = actorRefList.size();
			int randomIndex = rand() % listSize;
			std::cout << randomIndex << "\n";
			/*DONT USE FIND FUNCTION
			* To avoid problems where the program is comparing address, instead of values within pointers
			* Do a for loop 
			*/
			int currentIndex = 0;
			for (auto element_ : actorRefList) {
				if (currentIndex == randomIndex) {
					actorData_ = element_.second;
				}
				currentIndex++;
			}
		}
		else {
			actorData_ = GetActor(actorRefName);
		}

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
		room_->AddActor(actorName, actor_);
		actorData = actorData->NextSiblingElement("Actor");
	}

	return scene_;
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
