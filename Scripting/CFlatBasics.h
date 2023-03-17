#pragma once
#include "CFlat.h"
#include <string>


namespace CFlat {


	class Split : public IBox {

	public:
		Split(int op, int id);
		IBox* otherRoute;
		void setOtherRoute(IBox*);
	};




	namespace Attributes {

		IBox* createAttribute(ParamType type, int id);
		IBox* createFloat(float value, int id);
		IBox* createInt(int value, int id);
		IBox* createBoolean(int value, int id);
		IBox* createString(std::string value, int id);
	}
}

