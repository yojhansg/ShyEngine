#pragma once

#include <fstream>


namespace CFlat {

	class IBox;

	struct OperatorManager
	{
		enum Operations {
			Print, If, Loop,
			
			//Math operations
			Add, Subtract, Multiply, Division, Power, SquareRoot, Mod, PlusOne,

			//Logic operations
			Equal, Different, Minor, Greater, MinorEqual, GreaterEqual, And, Or, Not,

			//Attributes
			getInteger, setInteger, getDecimal, setDecimal, getBoolean, setBoolean , getString, setString,



			OperatorCount
		};
		typedef void(*Funct)(IBox*, std::ofstream& stream);

		static Funct operations[OperatorCount];

		static void Initialise();

		static void ProccessOperation(int operationIdx, IBox* node, std::ofstream& stream);

		
	};

	namespace BoxOperations {


		namespace Control {

			void If(IBox* node, std::ofstream& stream);
			void Loop(IBox* node, std::ofstream& stream);
		}

		namespace Math {

			void Add(IBox* node, std::ofstream& stream);
			void Subtract(IBox* node, std::ofstream& stream);
			void Multiply(IBox* node, std::ofstream& stream);
			void Division(IBox* node, std::ofstream& stream);
			void Power(IBox* node, std::ofstream& stream);
			void SquareRoot(IBox* node, std::ofstream& stream);
			void Mod(IBox* node, std::ofstream& stream);
			void PlusOne(IBox* node, std::ofstream& stream);
		};

		namespace Logic {

			void Equals(IBox* node, std::ofstream& stream);
			void Different(IBox* node, std::ofstream& stream);
			void Minor(IBox* node, std::ofstream& stream);
			void Greater(IBox* node, std::ofstream& stream);
			void MinorEqual(IBox* node, std::ofstream& stream);
			void GreaterEqual(IBox* node, std::ofstream& stream);
			void And(IBox* node, std::ofstream& stream);
			void Or(IBox* node, std::ofstream& stream);
			void Not(IBox* node, std::ofstream& stream);
		}

		namespace Attributes {

			void getInteger(IBox* node, std::ofstream& stream);
			void getDecimal(IBox* node, std::ofstream& stream);
			void getBoolean(IBox* node, std::ofstream& stream);
			void getString(IBox* node, std::ofstream& stream);


			void setInteger(IBox* node, std::ofstream& stream);
			void setDecimal(IBox* node, std::ofstream& stream);
			void setBoolean(IBox* node, std::ofstream& stream);
			void setString(IBox* node, std::ofstream& stream);


		}

		namespace IO {
			void Print(IBox* node, std::ofstream& stream);

		};
	}

}

