#include "CodePuzzleRoom.h"
#include "CodeActor.h"
#include "AssetManager.h"
#include "AudioManager.h"
#include "SceneManager.h"
#include "UIManager.h"
#include "Physics.h"
#include "Debug.h"

using namespace PHYSICS;

CodePuzzleRoom::CodePuzzleRoom(int width_, int length_, int height_, float passCode_): Room(width_, length_, height_) {
	SetCode(passCode_);

}

CodePuzzleRoom::~CodePuzzleRoom(){

}

bool CodePuzzleRoom::OnCreate(){

	//Create code panel
	Ref<CodeActor> codeActor_ = std::make_shared<CodeActor>(nullptr);
	Ref<Actor> codeActorData_ = AssetManager::GetInstance()->GetActor("CubeWhite");
	Ref<MeshComponent> mesh_ = codeActorData_->GetComponent<MeshComponent>();
	Ref<MaterialComponent> material_ = codeActorData_->GetComponent<MaterialComponent>();
	Ref<ShaderComponent> shader_ = codeActorData_->GetComponent<ShaderComponent>();
	codeActor_->AddComponent(mesh_);
	codeActor_->AddComponent(material_);
	codeActor_->AddComponent(shader_);

	Ref<TransformComponent> transform_ = std::make_shared<TransformComponent>(nullptr, Vec3(-7.0f, 1.0f, -25.0f), Quaternion(), Vec3(0.5f, 0.5f, 0.1f));
	codeActor_->AddComponent(transform_);
	codeActor_->AddComponent<AABB>(codeActor_.get(), transform_, transform_->GetPosition(), Vec3(1.5f, 2.0f, 1.5f), transform_->GetOrientation());
	codeActor_->OnCreate();
	AddActor("CodePanel", codeActor_);

	codePanel = codeActor_;

	//Add collision for trigger panel
	Ref<Actor> trigger = GetActor("Trigger");
	if (trigger) {
		trigger->AddComponent<AABB>(trigger.get(), trigger->GetComponent<TransformComponent>(),
			trigger->GetComponent<TransformComponent>()->GetPosition(),
			Vec3(trigger->GetComponent<TransformComponent>()->GetScale().x - 0.5f, 1.0f,
				trigger->GetComponent<TransformComponent>()->GetScale().z - 0.5f));
	}


	return true;
}

void CodePuzzleRoom::Update(float deltaTime){


	//Check when flashing stops
	for (auto actor_ : GetActorList()) {
		if (actor_.second->IsFlashing()) {
			reset = true;
			break;
		}
		reset = false;
	}

	Ref<Actor> localPlayer = GetActor("LocalPlayer");
	Ref<Actor> remotePlayer = GetActor("RemotePlayer");
	Ref<Actor> trigger = GetActor("Trigger");

	if (!trigger) return;

	bool status1 = Physics::TestTwoAABB(localPlayer->GetComponent<AABB>(), trigger->GetComponent<AABB>());
	bool status2 = Physics::TestTwoAABB(remotePlayer->GetComponent<AABB>(), trigger->GetComponent<AABB>());

	if ((status1 || status2) && !reset) {
		Reset();
	}


}

void CodePuzzleRoom::OnDestroy(){
}

void CodePuzzleRoom::CheckPuzzle(){

	std::vector<int> codeEntered = codePanel->GetCodeEntered();
	if (codeEntered.size() != CODE_SIZE) return;
	//std::cout << "Check Puzzled\n";
	for (int i = 0; i < CODE_SIZE; i++) {
		int code_ = codeEntered.at(i);
		int passCode_ = passCode.at(i);
		//printf("%d, %d\n", code_, passCode_);
		if (code_ != passCode_) {
			return;
		}
	}
	puzzleSolved = true;
}

void CodePuzzleRoom::Reset(){

	for (auto actor_ : GetActorList()) {
		actor_.second->ResetFlash();
	}
	reset = true;
	AudioManager::getInstance()->PlaySoundEffects("audio/winCoin.wav");
}

void CodePuzzleRoom::SetCode(const char* code_){
	ParseCode(atoi(code_));
}

void CodePuzzleRoom::SetCode(float code_) {
	//TODO::Set code
	ParseCode(code_);
}

void CodePuzzleRoom::ParseCode(int tempCode){
	//If it is greater than 4 digits don't set it
	if (tempCode >= 10000) {
		std::string tempStr = "Passcode over 4 digits: " + tempCode;
		Debug::Warning(tempStr, __FILE__, __LINE__);
		return;
	}

	//Clean the previous code
	passCode.clear();
	//Add code to vector
	passCode.push_back(tempCode / 1000.0f);
	tempCode = tempCode % 1000;
	passCode.push_back(tempCode / 100.0f);
	tempCode = tempCode % 100;
	passCode.push_back(tempCode / 10.0f);
	tempCode = tempCode % 10;
	passCode.push_back(tempCode);
}