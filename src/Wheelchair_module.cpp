#include "Wheelchair_module.h"
#include "DMC-4143_Controller.h"
#include "WmraTypes.h"

using namespace WMRA;
DMC4143 controller;

Wheelchair_module::Wheelchair_module(void)
{
}

bool Wheelchair_module::isInitialized()
{
	return controller.isInitialized();
}
