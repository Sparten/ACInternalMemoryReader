/*
Created to add functinality to Crew Chief that is otherwise inaccessible.
Classes are as minimal as can be to prevent the less moraly gifted from exploiting the Sim.
*/

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

#define LOWORD(_dw)     ((WORD)(((DWORD_PTR)(_dw)) & 0xffff))
#define HIWORD(_dw)     ((WORD)((((DWORD_PTR)(_dw)) >> 16) & 0xffff))
#define LODWORD(_qw)    ((DWORD)(_qw))
#define HIDWORD(_qw)    ((DWORD)(((_qw) >> 32) & 0xffffffff))

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
	char padding0[0x74C];
	float tyreVirtualKM[4];
	char padding1[0x4C];
	float engineLifeLeft;
	char padding2[0x30];
	float tyreInflation[4];
	char padding3[0x1C];
	float tyreFlatSpot[4];
	char padding4[0x358];
};

class ISuspension
{
public:
	virtual  ~ISuspension();
	virtual void vfun1c();
	virtual void vfun2c();
	virtual void vfunc3();
	virtual void vfunc4();
	virtual void vfunc5();
	virtual void vfunc6();
	virtual void vfunc7();
	virtual void vfunc8();
	virtual void vfunc9();
	virtual void vfunc10();
	virtual void vfunc11();
	virtual void vfunc12();
	virtual void vfunc13();
	virtual void vfunc14();
	virtual void vfunc15();
	virtual void vfunc16();
	virtual float getDamage();
};

class Car
{
public:
	virtual void vfunc0();
	char padding0[0x2C80];
	vector<ISuspension*> suspensions;
};

class CarAvatar : public GameObject
{
public:
	char padding0[0x210];
	CarPhysicsState physicsState;
	char padding1[0x384];
	Car *physics;
};

class Sim : public GameObject,
	public IKeyEventListener
{
public:
	char padding0[0x1A0];
	vector<CarAvatar*> cars;
};

class PythonInterface : public GameObject
{
public:
	Sim * pSim;
};