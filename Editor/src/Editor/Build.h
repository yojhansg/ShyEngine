#pragma once

#include <atomic>
#include <thread>
#include <string>

namespace ShyEditor {

   

	class Build
	{

	private:


		bool popUpOpen;
		std::thread thread;
		std::atomic<bool> inProgress;
		std::atomic<float> progression;

		std::string buildPath;

		void Copy(std::string file, std::string path);

		void BuildThread();
		bool SelectFolder();

	public:

		Build();
		~Build();

		void BuildProgression();
		void GenerateBuild();

	};

}