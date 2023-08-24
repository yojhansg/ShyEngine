#pragma once

#include <atomic>
#include <thread>
#include <string>

namespace ShyEditor {

   

	class Build
	{

	private:


		bool popUpOpen;
		bool inProgress;
		std::thread thread;
		std::atomic<float> progression;



		void CopyFile(std::string file, std::string path, int size);
		void BuildThread();

	public:

		Build();

		void BuildProgression();
		void GenerateBuild();

	};

}