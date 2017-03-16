//.//.data:0000000141515E70; PythonInterface *pyi
//1515E70 //00007FF6EF140000
/*
.text:000000014017EF8C loc_14017EF8C:                          ; CODE XREF: PythonInterface::PythonInterface(Sim *)+2AFj
.text:000000014017EF8C                 mov     cs:?pyi@@3PEAVPythonInterface@@EA, rbx ; PythonInterface * pyi
.text:000000014017EF93                 lea     rcx, aInitialisingPy ; "Initialising Python Interface\n"
*/

#include <Windows.h>
#include <vector>
#include <memory>
#include <string>
#include <memory>
using namespace std;
class Sim;
class Car;
#define DllExport   __declspec( dllexport ) 
class GameObject
{
public:
	virtual void vfunc0();
	intptr_t game;
	wstring name;
	bool isActive;
	char padding[7];
	intptr_t parent;
	vector<GameObject*> gameObjects;
};

class IKeyEventListener
{
	virtual void vfunc0();
};

class CarPhysicsState
{
public:
	char padding0[1960];
	float engineLifeLeft;
	char padding1[48];
	float tyreInflation[4];
	char padding2[48];
	float susDamage[4];
	float tyreFlatSpot[4];
	char tyreThermalStates[736];
	float discTemps[4];
	float wear[4];
	float wearMult[4];
	float lockControlsTime;
	float kersCurrentKJ;
	bool kersIsCharging;
	char padding3[3];
	byte statusBytes[4];
	char padding4[4];
};

class ISuspensionAvatar
{
public:
	virtual void vfunc0();

};

class SuspensionAvatar : public ISuspensionAvatar
{
public:
	intptr_t wheelTransforms[4];
	intptr_t susTransforms[4];
	intptr_t discTransforms[4];
	intptr_t rearAxle;
	Car *car;
	char graphicsRadius[8];
	bool isGraphicsRadiusInitialized;
	char padding1[7];
};
class ISuspension 
{
public:
	virtual  ~ISuspension();
	virtual void getHubWorldMatrix(void);
	virtual void getPointVelocity();
	virtual void addForceAtPos();
	virtual void addTorque();
	virtual void setSteerLengthOffset();
	virtual void getSteerTorque();
	virtual void getHubAngularVelocity();
	virtual void attach();
	virtual void getStatus();
	virtual void getBasePosition();
	virtual void getK();
	virtual void getDamper();
	virtual void getPackerRange();
	virtual void getDebugLines();
	virtual void setDamage();
	virtual void resetDamage();
	virtual float getDamage();
	virtual void getMass();
	virtual void stop();
	virtual void getVelocity();
	virtual void getSteerBasis();
	virtual void step();
	virtual void setERPCFM();
};

class SuspensionStrut : public ISuspension
{
public:
};
class Car
{
public:
	virtual void vfunc0();
	char padding0[9952];
	vector<SuspensionStrut*> suspensions;

};

class CarAvatar : public GameObject
{
public:

	char padding0[192];
	Sim  *sim;
	wstring unixName;
	wstring configName;
	wstring guiName;
	wstring guiShortName;
	intptr_t bodyTransform;
	intptr_t steerTransformHR;
	intptr_t steerTransformLR;
	char padding1[64];
	intptr_t carNode;
	char driverEyesPosition[16];
	char bodyMatrix[64];
	char padding2[4];
	CarPhysicsState physicsState;
	char aiState[0x34];
	char padding3[4];
	intptr_t carAudioFMOD;
	intptr_t skidMarkBuffers[4];
	char physicsInfo[312];
	unique_ptr<void> raceEngineer;
	char modelBoundaries[0x18];
	vector<void*> wingsStatus;
	char onBoardExposure[4];
	char outBoardExposure[4];
	char dashBoardExposure[4];
	char padding4[4];
	vector<void*> cameras;
	//bool isDriverHR;
	//char padding5[7];
	intptr_t driverModel_HR;
	intptr_t driverModel_LR;
	SuspensionAvatar *suspensionAvatar;
	char padding5[328];
	Car *physics;
	//SuspensionAvatar *suspensionAvatar;
	//char padding0[1952];
	//float susDamage[4];
};

class Sim : public GameObject,
	public IKeyEventListener
{
public:
	char padding0[400];
	vector<CarAvatar*> cars;
};

class PythonInterface : public GameObject
{
public:
	Sim * pSim;
};