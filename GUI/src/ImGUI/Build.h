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

		std::string buildPath;

		void Copy(std::string file, std::string path, int size);


		void BuildThread();
		bool SelectFolder();

	public:

		Build();

		void BuildProgression();
		void GenerateBuild();

	};

}